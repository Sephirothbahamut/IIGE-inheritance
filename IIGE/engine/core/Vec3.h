#pragma once
#include <ostream>
#include <algorithm>

#include <utils/math/math.h>
#include <utils/cout_utilities.h>

#include "Vec.h"

namespace engine::core
	{
	template <typename T> class Vec3;
	}

namespace utils::cout
	{
	template <typename T>
	inline std::ostream& operator<<(std::ostream& os, const engine::core::Vec3<T>& vec) noexcept 
		{
		namespace ccu = cout::support;
		os << ccu::type << "Vec3" << ccu::brace << "(" << ccu::value << vec.x << ccu::separ << ", " << ccu::value << vec.y << ccu::separ << ", " << ccu::value << vec.z << ccu::brace << ")";
		}
	}

namespace engine::core
	{
	//fast typenames
	using Vec3i   = Vec3<int>;
	using Vec3i8  = Vec3<int8_t>;
	using Vec3i16 = Vec3<int16_t>;
	using Vec3i32 = Vec3<int32_t>;
	using Vec3i64 = Vec3<int64_t>;

	using Vec3u   = Vec3<unsigned>;
	using Vec3u8  = Vec3<uint8_t>;
	using Vec3u16 = Vec3<uint16_t>;
	using Vec3u32 = Vec3<uint32_t>;
	using Vec3u46 = Vec3<uint64_t>;

	using Vec3s = Vec3<size_t>;
	using Vec3f = Vec3<float>;
	using Vec3d = Vec3<double>;

	template <typename T>
	class Vec3
		{
		public:
			// CONSTRUCTORS
			Vec3<T>()              noexcept = default;
			Vec3<T>(T x, T y, T z) noexcept : x(x), y(y), z(z) {};
			Vec3<T>(T xyz)         noexcept : x(xyz), y(xyz), z(xyz) {}
			static Vec3<T> rr() noexcept { return {T{ 1}, T{ 0}, T{ 0}}; }
			static Vec3<T> ll() noexcept { return {T{-1}, T{ 0}, T{ 0}}; }
			static Vec3<T> up() noexcept { return {T{ 0}, T{ 1}, T{ 0}}; }
			static Vec3<T> dw() noexcept { return {T{ 0}, T{-1}, T{ 0}}; }
			static Vec3<T> fw() noexcept { return {T{ 0}, T{ 0}, T{ 1}}; }
			static Vec3<T> bw() noexcept { return {T{ 0}, T{ 0}, T{-1}}; }
			static Vec3<T> right()     noexcept { return rr(); }
			static Vec3<T> left()      noexcept { return ll(); }
			static Vec3<T> down()      noexcept { return dw(); }
			static Vec3<T> forwards()  noexcept { return fw(); }
			static Vec3<T> backwards() noexcept { return bw(); }

			T x = 0, y = 0, z = 0;

			T magnitude2()   const noexcept { return x * x + y * y + z * z; }
			T magnitude()    const noexcept { return std::sqrt(magnitude2()); }
			Vec3<T> normal() const noexcept { return magnitude2() ? *this / magnitude() : *this; }
			Vec3<T>& normalize()   noexcept { return *this = normal(); }

			// OPERATORS
			Vec3<T>  operator-() const noexcept { return {-x, -y, -z}; }

			// VEC & SCALAR OPERATORS
			Vec3<T>  operator++()    noexcept { Vec3<T> old = *this; operator++(); return old; }
			Vec3<T>  operator--()    noexcept { Vec3<T> old = *this; operator--(); return old; }
			Vec3<T>& operator++(int) noexcept { return *this += T(1); }
			Vec3<T>& operator--(int) noexcept { return *this -= T(1); }

			Vec3<T>  operator+ (const T n) const noexcept { return {normal() * (magnitude() + n)}; }
			Vec3<T>  operator- (const T n) const noexcept { return {normal() * (magnitude() - n)}; }
			Vec3<T>  operator* (const T n) const noexcept { return {x * n, y * n, z * n}; }
			Vec3<T>  operator/ (const T n) const noexcept { return {x / n, y / n, z / n}; }

			Vec3<T>& operator+=(const T n) noexcept { return *this = *this + n; }
			Vec3<T>& operator-=(const T n) noexcept { return *this = *this - n; }
			Vec3<T>& operator*=(const T n) noexcept { return *this = *this * n; }
			Vec3<T>& operator/=(const T n) noexcept { return *this = *this / n; }
			Vec3<T>& operator= (const T n) noexcept { return normalize() *= n; }

			// VEC & VEC OPERATORS
			Vec3<T>  operator+ (const Vec3<T>& oth) const noexcept { return {x + oth.x, y + oth.y, z + oth.z}; }
			Vec3<T>  operator- (const Vec3<T>& oth) const noexcept { return {x - oth.x, y - oth.y, z - oth.z}; }
			Vec3<T>  operator* (const Vec3<T>& oth) const noexcept { return {x * oth.x, y * oth.y, z * oth.z}; }
			Vec3<T>  operator/ (const Vec3<T>& oth) const noexcept { return {x / oth.x, y / oth.y, z / oth.z}; }
			Vec3<T>  operator% (const Vec3<T>& oth) const noexcept //cross operator
				{
				T tmp_x = (y * oth.z - z * oth.y);
				T tmp_y = (x * oth.z - z * oth.x);
				T tmp_z = (x * oth.y - y * oth.x);
				return {tmp_x, tmp_y, tmp_z};
				}

			Vec3<T>& operator+=(const Vec3<T>& oth)       noexcept { return *this = *this + oth; }
			Vec3<T>& operator-=(const Vec3<T>& oth)       noexcept { return *this = *this - oth; }
			Vec3<T>& operator*=(const Vec3<T>& oth)       noexcept { return *this = *this * oth; }
			Vec3<T>& operator/=(const Vec3<T>& oth)       noexcept { return *this = *this / oth; }
			Vec3<T>& operator%=(const Vec3<T>& oth)       noexcept { return *this = *this % oth; }

			bool     operator==(const Vec3<T>& oth) const noexcept { return x == oth.x && y == oth.y && z == oth.z; }
			bool     operator!=(const Vec3<T>& oth) const noexcept { return !(*this == oth); }

			static Vec3<T> cross(const Vec3<T>& a, const Vec3<T>& b) noexcept { return a % b; }
			static T dot(const Vec3<T>& a, const Vec3<T>& b) noexcept { return a.x * b.x + a.y * b.y + a.z * b.z; }
			static T distance2(const Vec3<T>& a, const Vec3<T>& b) noexcept
				{
				T dx = a.x - b.x;
				T dy = a.y - b.y;
				T dz = a.z - b.z;
				return dx * dx + dy * dy + dz * dz;
				}
			static T distance(const Vec3<T>& a, const Vec3<T>& b) noexcept { return std::sqrt(distance2(a, b)); }

			// OTHER
			static Vec3<T> lerp(const Vec3<T>& a, const Vec3<T>& b, T t) noexcept
				{
				return {utils::lerp(a.x, b.x, t), utils::lerp(a.y, b.y, t), utils::lerp(a.z, b.z, t)};
				}
			static Vec3<T> slerp_fast(const Vec3<T>& a, const Vec3<T>& b, T t) noexcept
				{
				return Vec3{utils::lerp(a.x, b.x, t), utils::lerp(a.y, b.y, t), utils::lerp(a.z, b.z, t)}.normalize()
					* (utils::lerp(a.magnitude(), b.magnitude(), t));
				}
			static Vec3<T> tlerp_fast(const Vec3<T>& a, const Vec3<T>& b, T t) noexcept
				{
				return Vec3{utils::lerp(a.x, b.x, t), utils::lerp(a.y, b.y, t), utils::lerp(a.z, b.z, t)}.normalize()
					* std::sqrt(utils::lerp(a.magnitude2(), b.magnitude2(), t));
				}
			static Vec3<T> slerp(const Vec3<T>& a, const Vec3<T>& b, T t) noexcept
				{
				T dot = std::clamp(Vec3::dot(a, b), T(-1), T(1));
				T theta = std::acos(dot) * t;
				Vec3<T> relative_vec = (b - a * dot).normal();
				return ((a * std::cos(theta)) + (relative_vec * std::sin(theta)));
				}

		};
	}