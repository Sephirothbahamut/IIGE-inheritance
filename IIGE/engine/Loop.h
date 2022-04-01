#pragma once
#include <utils/logger.h>

#include <SFML/System.hpp>

#include "graphics/Window.h"
#include "Scene.h"
#include "Object.h"

namespace engine
	{
	template <typename Scene_t>
	class Loop
		{
		private:
			static constexpr bool log_enabled = true;

			Scene_t* scene{nullptr};

			graphics::Window* window{nullptr};

			const float steps_per_second = 1.f;
			const sf::Time fixed_delta_time{sf::seconds(1.f / steps_per_second)};
			const size_t max_frameskip = 5;


		public:
			Loop(Scene_t& scene, graphics::Window& window, float steps_per_second = 1.f) noexcept : scene(&scene), window(&window), steps_per_second(steps_per_second)
				{}

			void run()
				{
				// https://dewitters.com/dewitters-gameloop/
				Scene_t& scene = *this->scene;
				graphics::Window& window = *this->window;

				sf::Clock clock;
				sf::Time next_step_time = clock.getElapsedTime();
				size_t step_loops = 0;
				float interpolation = 0;

				sf::Clock fps_clock;
				uint32_t frames_counter = 0;

				while (window.is_open())
					{
					if (fps_clock.getElapsedTime() > sf::seconds(1))
						{
						utils::globals::logger.log("FPS: " + std::to_string(frames_counter / fps_clock.restart().asSeconds()) + " with #" + std::to_string(scene.active_objects_count()) + " active objects.");
						frames_counter = 0;
						}
					while (clock.getElapsedTime() > next_step_time && step_loops < max_frameskip)
						{sf::Event event;
						while (window.poll_event(event)) {}

						scene.update();
						scene.movement_step();
						scene.collisions();

						scene.step();

						next_step_time += fixed_delta_time;
						}

					interpolation = (clock.getElapsedTime() + fixed_delta_time - next_step_time) / fixed_delta_time;

					frames_counter++;

					//window.sf_window.clear();
					scene.draw(window, interpolation);
					//window.sf_window.display();
					}
				}
		};
	}