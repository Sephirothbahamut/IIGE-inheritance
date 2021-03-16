
#include "utils/containers/Matrix.h"
#include "utils/definitions.h"
#include <vector>
#include "engine/graphics/Window.h"
#include "engine/Scene.h"
#include "engine/Loop.h"

#include "utils/variadic.h"
#include "engine/logger.h"

namespace iige { using namespace engine; using namespace engine::core; using namespace engine::objects; }


namespace example
	{
	enum Collider_layer : size_t { Walls, SIZE };

	class Dummy : public virtual iige::Object, public iige::Move, public iige::Step, public iige::Draw, iige::Collide_discrete
		{
		private:
			size_t stop_moving;
			size_t stop_drawing;
			size_t stops_countdown = 6;
			sf::RectangleShape cs;

		public:
			Dummy(
				/*Engine required argument*/
				iige::Scene& scene, 
				/*free user arguments*/
				iige::Transform2 transform, sf::Color color, float z, size_t stops_countdown, size_t stop_moving, size_t stop_drawing
			)
				:
				/*Engine required initializations*/
				iige::Object(scene), iige::In_world(scene), iige::Move(scene), iige::Step(scene), iige::Draw(scene, z), 
				iige::Has_collision(scene), iige::Collide_discrete(scene),
				/*free user initializations*/ 
				stops_countdown(stops_countdown), stop_moving(stop_moving), stop_drawing(stop_drawing)
				{
				this->transform = transform; //assign starting position
				movement.position = {10.f, 0.f}; //assign some speed

				using namespace utils::angle::literals;
				movement.orientation = 10_deg;

				cs.setSize({15.f, 8.f});
				cs.setOrigin(cs.getSize().x / 2, cs.getSize().y / 2);
				cs.setFillColor(color);

				collider_ptr = std::make_unique<iige::collisions::shapes::Circle>(iige::core::Vec2f{}, 3);

				iige::Collide_discrete::collisions.push_back
					(
					iige::collisions::Collision_discrete
						{
						Collider_layer::Walls, 
						[](iige::collisions::Other other) { iige::logger << utils::message("FIRST COLLISION OF IIGEv2 DETECTED"); return false; }
						}
					);
				}


			virtual void step() override
				{
				movement.position += {0.f, 1.f};

				//iige::logger << iige::utils::message::wrn("Object stepping");
				if (stops_countdown == stop_moving)
					{
					//iige::logger << iige::utils::message::wrn("Stop moving!!!");
					//Move::disable();
					}
				if (stops_countdown == stop_drawing)
					{
					//iige::logger << iige::utils::message::wrn("Stop drawing!!!");
					//Draw::disable();
					}
				if (stops_countdown == 0)
					{
					//iige::logger << iige::utils::message::wrn("Stop stepping!!!");
					//Step::disable();
					}
				stops_countdown--;
				}

			virtual void draw(sf::RenderTarget& rt, float interpolation) override
				{
				iige::Transform2 interpolated = iige::Transform2::lerp(transform_previous, transform, interpolation);

				cs.setPosition(interpolated.position.x, interpolated.position.y);
				cs.setRotation(interpolated.orientation.value);
				rt.draw(cs);
				}
		};

	class Wall_circ : public iige::Has_collision, public iige::Draw
		{
		private:
			sf::CircleShape circ;

		public:
			Wall_circ(iige::Scene& scene, iige::Vec2f pos, float radius) : iige::Object(scene), iige::In_world(scene), iige::Has_collision(scene), iige::Draw(scene), circ(radius)
				{
				transform.position = pos;
				circ.setPosition(pos.x, pos.y);

				collider_ptr = std::make_unique<iige::collisions::shapes::Circle>(iige::core::Vec2f{}, radius);
				Has_collision::enable(Collider_layer::Walls);
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
		engine::Scene scene;
		scene.set_collider_lists(example::Collider_layer::SIZE);

		scene.create<example::Wall_circ>(iige::Vec2f{580, 380}, 10);

		scene.create<example::Dummy>(iige::Transform2{{500.f, 350.f}}, sf::Color::Green, 1, 10, 6, 3);
		//scene.create<example::Dummy>(iige::Transform2{{10.f, 20.f}}, sf::Color::Yellow, 0, 10, 3, 2);
		//scene.create<example::Dummy>(iige::Transform2{{ 0.f, 20.f}}, sf::Color::Cyan,   2, 10, 5, 1);
		for (size_t i = 0; i < 5800; i++)
			{
			//scene.create<example::Dummy>(iige::Transform2{{ 0.f, 20.f}}, sf::Color::Cyan, 2, 10, 5, 1);
			}

		engine::Loop loop(scene, window);
		loop.run();
		}
	catch (const std::exception& e) { engine::logger.emplace(utils::message::Type::err, e.what()); }

	return 0;
	}