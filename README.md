# ECL IDE

##  Building on windows

### Prerequisits 
* git (optional - needed to clone sources)
* cmake
* Visual Studio 2017 / 2019 / 2022
* NSIS (optional - needed to create install package)

**Note:**  The following instructions assume you are using the `git bash` terminal.  If you are using the `cmd` terminal, replace `rm` with `del` and `mkdir` with `md`.

**Note:**  ECL IDE now supports both 32-bit (x86) and 64-bit (x64) builds. Choose the appropriate architecture for your target platform.

### Clone the repository
```sh
git clone https://github.com/hpcc-systems/eclide.git eclide
cd eclide
git submodule update --init --recursive
```

### Remove old vcpkg.exe (if it exists)
```sh
rm ./vcpkg/vcpkg.exe
```

### Create a build folder (eclide/build)
```sh
mkdir build
cd build
```

### Generate Visual Studio Solution

**For 32-bit (x86) builds:**
```sh
cmake .. -A Win32 -DVCPKG_TARGET_TRIPLET=x86-windows
...or...
cmake .. -G "Visual Studio 16 2019" -A Win32 -DVCPKG_TARGET_TRIPLET=x86-windows
...or...
cmake .. -G "Visual Studio 17 2022" -A Win32 -DVCPKG_TARGET_TRIPLET=x86-windows
```

**For 64-bit (x64) builds:**
```sh
cmake .. -A x64 -DVCPKG_TARGET_TRIPLET=x64-windows
...or...
cmake .. -G "Visual Studio 16 2019" -A x64 -DVCPKG_TARGET_TRIPLET=x64-windows
...or...
cmake .. -G "Visual Studio 17 2022" -A x64 -DVCPKG_TARGET_TRIPLET=x64-windows
```

**Using CMake Presets (Recommended - triplet is set automatically):**
```sh
# 32-bit builds
cmake --preset vcpkg-VS-17
cmake --preset vcpkg-VS-16

# 64-bit builds
cmake --preset vcpkg-VS-17-x64
cmake --preset vcpkg-VS-16-x64
```

### Build

**Using standard cmake:**
```sh
cmake --build . --config RelWithDebInfo --parallel
```

**Using CMake Presets:**
```sh
# 32-bit builds
cmake --build --preset VS-17-RelWithDebInfo
cmake --build --preset VS-17-Debug

# 64-bit builds
cmake --build --preset VS-17-x64-RelWithDebInfo
cmake --build --preset VS-17-x64-Debug
```

Supported Builds
```sh
cmake --build . --config Debug --parallel
cmake --build . --config RelWithDebInfo --parallel
cmake --build . --config Release --parallel
cmake --build . --config MinSizeRel --parallel
```

### Create Installer
```sh
cmake --build . --config RelWithDebInfo --target package --parallel
```
