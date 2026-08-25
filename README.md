# SCGL - (Simple C GL) A simple OpenGL window template made with GLFW and GLAD.

## Windows.
On Windows, modify every `unixlike-gcc-debug` in [CMakeUserPrests.json]("./CMakeUserPresets.json") to `win-x64 release`, like so:
```json
    "configurePresets": [
        {
            "name": "win-x64-release-user",
            "displayName": "Win64 Release (User)",
            "inherits": "win-x64-release-"
        }
    ]
```
On UNIX-like Operating Systems you can just leave as is.

## Building.
To build the program on al OS's run
```cmake
cmake -S . -B build
cmake --build build
```

CMake will output a message saying where the executable is.
