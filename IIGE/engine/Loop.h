#pragma once
#include <thread>

#include <SFML/System.hpp>

#include "graphics/Window.h"
#include "Scene.h"
#include "Object.h"
#include "Logger.h"

namespace engine
	{
	class Loop
		{
		private:
			static constexpr bool log_enabled = true;

			Scene* scene{nullptr};
			graphics::Window* window{nullptr};

			const float steps_per_second = 1.f;
			const sf::Time fixed_delta_time{sf::seconds(1.f / steps_per_second)};
			const size_t max_frameskip = 5;

		public:
			Loop(Scene& scene, graphics::Window& window, float steps_per_second = 1.f) noexcept : scene(&scene), window(&window), steps_per_second(steps_per_second)
				{}


			void run() 
				{
				// https://dewitters.com/dewitters-gameloop/
				Scene& scene = *this->scene;
				graphics::Window& window = *this->window;

				sf::Clock clock;
				sf::Time next_step_time = clock.getElapsedTime();
				size_t step_loops = 0;
				float interpolation = 0;

				//TODO temporary fps counter, do better
				/*std::atomic_uint64_t fps = 0;
				std::atomic_bool fps_counter_running = true;
				std::thread fps_counter_thread([&]()
					{
					while (fps_counter_running)
						{
						sf::sleep(sf::seconds(1)); 
						uint64_t tmpfps = fps; fps = 0; //TODO i know it's wrong, i dont care if i lose a couple frames counted once every meteor that hits the planet
						logger << Message(std::to_string(tmpfps));
						}
					});*/

				while (window.is_open())
					{
					step_loops = 0;

					while (clock.getElapsedTime() > next_step_time && step_loops < max_frameskip)
						{
						sf::Event event;
						while (window.poll_event(event)) {}

						scene.movement_step();
						scene.collisions();
						scene.step();

						next_step_time += fixed_delta_time;
						//step_loops++;
						}

					interpolation = (clock.getElapsedTime() + fixed_delta_time - next_step_time) / fixed_delta_time;

					scene.draw(window, interpolation);
					//fps++;
					}

				//fps_counter_running = false;
				//fps_counter_thread.join();
				}
		};
	}