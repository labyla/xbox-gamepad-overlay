add_executable(xbox-gamepad-overlay
    src/main.cpp
    src/gamepad.cpp
    src/hotkey_input.cpp
    src/images.cpp
)

target_include_directories(xbox-gamepad-overlay PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(xbox-gamepad-overlay PRIVATE
    imgui
    d3dx11
    Xinput
)

set(APP_RESOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/resources)

add_custom_command(TARGET xbox-gamepad-overlay POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${APP_RESOURCES_DIR}/references
        $<TARGET_FILE_DIR:xbox-gamepad-overlay>/references
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${APP_RESOURCES_DIR}/fonts
        $<TARGET_FILE_DIR:xbox-gamepad-overlay>/fonts
    COMMENT "Copying application resources"
)
