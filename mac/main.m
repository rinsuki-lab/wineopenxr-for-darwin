#import <Cocoa/Cocoa.h> 
#include <wchar.h>

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

    return 0;
}

const void* __wine_unix_call_funcs[] = 
{
    &_Hello,
};
