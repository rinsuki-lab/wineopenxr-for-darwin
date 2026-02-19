#include <windows.h>
#include <ntstatus.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef UINT64 unixlib_handle_t;
extern unixlib_handle_t __wineopenxr_unixlib_handle;
extern __declspec(dllimport) NTSTATUS (WINAPI *__wine_unix_call_dispatcher)( unixlib_handle_t, unsigned int, void * );

NTSTATUS init_unix_call(void);

#define UNIX_CALL(code, params) __wine_unix_call_dispatcher(__wineopenxr_unixlib_handle, (code), (params))

#ifdef __cplusplus
}
#endif