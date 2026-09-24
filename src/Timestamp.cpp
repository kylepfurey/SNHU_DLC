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
        output << date.hour << ':' << date.minute << ':' << date.second << '\t';
    }
}
