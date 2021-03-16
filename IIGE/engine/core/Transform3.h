#pragma once

#include <utils/cout_utilities.h>

#include "Quaternion.h"
#include "Vec2.h"

namespace engine::core
	{
	//quick typenames
	class Translation3;
	class Rotation3;
	class Scaling3;
	using Position3    = Translation3;
	using Orientation3 = Rotation3;
	using Size3        = Scaling3;

	class Transform3;

	class Translation3
		{
		public:
			Translation3(float x, float y, float z) noexcept : vec(x, y, z) {}
			Translation3(Vec3f value = {}) noexcept : vec(value) {}

			Vec3f vec{};

			Vec3f    evaluate(const Vec3f& v)                 const noexcept { return v + vec; }
			Vec3f&   apply(Vec3f& v)                          const noexcept { return v += vec; }

			Translation3  operator+ (const Translation3& oth) const noexcept { return { vec + oth.vec }; }
			Translation3& operator+=(const Translation3& oth)       noexcept { return *this = *this + oth; }
			Translation3  operator- ()                        const noexcept { return { vec * -1 }; }
			Translation3& operator--()                              noexcept { return *this = -(*this); }

			Translation3  composite(const Translation3& oth)  const noexcept { return *this + oth; }
			Translation3& compose(const Translation3& oth)          noexcept { return *this += oth; }
			Translation3  inverse()                           const noexcept { return  -(*this); }
			Translation3& invert()                                  noexcept { return --(*this); }

			static Translation3 lerp(const Translation3& a, const Translation3& b, float t) noexcept
				{
				return { Vec3f::lerp(a.vec, b.vec, t) };
				}

		};
	class Rotation3
		{
		public:
			Quaternion q{};

			Vec3f     evaluate(const Vec3f& v)          const noexcept { return q * v; }
			Vec3f&    apply(Vec3f& v)                   const noexcept { return v = q * v; }

			Rotation3  operator+ (const Rotation3& oth) const noexcept { return {}; } // TODO
			Rotation3& operator+=(const Rotation3& oth)       noexcept { return *this = *this + oth; }
			Rotation3  operator- ()                     const noexcept { return { q.inverse() }; }
			Rotation3& operator--()                           noexcept { return *this = -(*this); }

			Rotation3  composite(const Rotation3& oth)  const noexcept { return *this +  oth; }
			Rotation3& compose(const Rotation3& oth)          noexcept { return *this += oth; }
			Rotation3  inverse()                        const noexcept { return  -(*this); }
			Rotation3& invert()                               noexcept { return --(*this); }

			static Rotation3 lerp(Rotation3 a, Rotation3 b, float t) noexcept
				{
				return { Quaternion::slerp(a.q, b.q, t) };
				}

		};
	class Scaling3
		{
		public:
			Scaling3(float value = 1.f) noexcept : value(value) {}

			float value{};

			Vec3f     evaluate(const Vec3f& v)        const noexcept { return v * value; }
			Vec3f&    apply(Vec3f& v)                 const noexcept { return v *= value; }

			Scaling3  operator+ (const Scaling3& oth) const noexcept { return { value * oth.value }; }
			Scaling3& operator+=(const Scaling3& oth)       noexcept { return *this = *this + oth; }
			Scaling3  operator- ()                    const noexcept { return { 1.f / value }; }
			Scaling3& operator--()                          noexcept { return *this = -(*this); }

			Scaling3  composite(const Scaling3& oth)  const noexcept { return *this +  oth; }
			Scaling3& compose(const Scaling3& oth)          noexcept { return *this += oth; }
			Scaling3  inverse()                       const noexcept { return  -(*this); }
			Scaling3& invert()                              noexcept { return --(*this); }

			static Scaling3 lerp(Scaling3 a, Scaling3 b, float t) noexcept { return { utils::lerp(a.value, b.value, t) }; }
		};

	class Transform3
		{
		private:
			Translation3  t{};
			Rotation3     r{};
			Scaling3      s{};

		public:
			Transform3(Translation3 translation = {}, Rotation3 rotation = {}, Scaling3 scaling = {}) noexcept : t(translation), r(rotation), s(scaling) {}

			Translation3 get_t()            const noexcept { return t; };
			Rotation3    get_r()            const noexcept { return r; };
			Scaling3     get_s()            const noexcept { return s; };
			void set_t(const Translation3& value) noexcept { t = value; };
			void set_r(const Rotation3&    value) noexcept { r = value; };
			void set_s(const Scaling3&     value) noexcept { s = value; };
			__declspec(property(get = get_t, put = set_t)) Translation3 translation;
			__declspec(property(get = get_r, put = set_r)) Rotation3    rotation;
			__declspec(property(get = get_s, put = set_s)) Scaling3     scaling;
			__declspec(property(get = get_t, put = set_t)) Position3    position;
			__declspec(property(get = get_r, put = set_r)) Orientation3 orientation;
			__declspec(property(get = get_s, put = set_s)) Size3        size;

			// vec: translation | rotation | scale
			Vec3f     evaluate(const Vec3f& v)    const noexcept { Vec3f ret{ v }; apply(ret); return ret; }
			Vec3f&    apply(Vec3f& v)             const noexcept { return t.apply(r.apply(s.apply(v))); }

			Transform3  operator+ (const Transform3& oth) const noexcept
				{
				Transform3 ret{ t, r + oth.r, s + oth.s };
				oth.t.apply(oth.r.apply(oth.s.apply(ret.t.vec)));
				return ret;
				}
			Transform3& operator+=(const Transform3& oth)       noexcept { return *this = *this + oth; }
			Transform3  operator- ()                      const noexcept
				{
				Transform3 ret{ -t, -r, -s };
				ret.r.apply(ret.s.apply(ret.t.vec));
				return ret;
				}
			Transform3& operator--()                            noexcept { return *this = -(*this); }

			Transform3  composite(const Transform3& oth)  const noexcept { return *this + oth; }
			Transform3& compose(const Transform3& oth)          noexcept { return *this += oth; }
			Transform3  inverse()                         const noexcept { return -(*this); }
			Transform3& invert()                                noexcept { return --(*this); }

			static Transform3 lerp(Transform3 a, Transform3 b, float t) noexcept
				{
				return { Translation3::lerp(a.t, b.t, t), Rotation3::lerp(a.r, b.r, t), Scaling3::lerp(a.s, b.s, t) };
				}
		};
	}

namespace utils::cout
	{
	inline std::ostream& operator<<(std::ostream& os, const engine::core::Translation3& translation) noexcept
		{
		namespace ccu = cout::support;
		return os << ccu::type << "Translation" << translation.vec;
		}
	inline std::ostream& operator<<(std::ostream& os, const engine::core::Rotation3& rotation) noexcept
		{
		namespace ccu = cout::support;
		return os << ccu::type << "Rotation" << color::dw << "(TODO)";
		}
	inline std::ostream& operator<<(std::ostream& os, const engine::core::Scaling3& scale) noexcept
		{
		namespace ccu = cout::support;
		return os << ccu::type << "Scale" << ccu::brace << "("
			<< ccu::value << scale.value << ccu::brace << ")";
		}
	inline std::ostream& operator<<(std::ostream& os, const engine::core::Transform3& transform) noexcept
		{
		namespace ccu = cout::support;
		return os << ccu::type << "Transform" << ccu::brace << "("
			<< transform.translation << ccu::separ << ", "
			<< transform.rotation << ccu::separ << ", "
			<< transform.scaling << ccu::separ << ")";
		}
	}