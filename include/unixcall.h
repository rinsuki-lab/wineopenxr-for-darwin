#include <openxr/openxr.h>

#ifndef UNIXCALL_H
#define UNIXCALL_H
#pragma pack(push, 8)

#include "unixcall.generated.h"

struct PARAMS_GetOpenXRMetalDeviceRegistryID
{
    const XrInstance instance;
    const XrSystemId systemId;
    uint64_t metalDeviceRegistryId;
    XrResult result;
};

#pragma pack(pop)
#endif