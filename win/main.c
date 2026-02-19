#include <windows.h>
#include "wine_unixcall.h"
#include <stdio.h>
#include <stdint.h>
#include <openxr/openxr_loader_negotiation.h>
#include "../include/unixcall.h"

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

XRAPI_ATTR XrResult XRAPI_CALL wine_xrEnumerateInstanceExtensionProperties(const char* layerName, uint32_t propertyCapacityInput, uint32_t* propertyCountOutput, XrExtensionProperties* properties)
{
    struct PARAMS_xrEnumerateInstanceExtensionProperties params = {
        .layerName = layerName,
        .propertyCapacityInput = propertyCapacityInput,
        .propertyCountOutput = propertyCountOutput,
        .properties = properties,
    };
    NTSTATUS res = UNIX_CALL(3, &params);
    if (res != STATUS_SUCCESS) return XR_ERROR_RUNTIME_FAILURE;
    return params.result;
}

XRAPI_ATTR XrResult XRAPI_CALL ourXrGetInstanceProcAddr(XrInstance instance, const char* name, PFN_xrVoidFunction* function)
{
    if (strcmp(name, "xrEnumerateInstanceExtensionProperties") == 0)
    {
        *function = (PFN_xrVoidFunction)&wine_xrEnumerateInstanceExtensionProperties;
        return XR_SUCCESS;
    }
    MessageBoxA(NULL, name, "xrGetInstanceProcAddr called with name", MB_OK);
    return XR_ERROR_FUNCTION_UNSUPPORTED;
}

__declspec(dllexport) XRAPI_ATTR XrResult XRAPI_CALL xrNegotiateLoaderRuntimeInterface(
    const XrNegotiateLoaderInfo*                loaderInfo,
    XrNegotiateRuntimeRequest*                  runtimeRequest)
{
    NTSTATUS res;

    if (loaderInfo->structType != XR_LOADER_INTERFACE_STRUCT_LOADER_INFO) return XR_ERROR_INITIALIZATION_FAILED;
    if (loaderInfo->structVersion != XR_LOADER_INFO_STRUCT_VERSION) return XR_ERROR_INITIALIZATION_FAILED;
    if (loaderInfo->structSize != sizeof(XrNegotiateLoaderInfo)) return XR_ERROR_INITIALIZATION_FAILED;

    if (runtimeRequest->structType != XR_LOADER_INTERFACE_STRUCT_RUNTIME_REQUEST) return XR_ERROR_INITIALIZATION_FAILED;
    if (runtimeRequest->structVersion != XR_RUNTIME_INFO_STRUCT_VERSION) return XR_ERROR_INITIALIZATION_FAILED;
    if (runtimeRequest->structSize != sizeof(XrNegotiateRuntimeRequest)) return XR_ERROR_INITIALIZATION_FAILED;

    uint32_t loader_runtime_version = 0;
    res = UNIX_CALL(1, &loader_runtime_version);
    if (res != STATUS_SUCCESS) return XR_ERROR_INITIALIZATION_FAILED;

    if (loaderInfo->minInterfaceVersion > loader_runtime_version) return XR_ERROR_INITIALIZATION_FAILED;
    if (loaderInfo->maxInterfaceVersion < loader_runtime_version) return XR_ERROR_INITIALIZATION_FAILED;
    runtimeRequest->runtimeInterfaceVersion = loader_runtime_version;

    uint64_t loader_api_version = 0;
    res = UNIX_CALL(2, &loader_api_version);
    if (res != STATUS_SUCCESS) return XR_ERROR_INITIALIZATION_FAILED;
    runtimeRequest->runtimeApiVersion = loader_api_version;
    runtimeRequest->getInstanceProcAddr = (PFN_xrGetInstanceProcAddr)&ourXrGetInstanceProcAddr;

    return XR_SUCCESS;
}