#ifndef SHAPES_H
#define SHAPES_H

#include "matrix.hpp"
#include "vector.hpp"
#include "drawing.h"

class Shape
{
	public:
		Shape() {};
		~Shape() {};

		static Drawing* shapedraw;
		char name[100];
		vect3<float> position;
		vect3<float> scale;
		vect3<float> rotation;
		Mat4x4 transformations;

		virtual void draw() {};

	private:

};

Drawing* Shape::shapedraw = NULL;

class Ellipse : public Shape
{
	public:
		Ellipse();
		Ellipse(vect3<float> position, vect3<float> scale, vect3<float> rotation, float radius);
		~Ellipse();

		void draw() override;

	private:
		float radius;
};

Ellipse::Ellipse(){}

Ellipse::Ellipse(vect3<float> position, vect3<float> scale, vect3<float> rotation, float radius)
{
	this->position = position;
	this->scale = scale;
	this->rotation = rotation;
	this->radius = radius;

	Mat4x4 position_matrix = Mat4x4_MakeTranslation(this->position.x, this->position.y, this->position.z);
	Mat4x4 scale_matrix = Mat4x4_MakeScale(this->scale.x * radius, this->scale.y * radius, this->scale.z * radius);
	Mat4x4 rotation_x_matrix = Mat4x4_MakeRotationX(this->rotation.x);
	Mat4x4 rotation_y_matrix = Mat4x4_MakeRotationY(this->rotation.y);
	Mat4x4 rotation_z_matrix = Mat4x4_MakeRotationZ(this->rotation.z);
	this->transformations = Mat4x4_MakeIdentity();
	this->transformations = Mat4x4_MultiplyMatrix(scale_matrix, this->transformations);
	this->transformations = Mat4x4_MultiplyMatrix(rotation_x_matrix, this->transformations);
	this->transformations = Mat4x4_MultiplyMatrix(rotation_y_matrix, this->transformations);
	this->transformations = Mat4x4_MultiplyMatrix(rotation_z_matrix, this->transformations);
	this->transformations = Mat4x4_MultiplyMatrix(position_matrix, this->transformations);
}
Ellipse::~Ellipse(){}

void Ellipse::draw()
{
	int width = this->shapedraw->get_display()->width;
	int height = this->shapedraw->get_display()->height;
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{

			bool condicion = powf(x, 2.0f) + powf(y, 2.0f) < powf(this->radius, 2.0f);
			if (condicion) {
				vect4<float> P{ x, y, 10, 1 };
				P = Mat4x4_MultiplyVector(this->transformations, P);

				shapedraw->draw_pixel(P.x, P.y, C_RED);
			}

		}
	}
}



#endif // !SHAPES_H
