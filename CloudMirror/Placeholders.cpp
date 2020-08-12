// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved

#include "stdafx.h"

static int CallbackCount = 0;

void Placeholders::FetchPlaceholders(
    _In_ PCWSTR serverPathStr,
    _In_ CONST CF_CALLBACK_INFO* callbackInfo,
    _In_ CONST CF_CALLBACK_PARAMETERS* callbackParameters)
{
    NTSTATUS status = S_OK;
    CF_PLACEHOLDER_CREATE_INFO* placeholders = NULL;
    INT placeholdersCount = 0;

    try
    {
        WIN32_FIND_DATA findData;
        HANDLE hFileHandle;

        // Ensure that the source path ends in a backslash.
        std::wstring serverPath(serverPathStr);
        if (serverPath.back() != L'\\')
        {
            serverPath.push_back(L'\\');
        }

        std::wstring relativePath = L"";
        if (callbackInfo->FileIdentity != NULL)
        {
            relativePath = reinterpret_cast<wchar_t const*>(callbackInfo->FileIdentity);
        }

        // Ensure that a nonempty subdirectory ends in a backslash.
        if (relativePath.length() && relativePath.back() != '\\')
        {
            relativePath.push_back(L'\\');
        }

        std::wstring fileName(serverPath);
        fileName.append(relativePath);
        fileName.append(callbackParameters->FetchPlaceholders.Pattern);

        // Find out how many files there are and pre-allocate an array to hold their placeholders:
        placeholdersCount = Utilities::CountFilesInPath(fileName);
        if (placeholdersCount > 0)
        {
            placeholders = new CF_PLACEHOLDER_CREATE_INFO[placeholdersCount]{ 0 };
        }

        CallbackCount++;
        wprintf(L"Creating placeholders for - %s (called %d times)\n", fileName.c_str(), CallbackCount);

        INT index = 0;
        hFileHandle = FindFirstFileEx(fileName.data(), FindExInfoBasic, &findData, FindExSearchNameMatch, NULL, FIND_FIRST_EX_ON_DISK_ENTRIES_ONLY);
        if (hFileHandle != INVALID_HANDLE_VALUE)
        {
            do
            {
                if (index >= placeholdersCount)
                {
                    // No more room available in the pre-allocated array
                    break;
                }

                if (!wcscmp(findData.cFileName, L".") ||  !wcscmp(findData.cFileName, L".."))
                {
                    continue;
                }

                std::wstring relativeName(L"");
                relativeName.append(findData.cFileName);

                wchar_t* name = StrDupW(relativeName.c_str());
                placeholders[index].FileIdentity = name;
                placeholders[index].FileIdentityLength = (USHORT)(wcslen(relativeName.data()) + 1) * sizeof(WCHAR);
                placeholders[index].RelativeFileName = name;
                placeholders[index].Flags = CF_PLACEHOLDER_CREATE_FLAG_MARK_IN_SYNC;
                placeholders[index].FsMetadata.FileSize.QuadPart = ((ULONGLONG)findData.nFileSizeHigh << 32) + findData.nFileSizeLow;
                placeholders[index].FsMetadata.BasicInfo.FileAttributes = findData.dwFileAttributes;
                placeholders[index].FsMetadata.BasicInfo.CreationTime = Utilities::FileTimeToLargeInteger(findData.ftCreationTime);
                placeholders[index].FsMetadata.BasicInfo.LastWriteTime = Utilities::FileTimeToLargeInteger(findData.ftLastWriteTime);
                placeholders[index].FsMetadata.BasicInfo.LastAccessTime = Utilities::FileTimeToLargeInteger(findData.ftLastAccessTime);
                placeholders[index].FsMetadata.BasicInfo.ChangeTime = Utilities::FileTimeToLargeInteger(findData.ftLastWriteTime);
                if ((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
                {
                    placeholders[index].FsMetadata.FileSize.QuadPart = 0;
                }
                index++;

            } while (FindNextFile(hFileHandle, &findData));
            FindClose(hFileHandle);

            // The index determines how many placeholders were actually found - in case content changed since the initial count.
            placeholdersCount = index;
        }
    }
    catch (...)
    {
        wprintf(L"Could not create cloud file placeholders in the sync root with %08x\n", static_cast<HRESULT>(winrt::to_hresult()));
        status = S_FALSE;
    }

    // Respond to the callback with the placeholders found in the directory
    CF_OPERATION_INFO opInfo = { 0 };
    CF_OPERATION_PARAMETERS opParams = { 0 };

    opInfo.StructSize = sizeof(opInfo);
    opInfo.Type = CF_OPERATION_TYPE_TRANSFER_PLACEHOLDERS;
    opInfo.ConnectionKey = callbackInfo->ConnectionKey;
    opInfo.TransferKey = callbackInfo->TransferKey;

    opParams.ParamSize = sizeof(opParams);
    opParams.TransferPlaceholders.CompletionStatus = status;
    opParams.TransferPlaceholders.Flags = CF_OPERATION_TRANSFER_PLACEHOLDERS_FLAG_NONE;
    opParams.TransferPlaceholders.PlaceholderCount = placeholdersCount;
    opParams.TransferPlaceholders.PlaceholderTotalCount.QuadPart = placeholdersCount;
    opParams.TransferPlaceholders.PlaceholderArray = placeholders;

    winrt::check_hresult(CfExecute(&opInfo, &opParams));

    if (placeholdersCount > 0)
    {
        for (INT i = 0; i < placeholdersCount; i++)
        {
            LocalFree((HLOCAL)placeholders[i].RelativeFileName);
        }
        delete[] placeholders;
    }
}
