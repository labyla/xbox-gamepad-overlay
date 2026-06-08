#include "hotkey_input.h"

std::string GetHotkeyInputText() {
	std::string text = "";

	for (int key = 1; key < 256; key++) {
		if (GetAsyncKeyState(key) & 0x8000) {
			char buffer[256];
			if (GetKeyNameTextA(MapVirtualKeyA(key, 0) << 16, buffer, 256)) {
				text += buffer;
				text += " + ";
			}
		}
	}

	if (!text.empty()) {
		text = text.substr(0, text.length() - 3);
	}

	std::cout << text << std::endl;

	return text;
}