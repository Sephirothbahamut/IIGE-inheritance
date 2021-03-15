#pragma once

#include <optional>

#include "../utils/containers/polymorphic_storage.h"

#include "core/Vec2.h"

namespace engine
	{
	namespace collisions
		{
		struct Data
			{
			core::Vec2f normal;
			core::Vec2f position;
			float dist_from_start;
			};
		using Result = std::optional<Data>;
		using Callback = std::function<bool(Data)>;
		struct Collision { size_t collider_list_id; Callback callback; };
		using Collisions_list = std::vector<Collision>;


		namespace shapes
			{
			enum class Type { Point, Circ, Line, AABB, Rect };
			class Point; class Circ; class Line; class AABB; class Rect;
			class Shape
				{
				friend class Point; friend class Circ; friend class Line; friend class AABB; friend class Rect;
				private:
					Shape(Type type) : type(type) {}
					Type type;

					virtual Result _collide(const core::Vec2f position, const shapes::Point& other, const core::Vec2f other_position) const = 0;
					virtual Result _collide(const core::Vec2f position, const shapes::Circ&  other, const core::Vec2f other_position) const = 0;
					virtual Result _collide(const core::Vec2f position, const shapes::Line&  other, const core::Vec2f other_position) const = 0;
					virtual Result _collide(const core::Vec2f position, const shapes::AABB&  other, const core::Vec2f other_position) const = 0;
					virtual Result _collide(const core::Vec2f position, const shapes::Rect&  other, const core::Vec2f other_position) const = 0;

				public:
					Result discrete_collision(const core::Vec2f position, const Shape& other, const core::Vec2f other_position) const;

				};
			class Point : public Shape
				{
				private:

				public:
				};

			class Circ : public Shape
				{
				private:
					virtual Result _collide(const core::Vec2f position, const shapes::Point& other, const core::Vec2f other_position) const { return std::nullopt; } //TODO
					virtual Result _collide(const core::Vec2f position, const shapes::Circ&  other, const core::Vec2f other_position) const
						{
						float dist = core::Vec2f::distance(position, other_position);
						if (dist <= radius + other.radius) { return {Data{}}; }
						else { return std::nullopt; }
						}
					virtual Result _collide(const core::Vec2f position, const shapes::Line&  other, const core::Vec2f other_position) const { return std::nullopt; } //TODO
					virtual Result _collide(const core::Vec2f position, const shapes::AABB&  other, const core::Vec2f other_position) const { return std::nullopt; } //TODO
					virtual Result _collide(const core::Vec2f position, const shapes::Rect&  other, const core::Vec2f other_position) const { return std::nullopt; } //TODO

				public:
					Circ(float radius) : Shape(Type::Circ), radius(radius) {}
					float radius;

				};

			class Line : public Shape
				{};
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

			using Circle = Circ;
			using Axis_aligned_bounding_box = AABB;
			using Rectangle = Rect;

			inline Result Shape::discrete_collision(const core::Vec2f position, const Shape& other, const core::Vec2f other_position) const
				{
				switch (other.type)
					{
					case Type::Point: return _collide(position, reinterpret_cast<const Point&> (other), other_position);
					case Type::Circ:  return _collide(position, reinterpret_cast<const Circle&>(other), other_position);
					case Type::Line:  return _collide(position, reinterpret_cast<const Line&>  (other), other_position);
					case Type::AABB:  return _collide(position, reinterpret_cast<const AABB&>  (other), other_position);
					case Type::Rect:  return _collide(position, reinterpret_cast<const Rect&>  (other), other_position);
					}
				}
			}
		using Shape = shapes::Shape;
		}
	}