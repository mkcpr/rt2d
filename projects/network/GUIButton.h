#ifndef GUIBUTTON_H
#define GUIBUTTON_H

#include <rt2d/entity.h>
#include <rt2d/text.h>

#include <functional>	//std::function

class GUIButton : public Entity {
	public:
		GUIButton(std::string buttonText);
		virtual ~GUIButton();

		virtual void update(float deltaTime);

		void setCallbackFunction(std::function<void()> callbackFunction);
		void enable(); 
		void disable();

	private:
		Text* _textbox;
		int _buttonState;
		std::function<void()> _callbackFunction;
};

#endif /* GUIBUTTON_H */
