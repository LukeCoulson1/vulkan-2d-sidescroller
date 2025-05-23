@echo off
REM Batch file to configure and build the project using CMake and vcpkg

REM Remove old build folder
IF EXIST build (
    rmdir /s /q build
)

REM Create build folder
mkdir build
cd build

REM Configure with CMake and vcpkg toolchain
cmake -DCMAKE_TOOLCHAIN_FILE=C:/Users/lukea/vcpkg/scripts/buildsystems/vcpkg.cmake -DVCPKG_TARGET_TRIPLET=x64-windows ..

REM Build the project
cmake --build .

cd ..
echo Build complete.
pause