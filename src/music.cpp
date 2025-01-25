#include "music.hpp"

#include <assets/dragonfly.ogg.h>

Music::Music()
    : music(dragonfly_ogg, sizeof(dragonfly_ogg)) {
}

void Music::play() {
    music.fade_in(0, 0);
}
