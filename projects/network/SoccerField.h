#ifndef SOCCERFIELD_H
#define SOCCERFIELD_H

//#include <rt2d/core.h>
#include <vector>

#include <rt2d/entity.h>
#include <rt2d/camera.h>


class SoccerField : public Entity {
public:
	SoccerField();
	virtual ~SoccerField();

	virtual void update(float deltaTime);
	virtual void render(glm::mat4 modelMatrix, Camera* camera);


	Line* Field_SideLines;

	//SoccerBall* Ball;
	
	//SoccerTeam* Team_Home;
	//SoccerTeam* Team_Visitor;

	//Goal* Goal_Home;
	//Goal* Goal_Visitor;
};

#endif /* SOCCERFIELD_H */