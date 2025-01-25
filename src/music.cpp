#include "music.hpp"

#include <assets/dragonfly.ogg.h>
#include <assets/intro.ogg.h>

Music::Music()
    : intro_music(intro_ogg, sizeof(intro_ogg)),
      chase_music(dragonfly_ogg, sizeof(dragonfly_ogg)) {
    engine::audio::Music::set_volume(MIX_MAX_VOLUME / 2);
}

void Music::chase() {
    chase_music.fade_in(-1, 50);
}

void Music::intro() {
    intro_music.fade_in(-1, 50);
}
