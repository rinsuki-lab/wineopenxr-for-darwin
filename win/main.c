#define XR_USE_GRAPHICS_API_D3D11

#include <windows.h>
#include "wine_unixcall.h"
#include <stdio.h>
#include <stdint.h>
#include <openxr/openxr_loader_negotiation.h>

#include <d3d11.h>
#include <openxr/openxr_platform.h>

#include "../include/unixcall.h"
#include "thunks.generated.h"

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

XRAPI_ATTR XrResult XRAPI_CALL wine_xrGetD3D11GraphicsRequirementsKHR(XrInstance instance, XrSystemId systemId, XrGraphicsRequirementsD3D11KHR* graphicsRequirements)
{
    // @see https://github.com/3Shain/dxmt/blob/58e4115e196c1d7c50c0843462ed9c738950f7fa/src/dxgi/dxgi_adapter.cpp#L16-L19
    struct PARAMS_GetOpenXRMetalDeviceRegistryID params = {
        .instance = instance,
        .systemId = systemId,
    };
    NTSTATUS res = UNIX_CALL(5, &params);
    if (res != STATUS_SUCCESS) return XR_ERROR_RUNTIME_FAILURE;
    if (params.result != XR_SUCCESS) return params.result;
    uint64_t swappedId = __builtin_bswap64(params.metalDeviceRegistryId);
    memcpy(&graphicsRequirements->adapterLuid, &swappedId, sizeof(uint64_t));
    graphicsRequirements->minFeatureLevel = D3D_FEATURE_LEVEL_11_0;
    return XR_SUCCESS;
}

XRAPI_ATTR XrResult XRAPI_CALL ourXrGetInstanceProcAddr(XrInstance instance, const char* name, PFN_xrVoidFunction* function)
{
    if (strcmp(name, "xrGetD3D11GraphicsRequirementsKHR") == 0)
    {
        *function = (PFN_xrVoidFunction)wine_xrGetD3D11GraphicsRequirementsKHR;
        return XR_SUCCESS;
    }
    if (wine_xrGetInstanceProcAddr(instance, name, function) == XR_SUCCESS)
    {
        return XR_SUCCESS;
    }
    fprintf(stderr, "Unknown function requested: %s\n", name);
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