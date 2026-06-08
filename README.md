# Xbox Gamepad Overlay

Xbox Gamepad Overlay is a Windows desktop overlay that visualizes Xbox-compatible controller input in real time. It renders a transparent, always-on-top DirectX 11 overlay with button states, analog stick movement, trigger pressure, and simple vibration test controls.

## Demo

[![Xbox Gamepad Overlay demo](assets/xbox-gamepad-overlay-preview.gif)](https://www.youtube.com/watch?v=0wuJJN87KQA)

Watch the full demo on [YouTube](https://www.youtube.com/watch?v=0wuJJN87KQA).

## Features

- Real-time Xbox controller visualization through XInput.
- Transparent topmost overlay rendered with Dear ImGui and DirectX 11.
- Button, D-pad, bumper, trigger, and stick state display.
- Left and right motor vibration test sliders.
- Runtime resources are copied next to the executable during the CMake build.

## Requirements

This project is Windows-only.

Install the following tools before building:

1. **Visual Studio 2022** or **Visual Studio Build Tools 2022**
   - Install the `Desktop development with C++` workload.
   - Make sure the MSVC compiler and Windows SDK components are selected.
2. **CMake 3.20 or newer**
   - Download it from <https://cmake.org/download/> or install it with WinGet:

   ```powershell
   winget install Kitware.CMake
   ```

3. **Git**
   - Required if you want to clone the repository from the command line.

   ```powershell
   winget install Git.Git
   ```

4. **DirectX runtime support**
   - The project vendors its D3DX11 headers and import library under `project/third_party/d3dx11`.
   - If the built executable fails to start because `D3DX11_43.dll` is missing, install the Microsoft DirectX End-User Runtime.

## Source Layout

```text
assets/                    Demo media used by this README
project/
  CMakeLists.txt           Main CMake project
  app/                     Overlay application source and resources
  third_party/             Vendored ImGui and D3DX11 dependencies
```

## Build Tutorial

Run the following commands from the repository root.

Create the build directory and generate the build system. This step only needs to be done once:

```powershell
cmake -S project -B build
```

Build the Release configuration:

```powershell
cmake --build build --config Release
```

After a successful Visual Studio generator build, the executable is typically located at:

```text
build/app/Release/xbox-gamepad-overlay.exe
```

The build also copies the required `references/` and `fonts/` resource directories next to the executable.

## Running

1. Connect an Xbox-compatible controller.
2. Run `xbox-gamepad-overlay.exe`.
3. Press `1 + 2` to show or hide the overlay.
4. Use the vibration sliders to test the left and right controller motors.

If no controller is connected, the overlay displays a `Gamepad is not connected` message.

## Development Notes

- The application target is defined in `project/app/CMakeLists.txt`.
- Dear ImGui is built from the vendored sources in `project/third_party/imgui`.
- Controller input and vibration are handled through Windows XInput.
- Rendering is implemented with DirectX 11 and the ImGui Win32/DX11 backends.
