#pragma once
#include <nlohmann/json.hpp>

namespace dlc {
    //
    // The Config.json configuration file.
    // This is a file on disk used for changing settings without editing code.
    // Access its data like a map with CONFIG["propertyName"].
    //
    using Config = nlohmann::json;

    //
    // Loads a config file into memory.
    //
    void ReloadConfig(Config &config, const char jsonFile[] = "Config.json");
}
