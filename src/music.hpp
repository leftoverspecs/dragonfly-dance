#pragma once

#include <audio/music.hpp>

class Music {
public:
    Music();

    void fade_in_chase() const;
    void fade_in_intro() const;

    static void fade_out();

private:
    engine::audio::Music intro_music;
    engine::audio::Music chase_music;
};
