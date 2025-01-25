#pragma once

#include <audio/music.hpp>

class Music {
public:
    Music();

    void chase();
    void intro();
private:
    engine::audio::Music intro_music;
    engine::audio::Music chase_music;
};
