//
// Created by bryant on 4/19/19.
//

#include "button.h"

Button::Button(double x, double tx, double y, double ty, double h, double w, double tw, std::string c, float sc) {
    _topLeftX = x;
    _textX = tx;
    _topLeftY = y;
    _textY = ty;
    _height = h;
    _width = w;
    _textWidth = tw;
    _content = c;
    _screenScale = sc;
}

std::string Button::getContent() {
    return _content;
}

void Button::setContent(std::string value) {
    _content = value;
}

double Button:: getX() {
    return _topLeftX;
}

double Button::getY() {
    return _topLeftY;
}

double Button::getW() {
    return _width;
}

double Button::getH() {
    return _height;
}

double Button::getTX() {
    return _textX;
}

double Button::getTY() {
    return _textY;
}

double Button::getTW() {
    return _textWidth;
}

bool Button::getDisabled() {
    return _disabled;
}

void Button::setDisabled(bool value) {
    _disabled = value;
}

bool Button::getGuiStateDependent() {
    return _guiStateDependent;
}

void Button::setGuiStateDependent(bool value) {
    _guiStateDependent = value;
}

bool Button::wasClicked(double mouseX, double mouseY) {
    if(_disabled) { return false; }

    double minX = _screenScale * _topLeftX;
    double maxX = minX + (_screenScale * _width);
    double minY = _screenScale * _topLeftY;
    double maxY = minY + (_screenScale * _height);

    _clicked = mouseX >= minX && mouseX <= maxX && mouseY >= minY && mouseY <= maxY;
    return _clicked;
}

void Button::updateClickData() {
    if(_clicked && --_clickFrameCount < 0) {
        _clickFrameCount = 8;
        _clicked = false;
    }
}

SDL_Color Button::getColor() {
    if(_disabled) { return {178, 34, 34, 255}; }
    else if(_clicked && _clickFrameCount > 0) { return {0, 200, 0, 255}; }
    else { return {106, 75, 53, 255}; }
}