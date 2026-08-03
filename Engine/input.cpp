#include "pch.h"
#include "Input.h"

#include <SDL3/SDL.h>
#include <algorithm>

namespace nu {

    bool Input::Initialize()
    {
        int numKeys;
        const bool* keyState = SDL_GetKeyboardState(&numKeys);

        a_keyStates.resize(numKeys);
        a_prevKeyStates.resize(numKeys);

        std::copy(
            keyState,
            keyState + numKeys,
            a_keyStates.begin()
        );

        a_prevKeyStates = a_keyStates;

        return true;
    }


    void Input::Shutdown()
    {
        //
    }


    void Input::Update()
    {
        // Save previous keyboard state
        a_prevKeyStates = a_keyStates;

        // Get current keyboard state
        const bool* keyState = SDL_GetKeyboardState(nullptr);

        std::copy(
            keyState,
            keyState + a_keyStates.size(),
            a_keyStates.begin()
        );


        // Save previous mouse buttons
        a_prevButtonStates = a_buttonStates;

        // Get current mouse buttons
        a_buttonStates = SDL_GetMouseState(nullptr, nullptr);


        // Mouse position
        float mouseX;
        float mouseY;

        SDL_GetMouseState(&mouseX, &mouseY);

        a_mousePosition.x = mouseX;
        a_mousePosition.y = mouseY;
    }


    uint32_t Input::GetButtonBit(MouseButton button) const
    {
        return SDL_BUTTON_MASK((uint32_t)button);
    }

}