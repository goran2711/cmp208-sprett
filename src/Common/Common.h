#pragma once
#include <gef.h>
#include <array>

/*
	This file contains variables and types that are commonly used
	throughout the program.
*/

constexpr float WINDOW_WIDTH = 1280.f;
constexpr float WINDOW_HEIGHT = 720.f;

constexpr float ASPECT_RATIO = WINDOW_WIDTH / WINDOW_HEIGHT;

constexpr float CENTER_X = WINDOW_WIDTH / 2.f;
constexpr float CENTER_Y = WINDOW_HEIGHT / 2.f;

constexpr UInt32 UI_DEFAULT_COLOUR_1	= 0xFF507D9A;
constexpr UInt32 UI_HIGHLIGHT_COLOUR_1	= 0xFF7B9DB3;
constexpr UInt32 UI_DEFAULT_COLOUR_2	= 0xFF655DBD;
constexpr UInt32 UI_HIGHLIGHT_COLOUR_2	= 0xFF8A83CF;

constexpr unsigned NUM_HIGHSCORES = 10u;

using Scoreboard = std::array<float, NUM_HIGHSCORES>;