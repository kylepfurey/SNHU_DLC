#include <SDL3/SDL.h>
#include "Timestamp.hpp"

namespace dlc {
    void Timestamp(std::ostream &output) {
        SDL_Time time;
        if (!SDL_GetCurrentTime(&time)) {
            output << "?:??:??\t\t" << SDL_GetError() << std::endl;
            return;
        }
        SDL_DateTime date;
        SDL_TimeToDateTime(time, &date, true);
        output << date.hour << ':' << date.minute << ':' << date.second << "\t\t";
    }
}
