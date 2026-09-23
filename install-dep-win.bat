@echo off
cd extern
git clone https://github.com/libsdl-org/SDL.git
git clone https://github.com/mavlink/MAVSDK.git
cd MAVSDK\cpp
git submodule update --init --recursive
cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=ON ^
      -DCMAKE_INSTALL_PREFIX="%cd%\..\mavsdk-install" ^
      -Bbuild -H.
cmake --build build --target install --config Release
