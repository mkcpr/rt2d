#include "SoccerField.h"



SoccerField::SoccerField() : Entity() {

	Field_SideLines = new Line();
	Field_SideLines->color = RGBAColor(255, 255, 255, 255);
	
	int width = SWIDTH * 0.7;
	int height = SHEIGHT * 0.7;
	int widthSpace = (SWIDTH - width) / 2;
	int heightSpace = (SHEIGHT - height) / 2;
	
	Field_SideLines->addPoint(widthSpace, heightSpace);
	Field_SideLines->addPoint(widthSpace + width, heightSpace);
	Field_SideLines->addPoint(widthSpace + width, heightSpace + height);
	Field_SideLines->addPoint(widthSpace, heightSpace + height);
	Field_SideLines->addPoint(widthSpace, heightSpace);

	this->addLine(Field_SideLines);
}

SoccerField::~SoccerField() {

}

void SoccerField::update(float deltaTime) {
	
}