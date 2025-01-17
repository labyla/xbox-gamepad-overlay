#pragma once
#include <iostream>
#include <Windows.h>
#include <Xinput.h>

class Gamepad {
private:
    DWORD dwUserIndex = -1;
    XINPUT_BATTERY_INFORMATION battery;
public:
    int LeftMotorSpeed = 0, RightMotorSpeed = 0;

    bool InitializeGamepad();

    DWORD getUserIndex();

    std::string getBatteryLevel();

    void SetVibrate();

    XINPUT_GAMEPAD GetGamepadInput();
};