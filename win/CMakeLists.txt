cmake_minimum_required(VERSION 3.20)
project(wineopenxr-win C CXX)

set(WINE_LIB_PATH "/Applications/Wine Staging.app/Contents/Resources/wine/lib/wine/x86_64-windows"
    CACHE PATH "Path to Wine's x86_64-windows library directory")

add_library(wineopenxr SHARED main.cpp wine_unixcall.c)

target_include_directories(wineopenxr PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/OpenXR-SDK/include
)

target_link_directories(wineopenxr PRIVATE "${WINE_LIB_PATH}")
target_link_libraries(wineopenxr PRIVATE ntdll user32 ucrt)

set_target_properties(wineopenxr PROPERTIES
    PREFIX ""
    OUTPUT_NAME "wineopenxr_darwin64"
)

# Copy openxr64.json next to the built target so it's available as an artifact
add_custom_command(TARGET wineopenxr POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/openxr64.json"
        "$<TARGET_FILE_DIR:wineopenxr>/openxr64.json"
)
