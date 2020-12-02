#ifndef IMAGEBUTTON_H
#define IMAGEBUTTON_H

#include <rt2d/entity.h>
#include <rt2d/text.h>

#include <functional>

class ImageButton : public Entity {
public:
	ImageButton(std::string ButtonText);
	virtual ~ImageButton();

	virtual void update(float deltaTime);

	Text* textbox;

	void setCallbackFunction(std::function<void()> CallbackFunction);
	void Disable();
	void Enable();

private:
	int ButtonState;
	int SpriteFrame;

	std::function<void()> _CallbackFunction;
};

#endif /* IMAGEBUTTON_H */