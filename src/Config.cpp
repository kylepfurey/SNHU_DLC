#include <SDL3/SDL.h>
#include "Timestamp.hpp"
#include "Config.hpp"

namespace dlc {
    void ReloadConfig(Config &config, const char jsonFile[]) {
        size_t size;
        auto *file = static_cast<char *>(SDL_LoadFile(jsonFile, &size));
        if (file != nullptr) {
            config = nlohmann::json::parse(file, nullptr, false, true);
            SDL_free(file);
        } else {
            Timestamp(std::cerr);
            std::cerr << "No " << jsonFile << " file found!" << std::endl;
        }
    }
}
