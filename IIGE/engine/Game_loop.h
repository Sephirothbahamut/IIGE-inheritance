#pragma once

#include <vector>

#include <SFML/System.hpp>

#include "Object.h"
#include "graphics/Window.h"

namespace engine
	{
	class Game_state
		{
		private:
			std::vector<Object*> objects;

		public:
		};

	class Game_loop
		{
		private:
			engine::graphics::Window* window;
			engine::Game_state state;

			bool running{true};
			size_t time_per_step;
			sf::Time fixed_delta_time = sf::seconds(1.f / 60.f);

			void fixed_time_step()
				{
				sf::Time time;

				while (running)
					{
					}
				}


		public:
			Game_loop(engine::graphics::Window& window) : window(&window)
				{

				}
		};
	}