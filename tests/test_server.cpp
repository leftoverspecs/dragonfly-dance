#include <common/initialize.hpp>
#include <csignal>
#include <iomanip>
#include <networking/initialize.hpp>
#include <networking/server.hpp>

#include <iostream>

namespace {

bool running = true;

void sigint_handler(int dummy) {
    running = false;
}

}// namespace

class test_handler : public engine::networking::handler {
public:
    std::vector<uint8_t> receive(int id, const std::vector<uint8_t> &data) override {
        for (const auto& e : data) {
            std::cout << "0x" << std::hex << std::setfill('0') << std::setw(2) << std::right << static_cast<int>(e) << ' ';
        }

        std::cout << '\n';
        std::vector<uint8_t> response;
        response.reserve(data.size() * 2);
        response.insert(response.end(), data.begin(), data.end());
        response.insert(response.end(), data.begin(), data.end());
        return response;
    }

    bool idle() override { return running; }
};

int main() {
    signal(SIGINT, sigint_handler);

    engine::common::initialize sdl(SDL_INIT_EVERYTHING);
    engine::networking::initialize sdlnet(sdl);

    engine::networking::server s(10000, 10);
    test_handler h;
    s.run(1000, h);

    return 0;
}
