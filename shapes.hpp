#ifndef SHAPES_H
#define SHAPES_H

#include "matrix.hpp"
#include "vector.hpp"
#include "drawing.h"
#include "osgui.hpp"

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
		DraggableCircle center;
};

Ellipse::Ellipse(){}

Ellipse::Ellipse(vect3<float> position, vect3<float> scale, vect3<float> rotation, float radius)
{
	this->position = position;
	this->scale = scale;
	this->rotation = rotation;
	this->radius = radius;
	this->center.GUI_init(this->position.x, this->position.y);
	this->transformations = { {
		{1, 0,  0, 0},
		{0, 1,  0, 0},
		{0, 0, -1, 0},
		{0, 0,	0, 1},
	} };
}
Ellipse::~Ellipse(){}

void Ellipse::draw()
{
	// events of GUI
	this->center.poll_events();

	int cx = this->center.get_position().x;
	int cy = this->center.get_position().y;
	
	Mat4x4 worldM = Mat4x4_MakeIdentity();	
	Mat4x4 scaleM = Mat4x4_MakeScale(this->scale.x * this->radius, this->scale.y * this->radius, 1);
	worldM = Mat4x4_MultiplyMatrix(this->transformations, worldM);
	worldM = Mat4x4_MultiplyMatrix(scaleM, worldM);

	for (float x = -1; x < 1; x += 0.01)
	{
		for (float y = -1; y < 1; y += 0.01)
		{

			bool equation = powf(x, 2.0f) + powf(y, 2.0f) < 1;
			if (equation) {
				vect4<float> P{ x, y, 1, 1 };
				P = Mat4x4_MultiplyVector(worldM, P);
				this->shapedraw->draw_pixel(cx + P.x, cy + P.y, C_GREEN);
			}

		}

	}

	this->center.draw();
}



#endif // !SHAPES_H
