#pragma once

#include <optional>
#include <functional>

#include <SFML/Graphics.hpp>

#include <utils/containers/polymorphic_storage.h>

#include "core/Transform2.h"
#include "core/Vec2.h"


namespace engine
	{
	namespace objects { class Has_collision; }
	namespace collisions
		{
		struct Other { ::engine::objects::Has_collision* other; };//TODO understand why it doesn't work as std::function<bool(objects::Has_collision*)>
		struct Data
			{
			objects::Has_collision* other;
			core::Vec2f normal;
			core::Transform2 trnition;
			float dist_from_start;
			};

		using Result = std::optional<Data>;
		using  Callback_continuous = std::function<bool(Data)>;
		struct Collision_continuous { size_t collider_list_id; Callback_continuous callback; };
		using  Collisions_list_continuous = std::vector<Collision_continuous>;

		using  Callback_discrete = std::function<bool(Other)>;
		struct Collision_discrete { size_t collider_list_id; Callback_discrete callback; };
		using  Collisions_list_discrete = std::vector<Collision_discrete>;


		namespace shapes
			{
			enum class Type { Poin, Circ, Line, Poly, AABB, Rect };
			class Poin; class Circ; class Line; class Poly; class AABB; class Rect;
			class Shape
				{
				friend class Poin; friend class Circ; friend class Line; friend class AABB; friend class Rect;
				private:
					Shape(Type type, const core::Vec2f& origin) : type{type}, origin{origin} {}
					Type type;
					
					virtual bool   _collide_discrete  (const shapes::Poin& other) const = 0;
					virtual bool   _collide_discrete  (const shapes::Circ& other) const = 0;
					virtual bool   _collide_discrete  (const shapes::Line& other) const = 0;
					virtual bool   _collide_discrete  (const shapes::Poly& other) const = 0;
					virtual bool   _collide_discrete  (const shapes::AABB& other) const = 0;
					virtual bool   _collide_discrete  (const shapes::Rect& other) const = 0;

				public:
					core::Vec2f origin;

					bool collide(const Shape& other) const;

					virtual void update(const core::Transform2& transform) = 0;
					virtual void draw(sf::RenderTarget& rt) const = 0;
				};

			class Poin : public Shape
				{
				friend class Poin; friend class Circ; friend class Line; friend class AABB; friend class Rect;
				private:
					core::Vec2f global_position{};

					virtual bool   _collide_discrete  (const shapes::Poin& other) const override;
					virtual bool   _collide_discrete  (const shapes::Circ& other) const override;
					virtual bool   _collide_discrete  (const shapes::Line& other) const override;
					virtual bool   _collide_discrete  (const shapes::Poly& other) const override;
					virtual bool   _collide_discrete  (const shapes::AABB& other) const override;
					virtual bool   _collide_discrete  (const shapes::Rect& other) const override;

				public:
					Poin(const core::Vec2f& origin) : Shape{Type::Poin, origin} {}

					virtual void update(const core::Transform2& transform) override { global_position = transform.transformed(origin); }
					virtual void draw(sf::RenderTarget& rt) const override
						{
						sf::CircleShape cs(2);
						cs.setFillColor(sf::Color::Red);
						cs.setPosition(global_position.x, global_position.y);
						rt.draw(cs);
						}
				};

			class Circ : public Shape
				{
				friend class Poin; friend class Circ; friend class Line; friend class AABB; friend class Rect;
				private:
					core::Vec2f global_position{};

					virtual bool   _collide_discrete  (const shapes::Poin& other) const override;
					virtual bool   _collide_discrete  (const shapes::Circ& other) const override;
					virtual bool   _collide_discrete  (const shapes::Line& other) const override;
					virtual bool   _collide_discrete  (const shapes::Poly& other) const override;
					virtual bool   _collide_discrete  (const shapes::AABB& other) const override;
					virtual bool   _collide_discrete  (const shapes::Rect& other) const override;
					
				public:
					Circ(const core::Vec2f& origin, float radius) : Shape{Type::Circ, origin}, radius{radius} {}
					float radius;

					virtual void update(const core::Transform2& transform) override { global_position = transform.transformed(origin); }
					virtual void draw(sf::RenderTarget& rt) const override
						{
						sf::CircleShape cs(radius);
						cs.setOutlineColor(sf::Color::Red);
						cs.setOutlineThickness(1);
						cs.setFillColor(sf::Color::Transparent);
						cs.setPosition(global_position.x, global_position.y);
						cs.setOrigin(radius, radius);
						rt.draw(cs);
						}
				};

			class Line : public Shape
				{
				};
			class AABB : public Shape
				{
				private:

				public:
					core::Vec2f size;
				};

			class Rect : public Shape
				{
				private:

				public:
					core::Vec2f size;
				};

			using Point                     = Poin;
			using Circle                    = Circ;
			using Polygon                   = Poly;
			using Axis_aligned_bounding_box = AABB;
			using Rectangle                 = Rect;

			}
		using Shape = shapes::Shape;
		}
	}