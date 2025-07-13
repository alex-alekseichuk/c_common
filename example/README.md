Example of an executable application.
The library is linked dynamically/statically.
Configured in cmake via add_subdirectory.

Copy example/ directory one level up
```bash
cp -r example ..
cd ../example
```

We will use Conan to install external dependencies and generate the files that CMake needs to find this library and build our project
```bash
conan install . --output-folder=build --build=missing
cd build
```

linux:
```bash
cmake .. -G "Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE="conan_toolchain.cmake" -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

windows:
```bash
cmake .. -G "Visual Studio 17 2022" -DCMAKE_TOOLCHAIN_FILE="conan_toolchain.cmake"
cmake --build . --config Release
```
