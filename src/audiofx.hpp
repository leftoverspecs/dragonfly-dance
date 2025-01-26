#pragma once

#include <audio/chunk.hpp>

class AudioFx {
public:
    AudioFx();

    void play_jump();
    void play_bubble();
private:
    engine::audio::Chunk jump;
    engine::audio::Chunk bubble;
};
