/**
* This file is part of RT2D, a 2D OpenGL framework.
*
* - Copyright 2015 Rik Teerling <rik@onandoffables.com>
*/
#include "scene01.h"

Scene01::Scene01() : Scene() {
	
	SoccerField* sf = new SoccerField();
	this->addChild(sf);

	Agent* a = new Agent();
	this->addChild(a);


}

Scene01::~Scene01() {
	//
}

void Scene01::update(float deltaTime) {
	if (input()->getKeyUp(KeyCode::EscapeKey)) {
		this->stop();
	}
}

void Scene01::render(glm::mat4 modelMatrix, Camera* camera) {

}
