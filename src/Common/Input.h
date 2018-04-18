#pragma once
#include <gef.h>
#include <input/input_manager.h>
#include <input/keyboard.h>

/*
	The input class works as a bridge between PC controls and Vita controls,
	 with the added bonus of decreasing code duplication in every class/state
	 where input is needed.
	Enums are mapped to one or more buttons/actions on each device.
*/

namespace gef
{
	class SonyController;
}

class Input
{
	using Button = UInt32;
	using KeyCode = gef::Keyboard::KeyCode;
public:
	Input() = delete;

	enum Key
	{
		ACTION,
		MENU,
		JUMP,
		LEFT,
		RIGHT,
		UP,
		DOWN
	};
	
	static void init(gef::Platform& platform);
	static void update();
	static void cleanUp();
	
	static bool isPressed(Key key);
	static bool isDown(Key key);

	static float leftJoystickX();
	static float leftJoystickY();

private:
	static bool isKeyPressed(KeyCode key);
	static bool isButtonPressed(Button button);

	static bool isKeyDown(KeyCode key);
	static bool isButtonDown(Button button);

	static bool isLeftJoystickUp();
	static bool isLeftJoystickDown();
	static bool isLeftJoystickLeft();
	static bool isLeftJoystickRight();
	
	static gef::InputManager* m_input;
	static gef::Keyboard* m_keyboard;
	static const gef::SonyController* m_controller;
	static float m_deadzone;
	static bool m_isJoystickGesturing;
};