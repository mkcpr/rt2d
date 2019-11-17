#include "agent.h"

Agent::Agent() : Entity() {
	this->addSprite("assets/boid.tga", 0.25f, 0.50f);
	this->sprite()->color = WHITE;

	seekLine1 = new Line();
	seekLine1->dynamic(true);
	seekLine1->color = RGBAColor(0, 127, 0, 255);
	seekLine1->addPoint(0, 0);
	seekLine1->addPoint(0, 0);
	seekLine1->addPoint(0, 0);
	seekLine1->addPoint(0, 0);
	showSeekLine = false;

	paused = false;

	_maxSpeed = 200.0f;
	_maxForce = 10.0f;

	this->position = Vector2(500, 500);
}

Agent::~Agent(){

}

void Agent::update(float deltaTime){
	Vector2 steeringForce = calculateSteering() * deltaTime;

	if (input()->getKeyDown(KeyCode::Space)) {
		paused = !paused;
	}
	
	if (!paused){
		_velocity += steeringForce;
		_velocity.limit(_maxSpeed);
		this->position += _velocity * deltaTime;
		this->rotation.z = this->_velocity.getAngle();
	}

	//ddClear();
	//parent()->ddLine(position.x, position.y, position.x + (steeringForce.x * 10), position.y + (steeringForce.y * 10), PINK);
	//parent()->ddLine(position.x, position.y, position.x + (_velocity.x * 10), position.y + (_velocity.y * 10), GREEN);
	//ddCircle(0, 0, 16, GREEN);
}

Vector2 Agent::calculateSteering() {
	int mousex = input()->getMouseX();
	int mousey = input()->getMouseY();
	Vector2 mousepos = Vector2(mousex, mousey);

	//Vector2 steeringForce = seek(mousepos);
	//Vector2 steeringForce = flee(mousepos);
	Vector2 steeringForce = arrive(mousepos);

	return steeringForce;
}

Vector2 Agent::seek(Vector2 targetPosition) {
	Vector2 desiredVelocity = targetPosition - this->position;
	desiredVelocity.normalize();
	desiredVelocity *= _maxSpeed;
	desiredVelocity -= this->_velocity;
	
	//###############################################################
	if (true) {
		parent()->ddLine(position.x, position.y, position.x + desiredVelocity.x, position.y + desiredVelocity.y, GREEN);
	}
	//###############################################################

	return (desiredVelocity);
}

Vector2 Agent::flee(Vector2 targetPosition) {

	if (Vector2(this->position - targetPosition).getLengthSquared() > 100000) {
		return Vector2(0, 0);
	}

	Vector2 desiredVelocity = this->position - targetPosition;
	desiredVelocity.normalize();
	desiredVelocity *= _maxSpeed;

	desiredVelocity -= this->_velocity;

	//###############################################################
	if (true) {
		parent()->ddLine(position.x, position.y, position.x + desiredVelocity.x, position.y + desiredVelocity.y, GREEN);
	}
	//###############################################################

	return (desiredVelocity);
}

Vector2 Agent::arrive(Vector2 targetPosition) {
	Vector2 desiredVelocity = targetPosition - this->position;
	float distance = desiredVelocity.getLength();

	if (distance > 0.0f) {

		float speed = distance / 3.0f;
		//float speed = distance / 1.0f;
		if (speed > _maxSpeed) {
			speed = _maxSpeed;
		}

		desiredVelocity = desiredVelocity * speed / distance;
		
		//dit is hetzelfde
		//desiredVelocity.normalize();
		//desiredVelocity *= speed;

		desiredVelocity -= this->_velocity;

		//###############################################################
		if (true) {
			parent()->ddLine(position.x, position.y, position.x + desiredVelocity.x, position.y + desiredVelocity.y, GREEN);
		}
		//###############################################################

		return (desiredVelocity);
	}

	return Vector2(0, 0);
}