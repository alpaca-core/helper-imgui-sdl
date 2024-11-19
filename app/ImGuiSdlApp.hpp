// Copyright (c) Alpaca Core
// SPDX-License-Identifier: MIT
//
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>
#include <SDL.h>

#include <iostream>
#include <stdexcept>

namespace ac {

class ImGuiSdlApp {
protected:
    SDL_Window* m_window = nullptr;
    SDL_Renderer* m_renderer = nullptr;

    static ImGuiIO& initImGui() {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        return ImGui::GetIO();
    }
    ImGuiIO& m_imguiIo = initImGui();
public:
    ImGuiSdlApp() = default;

    ~ImGuiSdlApp() {
        if (m_renderer) {
            ImGui_ImplSDLRenderer2_Shutdown();
            ImGui_ImplSDL2_Shutdown();
        }

        ImGui::DestroyContext();

        if (m_renderer) SDL_DestroyRenderer(m_renderer);
        if (m_window) SDL_DestroyWindow(m_window);
        SDL_Quit();
    }

    static void throwSdlError(const char* msg) {
        throw std::runtime_error(std::string(msg) + ": " + SDL_GetError());
    }

    void init(const char* windowTitle, std::pair<int, int> windowSize, int sdlFlags = 0) {
        // sdl

        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | sdlFlags) != 0) {
            throwSdlError("SDL_Init");
        }

        SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");

        m_window = SDL_CreateWindow(
            windowTitle,
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            windowSize.first, windowSize.second,
            SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
        if (!m_window) {
            throwSdlError("SDL_CreateWindow");
        }

        m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
        if (m_renderer == nullptr) {
            throwSdlError("SDL_CreateRenderer");
        }

        SDL_RendererInfo info;
        SDL_GetRendererInfo(m_renderer, &info);
        std::cout << "SDL_Renderer: " << info.name << "\n";

        // imgui

        m_imguiIo.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        ImGui::StyleColorsDark();

        // setup backends
        ImGui_ImplSDL2_InitForSDLRenderer(m_window, m_renderer);
        ImGui_ImplSDLRenderer2_Init(m_renderer);
    }

    void processInput(bool& quitRequested) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                quitRequested = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(m_window))
                quitRequested = true;
        }
    }

    void beginFrame() {
        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
    }

    void endFrame() {
        ImGui::Render();
        SDL_RenderSetScale(m_renderer, m_imguiIo.DisplayFramebufferScale.x, m_imguiIo.DisplayFramebufferScale.y);
        SDL_RenderClear(m_renderer);
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), m_renderer);
        SDL_RenderPresent(m_renderer);
    }
};

} // namespace ac
