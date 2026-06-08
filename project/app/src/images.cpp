#include "images.h"

namespace GamepadImages {
	ID3D11ShaderResourceView* Gamepad;

	ID3D11ShaderResourceView* ButtonA;
	ID3D11ShaderResourceView* ButtonB;
	ID3D11ShaderResourceView* ButtonY;
	ID3D11ShaderResourceView* ButtonX;

	ID3D11ShaderResourceView* ButtonA_Pressed;
	ID3D11ShaderResourceView* ButtonB_Pressed;
	ID3D11ShaderResourceView* ButtonY_Pressed;
	ID3D11ShaderResourceView* ButtonX_Pressed;

	ID3D11ShaderResourceView* Stick_Left;
	ID3D11ShaderResourceView* Stick_Left_Pressed;

	ID3D11ShaderResourceView* Stick_Right;
	ID3D11ShaderResourceView* Stick_Right_Pressed;

	ID3D11ShaderResourceView* Select_Pressed;
	ID3D11ShaderResourceView* Start_Pressed;

	ID3D11ShaderResourceView* DPAD_Pressed_Left;
	ID3D11ShaderResourceView* DPAD_Pressed_Right;
	ID3D11ShaderResourceView* DPAD_Pressed_Down;
	ID3D11ShaderResourceView* DPAD_Pressed_Upper;

	ID3D11ShaderResourceView* Trigger_Pressed_Left;
	ID3D11ShaderResourceView* Trigger_Pressed_Right;

	ID3D11ShaderResourceView* Bumper_Pressed_Left;
	ID3D11ShaderResourceView* Bumper_Pressed_Right;
}


bool ImagesInitialize(ID3D11Device* pDevice) {
	D3DX11_IMAGE_LOAD_INFO info;
	ID3DX11ThreadPump* pump = nullptr;

	D3DX11CreateShaderResourceViewFromFileW(pDevice, L"./references/gamepad.png", &info, pump, &GamepadImages::Gamepad, 0);

	D3DX11CreateShaderResourceViewFromFileW(pDevice, L"./references/a.png", &info, pump, &GamepadImages::ButtonA, 0);
	D3DX11CreateShaderResourceViewFromFileW(pDevice, L"./references/b.png", &info, pump, &GamepadImages::ButtonB, 0);
	D3DX11CreateShaderResourceViewFromFileW(pDevice, L"./references/y.png", &info, pump, &GamepadImages::ButtonY, 0);
	D3DX11CreateShaderResourceViewFromFileW(pDevice, L"./references/x.png", &info, pump, &GamepadImages::ButtonX, 0);

	D3DX11CreateShaderResourceViewFromFileW(pDevice, L"./references/a-pressed.png", &info, pump, &GamepadImages::ButtonA_Pressed, 0);
	D3DX11CreateShaderResourceViewFromFileW(pDevice, L"./references/b-pressed.png", &info, pump, &GamepadImages::ButtonB_Pressed, 0);
	D3DX11CreateShaderResourceViewFromFileW(pDevice, L"./references/y-pressed.png", &info, pump, &GamepadImages::ButtonY_Pressed, 0);
	D3DX11CreateShaderResourceViewFromFileW(pDevice, L"./references/x-pressed.png", &info, pump, &GamepadImages::ButtonX_Pressed, 0);

	D3DX11CreateShaderResourceViewFromFileW(pDevice, L"./references/stick-left.png", &info, pump, &GamepadImages::Stick_Left, 0);
	D3DX11CreateShaderResourceViewFromFileW(pDevice, L"./references/stick-left-pressed.png", &info, pump, &GamepadImages::Stick_Left_Pressed, 0);

	D3DX11CreateShaderResourceViewFromFileW(pDevice, L"./references/stick-right.png", &info, pump, &GamepadImages::Stick_Right, 0);
	D3DX11CreateShaderResourceViewFromFileW(pDevice, L"./references/stick-right-pressed.png", &info, pump, &GamepadImages::Stick_Right_Pressed, 0);

	D3DX11CreateShaderResourceViewFromFileW(pDevice, L"./references/select-pressed.png", &info, pump, &GamepadImages::Select_Pressed, 0);
	D3DX11CreateShaderResourceViewFromFileW(pDevice, L"./references/start-pressed.png", &info, pump, &GamepadImages::Start_Pressed, 0);

	D3DX11CreateShaderResourceViewFromFileW(pDevice, L"./references/dpad-left-pressed.png", &info, pump, &GamepadImages::DPAD_Pressed_Left, 0);
	D3DX11CreateShaderResourceViewFromFileW(pDevice, L"./references/dpad-right-pressed.png", &info, pump, &GamepadImages::DPAD_Pressed_Right, 0);
	D3DX11CreateShaderResourceViewFromFileW(pDevice, L"./references/dpad-down-pressed.png", &info, pump, &GamepadImages::DPAD_Pressed_Down, 0);
	D3DX11CreateShaderResourceViewFromFileW(pDevice, L"./references/dpad-upper-pressed.png", &info, pump, &GamepadImages::DPAD_Pressed_Upper, 0);

	D3DX11CreateShaderResourceViewFromFileW(pDevice, L"./references/trigger-left-pressed.png", &info, pump, &GamepadImages::Trigger_Pressed_Left, 0);
	D3DX11CreateShaderResourceViewFromFileW(pDevice, L"./references/trigger-right-pressed.png", &info, pump, &GamepadImages::Trigger_Pressed_Right, 0);

	D3DX11CreateShaderResourceViewFromFileW(pDevice, L"./references/bumper-left-pressed.png", &info, pump, &GamepadImages::Bumper_Pressed_Left, 0);
	D3DX11CreateShaderResourceViewFromFileW(pDevice, L"./references/bumper-right-pressed.png", &info, pump, &GamepadImages::Bumper_Pressed_Right, 0);
	
	return true;
};