#pragma once
#include "Explicit2.h"
#include "../../utils/math/angle.h"
#include "../../utils/cout_utilities.h"

namespace engine::core
	{
	//quick typenames
	class Translation2;
	class Rotation2;
	class Scaling2;

	class Transform2;

	class Translation2
		{
		public:
			Translation2(float x, float y) noexcept : vec(x, y) {}
			Translation2(Vec2f value = {}) noexcept : vec(value) {}

			Vec2f vec{};

			Vec2f    evaluate(const Vec2f& v)                 const noexcept { return v +  vec; }
			Vec2f&   apply(Vec2f& v)                          const noexcept { return v += vec; }
			Point2f  evaluate(const Point2f& p)               const noexcept { return evaluate(p.v); }
			Point2f& apply(Point2f& p)                        const noexcept { apply(p.v); return p; }

			Translation2  operator+ (const Translation2& oth) const noexcept { return {vec + oth.vec}; }
			Translation2& operator+=(const Translation2& oth)       noexcept { return *this = *this + oth; }
			Translation2  operator- ()                        const noexcept { return {vec * -1}; }
			Translation2& operator--()                              noexcept { return *this = -(*this); }

			Translation2  composite(const Translation2& oth)  const noexcept { return *this +  oth; }
			Translation2& compose(const Translation2& oth)          noexcept { return *this += oth; }
			Translation2  inverse()                           const noexcept { return  -(*this); }
			Translation2  invert()                                  noexcept { return --(*this); }

			static Translation2 lerp(Translation2 a, Translation2 b, float t)
				{
				return { Vec2f::lerp(a.vec, b.vec, t) };
				}
		};

	class Rotation2
		{
		public:
			Rotation2(utils::angle::deg angle = {0}) : angle(angle) {}

			utils::angle::deg angle{};

			Vec2f     evaluate(const Vec2f& v)          const noexcept { return v +  angle; }
			Vec2f&    apply(Vec2f& v)                   const noexcept { return v += angle; } // TODO
			Point2f   evaluate(const Point2f& p)        const noexcept { return evaluate(p.v); }
			Point2f&  apply(Point2f& p)                 const noexcept { apply(p.v); return p; }
			Vector2f  evaluate(const Vector2f& v)       const noexcept { return evaluate(v.v); }
			Vector2f& apply(Vector2f& v)                const noexcept { apply(v.v); return v; }
			Unit2f    evaluate(const Unit2f& u)         const noexcept { return evaluate(u.v); }
			Unit2f&   apply(Unit2f& u)                  const noexcept { apply(u.v); return u; }

			Rotation2  operator+ (const Rotation2& oth) const noexcept { return angle + oth.angle; }
			Rotation2& operator+=(const Rotation2& oth)       noexcept { return *this = *this + oth; }
			Rotation2  operator- ()                     const noexcept { return {-angle}; }
			Rotation2& operator--()                           noexcept { return *this = -(*this); }

			Rotation2  composite(const Rotation2& oth)  const noexcept { return *this +  oth; }
			Rotation2& compose(const Rotation2& oth)          noexcept { return *this += oth; }
			Rotation2  inverse()                        const noexcept { return  -(*this); }
			Rotation2  invert()                               noexcept { return --(*this); }

			static Rotation2 lerp(Rotation2 a, Rotation2 b, float t)
				{
				return {utils::angle::deg{utils::lerp(a.angle.value, b.angle.value, t)}};
				}
		};

	class Scaling2
		{
		public:
			Scaling2(float value = 1.f) noexcept : value(value) {}

			float value{};

			Vec2f     evaluate(const Vec2f& v)        const noexcept { return v *  value; }
			Vec2f&    apply(Vec2f& v)                 const noexcept { return v *= value; }
			Point2f   evaluate(const Point2f& p)      const noexcept { return evaluate(p.v); }
			Point2f&  apply(Point2f& p)               const noexcept { apply(p.v); return p; }
			Vector2f  evaluate(const Vector2f& v)     const noexcept { return evaluate(v.v); }
			Vector2f& apply(Vector2f& v)              const noexcept { apply(v.v); return v; }

			Scaling2  operator+ (const Scaling2& oth) const noexcept { return { value * oth.value }; }
			Scaling2& operator+=(const Scaling2& oth)       noexcept { return *this = *this + oth; }
			Scaling2  operator- ()                    const noexcept { return { 1.f / value }; }
			Scaling2& operator--()                          noexcept { return *this = -(*this); }

			Scaling2  composite(const Scaling2& oth)  const noexcept { return *this +  oth; }
			Scaling2& compose(const Scaling2& oth)          noexcept { return *this += oth; }
			Scaling2  inverse()                       const noexcept { return  -(*this); }
			Scaling2  invert()                              noexcept { return --(*this); }

			static Scaling2 lerp(Scaling2 a, Scaling2 b, float t) { return { utils::lerp(a.value, b.value, t) }; }
		};

	class Transform2
		{
		private:
			Translation2  t{};
			Rotation2     r{};
			Scaling2      s{};

		public:
			Transform2(Translation2 translation = {}, Rotation2 rotation = {}, Scaling2 scaling = {}) noexcept : t(translation), r(rotation), s(scaling) {}
			
			      Translation2&      translation()       noexcept { return t; }
			const Translation2&      translation() const noexcept { return t; }
			      Rotation2&         rotation()          noexcept { return r; }
			const Rotation2&         rotation()    const noexcept { return r; }
			      Scaling2&          scaling()           noexcept { return s; }
			const Scaling2&          scaling()     const noexcept { return s; }
			      Vec2f&             position()	         noexcept { return t.vec; }
			const Vec2f&             position()    const noexcept { return t.vec; }
			      utils::angle::deg& orientation()       noexcept { return r.angle; }
			const utils::angle::deg& orientation() const noexcept { return r.angle; }
			      float&             size()		         noexcept { return s.value; }
			const float&             size()        const noexcept { return s.value; }

			// vec: translation | rotation | scale
			Vec2f     evaluate(const Vec2f& v)    const { Vec2f ret{ v }; apply(ret); return ret; }
			Vec2f&    apply(Vec2f& v)             const { return t.apply(r.apply(s.apply(v))); }
			// point: translation | rotation | scale
			Point2f   evaluate(const Point2f& p)  const { Point2f ret{ p }; apply(ret); return ret; }
			Point2f&  apply(Point2f& p)           const { return t.apply(r.apply(s.apply(p))); }
			// vector: rotation | scale
			Vector2f  evaluate(const Vector2f& v) const { Vector2f ret{ v }; apply(ret); return ret; }
			Vector2f& apply(Vector2f& v)          const { return r.apply(s.apply(v)); }
			// unit vector: rotation
			Unit2f    evaluate(const Unit2f& u)   const { return { r.evaluate(u) }; }
			Unit2f&   apply(Unit2f& u)            const { return r.apply(u); }

			Transform2  operator+ (const Transform2& oth) const noexcept
				{
				Transform2 ret{ t + oth.t, r + oth.r, s + oth.s };
				return ret;
				}
			Transform2& operator+=(const Transform2& oth)       noexcept
				{ return *this = *this + oth; }
			Transform2  operator- ()                      const noexcept
				{
				Transform2 ret{ -t, -r, -s };
				ret.r.apply(ret.s.apply(ret.t.vec));
				return ret;
				}
			Transform2& operator--()                            noexcept
				{ return *this = -(*this); }

			Transform2  composite(const Transform2& oth)  const noexcept { return *this +  oth; }
			Transform2& compose(const Transform2& oth)          noexcept { return *this += oth; }
			Transform2  inverse()                         const noexcept { return  -(*this); }
			Transform2  invert()                                noexcept { return --(*this); }

			static Transform2 lerp(Transform2 a, Transform2 b, float t)
				{
				return { Translation2::lerp(a.t, b.t, t),
					Rotation2::lerp(a.r, b.r, t),
					Scaling2::lerp(a.s, b.s, t) };
				}
		};
	}

namespace utils::cout
	{
	inline std::ostream& operator<<(std::ostream& os, const engine::core::Translation2& translation) noexcept
		{
		namespace ccu = cout::support;
		return os << ccu::type << "Translation" << translation.vec;
		}
	inline std::ostream& operator<<(std::ostream& os, const engine::core::Rotation2& rotation) noexcept
		{
		namespace ccu = cout::support;
		return os << ccu::type << "Rotation" << ccu::brace << "("
			<< ccu::value << rotation.angle << ccu::brace << ")";
		}
	inline std::ostream& operator<<(std::ostream& os, const engine::core::Scaling2& scale) noexcept
		{
		namespace ccu = cout::support;
		return os << ccu::type << "Scale" << ccu::brace << "("
			<< ccu::value << scale.value << ccu::brace << ")";
		}
	inline std::ostream& operator<<(std::ostream& os, const engine::core::Transform2& transform) noexcept
		{
		namespace ccu = cout::support;
		return os << ccu::type << "Transform" << ccu::brace << "("
			<< transform.translation() << ccu::separ << ", "
			<< transform.rotation() << ccu::separ << ", "
			<< transform.scaling() << ccu::separ << ")";
		}
	}
