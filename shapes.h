#ifndef SHAPES_H
#define SHAPES_H

#include "matrix.h"
#include "vector.h"
#include "drawing.h"
#include "osgui.h"

class Shape
{
	public:
		Shape() {};
		~Shape() {};

		void set_color(uint32_t color);
		uint32_t get_color();

		static Drawing* shapedraw;
		static vect2<float>* center_screen;
		char name[100];
		uint32_t color = C_WHITE;
		vect3<float> position;
		vect3<float> scale;
		vect3<float> rotation;
		Mat4x4 transformations;
		virtual void draw()	 = 0;
		virtual void update()	 = 0;
		void set_position(vect3<float> position);
		vect3<float>* get_position();
		void set_scale(vect3<float> scale);
		vect3<float>* get_scale();
		void set_rotation(vect3<float> rotation);
		vect3<float>* get_rotation();		
	private:
	protected:
		Mat4x4 scale_matrix;		
		Mat4x4 rotate_matrix_x;
		Mat4x4 rotate_matrix_y;
		Mat4x4 rotate_matrix_z;

};


Drawing* Shape::shapedraw = NULL;
vect2<float>* Shape::center_screen = NULL;

void Shape::set_color(uint32_t color) {
	logstd(color);
	this->color = color;
}

uint32_t Shape::get_color()
{
	return color;
}

void Shape::set_position(vect3<float> position)
{
	this->position = position;
}

vect3<float>* Shape::get_position()
{
	return &position;
}

void Shape::set_scale(vect3<float> scale)
{
	this->scale = scale;
	update();
}

vect3<float>* Shape::get_scale()
{
	return &scale;
}

void Shape::set_rotation(vect3<float> rotation)
{
	vect3<float> _rotation{ RAD(rotation.x), RAD(rotation.y), RAD(rotation.z) };
	this->rotation = rotation;
}

vect3<float>* Shape::get_rotation()
{
	return &rotation;
}

class Ellipse : public Shape
{
	public:
		Ellipse(const char* name, vect3<float> position, vect3<float> scale, vect3<float> rotation, float radius);
		~Ellipse();

		void draw() override;
		void update() override;

	private:
		float radius;
		DraggableCircle center;

		
};

Ellipse::Ellipse(const char* name, vect3<float> position, vect3<float> scale, vect3<float> rotation, float radius)
{
	strcpy(this->name, name);
	this->position = position;
	this->scale = scale;
	this->rotation = rotation;
	this->radius = radius;
	this->center.GUI_init(center_screen->x + this->position.x, center_screen->y + this->position.y);
	this->transformations = { {
		{1, 0,  0, 0},
		{0, 1,  0, 0},
		{0, 0, -1, 0},
		{0, 0,	0, 1},
	} };

	this->scale_matrix = Mat4x4_MakeScale(this->scale.x * this->radius, this->scale.y * this->radius, 1);
	this->rotate_matrix_x = Mat4x4_MakeRotationX(this->rotation.x);
	this->rotate_matrix_y = Mat4x4_MakeRotationY(this->rotation.y);
	this->rotate_matrix_z = Mat4x4_MakeRotationZ(this->rotation.z);
}
Ellipse::~Ellipse(){}

void Ellipse::draw()
{
	// events of GUI
	this->center.poll_events();

	int cx = this->center.get_position().x;
	int cy = this->center.get_position().y;

	Mat4x4 world_matrix = Mat4x4_MakeIdentity();
	world_matrix = Mat4x4_MultiplyMatrix(transformations, world_matrix);
	world_matrix = Mat4x4_MultiplyMatrix(scale_matrix, world_matrix);
	world_matrix = Mat4x4_MultiplyMatrix(rotate_matrix_x, world_matrix);
	world_matrix = Mat4x4_MultiplyMatrix(rotate_matrix_y, world_matrix);
	world_matrix = Mat4x4_MultiplyMatrix(rotate_matrix_z, world_matrix);

	for (float x = -1; x < 1; x += 0.01)
	{
		for (float y = -1; y < 1; y += 0.01)
		{

			bool equation = powf(x, 2.0f) + powf(y, 2.0f) < 1;
			if (equation) {
				vect4<float> P{ x, y, 1, 1 };
				P = Mat4x4_MultiplyVector(world_matrix, P);
				this->shapedraw->draw_pixel(cx + P.x, cy + P.y, this->color);
			}

		}

	}

	this->center.draw();
}

void Ellipse::update()
{
	this->scale_matrix = Mat4x4_MakeScale(this->scale.x * this->radius, this->scale.y * this->radius, 1);
	this->rotate_matrix_x = Mat4x4_MakeRotationX(this->rotation.x);
	this->rotate_matrix_y = Mat4x4_MakeRotationY(this->rotation.y);
	this->rotate_matrix_z = Mat4x4_MakeRotationZ(this->rotation.z);
}



#endif // !SHAPES_H
