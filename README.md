Common C library.

We use Conan to install external dependencies
and generate the files that CMake needs to find this library and build our projects.

Install conan CLI tool:
```bash
sudo apt install pipx
pipx install conan
conan --version
```

Besides the conanfile.txt, we need a Conan profile to build our project.
that's single time for the system (not for per project).
You can check it later: /home/alex/.conan2/profiles/default.
```bash
conan profile detect --force
conan profile path default
```

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
