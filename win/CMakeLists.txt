cmake_minimum_required(VERSION 3.20)
project(wineopenxr-win C)

set(WINE_LIB_PATH "/Applications/Wine Staging.app/Contents/Resources/wine/lib/wine/x86_64-windows"
    CACHE PATH "Path to Wine's x86_64-windows library directory")

add_library(wineopenxr SHARED main.c wine_unixcall.c)

target_include_directories(wineopenxr PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/OpenXR-SDK/include
)

target_link_directories(wineopenxr PRIVATE "${WINE_LIB_PATH}")
target_link_libraries(wineopenxr PRIVATE ntdll user32 ucrt)

set_target_properties(wineopenxr PROPERTIES
    PREFIX ""
    OUTPUT_NAME "wineopenxr"
)
