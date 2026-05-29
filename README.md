# seditor

!!! This project is not finished !!!
Text Editor in SDL


## Building

### Windows
1. Get external modules
```
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


