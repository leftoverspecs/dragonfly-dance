#include "music.hpp"

#include <assets/dragonfly.ogg.h>

Music::Music()
    : music(dragonfly_ogg, sizeof(dragonfly_ogg)) {
    engine::audio::Music::set_volume(MIX_MAX_VOLUME / 2);
}

void Music::play() {
    music.fade_in(-1, 0);
}
