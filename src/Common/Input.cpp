#include "Input.h"

#include <cmath>
#include <input/input_manager.h>
#include <input/keyboard.h>
#include <input/sony_controller_input_manager.h>

gef::InputManager*			Input::m_input					= nullptr;
gef::Keyboard*				Input::m_keyboard				= nullptr;
const gef::SonyController*	Input::m_controller				= nullptr;
float						Input::m_deadzone				= .3f;
bool						Input::m_isJoystickGesturing	= false;

bool Input::isPressed(Key key)
{
	switch (key)
	{
	case Key::ACTION:
		return isKeyPressed(KeyCode::KC_RETURN) || isButtonPressed(gef_SONY_CTRL_CROSS);
	case Key::MENU:
		return isKeyPressed(KeyCode::KC_ESCAPE) || isButtonPressed(gef_SONY_CTRL_START);
	case Key::JUMP:
		return isKeyPressed(KeyCode::KC_SPACE) || isButtonPressed(gef_SONY_CTRL_CROSS);
	case Key::DOWN:
		return isKeyPressed(KeyCode::KC_DOWN) || isButtonPressed(gef_SONY_CTRL_DOWN) || isLeftJoystickDown();
	case Key::UP:
		return isKeyPressed(KeyCode::KC_UP) || isButtonPressed(gef_SONY_CTRL_UP) || isLeftJoystickUp();
	case Key::RIGHT:
		return isKeyPressed(KeyCode::KC_RIGHT) || isKeyPressed(KeyCode::KC_D) || isButtonPressed(gef_SONY_CTRL_RIGHT) || isLeftJoystickRight();
	case Key::LEFT:
		return isKeyPressed(KeyCode::KC_LEFT) || isKeyPressed(KeyCode::KC_A) || isButtonPressed(gef_SONY_CTRL_LEFT) || isLeftJoystickLeft();
	}
	return false;
}

bool Input::isDown(Key key)
{
	switch (key)
	{
	case Key::ACTION:
		return isKeyDown(KeyCode::KC_RETURN) || isButtonDown(gef_SONY_CTRL_CROSS);
	case Key::MENU:
		return isKeyDown(KeyCode::KC_ESCAPE) || isButtonDown(gef_SONY_CTRL_START);
	case Key::JUMP:
		return isKeyDown(KeyCode::KC_SPACE) || isButtonDown(gef_SONY_CTRL_CROSS);
	case Key::DOWN:
		return isKeyDown(KeyCode::KC_DOWN) || isButtonDown(gef_SONY_CTRL_DOWN);
	case Key::UP:
		return isKeyDown(KeyCode::KC_UP) || isButtonDown(gef_SONY_CTRL_UP);
	case Key::RIGHT:
		return isKeyDown(KeyCode::KC_RIGHT) || isKeyDown(KeyCode::KC_D) || isButtonDown(gef_SONY_CTRL_RIGHT);
	case Key::LEFT:
		return isKeyDown(KeyCode::KC_LEFT) || isKeyDown(KeyCode::KC_A) || isButtonDown(gef_SONY_CTRL_LEFT);
	}
	return false;
}

void Input::init(gef::Platform& platform)
{
	if (m_input)
		return;

	m_input = gef::InputManager::Create(platform);

	if (!m_input)
		return;

	gef::SonyControllerInputManager* m_controllerManager = m_input->controller_input();
	if (m_controllerManager)
		m_controller = m_controllerManager->GetController(0);

	m_keyboard = m_input->keyboard();
}

void Input::update()
{
	m_input->Update();
}

void Input::cleanUp()
{
	delete m_input;
	m_input = nullptr;
}

bool Input::isKeyPressed(KeyCode key)
{
	if (!m_keyboard)
		return false;

	return m_keyboard->IsKeyPressed(key);
}

bool Input::isButtonPressed(Button button)
{
	if (!m_controller)
		return false;

	return (m_controller->buttons_pressed() & button) != 0;
}

bool Input::isKeyDown(KeyCode key)
{
	if (!m_keyboard)
		return false;

	return m_keyboard->IsKeyDown(key);
}

bool Input::isButtonDown(Button button)
{
	if (!m_controller)
		return false;

	return (m_controller->buttons_down() & button) != 0;
}

bool Input::isLeftJoystickUp()
{
	if (!m_controller)
		return false;

	if (m_controller->left_stick_y_axis() < -m_deadzone)
	{
		if (!m_isJoystickGesturing)
		{
			m_isJoystickGesturing = true;
			return true;
		}
	}

	if (std::fabsf(m_controller->left_stick_y_axis()) < m_deadzone)
		m_isJoystickGesturing = false;

	return false;
}

bool Input::isLeftJoystickDown()
{
	if (!m_controller)
		return false;

	if (m_controller->left_stick_y_axis() > m_deadzone)
	{
		if (!m_isJoystickGesturing)
		{
			m_isJoystickGesturing = true;
			return true;
		}
	}

	if (std::fabsf(m_controller->left_stick_y_axis()) < m_deadzone)
		m_isJoystickGesturing = false;

	return false;
}

bool Input::isLeftJoystickLeft()
{
	if (!m_controller)
		return false;

	if (m_controller->left_stick_x_axis() < -m_deadzone)
	{
		if (!m_isJoystickGesturing)
		{
			m_isJoystickGesturing = true;
			return true;
		}
	}

	if (std::fabsf(m_controller->left_stick_x_axis()) < m_deadzone)
		m_isJoystickGesturing = false;

	return false;
}

bool Input::isLeftJoystickRight()
{
	if (!m_controller)
		return false;

	if (m_controller->left_stick_x_axis() > m_deadzone)
	{
		if (!m_isJoystickGesturing)
		{
			m_isJoystickGesturing = true;
			return true;
		}
	}

	if (std::fabsf(m_controller->left_stick_x_axis()) < m_deadzone)
		m_isJoystickGesturing = false;

	return false;
}

float Input::leftJoystickX()
{
	if (!m_controller)
		return 0.f;

	return m_controller->left_stick_x_axis();
}

float Input::leftJoystickY()
{
	if (!m_controller)
		return 0.f;

	return m_controller->left_stick_y_axis();
}
