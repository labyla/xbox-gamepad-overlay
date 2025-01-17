#include "Gamepad.h"

bool Gamepad::InitializeGamepad() {
    for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
    {
        XINPUT_STATE ControllerState;
        if (XInputGetState(i, &ControllerState) == ERROR_SUCCESS) {
            dwUserIndex = i;
            return true;
        }
    }
    return false;
}

DWORD Gamepad::getUserIndex() {
    return dwUserIndex;
}

std::string Gamepad::getBatteryLevel() {
    if (XInputGetBatteryInformation(dwUserIndex, BATTERY_DEVTYPE_GAMEPAD, &battery) == ERROR_SUCCESS) {
        if (battery.BatteryLevel == BATTERY_LEVEL_EMPTY)
            return "Empty";
        if (battery.BatteryLevel == BATTERY_LEVEL_LOW)
            return "Low";
        if (battery.BatteryLevel == BATTERY_LEVEL_MEDIUM)
            return "Medium";
        return "Full";
    }
    return "None";
}

void Gamepad::SetVibrate() {
    XINPUT_VIBRATION vibration;

    vibration.wLeftMotorSpeed = LeftMotorSpeed * 1310;
    vibration.wRightMotorSpeed = RightMotorSpeed * 1310;

    XInputSetState(dwUserIndex, &vibration);
}

XINPUT_GAMEPAD Gamepad::GetGamepadInput() {
    XINPUT_STATE ControllerState;
    XInputGetState(dwUserIndex, &ControllerState);

    return ControllerState.Gamepad;
}