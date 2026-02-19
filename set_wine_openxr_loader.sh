#!/bin/bash
wine reg add HKLM\\SOFTWARE\\Khronos\\OpenXR\\1 /v ActiveRuntime /t REG_SZ /f /d "$(winepath -w $(dirname $0)/build/win/openxr64.json)"