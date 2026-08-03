#pragma once
#include <cmath>
#include <cassert>
#include <algorithm>

namespace nu {
	struct Vector2 {
		float x = 0.0f;
		float y = 0.0f;

		Vector2() = default;
		Vector2(float x, float y) : x{ x }, y{ y } {}
		Vector2(float xy) : x{ xy }, y{ xy } {}

		float operator [] (unsigned int i) const { assert(i < 2); return (&x)[i]; }
		float& operator [] (unsigned int i) { assert(i < 2); return (&x)[i]; }

		bool operator == (const Vector2& v) const { return (this->x == v.x && this->y == v.y); }
		bool operator != (const Vector2& v) const { return !(*this == v); }

		Vector2 operator + (const Vector2& v) const { return Vector2(this->x + v.x, this->y + v.y); }
		Vector2 operator - (const Vector2& v) const { return Vector2(this->x - v.x, this->y - v.y); }
		Vector2 operator * (const Vector2& v) const { return Vector2(this->x * v.x, this->y * v.y); }
		Vector2 operator / (const Vector2& v) const { return Vector2(this->x / v.x, this->y / v.y); }

		Vector2 operator + (float v) const { return Vector2(this->x + v, this->y + v); }
		Vector2 operator - (float v) const { return Vector2(this->x - v, this->y - v); }
		Vector2 operator * (float v) const { return Vector2(this->x * v, this->y * v); }
		Vector2 operator / (float v) const { return Vector2(this->x / v, this->y / v); }

		Vector2& operator += (const Vector2& v) { this->x += v.x; this->y += v.y; return *this; }
		Vector2& operator -= (const Vector2& v) { this->x -= v.x; this->y -= v.y; return *this; }
		Vector2& operator *= (const Vector2& v) { this->x *= v.x; this->y *= v.y; return *this; }
		Vector2& operator /= (const Vector2& v) { this->x /= v.x; this->y /= v.y; return *this; }

		Vector2& operator += (float v) { this->x += v; this->y += v; return *this; }
		Vector2& operator -= (float v) { this->x -= v; this->y -= v; return *this; }
		Vector2& operator *= (float v) { this->x *= v; this->y *= v; return *this; }
		Vector2& operator /= (float v) { this->x /= v; this->y /= v; return *this; }

		float LengthSqr() const {
			return (x * x) + (y * y);
		}

		float Length() const {
			return std::sqrt(LengthSqr());
		}

		Vector2 Normalized() const {
			float length = Length();

			if (length == 0.0f) {
				return Vector2{};
			}

			return (*this) / length;
		}

		float Dot(const Vector2& v) const {
			return (this->x * v.x) + (this->y * v.y);
		}

		float Cross(const Vector2& v) const {
			return (this->x * v.y) - (this->y * v.x);
		}

		float Angle() const {
			return std::atan2(this->y, this->x);
		}

		float AngleBetween(const Vector2& v) const {
			float combinedLength = Length() * v.Length();

			if (combinedLength == 0.0f) {
				return 0.0f;
			}

			float cosine = Dot(v) / combinedLength;
			cosine = std::clamp(cosine, -1.0f, 1.0f);

			return std::acos(cosine);
		}

		Vector2 Rotate(float radians) const {
			float cosine = std::cos(radians);
			float sine = std::sin(radians);

			return Vector2(
				(this->x * cosine) - (this->y * sine),
				(this->x * sine) + (this->y * cosine)
			);
		}
	};
}