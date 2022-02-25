
#include <vector>

#include "utils/variadic.h"
#include "utils/containers/Matrix.h"
#include "utils/definitions.h"
#include "utils/logger.h"

#include "engine/graphics/Window.h"
#include "engine/Scene.h"
#include "engine/Loop.h"
#include "engine/resources_manager.h"




namespace iige { using namespace engine; /*using namespace engine::core;*/ using namespace engine::objects; using namespace utils::math; }

constexpr float steps_per_second = 5.f;
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


			Scene_t* scene;
		public:
			Dummy(Scene_t& scene, iige::Transform2 transform, sf::Color color, float spawn_seconds, size_t spawn_count)
				: scene{&scene}, spawn_seconds{spawn_seconds}, spawn_step{static_cast<size_t>(spawn_seconds * seconds_per_step)}, spawn_count{spawn_count}
				{
				//Movement
				this->transform = transform; //assign starting position
				movement.position = iige::vec2f{10.f, 0.f} / steps_per_second; //assign some speed

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

				movement.position += (iige::vec2f{0.f, 1.f} / (steps_per_second * steps_per_second)); //acceleration per step
				if (step_count < spawn_step * 2) { step_count++; }
				
				/*if(step_count == spawn_step)
					{
					utils::tracking_ptr<Dummy> new_dummy = scene->create<Dummy>(std::ref(*scene), transform, cs.getFillColor(), spawn_seconds, spawn_count - 1);
					new_dummy->movement = movement.composite(engine::core::Transform2{{}, 30_deg, {}});
					}
				if (step_count == spawn_step * 2) { movement = engine::core::Transform2::zero(); }*/
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
			Wall_circ(iige::vec2f pos, float radius) : circ(radius)
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


#include <SFML/Graphics.hpp>
#include <utils/math/geometry/polygon.h>
#include <utils/math/geometry/circle.h>
#include <utils/math/geometry/transformations.h>
#include <utils/math/geometry/interactions.h>
namespace utmg = utils::math::geometry;
namespace utm = utils::math;

template <utmg::polygon_type_t polygon_type>
sf::VertexArray to_sf(const utmg::polygon<polygon_type>& polygon, const sf::Color& color) noexcept
	{
	sf::VertexArray ret{sf::LineStrip, polygon.get_vertices().size() + 1};
	for (size_t i = 0; i < polygon.get_vertices().size(); i++)
		{
		const auto& vertex{polygon.get_vertices()[i]};
		ret[i].position = {vertex.x, vertex.y};
		ret[i].color = color;
		}
	ret[polygon.get_vertices().size()] = ret[0];

	return ret;
	}
sf::CircleShape to_sf(const utmg::circle& circle, const sf::Color& color) noexcept
	{
	sf::CircleShape s{circle.radius};
	s.setPosition({circle.center.x, circle.center.y});
	s.setFillColor(sf::Color::Transparent);
	s.setOutlineColor(color);
	s.setOutlineThickness(1);
	s.setOrigin(circle.radius, circle.radius);
	return s;
	}
sf::VertexArray to_sf(const utmg::segment& segment, const sf::Color& color) noexcept
	{
	sf::VertexArray ret{sf::LineStrip, 2};

	ret[0].position = {segment.a.x, segment.a.y};
	ret[1].position = {segment.b.x, segment.b.y};
	ret[0].color = color;
	ret[1].color = color;

	return ret;
	}

sf::VertexArray to_sf(const utm::vec2f& point, const sf::Color& color) noexcept
	{
	sf::VertexArray ret{sf::Points, 1};

	ret[0].position = {point.x, point.y};
	ret[0].color = color;

	return ret;
	}

int main()
	{
	using namespace utils::angle::literals;
	using namespace utils::math::geometry::transformations;
	using poly_convex = utmg::polygon<utmg::polygon_type_t::convex>;
	using poly_gon    = utmg::polygon<utmg::polygon_type_t::concave>;

	sf::RenderWindow rw{{800, 600}, "pippo"};
	rw.setFramerateLimit(60);

	utmg::segment src_segment{{-10, 0}, {10, 0}};
	utm ::vec2f   src_point  {0, 0};
	poly_convex   src_poly   {{{-10, -10}, {10, -10}, {10, 10},         {-10, 10}}};
	poly_gon      src_cttv   {{{-10, -10}, {10, -10}, {10, 10}, {0, 0}, {-10, 10}}};
	utmg::circle  src_circle {{0, 0}, 10};

	utm::Transform2	tr_segment{{200, 300},  58_deg, 1.f};
	utm::Transform2	tr_point  {{100, 400},  24_deg, 1.f};
	utm::Transform2	tr_poly   {{250, 300}, 105_deg, 1.f};
	utm::Transform2	tr_cttv   {{250, 300}, 105_deg, 1.f};
	utm::Transform2	tr_circle {{350, 400},  75_deg, 1.f};

	utmg::segment segment{src_segment * tr_segment};
	utm ::vec2f   point  {src_point   * tr_point  };
	poly_convex   poly   {src_poly    * tr_poly   };
	poly_gon      cttv   {src_cttv    * tr_cttv   };
	utmg::circle  circle {src_circle  * tr_circle };

	utils::observer_ptr<utm::Transform2> selected_transform{nullptr};


	auto side{segment.point_side(point)};

	while (rw.isOpen())
		{

		//input
		sf::Event event;
		while (rw.pollEvent(event))
			{
			switch (event.type)
				{
				case sf::Event::Closed: rw.close(); break;

				case sf::Event::KeyPressed:
					if (true)
						{
						switch (event.key.code)
							{
							case sf::Keyboard::Num1: selected_transform = &tr_segment; break;
							case sf::Keyboard::Num2: selected_transform = &tr_point;   break;
							case sf::Keyboard::Num3: selected_transform = &tr_poly;    break;
							case sf::Keyboard::Num4: selected_transform = &tr_cttv;  break;
							case sf::Keyboard::Num5: selected_transform = &tr_circle;  break;
							}

						if (selected_transform != nullptr)
							{
							switch (event.key.code)
								{
								case sf::Keyboard::Right: selected_transform->position.x++; break;
								case sf::Keyboard::Left:  selected_transform->position.x--; break;
								case sf::Keyboard::Up:    selected_transform->position.y--; break;
								case sf::Keyboard::Down:  selected_transform->position.y++; break;

								case sf::Keyboard::PageUp:   selected_transform->orientation -= 1_deg; break;
								case sf::Keyboard::PageDown: selected_transform->orientation += 1_deg; break;

								case sf::Keyboard::Add:      selected_transform->size *= 2; break;
								case sf::Keyboard::Subtract: selected_transform->size /= 2; break;
								}
							}
						segment = {src_segment * tr_segment};
						point   = {src_point   * tr_point  };
						poly    = {src_poly    * tr_poly   };
						cttv    = {src_cttv    * tr_cttv   };
						circle  = {src_circle  * tr_circle };
						}
				}
			}
		//step
		bool st{utmg::collides(segment, point)};
		bool sp{utmg::collides(segment, poly)};
		bool sv{utmg::collides(segment, cttv)};
		bool tp{utmg::collides(point, poly)};
		bool tv{utmg::collides(point, cttv)};
		bool pc{utmg::collides(poly, circle)};
		bool vc{utmg::collides(cttv, circle)};
		bool tc{utmg::collides(point, circle)};
		bool cs{utmg::collides(circle, segment)};
		bool pv{utmg::collides(poly, cttv)};



		//draw
		rw.clear();

		rw.draw(to_sf(segment, (st || sp || cs || sv) ? sf::Color::Red : sf::Color::White));
		rw.draw(to_sf(point,   (tp || tc || st || tv) ? sf::Color::Red : sf::Color::White));
		rw.draw(to_sf(poly,    (tp || sp || pc || pv) ? sf::Color::Red : sf::Color::White));
		rw.draw(to_sf(circle,  (pc || tc || cs || vc) ? sf::Color::Red : sf::Color::White));
		rw.draw(to_sf(cttv,    (pv || vc || tv || sv) ? sf::Color::Red : sf::Color::White));

		rw.display();
		}
	}

int mainz()
	{
	try
		{
		engine::graphics::Window window(1024, 768, "Hello World");
		engine::Resources_manager resman;
		Scene_t scene;

		//Loading resources returns an uncopyable but moveable RAII container.
		auto fireball_text = resman.add_texture("data/textures/fireball_0.png");

		scene.create<example::Wall_circ>(iige::vec2f{580, 380}, 10.f);

		using namespace utils::literals;

		for (size_t i = 0; i < 1000; i++)
			{
			scene.create<example::Dummy>(std::ref(scene), iige::Transform2{{500.f, 350.f}}, sf::Color::Green,  2.f, 3_size);
			scene.create<example::Dummy>(std::ref(scene), iige::Transform2{{ 10.f,  20.f}}, sf::Color::Yellow, 2.f, 3_size);
			scene.create<example::Dummy>(std::ref(scene), iige::Transform2{{  0.f,  20.f}}, sf::Color::Cyan,   2.f, 3_size);
			}
		scene.update();

		engine::Loop loop{scene, window, steps_per_second};
		loop.run();
		}
	catch (const std::exception& e) { utils::globals::logger.err(e.what()); }

	return 0;
	}