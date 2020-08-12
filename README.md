---
page_type: sample
languages:
- cpp
products:
- windows-api-win32
name: Cloud Mirror sample
urlFragment: cloudmirror-sample
description: Demonstrates how to get started writing a cloud files provider using the cloud files API. 
extendedZipContent:
- path: LICENSE
  target: LICENSE
---

# Cloud Mirror sample

This sample shows how to get started writing a cloud files provider using the cloud files API.

This is the initial iteration, and as such it is an early preview, and far from final.

The following functionality is implemented:
* Declaring necessary Extensions and Capabilities in the *Package.appxmanifest*. Note that these declarations do not appear in the Visual Studio manifest editor.
* Registering/Unregistering a Sync Root which will show up in the Navigation Pane of Windows Explorer.
* Generating the initial placeholders in the Sync Root, using a physical "server" folder on the development machine as the fake cloud.
* Simulating Hydration of a file from a cloud service by slowly copying a file from a physical "server" folder on the development machine to a physical "client" folder on the development machine, including showing progress.
* Setting up custom states.
* Providing thumbnails for the file placeholders.
* adding a custom entry to the context menu when the user clicks on a file in the Sync Root.
* Supplying the URI of the cloud location of a file.

**Note**   The Windows-classic-samples repo contains a variety of code samples that exercise the various programming models, platforms, features, and components available in Windows and/or Windows Server. This repo provides a Visual Studio solution (SLN) file for each sample, along with the source files, assets, resources, and metadata needed to compile and run the sample. For more info about the programming models, platforms, languages, and APIs demonstrated in these samples, check out the documentation on the [Windows Dev Center](https://dev.windows.com). This sample is provided as-is in order to indicate or demonstrate the functionality of the programming models and feature APIs for Windows and/or Windows Server.

This sample was created for Windows 10 Version 1809 using Visual Studio 2017 using the Windows SDK 10.0.17763.0, but in many cases it will run unaltered using later versions. Please provide feedback on this sample!

To get a copy of Windows, go to [Downloads and tools](http://go.microsoft.com/fwlink/p/?linkid=301696).

To get a copy of Visual Studio, go to [Visual Studio Downloads](http://go.microsoft.com/fwlink/p/?linkid=301697).

## Note:
Change the location of the mirror when running in Mirror.cpp:
```
  if (FakeCloudProvider::Start(L"C:\\Users\\test\\Desktop\\MirrorServer", L"C:\\Users\\test\\Desktop\\MirrorClient"))
```

## Related topics

[**Documentation for this sample**](https://docs.microsoft.com/windows/desktop/cfapi/build-a-cloud-file-sync-engine)

[**Cloud files API**](https://docs.microsoft.com/en-us/windows/desktop/cfApi/cloud-files-api-portal)

[**UWP StorageProvider**](https://docs.microsoft.com/en-us/uwp/api/windows.storage.provider)


## Related technologies

[**Cloud files API**](https://docs.microsoft.com/en-us/windows/desktop/cfApi/cloud-files-api-portal)

[**UWP StorageProvider**](https://docs.microsoft.com/en-us/uwp/api/windows.storage.provider)

## Operating system requirements

### Client

Windows 10 Version 1809
