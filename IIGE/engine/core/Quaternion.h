#pragma once
#include <algorithm> //std::min

#include "Explicit3.h"
#include "Euler.h"
#include "../../utils/math/angle.h"
#include "../../utils/math/constants.h"


namespace engine::core
	{
	namespace angle = utils::angle;

	class Quaternion
		{
		private:
			const float kEpsilon = 1E-06f; // should probably be used in the 0 tests in LookRotation or Slerp

		public:
			float x, y, z, w;

	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// ===== ===== ===== ===== ===== ===== =====            CONSTRUCTORS             ===== ===== ===== ===== ===== ===== =====
	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
			Quaternion() noexcept;
			Quaternion(float x, float y, float z, float w) noexcept;
			Quaternion(Vector3<float> v, float w) noexcept;
			Quaternion& operator=(const Quaternion& copy);

			static Quaternion identity() noexcept;
			static Quaternion x_180() noexcept;
			static Quaternion y_180() noexcept;
			static Quaternion z_180() noexcept;
			static Quaternion x__90() noexcept;
			static Quaternion y__90() noexcept;
			static Quaternion z__90() noexcept;
			static Quaternion x_n90() noexcept;
			static Quaternion y_n90() noexcept;
			static Quaternion z_n90() noexcept;


			// EULER
			static Quaternion euler(const Euler_rad& euler) noexcept;
			Quaternion(const Euler_rad& euler) noexcept;
			Euler_rad to_euler_rad() const noexcept;

			static Quaternion euler(const Euler_deg& euler);
			Quaternion(const Euler_deg& euler) noexcept;
			Euler_deg to_euler_deg() const noexcept;

			// ROTATION
			static Quaternion angle_axis(angle::rad rad, const Vector3<float>& axis) noexcept;
			Quaternion(angle::rad rad, const Vector3<float>& axis) noexcept;
			std::pair<angle::rad, Vector3<float>> to_angle_axis();

			static Quaternion from_to_rotation(Vector3<float> from, Vector3<float> to) noexcept;
			static Quaternion look_rotation(Vector3<float> forward, Vector3<float> upwards = Vector3<float>::up()) noexcept;
			static Quaternion rotation_towards(Quaternion from, Quaternion to, float maxDegreesDelta);

	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// ===== ===== ===== ===== ===== ===== =====               ACCESS                ===== ===== ===== ===== ===== ===== =====
	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====

			const Vector3<float> xyz() const noexcept;

	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// ===== ===== ===== ===== ===== ===== =====              OPERATORS              ===== ===== ===== ===== ===== ===== =====
	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====

			Quaternion  operator* (const Quaternion& oth) const noexcept;
			Quaternion& operator*=(const Quaternion& oth) noexcept;
			static float dot(const Quaternion& a, const Quaternion& b) noexcept;
			bool operator==(const Quaternion& oth) const noexcept;
			bool operator!=(const Quaternion& oth) const noexcept;

	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// ===== ===== ===== ===== ===== ===== =====               UTILITY               ===== ===== ===== ===== ===== ===== =====
	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====

			Quaternion  inverse() const noexcept;
			Quaternion& invert()  noexcept;

			static angle::rad angle_rad(const Quaternion& a, const Quaternion& b) noexcept;
			static angle::deg angle_deg(const Quaternion& a, const Quaternion& b) noexcept;

			float magnitude2() const noexcept;
			float magnitude()  const noexcept;

			Quaternion  normal()    const noexcept;
			Quaternion& normalize() noexcept;

			Vector3<float> operator*(const Vector3<float>& vector) const noexcept;

			static Quaternion slerp(Quaternion a, Quaternion b, float t);

			friend std::ostream& operator<<(std::ostream& os, const Quaternion& q) noexcept;
		};
	}