#ifndef LIGHT_H
#define LIGHT_H

#include "vector.h"

enum SHADING_TYPE {
	FLAT = 0,
	DISABLED
};

typedef struct {
	vect3<float> direction;
} Light;

class GlobalLight
{
	public:
		~GlobalLight();
		static GlobalLight* get_instance();
		Light light;
		SHADING_TYPE shading = SHADING_TYPE::FLAT;
		void draw();
		void set_position(vect3<float> position);		
		vect3<float>* get_position();
	private:
		GlobalLight() { light.direction = { 0, 0, 1 }; };
		static GlobalLight* global;

};

#endif // !LIGHT_H
