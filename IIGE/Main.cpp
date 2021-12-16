
#include <vector>

#include "utils/variadic.h"
#include "utils/containers/Matrix.h"
#include "utils/definitions.h"
#include "utils/logger.h"

#include "engine/graphics/Window.h"
#include "engine/Scene.h"
#include "engine/Loop.h"
#include "engine/resources_manager.h"


namespace iige { using namespace engine; using namespace engine::core; using namespace engine::objects; }

const float steps_per_second = 5.f;

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
			size_t step_count = 0;
			size_t spawn_step;
			size_t spawn_count;
			sf::RectangleShape cs;
			Scene_t* scene;
		public:
			Dummy(Scene_t& scene, iige::Transform2 transform, sf::Color color, size_t spawn_step, size_t spawn_count)
				: scene(&scene), spawn_step(spawn_step), spawn_count(spawn_count)
				{
				//Movement
				this->transform = transform; //assign starting position
				movement.position = iige::Vec2f{10.f, 0.f} / steps_per_second; //assign some speed

				using namespace utils::angle::literals;
				movement.orientation = 40_deg / steps_per_second;

				//Collisions
				//collider_ptr = utils::make_polymorphic_value<iige::collisions::shapes::Circle>(iige::core::Vec2f{}, 3.f);
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
				using namespace utils::angle::literals;

				movement.position += (iige::Vec2f{0.f, 1.f} / (steps_per_second * steps_per_second)); //acceleration per step
				if (step_count < spawn_step * 2) { step_count++; }
				
				if(step_count == spawn_step)
					{
					utils::tracking_ptr<Dummy> new_dummy = scene->create<Dummy>(std::ref(*scene), transform, cs.getFillColor(), spawn_step, spawn_count - 1);
					new_dummy->movement = movement.composite(engine::core::Transform2{{}, 30_deg, {}});
					}
				if (step_count == spawn_step * 2) { movement = engine::core::Transform2::zero(); }
				}

			virtual void draw(sf::RenderTarget& rt, float interpolation) override
				{
				iige::Transform2 interpolated = iige::Transform2::lerp(transform_previous, transform, interpolation);

				cs.setPosition(interpolated.position.x, interpolated.position.y);
				cs.setRotation(interpolated.orientation.value);
				rt.draw(cs);
				}
		};

	class Wall_circ : public iige::Object, public iige::In_world/*, public iige::Has_collision*/, public iige::Draw
		{
		private:
			sf::CircleShape circ;

		public:
			Wall_circ(iige::Vec2f pos, float radius) : circ(radius)
				{
				transform.position = pos;
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

		scene.create<example::Wall_circ>(iige::Vec2f{580, 380}, 10.f);

		using namespace utils::literals;

		for (size_t i = 0; i < 100; i++)
			{
			scene.create<example::Dummy>(std::ref(scene), iige::Transform2{{500.f, 350.f}}, sf::Color::Green, 10_size, 3_size);
			scene.create<example::Dummy>(std::ref(scene), iige::Transform2{{ 10.f,  20.f}}, sf::Color::Yellow, 10_size, 3_size);
			scene.create<example::Dummy>(std::ref(scene), iige::Transform2{{  0.f,  20.f}}, sf::Color::Cyan, 10_size, 3_size);
			}
		scene.update();

		engine::Loop loop(scene, window, steps_per_second);
		loop.run();
		}
	catch (const std::exception& e) { utils::globals::logger.err(e.what()); }

	return 0;
	}