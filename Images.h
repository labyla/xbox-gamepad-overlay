#pragma once
#include <d3d11.h>
#include <D3DX11tex.h>

namespace GamepadImages {
	extern ID3D11ShaderResourceView* Gamepad;

	extern ID3D11ShaderResourceView* ButtonA;
	extern ID3D11ShaderResourceView* ButtonB;
	extern ID3D11ShaderResourceView* ButtonY;
	extern ID3D11ShaderResourceView* ButtonX;

	extern ID3D11ShaderResourceView* ButtonA_Pressed;
	extern ID3D11ShaderResourceView* ButtonB_Pressed;
	extern ID3D11ShaderResourceView* ButtonY_Pressed;
	extern ID3D11ShaderResourceView* ButtonX_Pressed;

	extern ID3D11ShaderResourceView* Stick_Left;
	extern ID3D11ShaderResourceView* Stick_Left_Pressed;

	extern ID3D11ShaderResourceView* Stick_Right;
	extern ID3D11ShaderResourceView* Stick_Right_Pressed;

	extern ID3D11ShaderResourceView* Select_Pressed;
	extern ID3D11ShaderResourceView* Start_Pressed;

	extern ID3D11ShaderResourceView* DPAD_Pressed_Left;
	extern ID3D11ShaderResourceView* DPAD_Pressed_Right;
	extern ID3D11ShaderResourceView* DPAD_Pressed_Down;
	extern ID3D11ShaderResourceView* DPAD_Pressed_Upper;

	extern ID3D11ShaderResourceView* Trigger_Pressed_Left;
	extern ID3D11ShaderResourceView* Trigger_Pressed_Right;

	extern ID3D11ShaderResourceView* Bumper_Pressed_Left;
	extern ID3D11ShaderResourceView* Bumper_Pressed_Right;
}

bool ImagesInitialize(ID3D11Device* pDevice);