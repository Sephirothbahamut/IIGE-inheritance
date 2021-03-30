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
	class Key
		{
		public:
			using Code = sf::Keyboard::Key;
			enum class State { up, pressed, down, released };

			Key& operator=(State state) { this->state = state; }
			State state;
			operator bool() const { return state == State::pressed || state == State::down; }
			float operator-(const Key& other) const { return ((*this) ? 1.f : 0.f) - (other ? 1.f : 0.f); }
		};

	class Keyboard
		{
		public:
			std::array<Key, Key::Code::KeyCount> keys;
			std::vector<Key::Code> pressed, released;

			const Key& operator[](Key::Code keycode) const { return keys[keycode]; }

			void pre_update()
				{
				for (const auto& keycode : pressed) { keys[keycode] = Key::State::down; }
				for (const auto& keycode : released) { keys[keycode] = Key::State::up; }
				}

			void register_event(const sf::Event& event)
				{
				switch (event.type)
					{
					case sf::Event::KeyPressed:
						keys[event.key.code] = Key::State::pressed; pressed.push_back(event.key.code);
						break;
					case sf::Event::KeyReleased:
						keys[event.key.code] = Key::State::released; released.push_back(event.key.code);
						break;
					}
				}

		};


	class Input_state
		{
		public:
			Keyboard keyboard;

			void pre_update() { keyboard.pre_update(); }
			void register_event(const sf::Event event)
				{				
				switch (event.type)
					{
					case sf::Event::KeyPressed: case sf::Event::KeyReleased: keyboard.register_event(event);
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
				float new_x = input_state.keyboard[Key::Code::D] - input_state.keyboard[Key::Code::A];
				float new_y = input_state.keyboard[Key::Code::S] - input_state.keyboard[Key::Code::W];
				if (new_x == x && new_y == y) { return false; } //Event callback won't be called this step
				else { x = new_x; y = new_y; return true; }
				}
		};

	template <typename ...Input_types>
	class Input_manager
		{

		public:
			void update(const graphics::Window& window, Scene& scene)
				{
				poll_events(window);

				utils::tuple::for_each_in_tuple(input_groups, [&](Input_group& input_group)
					{
					input_group.update(state);

					//TODO
					scene.for_each_of_type<Input<type_of_current_tuple_element>>(&Input<type_of_current_tuple_element>::input);
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
		};

	}