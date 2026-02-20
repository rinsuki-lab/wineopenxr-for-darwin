add_library(wineopenxr_unix SHARED main.m)
# Link against the bundled OpenXR loader (built from third_party/OpenXR-SDK)
target_link_libraries(wineopenxr_unix PRIVATE openxr_loader "-framework Cocoa")
set_target_properties(wineopenxr_unix PROPERTIES
    OUTPUT_NAME "wineopenxr_darwinside"
    SUFFIX ".so"
    PREFIX ""
    OSX_ARCHITECTURES "x86_64"
)
