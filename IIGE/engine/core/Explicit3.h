#pragma once

#include <utils/cout_utilities.h>

#include "Vec3.h"

// Declared types in this file are more specific and only implement methods and operators
// which make sense for that specific type

namespace engine::core
	{
	template <typename T>
	class Point3;
	template <typename T>
	class Unit3;
	template <typename T>
	class Vector3;
	}

// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
// ===== ===== ===== ===== ===== ===== =====          Output operators           ===== ===== ===== ===== ===== ===== =====
// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====

namespace utils::cout
	{
	namespace ccu = cout::support;
	template <typename T> inline std::ostream& operator<<(std::ostream& os, const engine::core::Point3<T>& v)  noexcept { return os << ccu::type << "Point-" << v.v; }
	template <typename T> inline std::ostream& operator<<(std::ostream& os, const engine::core::Vector3<T>& v) noexcept { return os << ccu::type << "Vector-" << v.v; }
	template <typename T> inline std::ostream& operator<<(std::ostream& os, const engine::core::Unit3<T>& v)   noexcept { return os << ccu::type << "Unit-" << v.v; }
	}

namespace engine::core
	{
	//fast typenames
	using Point3f  = Point3<float>;
	using Unit3f   = Unit3<float>;
	using Vector3f = Vector3<float>;
	using Point3d  = Point3<double>;
	using Unit3d   = Unit3<double>;
	using Vector3d = Vector3<double>;

	//for the power of friendship
	class Translation3;
	class Rotation3;
	class Scaling3;

	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// ===== ===== ===== ===== ===== ===== =====              Point3<T>              ===== ===== ===== ===== ===== ===== =====
	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	template <typename T>
	class Point3
		{
		friend class Vector3<T>; friend class Unit3<T>;
		friend class Translation3; friend class Rotation3; friend class Scaling3;

		private:
			Vec3<T> v;

		public:
			Point3<T>()              noexcept = default;
			Point3<T>(T x, T y, T z) noexcept : v(x, y, z) {};
			Point3<T>(T xyz)         noexcept : v(xyz) {};
			Point3<T>(Vec3<T> v)     noexcept : v(v) {};
			operator Vec3<T>()       noexcept { return v; }

			// ACCESS
			T get_x() const noexcept { return v.x; };
			void set_x(const T& value) noexcept { v.x = value; };
			T get_y() const noexcept { return v.y; };
			void set_y(const T& value) noexcept { v.y = value; };
			T get_z() const noexcept { return v.z; };
			void set_z(const T& value) noexcept { v.z = value; };
			__declspec(property(get = get_x, put = set_x)) T x;
			__declspec(property(get = get_y, put = set_y)) T y;
			__declspec(property(get = get_z, put = set_z)) T z;

			// VEC & VEC OPERATORS
			Point3 <T> operator+(const Vector3<T>& oth) const noexcept { return { v + oth.v }; }
			Point3 <T> operator-(const Vector3<T>& oth) const noexcept { return { v - oth.v }; }
			Vector3<T> operator-(const Point3 <T>& oth) const noexcept { return { v - oth.v }; }
			static T distance(const Point3<T>& a, const Point3<T>& b) noexcept { return Vec3<T>::distance(a.v, b.v); }

			Point3 <T>& operator+=(const Vector3<T>& oth) noexcept { return *this = *this + oth; }
			Point3 <T>& operator-=(const Vector3<T>& oth) noexcept { return *this = *this - oth; }

			//OTHER
			//slides quote: "Multiplying a point with a scalar? Summing two points? Are these operations even legal?"
			//Well... I'm cheating, it's been a Vec3 inside all this time ;)
			static Point3<T> lerp(const Point3<T>& a, const Point3<T>& b, T t) noexcept { return { Vec3<T>::lerp(a.v, b.v, t) }; }
		
			friend std::ostream& utils::cout::operator<<(std::ostream& os, const Point3<T>& v) noexcept;
		};

	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// ===== ===== ===== ===== ===== ===== =====             Vector3<T>              ===== ===== ===== ===== ===== ===== =====
	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	template <typename T>
	class Vector3
		{
		friend class Point3<T>; friend class Unit3<T>;
		friend class Translation3; friend class Rotation3; friend class Scaling3;

		private:
			Vec3<T> v;

		public:
			// CONSTRUCTORS
			Vector3<T>() noexcept = default;
			Vector3<T>(T x, T y, T z) noexcept : v(x, y, z) {};
			Vector3<T>(T xyz) noexcept : v(xyz) {};
			Vector3<T>(Vec3<T> v) noexcept : v(v) {};
			static Vector3<T> rr() noexcept { return { Vec3<T>::rr() }; }
			static Vector3<T> ll() noexcept { return { Vec3<T>::ll() }; }
			static Vector3<T> up() noexcept { return { Vec3<T>::up() }; }
			static Vector3<T> dw() noexcept { return { Vec3<T>::dw() }; }
			static Vector3<T> fw() noexcept { return { Vec3<T>::fw() }; }
			static Vector3<T> bw() noexcept { return { Vec3<T>::bw() }; }
			static Vector3<T> right() noexcept { return rr(); }
			static Vector3<T> left() noexcept { return ll(); }
			static Vector3<T> down() noexcept { return dw(); }
			static Vector3<T> forwards() noexcept { return fw(); }
			static Vector3<T> backwards() noexcept { return bw(); }
			operator Vec3<T>() noexcept { return v; }

			// ACCESS
			T get_x() const noexcept { return v.x; };
			void set_x(const T& value) noexcept { v.x = value; };
			T get_y() const noexcept { return v.y; };
			void set_y(const T& value) noexcept { v.y = value; };
			T get_z() const noexcept { return v.z; };
			void set_z(const T& value) noexcept { v.z = value; };
			__declspec(property(get = get_x, put = set_x)) T x;
			__declspec(property(get = get_y, put = set_y)) T y;
			__declspec(property(get = get_z, put = set_z)) T z;

			// OPERATORS
			Vector3<T>  operator-() const noexcept { return { -v }; }

			// VEC & SCALAR OPERATORS
			Vector3<T>  operator+ (const T n) const noexcept { return { v + n }; }
			Vector3<T>  operator- (const T n) const noexcept { return { v - n }; }
			Vector3<T>& operator+=(const T n) noexcept { return *this = *this + n; }
			Vector3<T>& operator-=(const T n) noexcept { return *this = *this - n; }
			Vector3<T>  operator* (const T n) const noexcept { return { v * n }; }
			Vector3<T>  operator/ (const T n) const noexcept { return { v / n }; }
			Vector3<T>& operator*=(const T n) noexcept { return *this = *this * n; }
			Vector3<T>& operator/=(const T n) noexcept { return *this = *this / n; }

			// VEC & VEC OPERATORS
			Vector3<T> operator+(const Vector3<T>& oth) const noexcept { return { v + oth.v }; }
			Vector3<T> operator-(const Vector3<T>& oth) const noexcept { return { v - oth.v }; }
			Vector3<T> operator%(const Vector3<T>& oth) const noexcept { return { v % oth.v }; }
			static Vector3<T> cross(const Vector3<T>& a, const Vector3<T>& b) noexcept { return { Vec3<T>::cross(a.v, b.v) }; }
			static T dot(const Vector3<T>& a, const Vector3<T>& b) noexcept { return Vec3<T>::dot(a.v, b.v); }

			Vector3<T>& operator+=(const Vector3<T>& oth) noexcept { return *this = *this + oth; }
			Vector3<T>& operator-=(const Vector3<T>& oth) noexcept { return *this = *this - oth; }
			Vector3<T>& operator%=(const Vector3<T>& oth) noexcept { return *this = *this % oth; }

			// OTHER
			T magnitude() const noexcept { return v.magnitude(); }
			T magnitude2() const noexcept { return v.magnitude2(); }
			Vector3<T>  normal() const noexcept { return { v.normal() }; }
			Vector3<T>& normalize() noexcept { v.normalize(); return *this; }
			Unit3  <T>  unit() const noexcept { return { v }; }

			static Vector3<T>  lerp(const Vector3<T>& a, const Vector3<T>& b, T t) noexcept { return { Vec3<T>::lerp(a.v, b.v, t) }; }
			static Vector3<T> slerp(const Vector3<T>& a, const Vector3<T>& b, T t) noexcept { return { Vec3<T>::slerp(a.v, b.v, t) }; }
		
			friend std::ostream& utils::cout::operator<<(std::ostream& os, const Vector3<T>& v) noexcept;
		};

	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// ===== ===== ===== ===== ===== ===== =====              Unit3<T>               ===== ===== ===== ===== ===== ===== =====
	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	template <typename T>
	class Unit3
		{
		friend class Point3<T>; friend class Vector3<T>;
		friend class Translation3; friend class Rotation3; friend class Scaling3;

		private:
			Vec3<T> v;

		public:
			// CONSTRUCTORS
			Unit3<T>() = default;
			Unit3<T>(Vec3<T> v) : v(v.normalize()) {};
			static Unit3<T> rr() noexcept { return { Vec3<T>::rr() }; }
			static Unit3<T> ll() noexcept { return { Vec3<T>::ll() }; }
			static Unit3<T> up() noexcept { return { Vec3<T>::up() }; }
			static Unit3<T> dw() noexcept { return { Vec3<T>::dw() }; }
			static Unit3<T> fw() noexcept { return { Vec3<T>::fw() }; }
			static Unit3<T> bw() noexcept { return { Vec3<T>::bw() }; }
			static Unit3<T> right() noexcept { return rr(); }
			static Unit3<T> left() noexcept { return ll(); }
			static Unit3<T> down() noexcept { return dw(); }
			static Unit3<T> forwards() noexcept { return fw(); }
			static Unit3<T> backwards() noexcept { return bw(); }
			operator Vec3<T>() noexcept { return v; }

			// ACCESS
			T get_x() const noexcept { return v.x; };
			T get_y() const noexcept { return v.y; };
			T get_z() const noexcept { return v.z; };
			__declspec(property(get = get_x)) T x;
			__declspec(property(get = get_y)) T y;
			__declspec(property(get = get_z)) T z;

			// VEC & SCALAR OPERATORS
			Vector3<T>  operator* (const T n) const noexcept { return { v * n }; }
			Vector3<T>  operator/ (const T n) const noexcept { return { v / n }; }

			// OTHER
			bool verify() noexcept { return (1 - v.magnitude()) < (std::numeric_limits<T>::min)() * 10; }
			void adjust() noexcept { if (!verify()) { v.normalize(); } }
			static Vector3<T>       lerp(const Unit3<T>& a, const Unit3<T>& b, T t) noexcept { return { Vec3<T>::lerp(a.v, b.v, t) }; }
			static Unit3  <T>      slerp(const Unit3<T>& a, const Unit3<T>& b, T t) noexcept { return { Vec3<T>::slerp(a.v, b.v, t) }; }
			static Unit3  <T> slerp_fast(const Unit3<T>& a, const Unit3<T>& b, T t) noexcept { return { Vec3<T>::lerp(a.v, b.v, t) }; }
		
			friend std::ostream& utils::cout::operator<<(std::ostream& os, const Unit3<T>& v) noexcept;
		};
	}
