#pragma once
#include <GL/freeglut.h>
#include <windows.h>
#include <vector>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <time.h>
using namespace std;

const double TWO_PI = 6.2831853;

struct Vector2 {
	GLdouble x, y;
	Vector2() : x(0), y(0) {}
	Vector2(GLdouble a, GLdouble b) : x(a), y(b) {}
	Vector2(const Vector2 &v) {
		x = v.x;
		y = v.y;
	}
	void afis() {
		cout << x << " " << y << endl;
	}

	Vector2& operator =(const Vector2 &v) {
		x = v.x;
		y = v.y;
		return *this;
	}
	bool operator ==(const Vector2 &v) {
		if (this->x == v.x && this->y == v.y)
			return true;
		return false;
	}
	bool operator <(const Vector2 &v) {
		if (this->x < v.x && this->y < v.y)
			return true;
		return false;
	}
	GLdouble Length()
	{
		return sqrt(x * x + y * y);
	}
	static Vector2 Substract(Vector2 pStart, Vector2 pEnd) {
		return Vector2(pEnd.x - pStart.x, pEnd.y - pStart.y);
	}
	Vector2 & Normalize()
	{
		GLdouble lenInv = 1.0f / Length();
		x *= lenInv;
		y *= lenInv;

		return *this;
	}
};

struct Segment {
	Vector2 M, N;
	Segment() {}
	Segment(Vector2 A, Vector2 B) : M(A), N(B) {}
	Segment(const Segment &s) {
		M = s.M;
		N = s.N;
	}
};

struct Vector3 {
	GLdouble x, y, z;
	Vector3() : x(0), y(0), z(0) {}
	Vector3(GLdouble a, GLdouble b, GLdouble c) : x(a), y(b), z(c) {}
	Vector3(const Vector3 &v) {
		x = v.x;
		y = v.y;
		z = v.z;
	}
	Vector3& operator =(const Vector3 &v) {
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}
	bool operator ==(const Vector3 &v) {
		if (this->x == v.x && this->y == v.y && this->z == v.z)
			return true;
		return false;
	}
	Vector3 operator *(GLdouble k) {
		return Vector3(x * k, y * k, z * k);
	}
	Vector3 operator +(const Vector3 &v) {
		return Vector3(x + v.x, y + v.y, z + v.z);
	}
	Vector3 operator -(const Vector3 &v) {
		return Vector3(x - v.x, y - v.y, z - v.z);
	}
	void afis() {
		cout << x << " " << y << " " << z << endl;
	}

	// returns the vector between two points
	static Vector3 Substract(Vector3 pStart, Vector3 pEnd) {
		return Vector3(pEnd.x - pStart.x, pEnd.y - pStart.y, pEnd.z - pStart.z);
	}

	// returns the cross product of two vectors
	static Vector3 Cross(Vector3 v1, Vector3 v2) {
		Vector3 v;
		v.x = v1.y * v2.z - v1.z * v2.y;
		v.y = v1.x * v2.z - v1.z * v2.x;
		v.z = v1.x * v2.y - v1.y * v2.x;
		return v;
	}

	static double Dot(Vector3 v1, Vector3 v2) {
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

	GLdouble Length()
	{
		return sqrt(x * x + y * y + z * z);
	}

	Vector3 & Normalize()
	{
		GLdouble lenInv = 1.0f / Length();
		x *= lenInv;
		y *= lenInv;
		z *= lenInv;

		return *this;
	}
};

struct Matrix {
	// assumes matrix indices start from 0 (0,1,2 and 3)

	static GLdouble det3x3(GLdouble a[3][3]) {
		return
			a[0][0] * (a[1][1] * a[2][2] - a[2][1] * a[1][2]) -
			a[0][1] * (a[1][0] * a[2][2] - a[2][0] * a[1][2]) +
			a[0][2] * (a[1][0] * a[2][1] - a[2][0] * a[1][1]);
	}

	static GLdouble det4x4(GLdouble m[4][4]) {
		return
			m[0][3] * m[1][2] * m[2][1] * m[3][0] - m[0][2] * m[1][3] * m[2][1] * m[3][0] -
			m[0][3] * m[1][1] * m[2][2] * m[3][0] + m[0][1] * m[1][3] * m[2][2] * m[3][0] +
			m[0][2] * m[1][1] * m[2][3] * m[3][0] - m[0][1] * m[1][2] * m[2][3] * m[3][0] -
			m[0][3] * m[1][2] * m[2][0] * m[3][1] + m[0][2] * m[1][3] * m[2][0] * m[3][1] +
			m[0][3] * m[1][0] * m[2][2] * m[3][1] - m[0][0] * m[1][3] * m[2][2] * m[3][1] -
			m[0][2] * m[1][0] * m[2][3] * m[3][1] + m[0][0] * m[1][2] * m[2][3] * m[3][1] +
			m[0][3] * m[1][1] * m[2][0] * m[3][2] - m[0][1] * m[1][3] * m[2][0] * m[3][2] -
			m[0][3] * m[1][0] * m[2][1] * m[3][2] + m[0][0] * m[1][3] * m[2][1] * m[3][2] +
			m[0][1] * m[1][0] * m[2][3] * m[3][2] - m[0][0] * m[1][1] * m[2][3] * m[3][2] -
			m[0][2] * m[1][1] * m[2][0] * m[3][3] + m[0][1] * m[1][2] * m[2][0] * m[3][3] +
			m[0][2] * m[1][0] * m[2][1] * m[3][3] - m[0][0] * m[1][2] * m[2][1] * m[3][3] -
			m[0][1] * m[1][0] * m[2][2] * m[3][3] + m[0][0] * m[1][1] * m[2][2] * m[3][3];
	}
};