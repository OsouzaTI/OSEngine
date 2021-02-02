#include "light.h"

GlobalLight* GlobalLight::global = NULL;

GlobalLight* GlobalLight::get_instance()
{
	if (GlobalLight::global == NULL) {
		GlobalLight::global = new GlobalLight();
	}
	return GlobalLight::global;
}

void GlobalLight::draw()
{

}

void GlobalLight::set_position(vect3<float> position)
{
	light.direction = position;
}

vect3<float>* GlobalLight::get_position()
{
	return &light.direction;
}

GlobalLight::~GlobalLight() {}