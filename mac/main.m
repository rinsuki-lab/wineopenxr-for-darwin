#import <Cocoa/Cocoa.h> 
#include <wchar.h>
#include <openxr/openxr_loader_negotiation.h>
#include "../include/unixcall.h"

typedef int NTSTATUS;
#define STATUS_SUCCESS 0

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

const void* __wine_unix_call_funcs[] = 
{
    &_Hello,
    &_GetOpenXRLoaderRuntimeVersion,
    &_GetOpenXRAPIVersion,
    &_xrEnumerateInstanceExtensionProperties,
};
