#include "Quaternion.h"

namespace engine::core
	{
// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
// ===== ===== ===== ===== ===== ===== =====            CONSTRUCTORS             ===== ===== ===== ===== ===== ===== =====
// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====

	Quaternion::Quaternion() noexcept : x(0), y(0), z(0), w(0) {}
	Quaternion::Quaternion(float x, float y, float z, float w) noexcept : x(x), y(y), z(z), w(w) {}
	Quaternion::Quaternion(Vec3<float> v, float w) noexcept : x(v.x), y(v.y), z(v.z), w(w) {}

	Quaternion& Quaternion::operator=(const Quaternion& copy) { x = copy.x; y = copy.y; z = copy.z; w = copy.w; return *this; };

	Quaternion Quaternion::identity() noexcept { return { 0.f, 0.f, 0.f, 1.f }; }

	const float sqr = sqrt(0.5f);
	const float nqr = -sqr;

	Quaternion Quaternion::x_180() noexcept { return { 1.f, 0.f, 0.f, 0.f }; }
	Quaternion Quaternion::y_180() noexcept { return { 0.f, 1.f, 0.f, 0.f }; }
	Quaternion Quaternion::z_180() noexcept { return { 0.f, 0.f, 1.f, 0.f }; }
	Quaternion Quaternion::x__90() noexcept { return { sqr, 0.f, 0.f, sqr }; }
	Quaternion Quaternion::y__90() noexcept { return { 0.f, sqr, 0.f, sqr }; }
	Quaternion Quaternion::z__90() noexcept { return { 0.f, 0.f, sqr, sqr }; }
	Quaternion Quaternion::x_n90() noexcept { return { nqr, 0.f, 0.f, sqr }; }
	Quaternion Quaternion::y_n90() noexcept { return { 0.f, nqr, 0.f, sqr }; }
	Quaternion Quaternion::z_n90() noexcept { return { 0.f, 0.f, nqr, sqr }; }

	Quaternion Quaternion::euler(const Euler_rad& euler) noexcept { return { euler }; }

	Quaternion::Quaternion(const Euler_rad& euler) noexcept
		{
		angle::rad yawOver2 = euler.yaw * 0.5f;
		float cosYawOver2 = yawOver2.cos();
		float sinYawOver2 = yawOver2.sin();
		angle::rad pitchOver2 = euler.pitch * 0.5f;
		float cosPitchOver2 = pitchOver2.cos();
		float sinPitchOver2 = pitchOver2.sin();
		angle::rad rollOver2 = euler.roll * 0.5f;
		float cosRollOver2 = rollOver2.cos();
		float sinRollOver2 = rollOver2.sin();

		w = cosYawOver2 * cosPitchOver2 * cosRollOver2 + sinYawOver2 * sinPitchOver2 * sinRollOver2;
		x = sinYawOver2 * cosPitchOver2 * cosRollOver2 + cosYawOver2 * sinPitchOver2 * sinRollOver2;
		y = cosYawOver2 * sinPitchOver2 * cosRollOver2 - sinYawOver2 * cosPitchOver2 * sinRollOver2;
		z = cosYawOver2 * cosPitchOver2 * sinRollOver2 - sinYawOver2 * sinPitchOver2 * cosRollOver2;
		}

	Euler_rad Quaternion::to_euler_rad() const noexcept
		{ //TODO heavily check, confused return type, inside it returns deg, not rad ???
		float sqw = w * w;
		float sqx = x * x;
		float sqy = y * y;
		float sqz = z * z;
		float unit = sqx + sqy + sqz + sqw; // if normalised is one, otherwise is correction factor
		float test = x * w - y * z;
		Euler_rad euler;

		if (test > 0.4995f * unit)
			{ // singularity at north pole
			euler.y = angle::trigonometry::atan2(y, x) * 2.f;
			euler.x = utils::constants::PI / 2;
			euler.z = 0;
			return euler.to_deg().normalize_angles();
			}

		if (test < -0.4995f * unit)
			{ // singularity at south pole
			euler.y = angle::trigonometry::atan2(y, x) * -2.f;
			euler.x = -utils::constants::PI / 2;
			euler.z = 0;
			return euler.to_deg().normalize_angles();
			}
		Quaternion q{ w, z, x, y };
		euler.y = angle::trigonometry::atan2(2.f * q.x * q.w + 2.f * q.y * q.z, 1 - 2.f * (q.z * q.z + q.w * q.w)); // Yaw
		euler.x = angle::trigonometry::asin(2.f * (q.x * q.z - q.w * q.y));                                         // Pitch
		euler.z = angle::trigonometry::atan2(2.f * q.x * q.y + 2.f * q.z * q.w, 1 - 2.f * (q.y * q.y + q.z * q.z)); // Roll
		return euler.to_deg().normalize_angles();
		}

	Quaternion Quaternion::euler(const Euler_deg& euler) { return { euler }; }
	Quaternion::Quaternion(const Euler_deg& euler) noexcept : Quaternion(euler.to_rad()) {};
	Euler_deg Quaternion::to_euler_deg() const noexcept { return to_euler_rad().to_deg(); }

	Quaternion Quaternion::angle_axis(angle::rad rad, const Vec3<float>& axis) noexcept { return { rad, axis }; }

	Quaternion::Quaternion(angle::rad rad, const Vec3<float>& axis) noexcept
		{
		if (axis.magnitude2() == 0.0f) { x = 0.f; y = 0.f; z = 0.f; w = 1.f; return; }

		rad *= 0.5f;
		rad *= 0.5f;
		Vec3<float> ax(axis);
		ax.normalize();
		ax = ax * rad.sin();
		x = ax.x;
		y = ax.y;
		z = ax.z;
		w = rad.cos();

		normalize();
		}

	std::pair<angle::rad, Vec3<float>> Quaternion::to_angle_axis()
		{
		Quaternion q{ *this };
		if (abs(w) > 1.0f) { q.normalize(); }
		angle::rad angle{ angle::trigonometry::acos(q.w) * 2.0f }; // angle
		float den = sqrtf(1.0f - float(double(q.w) * double(q.w)));
		if (den > 0.0001f) { return { angle, Vec3<float>{q.x / den, q.y / den, q.z / den} }; }
		else
			{
			// This occurs when the angle is zero. 
			// Not a problem: just set an arbitrary normalized axis.
			return { angle, Vec3<float>{1.f, 0.f, 0.f} };
			}
		}

	Quaternion Quaternion::from_to_rotation(Vec3<float> from, Vec3<float> to) noexcept
		{
		return rotation_towards(look_rotation(from), look_rotation(to), (std::numeric_limits<float>::max)());
		}

	Quaternion Quaternion::look_rotation(Vec3<float> forward, Vec3<float> upwards) noexcept
		{
		forward.normalize();
		Vec3<float> right = Vec3<float>::cross(upwards, forward).normal();
		upwards = Vec3<float>::cross(forward, right);
		float m00 = right.x;
		float m01 = right.y;
		float m02 = right.z;
		float m10 = upwards.x;
		float m11 = upwards.y;
		float m12 = upwards.z;
		float m20 = forward.x;
		float m21 = forward.y;
		float m22 = forward.z;

		float num8 = (m00 + m11) + m22;

		Quaternion quaternion = Quaternion();
		if (num8 > 0.f)
			{
			float num = sqrt(num8 + 1.f);
			quaternion.w = num * 0.5f;
			num = 0.5f / num;
			quaternion.x = (m12 - m21) * num;
			quaternion.y = (m20 - m02) * num;
			quaternion.z = (m01 - m10) * num;
			return quaternion;
			}
		if ((m00 >= m11) && (m00 >= m22))
			{
			float num7 = sqrt(((1.f + m00) - m11) - m22);
			float num4 = 0.5f / num7;
			quaternion.x = 0.5f * num7;
			quaternion.y = (m01 + m10) * num4;
			quaternion.z = (m02 + m20) * num4;
			quaternion.w = (m12 - m21) * num4;
			return quaternion;
			}
		if (m11 > m22)
			{
			float num6 = sqrt(((1.f + m11) - m00) - m22);
			float num3 = 0.5f / num6;
			quaternion.x = (m10 + m01) * num3;
			quaternion.y = 0.5f * num6;
			quaternion.z = (m21 + m12) * num3;
			quaternion.w = (m20 - m02) * num3;
			return quaternion;
			}
		float num5 = sqrt(((1.f + m22) - m00) - m11);
		float num2 = 0.5f / num5;
		quaternion.x = (m20 + m02) * num2;
		quaternion.y = (m21 + m12) * num2;
		quaternion.z = 0.5f * num5;
		quaternion.w = (m01 - m10) * num2;
		return quaternion;
		}

	Quaternion Quaternion::rotation_towards(Quaternion from, Quaternion to, float maxDegreesDelta)
		{
		float num = Quaternion::angle_deg(from, to).value;
		if (num == 0.f) { return to; }
		float t = (std::min)(1.f, maxDegreesDelta / num);
		return Quaternion::slerp(from, to, t);
		}

	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// ===== ===== ===== ===== ===== ===== =====               ACCESS                ===== ===== ===== ===== ===== ===== =====
	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====

	const Vec3<float> Quaternion::xyz() const noexcept { return { x, y, z }; }

	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// ===== ===== ===== ===== ===== ===== =====              OPERATORS              ===== ===== ===== ===== ===== ===== =====
	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====

	Quaternion Quaternion::operator*(const Quaternion& oth) const noexcept
		{
		return { w * oth.x + x * oth.w + y * oth.z - z * oth.y, w * oth.y + y * oth.w + z * oth.x - x * oth.z, w * oth.z + z * oth.w + x * oth.y - y * oth.x, w * oth.w - x * oth.x - y * oth.y - z * oth.z };
		}

	Quaternion& Quaternion::operator*=(const Quaternion& oth) noexcept { return *this = *this * oth; }

	float Quaternion::dot(const Quaternion& a, const Quaternion& b) noexcept { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }

	bool Quaternion::operator==(const Quaternion& oth) const noexcept
		{
		return Quaternion::dot(*this, oth) > 0.999999f;
		}

	bool Quaternion::operator!=(const Quaternion& oth) const noexcept
		{
		return Quaternion::dot(*this, oth) <= 0.999999f;
		}

	Quaternion Quaternion::inverse() const noexcept
		{
		float lengthSq = magnitude2();
		if (lengthSq != 0.0)
			{
			float i = 1.0f / lengthSq;
			return { xyz() * -i, w * i };
			}
		return { *this };
		}

	Quaternion& Quaternion::invert() noexcept { return *this = inverse(); }

	angle::rad Quaternion::angle_rad(const Quaternion& a, const Quaternion& b) noexcept
		{
		float f = Quaternion::dot(a, b);
		return angle::trigonometry::acos((std::min)(std::abs(f), 1.f)) * 2.f;
		}

	angle::deg Quaternion::angle_deg(const Quaternion& a, const Quaternion& b) noexcept { return angle_rad(a, b).to_deg(); }

	float Quaternion::magnitude2() const noexcept { return x * x + y * y + z * z + w * w; }
	float Quaternion::magnitude() const noexcept { return sqrt(magnitude2()); }

	Quaternion Quaternion::normal() const noexcept
		{
		float scale = 1.0f / magnitude();
		return { x * scale, y * scale, z * scale, w * scale };
		}
	Quaternion& Quaternion::normalize() noexcept { return *this = normal(); }

	Vec3<float> Quaternion::operator*(const Vec3<float>& vector) const noexcept
		{
		float num = x * 2.f;
		float num2 = y * 2.f;
		float num3 = z * 2.f;
		float num4 = x * num;
		float num5 = y * num2;
		float num6 = z * num3;
		float num7 = x * num2;
		float num8 = x * num3;
		float num9 = y * num3;
		float num10 = w * num;
		float num11 = w * num2;
		float num12 = w * num3;
		Vec3<float> result;
		result.x = (1.f - (num5 + num6)) * vector.x + (num7 - num12) * vector.y + (num8 + num11) * vector.z;
		result.y = (num7 + num12) * vector.x + (1.f - (num4 + num6)) * vector.y + (num9 - num10) * vector.z;
		result.z = (num8 - num11) * vector.x + (num9 + num10) * vector.y + (1.f - (num4 + num5)) * vector.z;
		return result;
		}

	Quaternion Quaternion::slerp(Quaternion a, Quaternion b, float t)
		{
		if (a.magnitude2() == 0.0f)
			{
			if (b.magnitude2() == 0.0f)
				{
				return identity();
				}
			return b;
			}
		else if (b.magnitude2() == 0.0f)
			{
			return a;
			}

		float cosHalfAngle = a.w * b.w + Vec3<float>::dot(a.xyz(), b.xyz());

		if (cosHalfAngle >= 1.0f || cosHalfAngle <= -1.0f)
			{// angle = 0.0f, so just return one input.
			return a;
			}
		else if (cosHalfAngle < 0.0f)
			{
			b.x = -b.x; b.y = -b.y; b.z = -b.z; b.w = -b.w; cosHalfAngle = -cosHalfAngle;
			}

		float blend_a;
		float blend_b;
		if (cosHalfAngle < 0.99f)
			{
			// do proper slerp for big angles
			angle::rad halfAngle = angle::trigonometry::acos(cosHalfAngle);
			float sinHalfAngle = halfAngle.sin();
			float oneOverSinHalfAngle = 1.0f / sinHalfAngle;
			blend_a = (halfAngle * (1.0f - t)).sin() * oneOverSinHalfAngle;
			blend_b = (halfAngle * t).sin() * oneOverSinHalfAngle;
			}
		else
			{
			// do lerp if angle is really small.
			blend_a = 1.0f - t;
			blend_b = t;
			}

		Quaternion result{ a.xyz() * blend_a + b.xyz() * blend_b, a.w * blend_a + b.w * blend_b };
		if (result.magnitude2() > 0.0f) { return result.normal(); }
		else { return identity(); }
		}

	std::ostream& operator<<(std::ostream& os, const Quaternion& q) noexcept
		{
		return os << "(" << q.x << ", " << q.y << ", " << q.z << ", " << q.w << ")";
		}
	}