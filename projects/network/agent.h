#ifndef AGENT_H
#define AGENT_H

#include <vector>
#include <rt2d/entity.h>
#include <rt2d/camera.h>

class Agent : public Entity {
public:
	Agent();
	virtual ~Agent();

	virtual void update(float deltaTime);

	bool paused;

private:
	Vector2 _velocity;
	float _maxSpeed;
	float _maxForce;
	//Vector2 _acceleration;

	Vector2 calculateSteering();
	
	Vector2 seek(Vector2 targetPosition);
	Vector2 flee(Vector2 targetPosition);
	Vector2 arrive(Vector2 targetPosition);

	bool showSeekLine;
	Line* seekLine1;
};

#endif /* AGENT_H */
