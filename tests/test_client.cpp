#include <iomanip>
#include <iostream>
#include <networking/client.hpp>
#include <numeric>

#include <common/initialize.hpp>
#include <networking/initialize.hpp>

int main() {
    engine::common::initialize sdl(SDL_INIT_EVERYTHING);
    engine::networking::initialize sdlnet(sdl);
    engine::networking::client c("localhost", 10000);
    std::vector<uint8_t> data(1024);
    std::iota(data.begin(), data.end(), 1);
    auto result = c.send(data);
    for (const auto& e : result) {
        std::cout << "0x" << std::hex << std::setfill('0') << std::setw(2) << std::right << static_cast<int>(e) << ' ';
    }
    std::cout << '\n';
    result = c.send(data);
    result = c.send({});
    return 0;
}
