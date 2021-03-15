#pragma once

#include <optional>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "../compile_settings.h"
#include "../../utils/memory.h"
#include "../Logger.h"



namespace engine::graphics
	{
	class Window
		{
		private:
			static constexpr bool log_enabled = false;
			//sf::RenderWindow sf_window;

		public:
			sf::RenderWindow sf_window;
			class Initialize
				{
				public:
					Initialize() {};
					~Initialize() {}
				};

			Window(unsigned int width = 800, unsigned int height = 600, const char* window_name = "Window") : sf_window(sf::VideoMode{width, height}, window_name, sf::Style::Resize, sf::ContextSettings{0, 0, 8})
				{

				if constexpr (log_enabled) { engine::logger << Message::log("SFML Window created."); }
				}

			Window(const Window& copy) = delete;
			Window& operator=(const Window& copy) = delete;
			Window(Window&& move) noexcept = default;
			Window& operator=(Window&& move) = default;

			bool poll_event(sf::Event& event)
				{
				bool ret = sf_window.pollEvent(event);
				if(ret)
					{
					switch (event.type)
						{
						case sf::Event::EventType::Closed: close();
						}
					}
				return ret;
				}
			void close() { sf_window.close(); }
			bool is_open() { return sf_window.isOpen(); }
		};
	}