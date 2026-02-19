#include <openxr/openxr.h>

#ifndef UNIXCALL_H
#define UNIXCALL_H
#pragma pack(push, 8)

struct PARAMS_xrEnumerateInstanceExtensionProperties
{
    const char* layerName;
    const uint32_t propertyCapacityInput;
    uint32_t* propertyCountOutput;
    XrExtensionProperties* properties;
    XrResult result;
};

#pragma pack(pop)
#endif