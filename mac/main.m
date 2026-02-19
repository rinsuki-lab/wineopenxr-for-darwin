#import <Cocoa/Cocoa.h> 
#import <Metal/Metal.h>

#include <wchar.h>
#include <openxr/openxr_loader_negotiation.h>

#define XR_USE_GRAPHICS_API_METAL
#include <openxr/openxr_platform.h>

#include "../include/unixcall.h"

typedef int NTSTATUS;
#define STATUS_SUCCESS 0

#include "thunks.generated.h"

static NTSTATUS _Hello(const uint8_t* message)
{
    dispatch_sync(dispatch_get_main_queue(), ^{
        NSAlert* alert = [[NSAlert alloc] init];
        [alert setMessageText: [NSString stringWithUTF8String: (const char*)message]];
        [alert addButtonWithTitle: @"OK"];
        [alert runModal];
        [alert release];
    });

    return STATUS_SUCCESS;
}

static NTSTATUS _GetOpenXRLoaderRuntimeVersion(uint32_t* version)
{
    *version = XR_CURRENT_LOADER_RUNTIME_VERSION;
    return STATUS_SUCCESS;
}

static NTSTATUS _GetOpenXRAPIVersion(uint64_t* version)
{
    *version = XR_CURRENT_API_VERSION;
    return STATUS_SUCCESS;
}

static NTSTATUS _xrEnumerateInstanceExtensionProperties(struct PARAMS_xrEnumerateInstanceExtensionProperties* params)
{
    params->result = xrEnumerateInstanceExtensionProperties(params->layerName, params->propertyCapacityInput, params->propertyCountOutput, params->properties);
    if (params->layerName == NULL || *params->layerName == '\0') {
        // We need to inject XR_KHR_D3D11_enable extension
        uint32_t originalCount = *params->propertyCountOutput;
        (*params->propertyCountOutput) += 1;
        if (params->propertyCapacityInput > originalCount) {
            strcpy(params->properties[originalCount].extensionName, "XR_KHR_D3D11_enable");
            params->properties[originalCount].extensionVersion = 1;
        }
    }
    return STATUS_SUCCESS;
}

static NTSTATUS _xrCreateInstance(struct PARAMS_xrCreateInstance* params)
{
    XrInstanceCreateInfo modifiedCreateInfo = *params->createInfo;
    // We need to modify XR_KHR_D3D11_enable to XR_KHR_metal_enable
    const char** modifiedExtensionNames = malloc(sizeof(char*) * modifiedCreateInfo.enabledExtensionCount);
    for (uint32_t i = 0; i < modifiedCreateInfo.enabledExtensionCount; i++) {
        if (strcmp(modifiedCreateInfo.enabledExtensionNames[i], "XR_KHR_D3D11_enable") == 0) {
            modifiedExtensionNames[i] = "XR_KHR_metal_enable";
        } else {
            modifiedExtensionNames[i] = modifiedCreateInfo.enabledExtensionNames[i];
        }
    }
    modifiedCreateInfo.enabledExtensionNames = modifiedExtensionNames;
    params->result = xrCreateInstance(&modifiedCreateInfo, params->instance);
    free(modifiedExtensionNames);
    return STATUS_SUCCESS;
}

static NTSTATUS _GetOpenXRMetalDeviceRegistryID(struct PARAMS_GetOpenXRMetalDeviceRegistryID* params)
{
    PFN_xrGetMetalGraphicsRequirementsKHR xrGetMetalGraphicsRequirementsKHR;
    params->result = xrGetInstanceProcAddr(params->instance, "xrGetMetalGraphicsRequirementsKHR", (PFN_xrVoidFunction*)&xrGetMetalGraphicsRequirementsKHR);
    if (params->result != XR_SUCCESS) return STATUS_SUCCESS;
    XrGraphicsRequirementsMetalKHR graphicsRequirements = { XR_TYPE_GRAPHICS_REQUIREMENTS_METAL_KHR };
    params->result = xrGetMetalGraphicsRequirementsKHR(params->instance, params->systemId, &graphicsRequirements);
    if (params->result != XR_SUCCESS) return STATUS_SUCCESS;
    id<MTLDevice> device = graphicsRequirements.metalDevice;
    params->metalDeviceRegistryId = [device registryID];
    return STATUS_SUCCESS;
}

const void* __wine_unix_call_funcs[] = 
{
    &_Hello,
    &_GetOpenXRLoaderRuntimeVersion,
    &_GetOpenXRAPIVersion,
    &_xrEnumerateInstanceExtensionProperties,
    &_xrCreateInstance,
    &_GetOpenXRMetalDeviceRegistryID,
    // you need to modify & regenerate generate_thunks.py after adding new functions
    GENERATED_UNIX_CALLS,
};

_Static_assert(sizeof(__wine_unix_call_funcs) / sizeof(void*) == LAST_UNIX_CALL + 1, "Generated unix call function count does not match LAST_UNIX_CALL");