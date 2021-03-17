#pragma once
#include <ostream>

#include <utils/math/math.h>
#include <utils/math/angle.h>
#include <utils/cout_utilities.h>

#include "Vec.h"

namespace engine::core
	{
	template <typename T> class Vec2;
	}

namespace utils::cout
	{
	template <typename T>
	inline std::ostream& operator<<(std::ostream& os, const engine::core::Vec2<T>& vec) noexcept
		{
		namespace ccu = cout::support;
		return os << ccu::type << "Vec2" << ccu::brace << "(" << ccu::value << vec.x << ccu::separ << ", " << ccu::value << vec.y << ccu::brace << ")";
		}
	}

namespace engine::core
	{
	//fast typenames
	using Vec2i   = Vec2<int>;
	using Vec2i8  = Vec2<int8_t>;
	using Vec2i16 = Vec2<int16_t>;
	using Vec2i32 = Vec2<int32_t>;
	using Vec2i64 = Vec2<int64_t>;

	using Vec2u   = Vec2<unsigned>;
	using Vec2u8  = Vec2<uint8_t>;
	using Vec2u16 = Vec2<uint16_t>;
	using Vec2u32 = Vec2<uint32_t>;
	using Vec2u46 = Vec2<uint64_t>;

	using Vec2s = Vec2<size_t>;
	using Vec2f = Vec2<float>;
	using Vec2d = Vec2<double>;

	template <typename T>
	class Vec2
		{
		public:
			Vec2<T>()         noexcept = default;
			Vec2<T>(T x, T y) noexcept : x(x), y(y) {};
			Vec2<T>(T xy)     noexcept : x(xy), y(xy) {}
			static Vec2<T> rr()    noexcept { return {T{ 1}, T{ 0}}; }
			static Vec2<T> ll()    noexcept { return {T{-1}, T{ 0}}; }
			static Vec2<T> up()    noexcept { return {T{ 0}, T{-1}}; }
			static Vec2<T> dw()    noexcept { return {T{ 0}, T{ 1}}; }
			static Vec2<T> right() noexcept { return rr(); }
			static Vec2<T> left()  noexcept { return ll(); }
			static Vec2<T> down()  noexcept { return dw(); }
			static Vec2<T> zero()  noexcept { return {}; }

			T x = 0, y = 0;

			T magnitude2()    const noexcept { return x * x + y * y; }
			T magnitude()     const noexcept { return std::sqrt(magnitude2()); }
			Vec2<T>  normal() const noexcept { return magnitude() ? *this / magnitude() : *this; }
			Vec2<T>& normalize()    noexcept { return *this = normal(); }

			// OPERATORS
			Vec2<T>  operator-() const noexcept { return {-x, -y}; }

			// VEC & SCALAR OPERATORS
			Vec2<T>  operator++() noexcept { return *this += T(1); }
			Vec2<T>  operator--() noexcept { return *this -= T(1); }

			Vec2<T>  operator+ (const T n) const noexcept { return {normal() * (magnitude() + n)}; }
			Vec2<T>  operator- (const T n) const noexcept { return {normal() * (magnitude() - n)}; }
			Vec2<T>  operator* (const T n) const noexcept { return {x * n, y * n}; }
			Vec2<T>  operator/ (const T n) const noexcept { return {x / n, y / n}; }

			Vec2<T>& operator+=(const T n)       noexcept { return *this = *this + n; }
			Vec2<T>& operator-=(const T n)       noexcept { return *this = *this - n; }
			Vec2<T>& operator*=(const T n)       noexcept { return *this = *this * n; }
			Vec2<T>& operator/=(const T n)       noexcept { return *this = *this / n; }
			Vec2<T>& operator= (const T n)       noexcept { return normalize() *= n; }

			// VEC & ANGLE OPERATIONS
			Vec2<T>  operator+ (const utils::angle::deg angle) const noexcept { utils::angle::rad rngle{angle}; return {x * rngle.cos() - y * rngle.sin(), x * rngle.sin() + y * rngle.cos()}; }
			Vec2<T>  operator+ (const utils::angle::rad angle) const noexcept { return {x * angle.cos() - y * angle.sin(), x * angle.sin() + y * angle.cos()}; }
			Vec2<T>  operator- (const utils::angle::deg angle) const noexcept { utils::angle::rad rngle{angle}; return {x * rngle.cos() - y * rngle.sin(), x * rngle.sin() + y * rngle.cos()}; }
			Vec2<T>  operator- (const utils::angle::rad angle) const noexcept { return {x * angle.cos() - y * angle.sin(), x * angle.sin() + y * angle.cos()}; }
			Vec2<T>& operator+=(const utils::angle::deg angle)       noexcept { return *this = *this + angle; }
			Vec2<T>& operator+=(const utils::angle::rad angle)       noexcept { return *this = *this + angle; }
			Vec2<T>& operator-=(const utils::angle::deg angle)       noexcept { return *this = *this - angle; }
			Vec2<T>& operator-=(const utils::angle::rad angle)       noexcept { return *this = *this - angle; }
			Vec2<T>& operator= (const utils::angle::deg angle)       noexcept { return *this = angle.to_rad(); }
			Vec2<T>& operator= (const utils::angle::rad angle)       noexcept { return *this = {angle.cos() * magnitude(), angle.sin() * magnitude()}; }

			// VEC & VEC OPERATORS
			Vec2<T>  operator+ (const Vec2<T>& oth) const noexcept { return {x + oth.x, y + oth.y}; }
			Vec2<T>  operator- (const Vec2<T>& oth) const noexcept { return {x - oth.x, y - oth.y}; }
			Vec2<T>  operator* (const Vec2<T>& oth) const noexcept { return {x * oth.x, y * oth.y}; }
			Vec2<T>  operator/ (const Vec2<T>& oth) const noexcept { return {x / oth.x, y / oth.y}; }

			Vec2<T>& operator+=(const Vec2<T>& oth)       noexcept { return *this = *this + oth; }
			Vec2<T>& operator-=(const Vec2<T>& oth)       noexcept { return *this = *this - oth; }
			Vec2<T>& operator*=(const Vec2<T>& oth)       noexcept { return *this = *this * oth; }
			Vec2<T>& operator/=(const Vec2<T>& oth)       noexcept { return *this = *this / oth; }

			bool     operator==(const Vec2<T>& oth) const noexcept { return x == oth.x && y == oth.y; }
			bool     operator!=(const Vec2<T>& oth) const noexcept { return !(*this == oth); }

			static T dot(const Vec2<T>& a, const Vec2<T>& b) noexcept { return a.x * b.x + a.y * b.y; }
			static T distance(const Vec2<T>& a, const Vec2<T>& b) noexcept
				{
				T dx = a.x - b.x;
				T dy = a.y - b.y;
				return std::sqrt(dx * dx + dy * dy);
				}

			// OTHER
			static Vec2<T> lerp(const Vec2<T>& a, const Vec2<T>& b, T t) noexcept
				{
				return {utils::lerp(a.x, b.x, t), utils::lerp(a.y, b.y, t)};
				}
			static Vec2<T> slerp_fast(const Vec2<T>& a, const Vec2<T>& b, T t) noexcept
				{
				return Vec2{utils::lerp(a.x, b.x, t), utils::lerp(a.y, b.y, t)}.normalize()
					* (utils::lerp(a.magnitude(), b.magnitude(), t));
				}
			static Vec2<T> tlerp_fast(const Vec2<T>& a, const Vec2<T>& b, T t) noexcept
				{
				return Vec2{utils::lerp(a.x, b.x, t), utils::lerp(a.y, b.y, t)}.normalize()
					* std::sqrt(utils::lerp(a.magnitude2(), b.magnitude2(), t));
				}
			static Vec2<T> slerp(const Vec2<T>& a, const Vec2<T>& b, T t) noexcept
				{
				T dot = std::clamp(Vec2::dot(a, b), -1.0f, 1.0f);
				T theta = std::acos(dot) * t;
				Vec2<T> relative_vec = (b - a * dot).normal();
				return ((a * std::cos(theta)) + (relative_vec * std::sin(theta)));
				}
		};
	}
