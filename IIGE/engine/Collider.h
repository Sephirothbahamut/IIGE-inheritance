#pragma once

#include <optional>
#include <functional>

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
					
					virtual Result _collide_continuous(const core::Transform2& trn_beg, const core::Transform2& trn_end, const shapes::Poin& other, const core::Transform2& oth_trn) const = 0;
					virtual Result _collide_continuous(const core::Transform2& trn_beg, const core::Transform2& trn_end, const shapes::Circ& other, const core::Transform2& oth_trn) const = 0;
					virtual Result _collide_continuous(const core::Transform2& trn_beg, const core::Transform2& trn_end, const shapes::Line& other, const core::Transform2& oth_trn) const = 0;
					virtual Result _collide_continuous(const core::Transform2& trn_beg, const core::Transform2& trn_end, const shapes::Poly& other, const core::Transform2& oth_trn) const = 0;
					virtual Result _collide_continuous(const core::Transform2& trn_beg, const core::Transform2& trn_end, const shapes::AABB& other, const core::Transform2& oth_trn) const = 0;
					virtual Result _collide_continuous(const core::Transform2& trn_beg, const core::Transform2& trn_end, const shapes::Rect& other, const core::Transform2& oth_trn) const = 0;
					virtual bool   _collide_discrete  (const core::Transform2& trn,                                      const shapes::Poin& other, const core::Transform2& oth_trn) const = 0;
					virtual bool   _collide_discrete  (const core::Transform2& trn,                                      const shapes::Circ& other, const core::Transform2& oth_trn) const = 0;
					virtual bool   _collide_discrete  (const core::Transform2& trn,                                      const shapes::Line& other, const core::Transform2& oth_trn) const = 0;
					virtual bool   _collide_discrete  (const core::Transform2& trn,                                      const shapes::Poly& other, const core::Transform2& oth_trn) const = 0;
					virtual bool   _collide_discrete  (const core::Transform2& trn,                                      const shapes::AABB& other, const core::Transform2& oth_trn) const = 0;
					virtual bool   _collide_discrete  (const core::Transform2& trn,                                      const shapes::Rect& other, const core::Transform2& oth_trn) const = 0;

				public:
					core::Vec2f origin;

					Result collide_continuous(const core::Transform2& trn_beg, const core::Transform2& trn_end, const Shape& other, const core::Transform2& oth_trn) const;
					bool collide(const core::Transform2& trn, const Shape& other, const core::Transform2& oth_trn) const;
				};

			class Poin : public Shape
				{
				friend class Poin; friend class Circ; friend class Line; friend class AABB; friend class Rect;
				private:
					virtual Result _collide_continuous(const core::Transform2& trn_beg, const core::Transform2& trn_end, const shapes::Poin& other, const core::Transform2& oth_trn) const;
					virtual Result _collide_continuous(const core::Transform2& trn_beg, const core::Transform2& trn_end, const shapes::Circ& other, const core::Transform2& oth_trn) const;
					virtual Result _collide_continuous(const core::Transform2& trn_beg, const core::Transform2& trn_end, const shapes::Line& other, const core::Transform2& oth_trn) const;
					virtual Result _collide_continuous(const core::Transform2& trn_beg, const core::Transform2& trn_end, const shapes::Poly& other, const core::Transform2& oth_trn) const;
					virtual Result _collide_continuous(const core::Transform2& trn_beg, const core::Transform2& trn_end, const shapes::AABB& other, const core::Transform2& oth_trn) const;
					virtual Result _collide_continuous(const core::Transform2& trn_beg, const core::Transform2& trn_end, const shapes::Rect& other, const core::Transform2& oth_trn) const;
					virtual bool   _collide_discrete  (const core::Transform2& trn,                                      const shapes::Poin& other, const core::Transform2& oth_trn) const;
					virtual bool   _collide_discrete  (const core::Transform2& trn,                                      const shapes::Circ& other, const core::Transform2& oth_trn) const;
					virtual bool   _collide_discrete  (const core::Transform2& trn,                                      const shapes::Line& other, const core::Transform2& oth_trn) const;
					virtual bool   _collide_discrete  (const core::Transform2& trn,                                      const shapes::Poly& other, const core::Transform2& oth_trn) const;
					virtual bool   _collide_discrete  (const core::Transform2& trn,                                      const shapes::AABB& other, const core::Transform2& oth_trn) const;
					virtual bool   _collide_discrete  (const core::Transform2& trn,                                      const shapes::Rect& other, const core::Transform2& oth_trn) const;

				public:
					Poin(const core::Vec2f& origin) : Shape{Type::Poin, origin} {}
				};

			class Circ : public Shape
				{
				friend class Poin; friend class Circ; friend class Line; friend class AABB; friend class Rect;
				private:
					virtual Result _collide_continuous(const core::Transform2& trn_beg, const core::Transform2& trn_end, const shapes::Poin& other, const core::Transform2& oth_trn) const;
					virtual Result _collide_continuous(const core::Transform2& trn_beg, const core::Transform2& trn_end, const shapes::Circ& other, const core::Transform2& oth_trn) const;
					virtual Result _collide_continuous(const core::Transform2& trn_beg, const core::Transform2& trn_end, const shapes::Line& other, const core::Transform2& oth_trn) const;
					virtual Result _collide_continuous(const core::Transform2& trn_beg, const core::Transform2& trn_end, const shapes::Poly& other, const core::Transform2& oth_trn) const;
					virtual Result _collide_continuous(const core::Transform2& trn_beg, const core::Transform2& trn_end, const shapes::AABB& other, const core::Transform2& oth_trn) const;
					virtual Result _collide_continuous(const core::Transform2& trn_beg, const core::Transform2& trn_end, const shapes::Rect& other, const core::Transform2& oth_trn) const;
					virtual bool   _collide_discrete  (const core::Transform2& trn,                                      const shapes::Poin& other, const core::Transform2& oth_trn) const;
					virtual bool   _collide_discrete  (const core::Transform2& trn,                                      const shapes::Circ& other, const core::Transform2& oth_trn) const;
					virtual bool   _collide_discrete  (const core::Transform2& trn,                                      const shapes::Line& other, const core::Transform2& oth_trn) const;
					virtual bool   _collide_discrete  (const core::Transform2& trn,                                      const shapes::Poly& other, const core::Transform2& oth_trn) const;
					virtual bool   _collide_discrete  (const core::Transform2& trn,                                      const shapes::AABB& other, const core::Transform2& oth_trn) const;
					virtual bool   _collide_discrete  (const core::Transform2& trn,                                      const shapes::Rect& other, const core::Transform2& oth_trn) const;
					
				public:
					Circ(const core::Vec2f& origin, float radius) : Shape{Type::Circ, origin}, radius{radius} {}
					float radius;
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