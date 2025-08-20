#pragma once
#include <cmath>

struct Vector2
{
	float x;
	float y;
	
	// コンストラクタ
	Vector2() : x(0.0f), y(0.0f) {}
	Vector2(float x, float y) : x(x), y(y) {}
	
	// 演算子オーバーロード
	Vector2 operator+(const Vector2& other) const {
		return Vector2(x + other.x, y + other.y);
	}
	
	Vector2 operator-(const Vector2& other) const {
		return Vector2(x - other.x, y - other.y);
	}
	
	Vector2 operator*(float scalar) const {
		return Vector2(x * scalar, y * scalar);
	}
	
	Vector2 operator/(float scalar) const {
		return Vector2(x / scalar, y / scalar);
	}
	
	Vector2& operator+=(const Vector2& other) {
		x += other.x;
		y += other.y;
		return *this;
	}
	
	Vector2& operator-=(const Vector2& other) {
		x -= other.x;
		y -= other.y;
		return *this;
	}
	
	Vector2& operator*=(float scalar) {
		x *= scalar;
		y *= scalar;
		return *this;
	}
	
	Vector2& operator/=(float scalar) {
		x /= scalar;
		y /= scalar;
		return *this;
	}
	
	bool operator==(const Vector2& other) const {
		return x == other.x && y == other.y;
	}
	
	bool operator!=(const Vector2& other) const {
		return !(*this == other);
	}
	
	// ユーティリティ関数
	float Length() const {
		return std::sqrt(x * x + y * y);
	}
	
	float LengthSquared() const {
		return x * x + y * y;
	}
	
	Vector2 Normalized() const {
		float len = Length();
		if (len > 0.0f) {
			return *this / len;
		}
		return Vector2(0.0f, 0.0f);
	}
	
	void Normalize() {
		float len = Length();
		if (len > 0.0f) {
			*this /= len;
		}
	}
	
	float Dot(const Vector2& other) const {
		return x * other.x + y * other.y;
	}
	
	float Distance(const Vector2& other) const {
		return (*this - other).Length();
	}
	
	// 静的関数
	static Vector2 Zero() { return Vector2(0.0f, 0.0f); }
	static Vector2 One() { return Vector2(1.0f, 1.0f); }
	static Vector2 Up() { return Vector2(0.0f, -1.0f); }
	static Vector2 Down() { return Vector2(0.0f, 1.0f); }
	static Vector2 Left() { return Vector2(-1.0f, 0.0f); }
	static Vector2 Right() { return Vector2(1.0f, 0.0f); }
	
	static Vector2 Lerp(const Vector2& a, const Vector2& b, float t) {
		return a + (b - a) * t;
	}
};

// スカラー値を左側にした乗算演算子
inline Vector2 operator*(float scalar, const Vector2& vector) {
	return vector * scalar;
}
