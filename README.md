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

Debug:

```bash
mkdir debug
cd debug
conan install .. --output-folder . --build=missing -s build_type=Debug
cmake .. -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=DEBUG
cmake --build .
ctest
```

Release:

```bash
mkdir release
cd release
conan install .. --output-folder . --build=missing -s build_type=Release
cmake .. -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=RELEASE
cmake --build .
ctest
```

Add `-DBUILD_TESTS=OFF` to avoid building tests.


Build on windows:

```bash
cmake .. -G "Visual Studio 17 2022" -DCMAKE_TOOLCHAIN_FILE="conan_toolchain.cmake"
cmake --build . --config Debug
```

Debug:
```bash
cmake --build . --config Debug
```
Release:
```bash
cmake --build . --config Release
```

There are tests in test/ subdirectorty.
Run test:
```bash
ctest --test-dir test
```
