#ifndef MATRIX_H
#define MATRIX_H

/*
 __  __       _        _
 |  \/  |     | |      (_)
 | \  / | __ _| |_ _ __ ___  __
 | |\/| |/ _` | __| '__| \ \/ /
 | |  | | (_| | |_| |  | |>  <
 |_|  |_|\__,_|\__|_|  |_/_/\_\

This lib contains all functions of vectors transformations
version: 0.1f
author: ozéias s. souza
date: 12/11/2020
*/


#include "vector.h"


/// <summary>
/// estrutura que representa uma matrix 4x4
/// </summary>
typedef struct {

	float m[4][4];

} Mat4x4;


/// <summary>
/// Make a identity matrix 4x4
/// </summary>
/// <returns>Mat4x4</returns>
inline Mat4x4 Mat4x4_MakeIdentity() {
	Mat4x4 m = { {
		{1, 0, 0, 0},
		{0, 1, 0, 0},
		{0, 0, 1, 0},
		{0, 0, 0, 1},
	} };
	return m;
}

inline Mat4x4 Mat4x4_MakeZero() {
	Mat4x4 m = { {
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0},
	} };
	return m;
}

inline Mat4x4 Mat4x4_MakeScale(float sx, float sy, float sz) {
	Mat4x4 m = Mat4x4_MakeIdentity();
	m.m[0][0] = sx;
	m.m[1][1] = sy;
	m.m[2][2] = sz;
	return m;
}

inline Mat4x4 Mat4x4_MakeScale(float s) {
	Mat4x4 m = Mat4x4_MakeIdentity();
	m.m[0][0] = s;
	m.m[1][1] = s;
	m.m[2][2] = s;
	return m;
}

inline Mat4x4 Mat4x4_MakeTranslation(float tx, float ty, float tz) {
	Mat4x4 m = Mat4x4_MakeIdentity();
	m.m[0][3] = tx;
	m.m[1][3] = ty;
	m.m[2][3] = tz;
	return m;
}

inline Mat4x4 Mat4x4_MakeRotationX(float angle) {
	Mat4x4 m = Mat4x4_MakeIdentity();
	m.m[1][1] = cosf(angle);
	m.m[1][2] = -sinf(angle);
	m.m[2][1] = sinf(angle);
	m.m[2][2] = cosf(angle);
	return m;
}

inline Mat4x4 Mat4x4_MakeRotationY(float angle) {
	Mat4x4 m = Mat4x4_MakeIdentity();
	m.m[0][0] = cosf(angle);
	m.m[0][2] = sinf(angle);
	m.m[2][0] = -sinf(angle);
	m.m[2][2] = cosf(angle);
	return m;
}

inline Mat4x4 Mat4x4_MakeRotationZ(float angle) {
	Mat4x4 m = Mat4x4_MakeIdentity();
	m.m[0][0] = cosf(angle);
	m.m[0][1] = -sinf(angle);
	m.m[1][0] = sinf(angle);
	m.m[1][1] = cosf(angle);
	return m;
}

inline Mat4x4 Mat4x4_MakeDistorcionXY(float angle) {
	Mat4x4 m = Mat4x4_MakeIdentity();
	m.m[0][1] = angle;
	return m;
}

inline Mat4x4 Mat4x4_MakeDistorcionYX(float angle) {
	Mat4x4 m = Mat4x4_MakeIdentity();
	m.m[1][0] = angle;
	return m;
}

inline Mat4x4 Mat4x4_MakePerspective(float fov, float aspect, float znear, float zfar) {

	Mat4x4 m = Mat4x4_MakeZero();
	m.m[0][0] = aspect * (1 / tanf(fov / 2));
	m.m[1][1] = 1 / tanf(fov / 2);
	m.m[2][2] = zfar / (zfar - znear);
	m.m[2][3] = (-zfar * znear) / (zfar - znear);
	m.m[3][2] = 1.0f;

	return m;
}


inline Mat4x4 Mat4x4_MultiplyMatrix(Mat4x4& a, Mat4x4& b) {
	Mat4x4 m;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			m.m[i][j] = a.m[i][0] * b.m[0][j] + a.m[i][1] * b.m[1][j] + a.m[i][2] * b.m[2][j] + a.m[i][3] * b.m[3][j];
		}
	}
	return m;
}

template<typename T>
vect4<T> Mat4x4_MultiplyVector(Mat4x4& m, vect4<T>& u) {
	vect4<T> v;
	v.x = m.m[0][0] * u.x + m.m[0][1] * u.y + m.m[0][2] * u.z + m.m[0][3] * u.w;
	v.y = m.m[1][0] * u.x + m.m[1][1] * u.y + m.m[1][2] * u.z + m.m[1][3] * u.w;
	v.z = m.m[2][0] * u.x + m.m[2][1] * u.y + m.m[2][2] * u.z + m.m[2][3] * u.w;
	v.w = m.m[3][0] * u.x + m.m[3][1] * u.y + m.m[3][2] * u.z + m.m[3][3] * u.w;
	return v;
}

template<typename T>
inline vect4<T> Mat4x4_MultiplyProjectVector(Mat4x4& m, vect4<T>& u) {
	vect4<T> v = Mat4x4_MultiplyVector(m, u);
	if (v.w != 0) {
		v.x /= v.w;
		v.y /= v.w;
		v.z /= v.w;
	}

	return v;
}

template<typename T>
inline Mat4x4 Mat4x4_Look_At(vect3<T>& eye, vect3<T>& target, vect3<T>& up) {

	// compute the forward (x), right (x), and up (y) vectors
	vect3<T> z = vsubvect<T>(target, eye);
	vnormalize<T>(&z);
	vect3<T> x = vcrossvect<T>(up, z);
	vnormalize<T>(&x);
	vect3<T> y = vcrossvect<T>(z, x);

	Mat4x4 view_matrix = { {
		{x.x, x.y, x.z, -vdotvect<T>(x, eye)},
		{y.x, y.y, y.z, -vdotvect<T>(y, eye)},
		{z.x, z.y, z.z, -vdotvect<T>(z, eye)},
		{0, 0, 0, 1},
	} };

	return view_matrix;
}
	


#endif // !MATRIX_H
