
#include <vector>
#include <array>
#include <iostream>

#include "utils/variadic.h"
#include "utils/containers/Matrix.h"
#include "utils/logger.h"

#include "engine/graphics/Window.h"
#include "engine/Scene.h"
#include "engine/Loop.h"
#include "engine/resources_manager.h"

namespace iige { using namespace engine; /*using namespace engine::core;*/ using namespace engine::objects; using namespace utils::math; }

constexpr float steps_per_second = 1.f;
constexpr float seconds_per_step = 1 / steps_per_second;

// List the types that will exist in the scene
namespace example { class Dummy; class Wall_circ; }

// Prepare a scene type with all the object types it may contain during playtime
using Scene_t = engine::Scene<example::Dummy, example::Wall_circ>;

// Prepare some example classes
namespace example
	{
	// Use a readable index name for collision layers
	enum Collider_layer : size_t { Walls, SIZE };

	class Dummy : public iige::Object, public iige::Move, public iige::Step, public iige::Draw//, public iige::Collide_discrete
		{
		private:
			float spawn_seconds;
			size_t step_count = 0;
			size_t spawn_step;
			size_t spawn_count;
			sf::RectangleShape cs;
			iige::transform2 interpolated;

			std::array<uint8_t, 2048> arr;

			Scene_t* scene;
		public:
			Dummy(Scene_t& scene, iige::transform2 transform, sf::Color color, float spawn_seconds, size_t spawn_count)
				: scene{&scene}, spawn_seconds{spawn_seconds}, spawn_step{static_cast<size_t>(spawn_seconds * seconds_per_step)}, spawn_count{spawn_count}
				{
				//Movement
				this->transform = transform; //assign starting position
				movement.translation = iige::vec2f{10.f, 0.f} / steps_per_second; //assign some speed

				using namespace utils::math::angle::literals;
				movement.rotation = 40_deg / steps_per_second;

				//Collisions
				//collider_ptr = utils::make_polymorphic_value<utils::math::geometry::shapes::Circle>(iige::core::Vec2f{}, 3.f);
				//collider_ptr->origin = {10.f, 0.f};

				//Draw
				cs.setSize({15.f, 8.f});
				cs.setOrigin(cs.getSize().x, cs.getSize().y);
				cs.setFillColor(color);
				}

			Dummy(const Dummy& copy) = default;
			Dummy& operator=(const Dummy&) = default;

			Dummy(Dummy&& move) noexcept = default;
			Dummy& operator=(Dummy&& move) noexcept = default;

			virtual void step() override
				{
				using namespace utils::math::angle::literals;

				movement.translation += (iige::vec2f{0.f, 1.f} / (steps_per_second * steps_per_second)); //acceleration per step
				if (step_count < spawn_step * 2) { step_count++; }
				
				if(step_count == spawn_step)
					{
					utils::tracking_ptr<Dummy> new_dummy = scene->create<Dummy>(std::ref(*scene), transform, cs.getFillColor(), spawn_seconds, spawn_count - 1);
					new_dummy->movement = movement.composite(utils::math::transform2{{}, 30_deg, {}});
					}
				if (step_count == spawn_step * 2) { movement = utils::math::transform2::zero(); }
				}

			virtual void draw(sf::RenderTarget& rt, float interpolation) override
				{
				interpolated = utils::math::lerp(transform_previous, transform, interpolation);

				cs.setPosition(interpolated.translation.x, interpolated.translation.y);
				cs.setRotation(interpolated.rotation.value);


				rt.draw(cs);
				}
		};

	class Wall_circ : public iige::Object, public iige::In_world/*, public iige::Has_collision*/, public iige::Draw
		{
		private:
			sf::CircleShape circ;

		public:
			Wall_circ(iige::vec2f pos, float radius) : circ(radius)
				{
				transform.translation = pos;
				circ.setPosition(pos.x, pos.y);

				//collider_ptr = std::make_unique<iige::collisions::shapes::Circle>(iige::core::Vec2f{}, radius);
				}

			virtual void draw(sf::RenderTarget& rt, float interpolation) override
				{
				rt.draw(circ);
				}
		};
	}


int main()
	{
	try
		{
		engine::graphics::Window window(1024, 768, "Hello World"); 
		engine::Resources_manager resman;
		Scene_t scene;

		//Loading resources returns an uncopyable but moveable RAII container.
		auto fireball_text = resman.add_texture("data/textures/fireball_0.png");

		scene.create<example::Wall_circ>(iige::vec2f{580, 380}, 10.f);

		using namespace utils::math::angle::literals;

		for (size_t i = 0; i < 1000; i++)
			{
			scene.create<example::Dummy>(std::ref(scene), iige::transform2{{500.f, 350.f}}, sf::Color::Green,  2.f, size_t{3});
			scene.create<example::Dummy>(std::ref(scene), iige::transform2{{ 10.f,  20.f}}, sf::Color::Yellow, 2.f, size_t{3});
			scene.create<example::Dummy>(std::ref(scene), iige::transform2{{  0.f,  20.f}}, sf::Color::Cyan,   2.f, size_t{3});
			}

		utils::math::transform2 t{.translation{0.f, 0.f}, .rotation{}};

		scene.update();

		engine::Loop loop{scene, window, steps_per_second};
		loop.run();
		}
	catch (const std::exception& e) { utils::globals::logger.err(e.what()); }

	return 0;
	}