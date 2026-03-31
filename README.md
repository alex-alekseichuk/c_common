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

To run tests in parallel:
```bash
ctest -j 4
```

Build on windows:

Debug:
```bash
cmake .. -G "Visual Studio 18 2026" -DCMAKE_BUILD_TYPE=DEBUG
cmake --build . --config Debug
ctest -C Debug
```

Release:
```bash
cmake .. -G "Visual Studio 18 2026" -DCMAKE_BUILD_TYPE=RELEASE
cmake --build . --config Release
ctest -C Release
```


TODO:

- str
- implemen Stack by Array and List
- implement DList
- stream: foreach, map, filter, reduce
- example: print tree
- example: calculator, polish-notation
- example: http server
- example: http client
- example: ast, json, arithmetic
