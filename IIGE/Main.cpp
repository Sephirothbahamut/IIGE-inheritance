
#include <vector>

#include "utils/variadic.h"
#include "utils/containers/Matrix.h"
#include "utils/definitions.h"

#include "engine/logger.h"
#include "engine/graphics/Window.h"
#include "engine/Scene.h"
#include "engine/Loop.h"
#include "engine/resources_manager.h"


namespace iige { using namespace engine; using namespace engine::core; using namespace engine::objects; }

const float steps_per_second = 5.f;

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

				//Movement
				this->transform = transform; //assign starting position
				movement.position = iige::Vec2f{10.f, 0.f} / steps_per_second; //assign some speed

				using namespace utils::angle::literals;
				movement.orientation = 40_deg / steps_per_second;

				//Collisions
				collider_ptr = std::make_unique<iige::collisions::shapes::Circle>(iige::core::Vec2f{}, 3.f);
				collider_ptr->origin = {10.f, 0.f};

				iige::Collide_discrete::collisions.push_back
					(
					iige::collisions::Collision_discrete
						{
						Collider_layer::Walls, 
						[](iige::collisions::Other other) { iige::logger << utils::message("FIRST COLLISION OF IIGEv2 DETECTED"); return false; }
						}
					);


				//Draw
				cs.setSize({15.f, 8.f});
				cs.setOrigin(cs.getSize().x, cs.getSize().y);
				cs.setFillColor(color);
				}


			virtual void step() override
				{
				movement.position += (iige::Vec2f{0.f, 1.f} / (steps_per_second * steps_per_second));

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
				Has_collision::add_to_list(Collider_layer::Walls);
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
		engine::Resources_manager resman;

		//Loading resources returns an uncopyable but moveable RAII container.
		auto fireball_text = resman.add_texture("data/textures/fireball_0.png"); //Compile time error if the return value is not used


		scene.set_collider_lists(example::Collider_layer::SIZE);

		scene.create<example::Wall_circ>(iige::Vec2f{580, 380}, 10.f);

		scene.create<example::Dummy>(iige::Transform2{{500.f, 350.f}}, sf::Color::Green, 1.f, 10, 6, 3);
		//scene.create<example::Dummy>(iige::Transform2{{10.f, 20.f}}, sf::Color::Yellow, 0.f, 10, 3, 2);
		//scene.create<example::Dummy>(iige::Transform2{{ 0.f, 20.f}}, sf::Color::Cyan,   2.f, 10, 5, 1);
		for (size_t i = 0; i < 5800; i++)
			{
			//scene.create<example::Dummy>(iige::Transform2{{ 0.f, 20.f}}, sf::Color::Cyan, 2.f, 10, 5, 1);
			}

		engine::Loop loop(scene, window, steps_per_second);
		loop.run();
		}
	catch (const std::exception& e) { engine::logger.emplace(utils::message::Type::err, e.what()); }

	return 0;
	}