#pragma once
#include <ostream>
#include <stdint.h>
#include <cmath> //trigonometry
#include <algorithm> //clamp
#include <variant> //Angle class

#include "../cout_utilities.h"
#include "constants.h"

namespace utils::angle
	{
	float deg_to_rad(float d) noexcept;
	float rad_to_deg(float r) noexcept;

	class deg;
	class rad;

	namespace literals
		{
		deg operator""_deg(long double value) noexcept;
		deg operator""_deg(unsigned long long value) noexcept;
		rad operator""_rad(long double value) noexcept;
		rad operator""_rad(unsigned long long value) noexcept;
		}

	using namespace constants;
	using namespace literals;

	namespace trigonometry
		{
		float sin(const rad a) noexcept;
		float cos(const rad a) noexcept;
		float tan(const rad a) noexcept;
		float sin(const deg a) noexcept;
		float cos(const deg a) noexcept;
		float tan(const deg a) noexcept;
		rad asin(float n) noexcept;
		rad acos(float n) noexcept;
		rad atan(float n) noexcept;
		rad atan2(float a, float b) noexcept;
		}

	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// ===== ===== ===== ===== ===== ===== =====                 DEG                 ===== ===== ===== ===== ===== ===== =====
	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	class deg
		{
		friend class rad;
		friend deg literals::operator""_deg(long double value) noexcept;
		friend deg literals::operator""_deg(unsigned long long value) noexcept;
		private:

		public:
			deg() noexcept;
			deg(float val) noexcept;
			float value;

			deg(const rad r) noexcept;
			rad to_rad() const noexcept;

			void clamp() noexcept;

			deg  operator+ (const deg oth) const noexcept;
			deg  operator- (const deg oth) const noexcept;
			deg& operator+=(const deg oth) noexcept;
			deg& operator-=(const deg oth) noexcept;
			deg  operator+ (const rad oth) const noexcept;
			deg  operator- (const rad oth) const noexcept;
			deg& operator+=(const rad oth) noexcept;
			deg& operator-=(const rad oth) noexcept;

			deg  operator* (float oth) const noexcept;
			deg  operator/ (float oth) const noexcept;
			deg& operator*=(float oth) noexcept;
			deg& operator/=(float oth) noexcept;

			deg  operator- () const noexcept;

			float sin() const noexcept;
			float cos() const noexcept;
			float tan() const noexcept;
			static float sin(const deg a) noexcept;
			static float cos(const deg a) noexcept;
			static float tan(const deg a) noexcept;
		};


	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// ===== ===== ===== ===== ===== ===== =====                 RAD                 ===== ===== ===== ===== ===== ===== =====
	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	class rad
		{
		friend class deg;
		friend rad literals::operator""_rad(long double value) noexcept;
		friend rad literals::operator""_rad(unsigned long long value) noexcept;
		private:

		public:
			rad() noexcept;
			rad(float value) noexcept;
			float value;

			rad(const deg d) noexcept;
			deg to_deg() const noexcept;

			void clamp() noexcept;

			rad  operator+ (const rad oth) const noexcept;
			rad  operator- (const rad oth) const noexcept;
			rad& operator+=(const rad oth) noexcept;
			rad& operator-=(const rad oth) noexcept;
			rad  operator+ (const deg oth) const noexcept;
			rad  operator- (const deg oth) const noexcept;
			rad& operator+=(const deg oth) noexcept;
			rad& operator-=(const deg oth) noexcept;

			rad  operator* (float oth) const noexcept;
			rad  operator/ (float oth) const noexcept;
			rad& operator*=(float oth) noexcept;
			rad& operator/=(float oth) noexcept;

			rad  operator- () const noexcept;

			float sin() const noexcept;
			float cos() const noexcept;
			float tan() const noexcept;
			static float sin(const rad a) noexcept;
			static float cos(const rad a) noexcept;
			static float tan(const rad a) noexcept;
			static rad asin(float n) noexcept;
			static rad acos(float n) noexcept;
			static rad atan(float n) noexcept;
			static rad atan2(float a, float b) noexcept;
		};


	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// ===== ===== ===== ===== ===== ===== =====                ANGLE                ===== ===== ===== ===== ===== ===== =====
	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	//Not sure if good idea or not. Bigger overhead, way more intuitive.
	class Angle
		{
		private:
			std::variant<deg, rad> value;
			void make_rad() noexcept;
			void make_deg() noexcept;

		public:
			Angle() noexcept;
			Angle(const deg d) noexcept;
			Angle(const rad r) noexcept;
			rad get_rad() noexcept;
			deg get_deg() noexcept;

			Angle  operator+ (const Angle& oth) const noexcept;
			Angle  operator- (const Angle& oth) const noexcept;
			Angle& operator+=(const Angle& oth) noexcept;
			Angle& operator-=(const Angle& oth) noexcept;
			Angle  operator+ (const rad oth) const noexcept;
			Angle  operator- (const rad oth) const noexcept;
			Angle& operator+=(const rad oth) noexcept;
			Angle& operator-=(const rad oth) noexcept;
			Angle  operator+ (const deg oth) const noexcept;
			Angle  operator- (const deg oth) const noexcept;
			Angle& operator+=(const deg oth) noexcept;
			Angle& operator-=(const deg oth) noexcept;

			Angle  operator* (float oth) const noexcept;
			Angle  operator/ (float oth) const noexcept;
			Angle& operator*=(float oth) noexcept;
			Angle& operator/=(float oth) noexcept;

			Angle  operator- () const noexcept;

			float sin() noexcept;
			float cos() noexcept;
			float tan() noexcept;
			static float sin(Angle& a) noexcept;
			static float cos(Angle& a) noexcept;
			static float tan(Angle& a) noexcept;
			static Angle asin(float n) noexcept;
			static Angle acos(float n) noexcept;
			static Angle atan(float n) noexcept;
			static Angle atan2(float a, float b) noexcept;

		};

	}

namespace utils::cout
	{
	std::ostream& operator<<(std::ostream& os, const angle::Angle& a) noexcept;
	std::ostream& operator<<(std::ostream& os, const angle::rad& r) noexcept;
	std::ostream& operator<<(std::ostream& os, const angle::deg& d) noexcept;
	}
namespace utils::cout_fancy
	{
	std::ostream& operator<<(std::ostream& os, const angle::Angle& a) noexcept;
	std::ostream& operator<<(std::ostream& os, const angle::rad& r) noexcept;
	std::ostream& operator<<(std::ostream& os, const angle::deg& d) noexcept;
	}



