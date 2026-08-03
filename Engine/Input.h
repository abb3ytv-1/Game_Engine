#pragma once
#include "Vector2.h"
#include <vector>

namespace nu {
	class Input {
	public:
		enum class MouseButton{
			Left = 1,
			Middle = 2,
			Right = 3
		};

	public:
		bool Initialize();
		void Shutdown();

		void Update();

		bool GetKeyDown(int key) const {return a_keyStates[key];}
		bool GetPrevKeyDown(int key) const {return a_prevKeyStates[key];}
		bool GetKeyPress(int key) const {return !a_prevKeyStates[key] && a_keyStates[key];}
		bool GetKeyReleased(int key) const {return a_prevKeyStates[key] && !a_keyStates[key];}

		bool GetMouseDown(MouseButton button) const { return a_buttonStates & GetButtonBit(button); }
		bool GetPrevMouseDown(MouseButton button) const { return a_prevButtonStates & GetButtonBit(button); }
		bool GetButtonPressed(MouseButton button) const { return !GetPrevMouseDown(button) && GetMouseDown(button); }
		bool GetButtonReleased(MouseButton button) const { return GetPrevMouseDown(button) && !GetMouseDown(button); }

		Vector2 GetMousePosition() { return a_mousePosition; }

	private:
		uint32_t GetButtonBit(MouseButton button) const;

	private:
		// keyboard
		std::vector<bool> a_keyStates;
		std::vector<bool> a_prevKeyStates;

		// mouse
		uint32_t a_buttonStates = 0;
		uint32_t a_prevButtonStates = 0;

		Vector2 a_mousePosition{ 0, 0 };
	};
}