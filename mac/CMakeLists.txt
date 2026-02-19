add_library(wineopenxr_unix SHARED main.m)
target_link_libraries(wineopenxr_unix PRIVATE "-framework Cocoa")
set_target_properties(wineopenxr_unix PROPERTIES
    OUTPUT_NAME "wineopenxr"
    SUFFIX ".so"
    PREFIX ""
    OSX_ARCHITECTURES "x86_64"
)
