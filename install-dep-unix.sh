cd extern
git clone https://github.com/microsoft/vcpkg.git "$HOME/vcpkg"
"$HOME/vcpkg/bootstrap-vcpkg.sh"
"$HOME/vcpkg/vcpkg" install sdl3 sdl3-ttf
git clone --recursive https://github.com/mavlink/MAVSDK.git
cd MAVSDK/cpp
cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=ON \
      -DCMAKE_INSTALL_PREFIX="${PWD}/../../mavsdk-install" \
      -Bbuild -S.
cmake --build build --target install --config Release
