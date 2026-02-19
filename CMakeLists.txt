cmake_minimum_required(VERSION 3.20)
project(wineopenxr-for-darwin C OBJC)

include(ExternalProject)

# --- Configuration ---
set(WINE_LIB_PATH "/Applications/Wine Staging.app/Contents/Resources/wine/lib/wine/x86_64-windows"
    CACHE PATH "Path to Wine's x86_64-windows library directory")

# --- macOS side (wineopenxr.so) ---
## Use the bundled OpenXR SDK loader from third_party when available
add_subdirectory(third_party/OpenXR-SDK)

# macOS build
add_subdirectory(mac)

# --- Windows side (wineopenxr.dll) ---
# Cross-compiled with mingw as a separate CMake project
ExternalProject_Add(wineopenxr_dll
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/win
    BINARY_DIR ${CMAKE_BINARY_DIR}/win
    CMAKE_ARGS
        -DCMAKE_TOOLCHAIN_FILE=${CMAKE_SOURCE_DIR}/cmake/x86_64-w64-mingw32.cmake
        -DWINE_LIB_PATH=${WINE_LIB_PATH}
    BUILD_BYPRODUCTS ${CMAKE_BINARY_DIR}/win/wineopenxr.dll
    INSTALL_COMMAND ""
)
