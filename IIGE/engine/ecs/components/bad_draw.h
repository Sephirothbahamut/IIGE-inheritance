#pragma once

#include <SFML/Graphics.hpp>

namespace iige::ecs::components
	{
	struct bad_draw
		{
		bad_draw(float size) : cs{{size, size}} {}

		sf::RectangleShape cs;
		};
	}