#pragma once

#include <audio/music.hpp>

class Music {
public:
    Music();

    void play();
private:
    engine::audio::Music music;
};
