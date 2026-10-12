# SoundBoard++

A lightweight soundboard built with Dear ImGui and SFML audio.

## Build
Requires CMake, Ninja, vcpkg (`VCPKG_ROOT` set), and on Linux: `zenity` and `autoconf-archive`.

```sh
cmake --preset vcpkg
cmake --build build
./build/SoundBoardPlusPlus
```
