/**
* This file is part of RT2D, a 2D OpenGL framework.
*
* - Copyright 2015 Rik Teerling <rik@onandoffables.com>
*/
#ifndef SCENE01_H
#define SCENE01_H

//#include <rt2d/core.h>

#include <rt2d/scene.h>
#include "agent.h"

#include "SoccerField.h"

#include "GUIButton.h"

class Scene01 : public Scene
{
public:
	Scene01();
	virtual ~Scene01();

	virtual void update(float deltaTime);

	void clickButtonTest();

private:
	Agent* a;
};

#endif /* SCENE01_H */
