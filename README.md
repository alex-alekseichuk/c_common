Project template for cross-platform C library.

We will use Conan to install external dependencies and generate the files that CMake needs to find this library and build our project.

Create build/ subfolder:
```bash
conan install . --output-folder=build --build=missing
cd build
```

Build on linux:
```bash
cmake .. -G "Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE="conan_toolchain.cmake" -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

Build on windows:
```bash
cmake .. -G "Visual Studio 17 2022" -DCMAKE_TOOLCHAIN_FILE="conan_toolchain.cmake"
cmake --build . --config Release
```

There is example of the executable application project in the example/ subfolder.
See example/README.md.
