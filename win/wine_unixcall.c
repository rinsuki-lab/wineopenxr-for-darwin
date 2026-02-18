// Since this file's contents are copied from the Wine project, their license applies to this file as well.
#include "wine_unixcall.h"
#include "ntsecapi.h"

typedef UINT64 unixlib_module_t;

#define __WINESRC__

#pragma region winternl.h // https://github.com/wine-mirror/wine/blob/wine-11.2/include/winternl.h#L4746
/**
 * Copyright (C) the Wine project
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */
typedef enum _MEMORY_INFORMATION_CLASS {
    MemoryBasicInformation = 0,
    MemoryWorkingSetInformation = 1,
    MemoryMappedFilenameInformation = 2,
    MemoryRegionInformation = 3,
    MemoryWorkingSetExInformation = 4,
    MemorySharedCommitInformation = 5,
    MemoryImageInformation = 6,
    MemoryRegionInformationEx = 7,
    MemoryPrivilegedBasicInformation = 8,
    MemoryEnclaveImageInformation = 9,
    MemoryBasicInformationCapped = 10,
    MemoryPhysicalContiguityInformation = 11,
    MemoryBadInformation = 12,
    MemoryBadInformationAllProcesses = 13,
    MemoryImageExtensionInformation = 14,
#ifdef __WINESRC__
    MemoryWineLoadUnixLib = 1000,
    MemoryWineLoadUnixLibWow64,
    MemoryWineLoadUnixLibByName,
    MemoryWineLoadUnixLibByNameWow64,
    MemoryWineUnloadUnixLib,
#endif
} MEMORY_INFORMATION_CLASS;
NTSYSAPI void      WINAPI RtlInitUnicodeString(PUNICODE_STRING,PCWSTR);
NTSYSAPI NTSTATUS  WINAPI NtQueryVirtualMemory(HANDLE,LPCVOID,MEMORY_INFORMATION_CLASS,PVOID,SIZE_T,SIZE_T*);
#pragma endregion

#pragma region unix_lib.c // from https://github.com/wine-mirror/wine/blob/wine-11.2/dlls/winecrt0/unix_lib.c
/*
 * Support for the Unix part of builtin dlls
 *
 * Copyright 2019 Alexandre Julliard
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

NTSTATUS WINAPI __wine_load_unix_lib( const UNICODE_STRING *name, unixlib_module_t *lib,
                                      unixlib_handle_t *handle )
{
    UINT64 res[2];
    NTSTATUS status = NtQueryVirtualMemory( GetCurrentProcess(), name, MemoryWineLoadUnixLibByName,
                                            res, handle ? sizeof(res) : sizeof(res[0]), NULL );
    if (!status)
    {
        if (lib) *lib = res[0];
        if (handle) *handle = res[1];
    }
    return status;
}

#pragma endregion

// ---

unixlib_handle_t __wineopenxr_unixlib_handle = 0;

NTSTATUS WINAPI init_unix_call(void)
{
    UNICODE_STRING name;
    RtlInitUnicodeString(&name, L"wineopenxr.so");
    NTSTATUS status = __wine_load_unix_lib(&name, NULL, &__wineopenxr_unixlib_handle );
    return status;
}