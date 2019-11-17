#include "GUIButton.h"

GUIButton::GUIButton(std::string buttonText) : Entity() {
	_buttonState = 3;
	_callbackFunction = nullptr;

	this->addSpriteSheet("assets/button.tga", 1, 4);
	this->sprite()->pivot = Point2(0, 0);
	this->sprite()->frame(_buttonState);

	_textbox = new Text();
	_textbox->scale = Point2(0.45f, 0.45f);
	_textbox->position = Point2(32, 28);
	_textbox->message(buttonText, BLACK);
	this->addChild(_textbox);
}

GUIButton::~GUIButton() {

}

void GUIButton::update(float deltaTime) {
	Point2 mousePosition = Point2(input()->getMouseX(), input()->getMouseY());
	Point2 position = worldposition();

	int left = position.x;
	int right = position.x + sprite()->size.x;
	int top = position.y;
	int bottom = position.y + sprite()->size.y;

	//Check if button is enabled
	if (_buttonState != 0) {
		//Check if the mouse cursor is inside the button region
		if (mousePosition.x > left && mousePosition.x < right && mousePosition.y > top && mousePosition.y < bottom) {
			
			//Change to MouseOver
			if (_buttonState == 3 || input()->getMouseUp(0)) {
				_buttonState = 1;
			}

			//Change to MouseDown
			if (input()->getMouseDown(0)) {
				_buttonState = 2;

				if (_callbackFunction != nullptr) {
					_callbackFunction();
				}
			}
		}
		else {	//Change to Normal
			_buttonState = 3;
		}
	}


	//Change textbox position, so it will look pressed down
	if (_buttonState == 2) {
		_textbox->position.y = 32;
	}
	else {
		_textbox->position.y = 28;
	}

	this->sprite()->frame(_buttonState);
}

void GUIButton::setCallbackFunction(std::function<void()> callbackFunction) {
	_callbackFunction = callbackFunction;
}

void GUIButton::enable() {
	_buttonState = 3;
}

void GUIButton::disable() {
	_buttonState = 0;
}