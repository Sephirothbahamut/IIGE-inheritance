#include "Collider.h"

namespace engine::collisions::shapes
	{
	bool Shape::collide(const Shape& other) const
		{
		switch (other.type)
			{
			case Type::Poin: return _collide_discrete(reinterpret_cast<const Poin&>(other));
			case Type::Circ: return _collide_discrete(reinterpret_cast<const Circ&>(other));
			case Type::Line: return _collide_discrete(reinterpret_cast<const Line&>(other));
			case Type::Poly: return _collide_discrete(reinterpret_cast<const Poly&>(other));
			case Type::AABB: return _collide_discrete(reinterpret_cast<const AABB&>(other));
			case Type::Rect: return _collide_discrete(reinterpret_cast<const Rect&>(other));
			}
		}

	bool   Poin::_collide_discrete  (const shapes::Poin& other) const { return core::Vec2f::distance(global_position, other.global_position) == 0; }
	bool   Poin::_collide_discrete  (const shapes::Circ& other) const { return core::Vec2f::distance(global_position, other.global_position) <= other.radius; }
	bool   Poin::_collide_discrete  (const shapes::Line& other) const { return false; }
	bool   Poin::_collide_discrete  (const shapes::Poly& other) const { return false; }
	bool   Poin::_collide_discrete  (const shapes::AABB& other) const { return false; }
	bool   Poin::_collide_discrete  (const shapes::Rect& other) const { return false; }
	
	bool   Circ::_collide_discrete  (const shapes::Poin& other) const { return other.collide(*this); }
	bool   Circ::_collide_discrete  (const shapes::Circ& other) const { return core::Vec2f::distance(global_position, other.global_position) <= radius + other.radius; }
	bool   Circ::_collide_discrete  (const shapes::Line& other) const { return false; }
	bool   Circ::_collide_discrete  (const shapes::Poly& other) const { return false; }
	bool   Circ::_collide_discrete  (const shapes::AABB& other) const { return false; }
	bool   Circ::_collide_discrete  (const shapes::Rect& other) const { return false; }
	}