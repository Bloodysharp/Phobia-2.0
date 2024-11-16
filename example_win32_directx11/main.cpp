#define IMGUI_DEFINE_MATH_OPERATORS

#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "imgui_edited.hpp"

#include "imgui_freetype.h"
#include <d3d11.h>
#include <tchar.h>

#include "font.h"
#include "texture.h"

#include <vector>
#include <string>

#include <imgui.h>
#include <vector>
#include <string>
#include <chrono>
#include <algorithm>

#include <D3DX11tex.h>
#pragma comment(lib, "D3DX11.lib")

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

static ID3D11Device*            g_pd3dDevice = nullptr;
static ID3D11DeviceContext*     g_pd3dDeviceContext = nullptr;
static IDXGISwapChain*          g_pSwapChain = nullptr;
static UINT                     g_ResizeWidth = 0, g_ResizeHeight = 0;
static ID3D11RenderTargetView*  g_mainRenderTargetView = nullptr;

bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

using namespace ImGui;
namespace texture
{
    ID3D11ShaderResourceView* logo = nullptr;
    ID3D11ShaderResourceView* logo_two = nullptr;
    ID3D11ShaderResourceView* foto_user = nullptr;
}

static int m_tabs;
bool particles = true;
namespace render_ui
{
    void rage_tab()
    {
        ImGui::SetCursorPos(ImVec2(169, 38));
        edited::BeginChild("Override config", ImVec2(320, 57));
        {
            const char* items[] = { "Auto Snipers (G3SG1, SCAR-20, SC...)", "Types", "Select" };
            static int select;

            edited::Combo("Select Weapon", "Select a weapon to customize", &select, items, IM_ARRAYSIZE(items), 3);
        }
        edited::EndChild();

        ImGui::SetCursorPos(ImVec2(169, 134));
        edited::BeginChild("General", ImVec2(320, 240));
        {
            static bool active = true;
            static bool inactive = false;

            edited::Checkbox("Enabled particle", &particles);

            edited::Checkbox("Autofire", &inactive);
            edited::Checkbox("Autowall", &active);
            edited::Checkbox("Silent aim", &active);
            edited::Checkbox("Quick peek", &inactive);

        }
        edited::EndChild();

        ImGui::SetCursorPos(ImVec2(169, 413));
        edited::BeginChild("Exploits", ImVec2(320, 107));
        {
            static bool active = true;
            static bool inactive = false;

            edited::Checkbox("Lag Peek", &active);
            edited::Checkbox("Hide Shots", &active);
            edited::Checkbox("Double Tab", &active);
        }
        edited::EndChild();

        ImGui::SetCursorPos(ImVec2(505, 38));
        edited::BeginChild("Accuracy", ImVec2(320, 286));
        {
            static bool active = true;
            static bool inactive = false;

            static int select = 0;
            const char* items[3]{ "One", "Two", "Three" };

            edited::Checkbox("Automatic stop", &active);

            edited::Combo("Select Weapon", "Select a weapon to customize", &select, items, IM_ARRAYSIZE(items), 3);

            edited::Checkbox("Hitchance", &active);

            static int procentw = 60;
            edited::SliderInt("Field of View", "", &procentw, 0, 100, "%d\xC2\xB0");

            static int procente = 20;
            edited::SliderInt("Damage value", "", &procente, 0, 100, "%d\xC2\xB0");

            static int procent = 56;
            edited::SliderInt("Damage override value", "", &procent, 0, 100, "%d\xC2\xB0");
        }
        edited::EndChild();

        ImGui::SetCursorPos(ImVec2(505, 363));
        edited::BeginChild("Misc", ImVec2(320, 157));
        {
            static bool active = true;
            static bool inactive = false;

            edited::Checkbox("Prefer a pont", &inactive);
            edited::Checkbox("Prefer body aim", &active);
            edited::Checkbox("ignore limbs when moving", &active);
        }
        edited::EndChild();
    }

    void RenderTab()
    {
        auto draw = ImGui::GetWindowDrawList();
        ImVec2 pos = ImGui::GetWindowPos();

        draw->AddText(font::poppins, 17, ImVec2(pos.x + 13, pos.y + 81), ImColor(105, 105, 105, int(255 * ImGui::GetStyle().Alpha)), "Aimbot");  // right bg
        draw->AddText(font::poppins, 17, ImVec2(pos.x + 13, pos.y + 210), ImColor(105, 105, 105, int(255 * ImGui::GetStyle().Alpha)), "Visuals");  // right bg
        draw->AddText(font::poppins, 17, ImVec2(pos.x + 13, pos.y + 348), ImColor(105, 105, 105, int(255 * ImGui::GetStyle().Alpha)), "Miscellaneous");  // right bg

        ImGui::SetCursorPos(ImVec2(13, 99));
        if (edited::Tab("r", "Legit Bot", !m_tabs)) m_tabs = 0;

        ImGui::SetCursorPos(ImVec2(13, 136));
        if (edited::Tab("e", "Rage Bot", m_tabs == 1)) m_tabs = 1;

        ImGui::SetCursorPos(ImVec2(13, 174));
        if (edited::Tab("a", "Anti-Aims", m_tabs == 2)) m_tabs = 2;

        ImGui::SetCursorPos(ImVec2(13, 228));
        if (edited::Tab("x", "Players", m_tabs == 3)) m_tabs = 3;

        ImGui::SetCursorPos(ImVec2(13, 266));
        if (edited::Tab("w", "World", m_tabs == 4)) m_tabs = 4;

        ImGui::SetCursorPos(ImVec2(13, 304));
        if (edited::Tab("v", "View", m_tabs == 5)) m_tabs = 5;

        ImGui::SetCursorPos(ImVec2(13, 369));
        if (edited::Tab("z", "Main", m_tabs == 6)) m_tabs = 6;

        ImGui::SetCursorPos(ImVec2(13, 407));
        if (edited::Tab("s", "Inventory", m_tabs == 7)) m_tabs = 7;

        ImGui::SetCursorPos(ImVec2(13, 445));
        if (edited::Tab("c", "Configs", m_tabs == 8)) m_tabs = 8;

        switch (m_tabs)
        {
        case 0: rage_tab(); break; // RAGEBOT
        case 1: break; // LEGITBOT
        case 2: break; // PLAYERS
        case 3: break; // VISUALS
        case 4: break; // MISC
        case 5: break; // SETTINGS
        }
    }

    void user_info()
    {
        auto draw = ImGui::GetWindowDrawList();
        ImVec2 pos = ImGui::GetWindowPos();

        GetBackgroundDrawList()->AddRectFilled(ImVec2(pos.x + 9, pos.y + 486), ImVec2(pos.x + 152, pos.y + 523), ImColor(41, 41, 41, int(255 * ImGui::GetStyle().Alpha)), c::background::rounding_o, ImDrawFlags_RoundCornersAll); // right bg
        GetBackgroundDrawList()->AddRect(ImVec2(pos.x + 9, pos.y + 486), ImVec2(pos.x + 152, pos.y + 523), ImColor(50, 50, 50, int(255 * ImGui::GetStyle().Alpha)), c::background::rounding_o, ImDrawFlags_RoundCornersAll, 1);  // right bg

        //парсим с сайта
        GetBackgroundDrawList()->AddColoredText(ImVec2(pos.x + 49, pos.y + 488), ImColor(105, 105, 105, int(255 * ImGui::GetStyle().Alpha)), ImColor(255, 255, 255, int(255 * ImGui::GetStyle().Alpha)), "Name: (User)");
        GetBackgroundDrawList()->AddColoredText(ImVec2(pos.x + 49, pos.y + 503), ImColor(105, 105, 105, int(255 * ImGui::GetStyle().Alpha)), ImColor(255, 255, 255, int(255 * ImGui::GetStyle().Alpha)), "Till: (11.11.2024)");

        ImGui::SetCursorPos(ImVec2(15, 490));
        ImGui::Image(texture::foto_user, ImVec2(28, 28));//logo 2
    }

    void Decoration()
    {
        const ImVec2& pos = GetWindowPos();
        const ImVec2& region = GetContentRegionMax();

        GetBackgroundDrawList()->AddRectFilled(ImVec2(pos.x, pos.y), ImVec2(pos.x + 161, pos.y + 535), ImColor(32, 32, 32, int(255 * ImGui::GetStyle().Alpha)), c::background::rounding, ImDrawFlags_RoundCornersLeft); // left bg
        GetBackgroundDrawList()->AddRect(ImVec2(pos.x, pos.y), ImVec2(pos.x + 161, pos.y + 535), ImColor(50, 50, 50, int(255 * ImGui::GetStyle().Alpha)), c::background::rounding, ImDrawFlags_RoundCornersLeft, 1);  // left bg

        GetBackgroundDrawList()->AddRectFilled(ImVec2(pos.x + 160, pos.y), ImVec2(pos.x + 838, pos.y + 535), ImColor(26, 26, 26, int(255 * ImGui::GetStyle().Alpha)), c::background::rounding, ImDrawFlags_RoundCornersRight); // right bg
        GetBackgroundDrawList()->AddRect(ImVec2(pos.x + 160, pos.y), ImVec2(pos.x + 838, pos.y + 535), ImColor(50, 50, 50, int(255 * ImGui::GetStyle().Alpha)), c::background::rounding, ImDrawFlags_RoundCornersRight, 1);  // right bg

        // на будующие делаю 2 лого не ебу какое вам нужно
        ImGui::SetCursorPos(ImVec2(23, 30));
        ImGui::Image(texture::logo, ImVec2(120, 25.1f));//logo 1

        ImGui::SetCursorPos(ImVec2(36, -7));
        ImGui::Image(texture::logo_two, ImVec2(85, 98));//logo 2
    }

    void Particles()
    {
        if (!particles)
            return;

        ImVec2 screen_size = { (float)GetSystemMetrics(SM_CXSCREEN), (float)GetSystemMetrics(SM_CYSCREEN) };

        static ImVec2 partile_pos[100];
        static ImVec2 partile_target_pos[100];
        static float partile_speed[100];
        static float partile_radius[100];


        for (int i = 1; i < 50; i++)
        {
            if (partile_pos[i].x == 0 || partile_pos[i].y == 0)
            {
                partile_pos[i].x = rand() % (int)screen_size.x + 1;
                partile_pos[i].y = 15.f;
                partile_speed[i] = 1 + rand() % 25;
                partile_radius[i] = rand() % 4;

                partile_target_pos[i].x = rand() % (int)screen_size.x;
                partile_target_pos[i].y = screen_size.y * 2;
            }

            partile_pos[i] = ImLerp(partile_pos[i], partile_target_pos[i], ImGui::GetIO().DeltaTime * (partile_speed[i] / 60));

            if (partile_pos[i].y > screen_size.y)
            {
                partile_pos[i].x = 0;
                partile_pos[i].y = 0;
            }

            ImGui::GetWindowDrawList()->AddCircleFilled(partile_pos[i], partile_radius[i], ImColor(174, 139, 148, 255 / 2));
        }

    }

    void Render()
    {
        NewFrame();
        {
            SetNextWindowSize(c::background::size);
            ImGui::Begin("Dear ImGui", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBackground);
            {
                Particles();
                Decoration();
                user_info();
                RenderTab();
            }
            ImGui::End();
        }
    }
}

int main(int, char**)
{
    // Create application window
    //ImGui_ImplWin32_EnableDpiAwareness();
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("ImGui Example"), NULL };
    ::RegisterClassEx(&wc);
    HWND hwnd = ::CreateWindow(wc.lpszClassName, _T("Dear ImGui DirectX9 Example"), WS_POPUPWINDOW, 0, 0, 1920, 1080, NULL, NULL, wc.hInstance, NULL);

    // Initialize Direct3D
    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    // Show the window
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; 

    //Font
    ImFontConfig cfg;
    cfg.FontBuilderFlags = ImGuiFreeTypeBuilderFlags_ForceAutoHint | ImGuiFreeTypeBuilderFlags_LightHinting | ImGuiFreeTypeBuilderFlags_LoadColor;
    io.Fonts->AddFontFromMemoryTTF(poppin_font, sizeof(poppin_font), 16, &cfg, io.Fonts->GetGlyphRangesCyrillic());
    font::poppins = io.Fonts->AddFontFromMemoryTTF(poppin_font, sizeof(poppin_font), 16, &cfg, io.Fonts->GetGlyphRangesCyrillic());
    font::poppinss = io.Fonts->AddFontFromMemoryTTF(poppin_font, sizeof(poppin_font), 18, &cfg, io.Fonts->GetGlyphRangesCyrillic());
    font::font_icon = io.Fonts->AddFontFromMemoryTTF(icon_font, sizeof(icon_font), 25.0f, &cfg, io.Fonts->GetGlyphRangesCyrillic());

    //Byte
    D3DX11_IMAGE_LOAD_INFO info; 
    ID3DX11ThreadPump* pump{ nullptr };

    if (texture::logo == nullptr)
        D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, logo_type, sizeof(logo_type), &info, pump, &texture::logo, 0);

    if (texture::logo_two == nullptr)
        D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, logo_type_two, sizeof(logo_type_two), &info, pump, &texture::logo_two, 0);

    if (texture::foto_user == nullptr)
        D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, user, sizeof(user), &info, pump, &texture::foto_user, 0);

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

    // Main loop
    bool done = false;
    while (!done)
    {
        // Poll and handle messages (inputs, window resize, etc.)
        // See the WndProc() function below for our to dispatch events to the Win32 backend.
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done) break;

        if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
        {
            CleanupRenderTarget();
            g_pSwapChain->ResizeBuffers(0, g_ResizeWidth, g_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
            g_ResizeWidth = g_ResizeHeight = 0;
            CreateRenderTarget();
        }

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();

        //render menu
        render_ui::Render();

        Render();

        const float clear_color_with_alpha[4] = { 0.f };
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        g_pSwapChain->Present(1, 0);
    }

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}

bool CreateDeviceD3D(HWND hWnd)
{
    // Setup swap chain
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;

    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res == DXGI_ERROR_UNSUPPORTED)
        res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res != S_OK) return false;

    CreateRenderTarget();
    return true;
}

void CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
}

void CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void CleanupRenderTarget()
{
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
            return 0;
        g_ResizeWidth = (UINT)LOWORD(lParam);
        g_ResizeHeight = (UINT)HIWORD(lParam);
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU)
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}
