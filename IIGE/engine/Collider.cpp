#include "Collider.h"

namespace engine::collisions::shapes
	{
	Result Shape::collide_continuous(const core::Transform2& trn_beg, const core::Transform2& trn_end, const Shape& other, const core::Transform2& oth_trn) const
		{
		switch (other.type)
			{
			case Type::Poin: return _collide_continuous(trn_beg, trn_end, reinterpret_cast<const Poin&>(other), oth_trn);
			case Type::Circ: return _collide_continuous(trn_beg, trn_end, reinterpret_cast<const Circ&>(other), oth_trn);
			case Type::Line: return _collide_continuous(trn_beg, trn_end, reinterpret_cast<const Line&>(other), oth_trn);
			case Type::Poly: return _collide_continuous(trn_beg, trn_end, reinterpret_cast<const Poly&>(other), oth_trn);
			case Type::AABB: return _collide_continuous(trn_beg, trn_end, reinterpret_cast<const AABB&>(other), oth_trn);
			case Type::Rect: return _collide_continuous(trn_beg, trn_end, reinterpret_cast<const Rect&>(other), oth_trn);
			}
		}
	bool Shape::collide(const core::Transform2& trn, const Shape& other, const core::Transform2& oth_trn) const
		{
		switch (other.type)
			{
			case Type::Poin: return _collide_discrete(trn, reinterpret_cast<const Poin&>(other), oth_trn);
			case Type::Circ: return _collide_discrete(trn, reinterpret_cast<const Circ&>(other), oth_trn);
			case Type::Line: return _collide_discrete(trn, reinterpret_cast<const Line&>(other), oth_trn);
			case Type::Poly: return _collide_discrete(trn, reinterpret_cast<const Poly&>(other), oth_trn);
			case Type::AABB: return _collide_discrete(trn, reinterpret_cast<const AABB&>(other), oth_trn);
			case Type::Rect: return _collide_discrete(trn, reinterpret_cast<const Rect&>(other), oth_trn);
			}
		}

	Result Poin::_collide_continuous(const core::Transform2& trn_beg, const core::Transform2& trn_end, const shapes::Poin& other, const core::Transform2& oth_trn) const { return std::nullopt; }
	Result Poin::_collide_continuous(const core::Transform2& trn_beg, const core::Transform2& trn_end, const shapes::Circ& other, const core::Transform2& oth_trn) const { return std::nullopt; }
	Result Poin::_collide_continuous(const core::Transform2& trn_beg, const core::Transform2& trn_end, const shapes::Line& other, const core::Transform2& oth_trn) const { return std::nullopt; }
	Result Poin::_collide_continuous(const core::Transform2& trn_beg, const core::Transform2& trn_end, const shapes::Poly& other, const core::Transform2& oth_trn) const { return std::nullopt; }
	Result Poin::_collide_continuous(const core::Transform2& trn_beg, const core::Transform2& trn_end, const shapes::AABB& other, const core::Transform2& oth_trn) const { return std::nullopt; }
	Result Poin::_collide_continuous(const core::Transform2& trn_beg, const core::Transform2& trn_end, const shapes::Rect& other, const core::Transform2& oth_trn) const { return std::nullopt; }
	bool   Poin::_collide_discrete  (const core::Transform2& trn,                                      const shapes::Poin& other, const core::Transform2& oth_trn) const { return core::Vec2f::distance(trn.transformed(origin), oth_trn.transformed(other.origin)) == 0; }
	bool   Poin::_collide_discrete  (const core::Transform2& trn,                                      const shapes::Circ& other, const core::Transform2& oth_trn) const { return core::Vec2f::distance(trn.transformed(origin), oth_trn.transformed(other.origin)) <= other.radius; }
	bool   Poin::_collide_discrete  (const core::Transform2& trn,                                      const shapes::Line& other, const core::Transform2& oth_trn) const { return false; }
	bool   Poin::_collide_discrete  (const core::Transform2& trn,                                      const shapes::Poly& other, const core::Transform2& oth_trn) const { return false; }
	bool   Poin::_collide_discrete  (const core::Transform2& trn,                                      const shapes::AABB& other, const core::Transform2& oth_trn) const { return false; }
	bool   Poin::_collide_discrete  (const core::Transform2& trn,                                      const shapes::Rect& other, const core::Transform2& oth_trn) const { return false; }
	
	Result Circ::_collide_continuous(const core::Transform2& trn_beg, const core::Transform2& trn_end, const shapes::Poin& other, const core::Transform2& oth_trn) const { return std::nullopt; }
	Result Circ::_collide_continuous(const core::Transform2& trn_beg, const core::Transform2& trn_end, const shapes::Circ& other, const core::Transform2& oth_trn) const { return std::nullopt; }
	Result Circ::_collide_continuous(const core::Transform2& trn_beg, const core::Transform2& trn_end, const shapes::Line& other, const core::Transform2& oth_trn) const { return std::nullopt; }
	Result Circ::_collide_continuous(const core::Transform2& trn_beg, const core::Transform2& trn_end, const shapes::Poly& other, const core::Transform2& oth_trn) const { return std::nullopt; }
	Result Circ::_collide_continuous(const core::Transform2& trn_beg, const core::Transform2& trn_end, const shapes::AABB& other, const core::Transform2& oth_trn) const { return std::nullopt; }
	Result Circ::_collide_continuous(const core::Transform2& trn_beg, const core::Transform2& trn_end, const shapes::Rect& other, const core::Transform2& oth_trn) const { return std::nullopt; }
	bool   Circ::_collide_discrete  (const core::Transform2& trn,                                      const shapes::Poin& other, const core::Transform2& oth_trn) const { return other.collide(oth_trn, *this, trn); }
	bool   Circ::_collide_discrete  (const core::Transform2& trn,                                      const shapes::Circ& other, const core::Transform2& oth_trn) const { return core::Vec2f::distance(trn.transformed(origin), oth_trn.transformed(other.origin)) <= radius + other.radius; }
	bool   Circ::_collide_discrete  (const core::Transform2& trn,                                      const shapes::Line& other, const core::Transform2& oth_trn) const { return false; }
	bool   Circ::_collide_discrete  (const core::Transform2& trn,                                      const shapes::Poly& other, const core::Transform2& oth_trn) const { return false; }
	bool   Circ::_collide_discrete  (const core::Transform2& trn,                                      const shapes::AABB& other, const core::Transform2& oth_trn) const { return false; }
	bool   Circ::_collide_discrete  (const core::Transform2& trn,                                      const shapes::Rect& other, const core::Transform2& oth_trn) const { return false; }
	}