#include "Scene.hpp"
#include "SceneDescription.hpp"
#include "Window.hpp"
#include "spdlog/spdlog.h"

int main() {
    spdlog::set_pattern("[%H:%M:%S] %v");
    spdlog::set_level(spdlog::level::debug);

    auto scene_description = oglv::SceneDescription();
    auto win = oglv::Window(scene_description);

    auto scene = oglv::Scene(scene_description);
    win.run(&scene);
    return 0;
}
