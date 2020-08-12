// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved

#pragma once
class Placeholders
{
public:
    static void FetchPlaceholders(
        _In_ PCWSTR serverPathStr,
        _In_ CONST CF_CALLBACK_INFO* callbackInfo,
        _In_ CONST CF_CALLBACK_PARAMETERS* callbackParameters);
};

