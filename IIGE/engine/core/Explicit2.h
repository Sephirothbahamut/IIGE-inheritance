#pragma once
#include "Vec2.h"
#include "../../utils/cout_utilities.h"

// Declared types in this file are more specific and only implement methods and operators
// which make sense for that specific type

namespace engine::core
	{
	template <typename T>
	class Point2;
	template <typename T>
	class Unit2;
	template <typename T>
	class Vector2;
	}

// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
// ===== ===== ===== ===== ===== ===== =====          Output operators           ===== ===== ===== ===== ===== ===== =====
// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====

namespace utils::cout
	{
	namespace ccu = cout::support;
	template <typename T> inline std::ostream& operator<<(std::ostream& os, const engine::core::Point2<T>& v)  noexcept { return os << ccu::type << "Point-" << v.v; }
	template <typename T> inline std::ostream& operator<<(std::ostream& os, const engine::core::Vector2<T>& v) noexcept { return os << ccu::type << "Vector-" << v.v; }
	template <typename T> inline std::ostream& operator<<(std::ostream& os, const engine::core::Unit2<T>& v)   noexcept { return os << ccu::type << "Unit-" << v.v; }
	}

namespace engine::core
	{
	//fast typenames
	using Point2f  = Point2<float>;
	using Unit2f   = Unit2<float>;
	using Vector2f = Vector2<float>;
	using Point2d  = Point2<double>;
	using Unit2d   = Unit2<double>;
	using Vector2d = Vector2<double>;

	//for the power of friendship
	class Translation2;
	class Rotation2;
	class Scaling2;

	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// ===== ===== ===== ===== ===== ===== =====              Point2<T>              ===== ===== ===== ===== ===== ===== =====
	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	template <typename T>
	class Point2
		{
		friend class Vector2<T>; friend class Unit2<T>;
		friend class Translation2; friend class Rotation2; friend class Scaling2;

		private:
			Vec2<T> v;

		public:
			Point2<T>()          noexcept = default;
			Point2<T>(T x, T y)  noexcept : v(x, y) {};
			Point2<T>(T xy)      noexcept : v(xy) {};
			Point2<T>(Vec2<T> v) noexcept : v(v) {};
			operator Vec2<T>()   noexcept { return v; }

			// ACCESS
			T get_x() const noexcept { return v.x; };
			void set_x(const T& value) noexcept { v.x = value; };
			T get_y() const noexcept { return v.y; };
			void set_y(const T& value) noexcept { v.y = value; };
			__declspec(property(get = get_x, put = set_x)) T x;
			__declspec(property(get = get_y, put = set_y)) T y;

			// VEC & VEC OPERATORS
			Point2 <T> operator+(const Vector2<T>& oth) const noexcept { return { v + oth.v }; }
			Point2 <T> operator-(const Vector2<T>& oth) const noexcept { return { v - oth.v }; }
			Vector2<T> operator-(const Point2 <T>& oth) const noexcept { return { v - oth.v }; }
			static T distance(const Point2<T>& a, const Point2<T>& b) noexcept { return Vec2<T>::distance(a.v, b.v); }

			Point2 <T>& operator+=(const Vector2<T>& oth) noexcept { return *this = *this + oth; }
			Point2 <T>& operator-=(const Vector2<T>& oth) noexcept { return *this = *this - oth; }

			//OTHER
			//slides quote: "Multiplying a point with a scalar? Summing two points? Are these operations even legal?"
			//Well... I'm cheating, it's been a Vec2 inside all this time ;)
			static Point2<T> lerp(const Point2<T>& a, const Point2<T>& b, T t) noexcept { return { Vec2<T>::lerp(a.v, b.v, t) }; }

			friend std::ostream& utils::cout::operator<<(std::ostream& os, const Point2<T>& v) noexcept;
		};

	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// ===== ===== ===== ===== ===== ===== =====             Vector2<T>              ===== ===== ===== ===== ===== ===== =====
	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	template <typename T>
	class Vector2
		{
		friend class Point2<T>; friend class Unit2<T>;
		friend class Translation2; friend class Rotation2; friend class Scaling2;

		private:
			Vec2<T> v;

		public:
			Vector2<T>() noexcept = default;
			Vector2<T>(T x, T y) noexcept : v(x, y) {};
			Vector2<T>(T xy) noexcept : v(xy) {};
			Vector2<T>(Vec2<T> v) noexcept : v(v) {};
			static Vector2<T> rr() noexcept { return { Vec2<T>.rr() }; }
			static Vector2<T> ll() noexcept { return { Vec2<T>.ll() }; }
			static Vector2<T> up() noexcept { return { Vec2<T>.up() }; }
			static Vector2<T> dw() noexcept { return { Vec2<T>.dw() }; }
			static Vector2<T> right() noexcept { return rr(); }
			static Vector2<T> left() noexcept { return ll(); }
			static Vector2<T> down() noexcept { return dw(); }
			operator Vec2<T>() noexcept { return v; }

			// ACCESS
			T get_x() const noexcept { return v.x; };
			void set_x(const T& value) noexcept { v.x = value; };
			T get_y() const noexcept { return v.y; };
			void set_y(const T& value) noexcept { v.y = value; };
			__declspec(property(get = get_x, put = set_x)) T x;
			__declspec(property(get = get_y, put = set_y)) T y;

			T magnitude() const noexcept { return v.magnitude(); }
			T magnitude2() const noexcept { return v.magnitude2(); }

			Vector2<T>  normal() const noexcept { return { v.normal() }; }
			Vector2<T>& normalize() noexcept { v.normalize(); return *this; }
			Unit2  <T>  unit() const noexcept { return { v }; }

			// OPERATORS
			Vector2<T>  operator-() const noexcept { return { -v }; }

			// VEC & SCALAR OPERATORS
			Vector2<T>  operator* (const T n) const noexcept { return { v * n }; }
			Vector2<T>  operator/ (const T n) const noexcept { return { v / n }; }
			Vector2<T>& operator*=(const T n) noexcept { return *this = *this * n; }
			Vector2<T>& operator/=(const T n) noexcept { return *this = *this / n; }

			// VEC & VEC OPERATORS
			Vector2<T> operator+(const Vector2<T>& oth) const noexcept { return { v + oth.v }; }
			Vector2<T> operator-(const Vector2<T>& oth) const noexcept { return { v - oth.v }; }
			static T dot(const Vector2<T>& a, const Vector2<T>& b) noexcept { return Vec2<T>::dot(a.v, b.v); }

			Vector2<T>& operator+=(const Vector2<T>& oth) noexcept { return *this = *this + oth; }
			Vector2<T>& operator-=(const Vector2<T>& oth) noexcept { return *this = *this - oth; }
			Vector2<T>& operator%=(const Vector2<T>& oth) noexcept { return *this = *this % oth; }

			// OTHER
			static Vector2<T> lerp(const Vector2<T>& a, const Vector2<T>& b, T t) noexcept { return { Vec2<T>::lerp(a.v, b.v, t) }; }

			friend std::ostream& utils::cout::operator<<(std::ostream& os, const Vector2<T>& v) noexcept;
		};

	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// ===== ===== ===== ===== ===== ===== =====              Unit2<T>               ===== ===== ===== ===== ===== ===== =====
	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	template <typename T>
	class Unit2
		{
		friend class Point2<T>; friend class Vector2<T>;
		friend class Translation2; friend class Rotation2; friend class Scaling2;

		private:
			Vec2<T> v;

		public:
			Unit2<T>() noexcept = default;
			Unit2<T>(Vec2<T> v) noexcept : v(v.normalize()) {};
			static Vector2<T> rr() noexcept { return { Vec2<T>.rr() }; }
			static Vector2<T> ll() noexcept { return { Vec2<T>.ll() }; }
			static Vector2<T> up() noexcept { return { Vec2<T>.up() }; }
			static Vector2<T> dw() noexcept { return { Vec2<T>.dw() }; }
			static Vector2<T> right() noexcept { return rr(); }
			static Vector2<T> left() noexcept { return ll(); }
			static Vector2<T> down() noexcept { return dw(); }
			operator Vec2<T>() noexcept { return v; }

			// ACCESS
			T get_x() const noexcept { return v.x; };
			T get_y() const noexcept { return v.y; };
			__declspec(property(get = get_x)) T x;
			__declspec(property(get = get_y)) T y;

			// VEC & SCALAR OPERATORS
			Vector2<T>  operator* (const T n) const noexcept { return { v * n }; }
			Vector2<T>  operator/ (const T n) const noexcept { return { v / n }; }

			// OTHER
			bool verify() noexcept { return (1 - v.magnitude()) < (std::numeric_limits<T>::min)() * 10; }
			void adjust() noexcept { if (!verify()) { v.normalize(); } }
			static Vector2<T>       lerp(const Unit2<T>& a, const Unit2<T>& b, T t) noexcept { return { Vec2<T>::lerp(a.v, b.v, t) }; }
			static Unit2  <T>      slerp(const Unit2<T>& a, const Unit2<T>& b, T t) noexcept { return { Vec2<T>::slerp(a.v, b.v, t) }; }
			static Unit2  <T> slerp_fast(const Unit2<T>& a, const Unit2<T>& b, T t) noexcept { return Unit2<T>{ Vec2<T>::lerp(a.v, b.v, t) }; }

			friend std::ostream& utils::cout::operator<<(std::ostream& os, const Unit2<T>& v) noexcept;
		};
	}
