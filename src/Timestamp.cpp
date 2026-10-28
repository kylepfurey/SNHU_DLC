#include <iomanip>
#include <SDL3/SDL.h>
#include "Timestamp.hpp"

namespace dlc {
    void Timestamp(std::ostream &output) {
        SDL_Time time;
        SDL_DateTime date;
        if (!SDL_GetCurrentTime(&time) ||
            !SDL_TimeToDateTime(time, &date, true)) {
            constexpr char badTime[] = "??:??:??\t";
            output << badTime << SDL_GetError() << std::endl;
            output << badTime;
            return;
        }
        if (date.hour == 0) {
            date.hour = 12;
        } else if (date.hour > 12) {
            date.hour -= 12;
        }
        output << std::setw(2) << std::setfill('0') << date.hour << ':'
                << std::setw(2) << std::setfill('0') << date.minute << ':'
                << std::setw(2) << std::setfill('0') << date.second;
        output << " - SNHU DLC:\t";
    }
}
