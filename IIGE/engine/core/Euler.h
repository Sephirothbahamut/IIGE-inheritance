#pragma once
#include "Vec3.h"
#include "../../utils/math/angle.h"
#include "../../utils/math/constants.h"

namespace engine::core
	{
	namespace angle = utils::angle;

	class Euler_rad;
	class Euler_deg
		{
		private:
			Vec3<angle::deg> v;

		public:
			inline Euler_deg() noexcept = default;
			inline Euler_deg(angle::rad x, angle::rad y, angle::rad z) noexcept : v(x, y, z) {}
			Euler_deg(const Euler_rad& oth) noexcept;
			Euler_rad to_rad() const noexcept;

			// ACCESS
			inline angle::deg get_x() const noexcept { return v.x; };
			inline void set_x(const angle::deg& value) noexcept { v.x = value; };
			inline angle::deg get_y() const noexcept { return v.y; };
			inline void set_y(const angle::deg& value) noexcept { v.y = value; };
			inline angle::deg get_z() const noexcept { return v.z; };
			inline void set_z(const angle::deg& value) noexcept { v.z = value; };
			__declspec(property(get = get_x, put = set_x)) angle::deg x;
			__declspec(property(get = get_y, put = set_y)) angle::deg y;
			__declspec(property(get = get_z, put = set_z)) angle::deg z;
			__declspec(property(get = get_x, put = set_x)) angle::deg yaw;
			__declspec(property(get = get_y, put = set_y)) angle::deg pitch;
			__declspec(property(get = get_z, put = set_z)) angle::deg roll;

			// UTILITY
			inline Euler_deg& normalize_angles() noexcept
				{
				x.clamp();
				y.clamp();
				z.clamp();
				return *this;
				}
		};

	class Euler_rad
		{
		private:
			Vec3<angle::rad> v;
		public:
			inline Euler_rad() noexcept = default;
			inline Euler_rad(angle::rad x, angle::rad y, angle::rad z) noexcept : v(x, y, z) {}
			Euler_rad(const Euler_deg& oth) noexcept;
			Euler_deg to_deg() const noexcept;

			// ACCESS //TODO standardize, MSVC only
			inline angle::rad get_x() const noexcept { return v.x; };
			inline void set_x(const angle::rad& value) noexcept { v.x = value; };
			inline angle::rad get_y() const noexcept { return v.y; };
			inline void set_y(const angle::rad& value) noexcept { v.y = value; };
			inline angle::rad get_z() const noexcept { return v.z; };
			inline void set_z(const angle::rad& value) noexcept { v.z = value; };
			__declspec(property(get = get_x, put = set_x)) angle::rad x;
			__declspec(property(get = get_y, put = set_y)) angle::rad y;
			__declspec(property(get = get_z, put = set_z)) angle::rad z;
			__declspec(property(get = get_x, put = set_x)) angle::rad yaw;
			__declspec(property(get = get_y, put = set_y)) angle::rad pitch;
			__declspec(property(get = get_z, put = set_z)) angle::rad roll;

			// UTILITY
			inline Euler_rad& normalize_angles() noexcept
				{
				x.clamp();
				y.clamp();
				z.clamp();
				return *this;
				}
		};
	}