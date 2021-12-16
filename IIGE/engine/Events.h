#pragma once

#include <tuple>
#include <array>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include <utils/variadic.h>

#include "graphics/Window.h"
#include "Scene.h"

namespace engine
	{
	class Input_state;

	class Keyboard
		{
		friend class Input_state;
		public:
			class Key
				{
				public:
					using Code = sf::Keyboard::Key;

					Key& operator=(bool pressed) { this->pressed = pressed; }
					bool pressed = false;
					operator bool() const { return pressed; }
					float operator-(const Key& other) const { return ((*this) ? 1.f : 0.f) - (other ? 1.f : 0.f); }
				};

			std::array<Key, Key::Code::KeyCount> keys;

			const Key& operator[](Key::Code keycode) const { return keys[keycode]; }

		private:
			bool register_event(const sf::Event& event)
				{
				switch (event.type)
					{
					case sf::Event::KeyPressed:  keys[event.key.code] = true; break;
					case sf::Event::KeyReleased: keys[event.key.code] = false; break;
					default: return false;
					}
				return true;
				}
		};

	class Mouse
		{
		friend class Input_state;
		public:
			class Button
				{
				public:
					using Code = sf::Mouse::Button;

					Button& operator=(bool pressed) { this->pressed = pressed; }
					bool pressed = false;
					operator bool() const { return pressed; }
					float operator-(const Button& other) const { return ((*this) ? 1.f : 0.f) - (other ? 1.f : 0.f); }
				};

			core::Vec2f position;

			std::array<Button, Button::Code::ButtonCount> buttons;

			const Button& operator[](Button::Code button_code) const { return buttons[button_code]; }

		private:
			bool register_event(const sf::Event& event)
				{
				switch (event.type)
					{
					case sf::Event::MouseButtonPressed:  buttons[event.mouseButton.button] = true;  break;
					case sf::Event::MouseButtonReleased: buttons[event.mouseButton.button] = false; break;
					case sf::Event::MouseMoved:          position = {event.mouseMove.x, event.mouseMove.y}; break;
					default: return false;
					}
				return true;
				}
		};

	template <typename Scene_t, typename ...Input_types>
	class Input_manager;

	class Input_state
		{
		template<typename, typename ...>
		friend class Input_manager;

		public:
			Keyboard keyboard;
			Mouse    mouse;

		private:
			void register_event(const sf::Event event)
				{				
				if (!keyboard.register_event(event))
					{
					mouse.register_event(event);
					}
				}
		};

	class Input_group
		{
		public:
			virtual bool update(const Input_state& input_state) = 0;
		};

	class Input_group_WASD_example : public Input_group
		{
		public:
			float x;
			float y;

			virtual bool update(const Input_state& input_state) override
				{
				float new_x = input_state.keyboard[Keyboard::Key::Code::D] - input_state.keyboard[Keyboard::Key::Code::A];
				float new_y = input_state.keyboard[Keyboard::Key::Code::S] - input_state.keyboard[Keyboard::Key::Code::W];
				if (new_x == x && new_y == y) { return false; } //Event callback won't be called this step
				else { x = new_x; y = new_y; return true; }
				}
		};

	template <typename Scene_t, typename ...Input_types>
	class Input_manager
		{
		public:
			void update(const graphics::Window& window, Scene_t& scene)
				{
				poll_events(window);

				utils::tuple::for_each_in_tuple(input_groups, [&](auto& input_group)
					{
					if (input_group.update(state))
						{
						//for_each_with_input(scene, input_group);
						}
					});
				}

		private:
			Input_state state;
			std::tuple<Input_types...> input_groups;

			void poll_events(const graphics::Window& window)
				{
				sf::Event event;
				while (window.poll_event(event)) { state.register_event(event); }
				}

			template <typename T>
			void for_each_with_input(Scene_t& scene, T input_group)
				{
				//scene.for_
				
				}
		};

	}