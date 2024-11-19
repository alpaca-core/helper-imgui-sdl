// Copyright (c) Alpaca Core
// SPDX-License-Identifier: MIT
//
#include <ImGuiSdlApp.hpp>

int main(int, char**) try { // this signature is required by SDL
    ac::ImGuiSdlApp app;
    app.init("ImGui SDL example", { 1280, 720 });
    bool done = false;
    while (!done) {
        app.processInput(done);
        app.beginFrame();
        ImGui::ShowDemoWindow();
        app.endFrame();
    }
    return 0;
}
catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << "\n";
    return -1;
}
catch (...) {
    std::cerr << "Unknown error\n";
    return -1;
}
