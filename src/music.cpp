#include "music.hpp"

#include <assets/dragonfly.ogg.h>
#include <assets/intro.ogg.h>

Music::Music()
    : intro_music(intro_ogg, sizeof(intro_ogg)),
      chase_music(dragonfly_ogg, sizeof(dragonfly_ogg)) {
    engine::audio::Music::set_volume(MIX_MAX_VOLUME / 4);
}

void Music::fade_in_chase() const {
    chase_music.fade_in(-1, 0);
}

void Music::fade_out() {
    engine::audio::Music::fade_out(50);
}

void Music::fade_in_intro() const {
    intro_music.fade_in(-1, 0);
}
