/**
* This file is part of RT2D, a 2D OpenGL framework.
*
* - Copyright 2015 Rik Teerling <rik@onandoffables.com>
*/
#include "scene01.h"

Scene01::Scene01() : Scene() {
	
	SoccerField* sf = new SoccerField();
	this->addChild(sf);

	a = new Agent();
	this->addChild(a);


	GUIButton* button1 = new GUIButton("test1");
	button1->setCallbackFunction(std::bind(&Scene01::clickButtonTest, this));
	this->addChild(button1);
}

Scene01::~Scene01() {
	//
}

void Scene01::clickButtonTest() {
	std::cout << "click button test";
}

void Scene01::update(float deltaTime) {
	if (input()->getKeyUp(KeyCode::EscapeKey)) {
		this->stop();
	}
	
	ddClear();
	ddLine(0, 0, a->position.x, a->position.y, RED);
}
