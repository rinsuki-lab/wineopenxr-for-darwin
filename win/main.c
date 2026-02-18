#include <windows.h>
#include "wine_unixcall.h"
#include <stdio.h>
#include <stdint.h>
#include <openxr/openxr_loader_negotiation.h>

__declspec(dllexport) BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call != DLL_PROCESS_ATTACH)
        return TRUE;
    DisableThreadLibraryCalls(hModule);

    if (init_unix_call() != STATUS_SUCCESS)
    {
        MessageBoxA(NULL, "Failed to initialize Wine's Unix Call.\n\nMaybe you're forgetten to copy the wineopenxr.so to Wine's x86_64-unix folder, or your Wine or macOS is too old to run WineOpenXR.", "WineOpenXR for Darwin", MB_OK | MB_ICONERROR);
        return FALSE;
    }

    return TRUE;
}

__declspec(dllexport) int hi()
{
    MessageBoxA(NULL, "Hello, world!", "", MB_OK);
    NTSTATUS res = UNIX_CALL(0, "Hello, world from Windows world!");
    return 0;
}


__declspec(dllexport) XRAPI_ATTR XrResult XRAPI_CALL xrNegotiateLoaderRuntimeInterface(
    const XrNegotiateLoaderInfo*                loaderInfo,
    XrNegotiateRuntimeRequest*                  runtimeRequest)
{
    if (loaderInfo->structType != XR_LOADER_INTERFACE_STRUCT_LOADER_INFO) return XR_ERROR_INITIALIZATION_FAILED;
    if (loaderInfo->structVersion != XR_LOADER_INFO_STRUCT_VERSION) return XR_ERROR_INITIALIZATION_FAILED;
    if (loaderInfo->structSize != sizeof(XrNegotiateLoaderInfo)) return XR_ERROR_INITIALIZATION_FAILED;

    if (runtimeRequest->structType != XR_LOADER_INTERFACE_STRUCT_RUNTIME_REQUEST) return XR_ERROR_INITIALIZATION_FAILED;
    if (runtimeRequest->structVersion != XR_RUNTIME_INFO_STRUCT_VERSION) return XR_ERROR_INITIALIZATION_FAILED;
    if (runtimeRequest->structSize != sizeof(XrNegotiateRuntimeRequest)) return XR_ERROR_INITIALIZATION_FAILED;

    return XR_SUCCESS; // TODO
}