#include "../utils.h"

namespace math {
	void sin_cos(float rad, float& sin, float& cos) {
		sin = math::sin(rad);
		cos = math::cos(rad);
	}

	qangle_t calculate_angle(vec3_t& src, vec3_t& dst)
	{
		qangle_t angle;
		vec3_t delta((src.x - dst.x), (src.y - dst.y), (src.z - dst.z));
		double hyp = sqrt(delta.x * delta.x + delta.y * delta.y);

		angle.x = float(math::atan(float(delta.z / hyp)) * 57.295779513082f);
		angle.y = float(math::atan(float(delta.y / delta.x)) * 57.295779513082f);
		angle.z = 0.0f;

		if (delta.x >= 0.0)
			angle.y += 180.0f;

		return angle;
	}

	void angle_matrix(const qangle_t& ang_view, matrix3x4_t& mat_output, const vec3_t& vec_origin)
	{
		float sp, sy, sr, cp, cy, cr;

		math::sin_cos(deg_to_rad(ang_view.x), sp, cp);
		math::sin_cos(deg_to_rad(ang_view.y), sy, cy);
		math::sin_cos(deg_to_rad(ang_view.z), sr, cr);

		const float_t crcy = cr * cy;
		const float_t crsy = cr * sy;
		const float_t srcy = sr * cy;
		const float_t srsy = sr * sy;

		mat_output = matrix3x4_t(vec3_t(cp * cy, cp * sy, -sp), vec3_t(sp * srcy - crsy, sp * srsy + crcy, sr * cp), vec3_t(sp * crcy + srsy, sp * crsy - srcy, cr * cp), vec_origin);
	}

	void angle_vectors(const qangle_t& angles, vec3_t* forward, vec3_t* right, vec3_t* up) {
		vec3_t cos, sin;

		sin_cos(deg_to_rad(angles.x), sin.x, cos.x);
		sin_cos(deg_to_rad(angles.y), sin.y, cos.y);
		sin_cos(deg_to_rad(angles.z), sin.z, cos.z);

		if (forward) {
			forward->x = cos.x * cos.y;
			forward->y = cos.x * sin.y;
			forward->z = -sin.x;
		}

		if (right) {
			right->x = -sin.z * sin.x * cos.y + -cos.z * -sin.y;
			right->y = -sin.z * sin.x * sin.y + -cos.z * cos.y;
			right->z = -sin.z * cos.x;
		}

		if (up) {
			up->x = cos.z * sin.x * cos.y + -sin.z * -sin.y;
			up->y = cos.z * sin.x * sin.y + -sin.z * cos.y;
			up->z = cos.z * cos.x;
		}
	}

	void vector_angles( const vec3_t& forward, qangle_t* angles )
	{
		float	tmp, yaw, pitch;

		if ( forward[ 1 ] == 0 && forward[ 0 ] == 0 ) {
			yaw = 0;
			if ( forward[ 2 ] > 0 )
				pitch = 270;
			else
				pitch = 90;
		}
		else {
			yaw = ( atan2( forward[ 1 ], forward[ 0 ] ) * 180 / m_pi );
			if ( yaw < 0 )
				yaw += 360;

			tmp = sqrt( forward[ 0 ] * forward[ 0 ] + forward[ 1 ] * forward[ 1 ] );
			pitch = ( atan2( -forward[ 2 ], tmp ) * 180 / m_pi );
			if ( pitch < 0 )
				pitch += 360;
		}

		angles->x = pitch;
		angles->y = yaw;
		angles->z = 0;
	}

	float normalize_yaw(float yaw) {
		while (yaw < -180.f)
			yaw += 360.f;
		while (yaw > 180.f)
			yaw -= 360.f;
		return yaw;
	}
}
