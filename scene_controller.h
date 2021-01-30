#ifndef SCENECONTROLLER_H
#define SCENECONTROLLER_H

#include "shapes.h"

class OSceneController
{
	public:
		static OSceneController* get_instance();
		template<typename T, typename... TArgs>
		T& add(TArgs&&... args);
		void draw();
		void update(int index, vect3<float> position, vect3<float> scale, vect3<float> rotate);
		std::vector<Shape*>* get_scene();
	private:
		OSceneController();
		~OSceneController();
		static OSceneController* scene_controller;
		std::vector<Shape*> SCENE;

};

OSceneController* OSceneController::scene_controller = NULL;

OSceneController* OSceneController::get_instance()
{
	if (OSceneController::scene_controller == NULL) {
		OSceneController::scene_controller = new OSceneController();
	}
	return OSceneController::scene_controller;
}

void OSceneController::draw()
{
	for (auto& scene : SCENE) {
		scene->draw();
	}
}

void OSceneController::update(int index, vect3<float> position, vect3<float> scale, vect3<float> rotate)
{
	if (index < 0) return;
	SCENE.at(index)->set_position(position);
	SCENE.at(index)->set_scale(scale);
	SCENE.at(index)->set_rotation(rotate);
	SCENE.at(index)->update();
}

std::vector<Shape*>* OSceneController::get_scene()
{
	return &SCENE;
}

OSceneController::OSceneController()
{
}

OSceneController::~OSceneController()
{
}

template<typename T, typename ...TArgs>
inline T& OSceneController::add(TArgs&& ...args)
{
	T* newScene(new T(std::forward<TArgs>(args)...));
	SCENE.emplace_back(newScene);
	return *newScene;
}

#endif // !SCENECONTROLLER_H

