@echo off
cd extern
git clone https://github.com/microsoft/vcpkg.git "%USERPROFILE%\vcpkg"
call "%USERPROFILE%\vcpkg\bootstrap-vcpkg.bat"
"%USERPROFILE%\vcpkg\vcpkg.exe" install sdl3 sdl3-ttf
git clone --recursive https://github.com/mavlink/MAVSDK.git
cd MAVSDK\cpp
cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=ON ^
      -DCMAKE_INSTALL_PREFIX="%cd%\..\..\mavsdk-install" ^
      -Bbuild -S.
cmake --build build --target install --config Release
