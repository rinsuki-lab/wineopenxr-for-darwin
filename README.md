# wineopenxr-for-darwin

OpenXR bridge for Wine on macOS. Heavily inspired from Proton's wineopenxr, but written from scratch.

**Note:** Your OpenXR runtime's dylib should have a `x86_64` binary, otherwise we couldn't load it. currently only known runtime to do that is https://github.com/rinsuki/FruitXR . (Meta XR Simulator on macOS only supports arm64, which means not compatible with wineopenxr).

Testing with:
* wine-staging 11.2 from Homebrew Cask
* https://github.com/3Shain/dxmt/releases/tag/v0.73

`win/` is for Windows x86_64, `mac/` is for Wine's Unix Call.
