#include <iostream>
#include "Images.h"
#include "Gamepad.h"
#include "HotkeyInput.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <D3DX11tex.h>
#include <tchar.h>
#include <dwmapi.h>
#include <Windows.h>
#include <thread>
#include <chrono>
#include <vector>

#pragma comment(lib,"XInput.lib")

static ID3D11Device* g_pd3dDevice = nullptr;
static ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
static ID3D11BlendState* g_pBlendState = nullptr;
static IDXGISwapChain* g_pSwapChain = nullptr;
static UINT                     g_ResizeWidth = 0, g_ResizeHeight = 0;
static ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;

bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void ChangeClickability(bool canclick, HWND ownd)
{
    long style = GetWindowLong(ownd, GWL_EXSTYLE);
    if (canclick) {
        style &= ~WS_EX_LAYERED;
        SetWindowLong(ownd, GWL_EXSTYLE, style);
        SetForegroundWindow(ownd);
        //windowstate = 1;
    }
    else {
        style |= WS_EX_LAYERED;
        SetWindowLong(ownd, GWL_EXSTYLE, style);
        //windowstate = 0;
    }
}

int main()
{
    //D3DX11CreateTextureFromFile();
    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"ImGui Example", nullptr };
    ::RegisterClassExW(&wc);
    HWND hwnd_overlay = ::CreateWindowExW(WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_NOACTIVATE, wc.lpszClassName, L"Dear ImGui DirectX11 Example", WS_POPUP, 0, 0, 1920, 1080, nullptr, nullptr, wc.hInstance, nullptr);
    MARGINS margins = { -1 };
    DwmExtendFrameIntoClientArea(hwnd_overlay, &margins);

    if (!CreateDeviceD3D(hwnd_overlay))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    ::ShowWindow(hwnd_overlay, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd_overlay);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImFont* DefaultFont = io.Fonts->AddFontFromFileTTF(".\\Fonts\\helvetica_bold.ttf", 20, NULL, io.Fonts->GetGlyphRangesCyrillic());
    ImFont* BigSizeDefaultFont = io.Fonts->AddFontFromFileTTF(".\\Fonts\\helvetica_bold.ttf", 80, NULL, io.Fonts->GetGlyphRangesCyrillic());

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(hwnd_overlay);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);
    ImagesInitialize(g_pd3dDevice);
    Gamepad gamepad;

    HWND current_window = NULL;
    bool show_overlay = false;
    std::string show_overlay_hotkey = "1 + 2";
    bool p_open = false;
    ChangeClickability(false, hwnd_overlay);
    bool done = false;

    bool show_hotkeys = false;

    float WindowBackgroundColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    float ImGuiElementsAlpha = 0.0f;
    SetCursorPos(0, 0);

    while (!done)
    {
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
        {
            CleanupRenderTarget();
            g_pSwapChain->ResizeBuffers(0, g_ResizeWidth, g_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
            g_ResizeWidth = g_ResizeHeight = 0;
            CreateRenderTarget();
        }


        HWND DekstopWindow = GetDesktopWindow();
        tagRECT DekstopWindowRect;
        GetWindowRect(DekstopWindow, &DekstopWindowRect);

        int DekstopWindowWidth = DekstopWindowRect.right;
        int DekstopWindowHeight = DekstopWindowRect.bottom;
        SetWindowPos(hwnd_overlay, NULL, 0, 0, DekstopWindowWidth, DekstopWindowHeight, NULL);

        if (!show_overlay) {
            HWND ForegroundWindow = GetForegroundWindow();
            if (ForegroundWindow != current_window)
                current_window = ForegroundWindow;
        }
        
        if (GetHotkeyInputText() == show_overlay_hotkey)
        {
            Sleep(150);
            if (show_overlay) {
                show_overlay = false;
                ChangeClickability(false, hwnd_overlay);
                //ChangeClickability(true, current_window);
                WindowBackgroundColor[3] = 0.0f;
                ImGuiElementsAlpha = 0.0f;
                gamepad.LeftMotorSpeed = gamepad.RightMotorSpeed = 0;
                gamepad.SetVibrate();
                //std::thread AnimationThread(HideAnimation, std::ref(WindowBackgroundColor[3]), std::ref(ImGuiElementsAlpha));
                //AnimationThread.join();
            }
            else {
                show_overlay = true;
                ChangeClickability(true, hwnd_overlay);
                //ChangeClickability(false, current_window);
                WindowBackgroundColor[3] = 0.4f;
                ImGuiElementsAlpha = 1.0f;
            }
        }


        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        if (show_overlay) {
            ImGui::SetNextWindowSize(ImVec2(DekstopWindowWidth, DekstopWindowHeight));
            ImGui::SetNextWindowPos(ImVec2(0, 0));
            ImGui::PushFont(DefaultFont);

            if (!gamepad.InitializeGamepad()) {
                ImGui::PushFont(BigSizeDefaultFont);

                ImGui::Begin("MainWindow", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
                ImGui::SetCursorPos(ImVec2((DekstopWindowWidth - ImGui::CalcTextSize("Gamepad is not connected").x) * 0.5f, DekstopWindowHeight * 0.5f));
                ImGui::Text("Gamepad is not connected");
                ImGui::End();

                ImGui::PopFont();
                //HelveticaFont->Scale = OldScale;
            }
            else {
                ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);
                ImGui::Begin("MainWindow", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);


                float GamepadWindowHeight = DekstopWindowWidth * 0.35f;
                float GamepadWindowWidth = GamepadWindowHeight * 1.20f;
                ImVec2 GamepadElementsSize(GamepadWindowWidth, GamepadWindowHeight);

                ImGui::SetCursorPosX((DekstopWindowWidth - GamepadWindowWidth) * 0.5f);
                ImGui::SetCursorPosY((DekstopWindowHeight - (GamepadWindowHeight + 125)) * 0.5f);
                ImGui::BeginChild("##MainChild", ImVec2(GamepadWindowWidth, GamepadWindowHeight + 125));
                ImGui::BeginChild("##Gamepad", ImVec2(GamepadWindowWidth + 40, GamepadWindowHeight));

                gamepad.SetVibrate();

                XINPUT_GAMEPAD GamepadInput = gamepad.GetGamepadInput();
                WORD GamepadPressedButtons = GamepadInput.wButtons;

                ImGui::SetCursorPos(ImVec2(0, 0));
                ImGui::Image((PVOID)GamepadImages::Gamepad, GamepadElementsSize);

                //AXYB
                ImGui::SetCursorPos(ImVec2(0, 0));
                if (GamepadPressedButtons & XINPUT_GAMEPAD_A)
                    ImGui::Image((PVOID)GamepadImages::ButtonA_Pressed, GamepadElementsSize);
                else
                    ImGui::Image((PVOID)GamepadImages::ButtonA, GamepadElementsSize);
                //-----------
                ImGui::SetCursorPos(ImVec2(0, 0));
                if (GamepadPressedButtons & XINPUT_GAMEPAD_X)
                    ImGui::Image((PVOID)GamepadImages::ButtonX_Pressed, GamepadElementsSize);
                else
                    ImGui::Image((PVOID)GamepadImages::ButtonX, GamepadElementsSize);
                //-----------
                ImGui::SetCursorPos(ImVec2(0, 0));
                if (GamepadPressedButtons & XINPUT_GAMEPAD_Y)
                    ImGui::Image((PVOID)GamepadImages::ButtonY_Pressed, GamepadElementsSize);
                else
                    ImGui::Image((PVOID)GamepadImages::ButtonY, GamepadElementsSize);
                //-----------
                ImGui::SetCursorPos(ImVec2(0, 0));
                if (GamepadPressedButtons & XINPUT_GAMEPAD_B)
                    ImGui::Image((PVOID)GamepadImages::ButtonB_Pressed, GamepadElementsSize);
                else
                    ImGui::Image((PVOID)GamepadImages::ButtonB, GamepadElementsSize);

                //Bumpers
                if (GamepadPressedButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) {
                    ImGui::SetCursorPos(ImVec2(0, 0));
                    ImGui::Image((PVOID)GamepadImages::Bumper_Pressed_Left, GamepadElementsSize);
                }
                if (GamepadPressedButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
                    ImGui::SetCursorPos(ImVec2(0, 0));
                    ImGui::Image((PVOID)GamepadImages::Bumper_Pressed_Right, GamepadElementsSize);
                }

                //Dpad
                if (GamepadPressedButtons & XINPUT_GAMEPAD_DPAD_UP) {
                    ImGui::SetCursorPos(ImVec2(0, 0));
                    ImGui::Image((PVOID)GamepadImages::DPAD_Pressed_Upper, GamepadElementsSize);
                }
                if (GamepadPressedButtons & XINPUT_GAMEPAD_DPAD_RIGHT) {
                    ImGui::SetCursorPos(ImVec2(0, 0));
                    ImGui::Image((PVOID)GamepadImages::DPAD_Pressed_Right, GamepadElementsSize);
                }
                if (GamepadPressedButtons & XINPUT_GAMEPAD_DPAD_DOWN) {
                    ImGui::SetCursorPos(ImVec2(0, 0));
                    ImGui::Image((PVOID)GamepadImages::DPAD_Pressed_Down, GamepadElementsSize);
                }
                if (GamepadPressedButtons & XINPUT_GAMEPAD_DPAD_LEFT) {
                    ImGui::SetCursorPos(ImVec2(0, 0));
                    ImGui::Image((PVOID)GamepadImages::DPAD_Pressed_Left, GamepadElementsSize);
                }

                //Select / Start
                if (GamepadPressedButtons & XINPUT_GAMEPAD_BACK) {
                    ImGui::SetCursorPos(ImVec2(0, 0));
                    ImGui::Image((PVOID)GamepadImages::Select_Pressed, GamepadElementsSize);
                }
                if (GamepadPressedButtons & XINPUT_GAMEPAD_START) {
                    ImGui::SetCursorPos(ImVec2(0, 0));
                    ImGui::Image((PVOID)GamepadImages::Start_Pressed, GamepadElementsSize);
                }

                //Left Trigger
                ImGui::SetCursorPos(ImVec2(0, 0));
                ImGui::PushStyleVar(ImGuiStyleVar_Alpha, (float)GamepadInput.bLeftTrigger / 255);
                ImGui::Image((PVOID)GamepadImages::Trigger_Pressed_Left, GamepadElementsSize);
                ImGui::PopStyleVar();
                //Right Trigger
                ImGui::SetCursorPos(ImVec2(0, 0));
                ImGui::PushStyleVar(ImGuiStyleVar_Alpha, (float)GamepadInput.bRightTrigger / 255);
                ImGui::Image((PVOID)GamepadImages::Trigger_Pressed_Right, GamepadElementsSize);
                ImGui::PopStyleVar();

                int Divider = 652 * 1920 / DekstopWindowWidth;
                ImGui::SetCursorPos(ImVec2(GamepadInput.sThumbLX / Divider, GamepadInput.sThumbLY / -Divider));
                if (GamepadPressedButtons & XINPUT_GAMEPAD_LEFT_THUMB)
                    ImGui::Image((PVOID)GamepadImages::Stick_Left_Pressed, GamepadElementsSize);
                else
                    ImGui::Image((PVOID)GamepadImages::Stick_Left, GamepadElementsSize);
                //-----------
                ImGui::SetCursorPos(ImVec2(GamepadInput.sThumbRX / Divider, GamepadInput.sThumbRY / -Divider));
                if (GamepadPressedButtons & XINPUT_GAMEPAD_RIGHT_THUMB)
                    ImGui::Image((PVOID)GamepadImages::Stick_Right_Pressed, GamepadElementsSize);
                else
                    ImGui::Image((PVOID)GamepadImages::Stick_Right, GamepadElementsSize);

                ImGui::EndChild();


                ImGui::SetCursorPosX((GamepadWindowWidth - ImGui::CalcTextSize("Test Gamepad Vibration").x) * 0.5f);
                ImGui::Text("Test Gamepad Vibration");
                //-----------
                ImGui::SetCursorPosX((GamepadWindowWidth - ImGui::CalcTextSize("Left Motor Speed").x) * 0.25f);
                ImGui::Text("Left Motor Speed");
                ImGui::SameLine();
                ImGui::SetCursorPosX((GamepadWindowWidth - ImGui::CalcTextSize("Right Motor Speed").x) * 0.75f);
                ImGui::Text("Right Motor Speed");
                //-----------
                ImGui::PushItemWidth(GamepadWindowWidth * 0.5f);
                ImGui::SliderInt("##LeftMotorSpeed", &gamepad.LeftMotorSpeed, 0, 50);
                ImGui::SameLine();
                ImGui::PushItemWidth(GamepadWindowWidth * 0.5f);
                ImGui::SliderInt("##RightMotorSpeed", &gamepad.RightMotorSpeed, 0, 50);
                if (ImGui::Button("Hotkeys", ImVec2(GamepadWindowWidth, 40)))
                    show_hotkeys = p_open = true;

                if (show_hotkeys && p_open) {
                    ImGui::SetNextWindowFocus();
                    ImGui::SetNextWindowBgAlpha(1.0f);
                    int HotkeysWindowWidth = DekstopWindowWidth * 0.4f;
                    int HotkeysWindowHeight = DekstopWindowHeight * 0.5f;
                    ImGui::SetNextWindowSize(ImVec2(HotkeysWindowWidth, HotkeysWindowHeight));
                    ImGui::SetNextWindowPos(ImVec2((DekstopWindowWidth - HotkeysWindowWidth) / 2, (DekstopWindowHeight - HotkeysWindowHeight) / 2));
                    ImGui::Begin("Hotkeys", &p_open, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
                    ImGui::Button("sdsds");
                    ImGui::End();
                }


                ImGui::PopStyleVar();
                ImGui::EndChild();
                ImGui::End();
            }

            ImGui::PopFont();
        }

        ImGui::Render();
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, WindowBackgroundColor);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        g_pSwapChain->Present(1, 0);
    }

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd_overlay);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}

bool CreateDeviceD3D(HWND hWnd)
{
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
    if (res != S_OK)
        return false;

    CreateRenderTarget();

    D3D11_BLEND_DESC desc;
    ZeroMemory(&desc, sizeof(desc));
    desc.AlphaToCoverageEnable = false;
    desc.RenderTarget[0].BlendEnable = true;
    desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_INV_DEST_ALPHA;
    desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
    desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    g_pd3dDevice->CreateBlendState(&desc, &g_pBlendState);

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