#include "angle.h"

namespace utils::angle
	{
	using namespace constants;
	using namespace literals;

	float deg_to_rad(float d) noexcept { return d / (180 / PI); }
	float rad_to_deg(float r) noexcept { return r * (180 / PI); }

	deg literals::operator""_deg(long double value) noexcept { return deg{ float(value) }; }
	deg literals::operator""_deg(unsigned long long value) noexcept { return deg{ float(value) }; }
	rad literals::operator""_rad(long double value) noexcept { return rad{ float(value) }; }
	rad literals::operator""_rad(unsigned long long value) noexcept { return rad{ float(value) }; }

	float trigonometry::sin(const rad a) noexcept { return a.sin(); }
	float trigonometry::cos(const rad a) noexcept { return a.cos(); }
	float trigonometry::tan(const rad a) noexcept { return a.tan(); }
	float trigonometry::sin(const deg a) noexcept { return rad{ a }.sin(); }
	float trigonometry::cos(const deg a) noexcept { return rad{ a }.cos(); }
	float trigonometry::tan(const deg a) noexcept { return rad{ a }.tan(); }
	rad trigonometry::asin(float n) noexcept { return rad::asin(n); }
	rad trigonometry::acos(float n) noexcept { return rad::acos(n); }
	rad trigonometry::atan(float n) noexcept { return rad::atan(n); }
	rad trigonometry::atan2(float a, float b) noexcept { return rad::atan2(a, b); }

	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// ===== ===== ===== ===== ===== ===== =====                 DEG                 ===== ===== ===== ===== ===== ===== =====
	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====

	deg::deg() noexcept : value(0.f) {}
	deg::deg(float value) noexcept : value(value) {}
	void deg::clamp() noexcept { value = std::clamp(value, 0.f, 2.f * PI); }

	deg::deg(rad r) noexcept : value(rad_to_deg(r.value)) {}
	rad deg::to_rad() const noexcept { return rad{ *this }; }

	deg  deg::operator+ (const deg oth) const noexcept { return deg{ value + oth.value }; }
	deg  deg::operator- (const deg oth) const noexcept { return deg{ value - oth.value }; }
	deg& deg::operator+=(const deg oth)       noexcept { return *this = *this + oth; }
	deg& deg::operator-=(const deg oth)       noexcept { return *this = *this - oth; }
	deg  deg::operator+ (const rad oth) const noexcept { return deg{ value + rad_to_deg(oth.value) }; }
	deg  deg::operator- (const rad oth) const noexcept { return deg{ value - rad_to_deg(oth.value) }; }
	deg& deg::operator+=(const rad oth)       noexcept { return *this = *this + oth; }
	deg& deg::operator-=(const rad oth)       noexcept { return *this = *this - oth; }

	deg  deg::operator* (float oth) const noexcept { return deg{ value * oth }; }
	deg  deg::operator/ (float oth) const noexcept { return deg{ value / oth }; }
	deg& deg::operator*=(float oth)       noexcept { return *this = *this * oth; }
	deg& deg::operator/=(float oth)       noexcept { return *this = *this / oth; }

	deg  deg::operator- ()          const noexcept { return {-value}; };

	float deg::sin() const noexcept { return std::sin(deg_to_rad(value)); }
	float deg::cos() const noexcept { return std::cos(deg_to_rad(value)); }
	float deg::tan() const noexcept { return std::tan(deg_to_rad(value)); }
	float deg::sin(const deg a) noexcept { return a.sin(); }
	float deg::cos(const deg a) noexcept { return a.cos(); }
	float deg::tan(const deg a) noexcept { return a.tan(); }

	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// ===== ===== ===== ===== ===== ===== =====                 RAD                 ===== ===== ===== ===== ===== ===== =====
	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====

	rad::rad() noexcept : value(0.f) {}
	rad::rad(float value) noexcept : value(value) {}
	void rad::clamp() noexcept { value = std::clamp(value, 0.f, 260.f); }

	rad::rad(deg d) noexcept : value(deg_to_rad(d.value)) {}
	deg rad::to_deg() const noexcept { return deg{ *this }; }

	rad  rad::operator+ (const rad oth) const noexcept { return { value + oth.value }; }
	rad  rad::operator- (const rad oth) const noexcept { return { value - oth.value }; }
	rad& rad::operator+=(const rad oth)       noexcept { return *this = *this + oth; }
	rad& rad::operator-=(const rad oth)       noexcept { return *this = *this - oth; }
	rad  rad::operator+ (const deg oth) const noexcept { return { value + deg_to_rad(oth.value) }; }
	rad  rad::operator- (const deg oth) const noexcept { return { value - deg_to_rad(oth.value) }; }
	rad& rad::operator+=(const deg oth)       noexcept { return *this = *this + oth; }
	rad& rad::operator-=(const deg oth)       noexcept { return *this = *this - oth; }

	rad  rad::operator* (float oth) const noexcept { return rad{ value * oth }; }
	rad  rad::operator/ (float oth) const noexcept { return rad{ value / oth }; }
	rad& rad::operator*=(float oth)       noexcept { return *this = *this * oth; }
	rad& rad::operator/=(float oth)       noexcept { return *this = *this / oth; }

	rad  rad::operator- ()          const noexcept { return {value - PI}; };

	float rad::sin() const noexcept { return std::sin(value); }
	float rad::cos() const noexcept { return std::cos(value); }
	float rad::tan() const noexcept { return std::tan(value); }
	float rad::sin(const rad a) noexcept { return a.sin(); }
	float rad::cos(const rad a) noexcept { return a.cos(); }
	float rad::tan(const rad a) noexcept { return a.tan(); }
	rad rad::asin(float n) noexcept { return { std::asin(n) }; }
	rad rad::acos(float n) noexcept { return { std::acos(n) }; }
	rad rad::atan(float n) noexcept { return { std::atan(n) }; }
	rad rad::atan2(float a, float b) noexcept { return{ std::atan2(a, b) }; }

	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// ===== ===== ===== ===== ===== ===== =====                ANGLE                ===== ===== ===== ===== ===== ===== =====
	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====

	void Angle::make_rad() noexcept { if (std::holds_alternative<deg>(value)) { value = rad{ std::get<deg>(value) }; } }
	void Angle::make_deg() noexcept { if (std::holds_alternative<rad>(value)) { value = deg{ std::get<rad>(value) }; } }

	Angle::Angle() noexcept : Angle(rad{}) {}
	Angle::Angle(deg d) noexcept : value(d) {}
	Angle::Angle(rad r) noexcept : value(r) {}

	rad Angle::get_rad() noexcept { make_rad(); return std::get<rad>(value); }
	deg Angle::get_deg() noexcept { make_deg(); return std::get<deg>(value); }

	Angle  Angle::operator+ (const Angle& oth) const noexcept
		{
		if (std::holds_alternative<deg>(value))
			{
			if (std::holds_alternative<deg>(oth.value)) { return { std::get<deg>(value) + std::get<deg>(oth.value) }; }
			else { return { std::get<deg>(value) + std::get<rad>(oth.value) }; }
			}
		else
			{
			if (std::holds_alternative<deg>(oth.value)) { return { std::get<rad>(value) + std::get<deg>(oth.value) }; }
			else { return { std::get<rad>(value) + std::get<rad>(oth.value) }; }
			}
		}
	Angle  Angle::operator- (const Angle& oth) const noexcept
		{
		if (std::holds_alternative<deg>(value))
			{
			if (std::holds_alternative<deg>(oth.value)) { return { std::get<deg>(value) - std::get<deg>(oth.value) }; }
			else { return { std::get<deg>(value) - std::get<rad>(oth.value) }; }
			}
		else
			{
			if (std::holds_alternative<deg>(oth.value)) { return { std::get<rad>(value) - std::get<deg>(oth.value) }; }
			else { return { std::get<rad>(value) - std::get<rad>(oth.value) }; }
			}
		}
	Angle& Angle::operator+=(const Angle& oth) noexcept { return *this = *this + oth; }
	Angle& Angle::operator-=(const Angle& oth) noexcept { return *this = *this - oth; }

	Angle  Angle::operator+ (const rad oth) const noexcept
		{
		if (std::holds_alternative<deg>(value)) { return { std::get<deg>(value) + oth }; }
		else { return{ std::get<rad>(value) + oth }; }
		}
	Angle  Angle::operator- (const rad oth) const noexcept
		{
		if (std::holds_alternative<deg>(value)) { return { std::get<deg>(value) - oth }; }
		else { return{ std::get<rad>(value) - oth }; }
		}
	Angle& Angle::operator+=(const rad oth) noexcept { return *this = *this + oth; }
	Angle& Angle::operator-=(const rad oth) noexcept { return *this = *this - oth; }

	Angle  Angle::operator+ (const deg oth) const noexcept
		{
		if (std::holds_alternative<deg>(value)) { return { std::get<deg>(value) + oth }; }
		else { return{ std::get<rad>(value) + oth }; }
		}
	Angle  Angle::operator- (const deg oth) const noexcept
		{
		if (std::holds_alternative<deg>(value)) { return { std::get<deg>(value) - oth }; }
		else { return{ std::get<rad>(value) - oth }; }
		}
	Angle& Angle::operator+=(const deg oth) noexcept { return *this = *this + oth; }
	Angle& Angle::operator-=(const deg oth) noexcept { return *this = *this - oth; }

	Angle  Angle::operator* (float oth) const noexcept
		{
		if (std::holds_alternative<deg>(value)) { return { std::get<deg>(value) * oth }; }
		else { return { std::get<rad>(value) * oth }; }
		}
	Angle  Angle::operator/ (float oth) const noexcept
		{
		if (std::holds_alternative<deg>(value)) { return { std::get<deg>(value) / oth }; }
		else { return { std::get<rad>(value) / oth }; }
		}
	Angle& Angle::operator*=(float oth) noexcept { return *this = *this * oth; }
	Angle& Angle::operator/=(float oth) noexcept { return *this = *this / oth; }

	float Angle::sin() noexcept { return get_rad().sin(); }
	float Angle::cos() noexcept { return get_rad().cos(); }
	float Angle::tan() noexcept { return get_rad().tan(); }
	float Angle::sin(Angle& a) noexcept { return a.sin(); }
	float Angle::cos(Angle& a) noexcept { return a.cos(); }
	float Angle::tan(Angle& a) noexcept { return a.tan(); }
	Angle Angle::asin(float n) noexcept { return { rad::asin(n) }; }
	Angle Angle::acos(float n) noexcept { return { rad::acos(n) }; }
	Angle Angle::atan(float n) noexcept { return { rad::atan(n) }; }
	Angle Angle::atan2(float a, float b) noexcept { return{ rad::atan2(a, b) }; }
	}

namespace utils::cout
	{
	namespace ccu = cout::support;
	std::ostream& operator<<(std::ostream& os, const angle::deg& d)   noexcept { return os << ccu::value << d.value << ccu::type << "d"; }
	std::ostream& operator<<(std::ostream& os, const angle::rad& r)   noexcept { return os << ccu::value << r.value << ccu::type << "r"; }
	std::ostream& operator<<(std::ostream& os, const angle::Angle& a) noexcept { return os << angle::Angle{a}.get_deg(); }
	}