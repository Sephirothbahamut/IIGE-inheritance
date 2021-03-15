#include "Euler.h"

namespace engine::core
	{
	Euler_deg::Euler_deg(const Euler_rad& oth) noexcept : v(oth.x, oth.y, oth.z) {}
	Euler_rad Euler_deg::to_rad() const noexcept { return { *this }; }

	Euler_rad::Euler_rad(const Euler_deg& oth) noexcept : v(oth.x, oth.y, oth.z) {}
	Euler_deg Euler_rad::to_deg() const noexcept { return { *this }; }
	}