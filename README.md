# seditor

!!! This project is not finished !!!
Text Editor in SDL


## Building

Notes about CMake:
If you are on Ubuntu or Debian-based distros, this project will most probably build fine,
but for other platforms (Arch, Windows), we need to modify the root CMakeLists file; change the version
of cmake_minimum_version_required() from 3.0 to 4.0 (provided the installed cmake version is >4.0).

### Windows
1. Get external modules
```
    cd seditor/
    git clone --depth 1 https://github.com/libsdl-org/SDL.git thirdparty/SDL
    cd thirdparty/SDL_ttf/external
    Get-GitModules.ps1
```
2. Build
```
    cd seditor/
    cmake -B build
    cmake --build build
```

### Linux
1. Get dependencies
    1. SDL dependencies
        https://wiki.libsdl.org/SDL3/README-linux#build-dependencies
    2. freetype and GTK
        on ubuntu/debian
        ```
        sudo apt install libgtk-3-dev libfreetype6-dev
        ```
2. Build
    ```
    cmake -B build
    cmake --build build
    ```


