//
// Created by bryant on 4/19/19.
//

#pragma once

#include "SDL2/SDL.h"
#include <string>

/**
 * @brief Button struct to contain data and methods that are the same among buttons.
 * @author Bryant Foster
 */
class Button {
private:
    std::string _content = "";
    double _topLeftX = 0;
    double _topLeftY = 0;
    double _height = 0;
    double _width = 0;

    double _textX = 0;
    double _textY = 0;
    double _textWidth = 0;

    float _screenScale = 2;

    bool _clicked = false;
    bool _disabled = false;
    bool _guiStateDependent = false;
    int _clickFrameCount = 8;

public:
    /**
     * @brief Default constructor
     * @author Bryant Foster
     */
    Button() = default;

    /**
     * @brief Overloaded constructor for the Button struct.
     * @param x The x coordinate of the top left side
     * @param tx The x coordinate of the top left side of the text
     * @param y The y coordinate of the top left side
     * @param ty The y coordinate of the top left side of the text
     * @param h The height
     * @param w The width
     * @param tw The width of the text
     * @param c The text to go over the button
     * @param sc The current screen scale
     * @author Bryant Foster
     */
    Button(double x, double tx, double y, double ty, double h, double w, double tw, std::string c, float sc);

    /**
     * @brief Getter for the content.
     * @return The button's content
     * @author Bryant Foster
     */
    std::string getContent();

    /**
     * @brief Setter for the content.
     * @param value Value to set the variable to
     * @author Bryant Foster
     */
    void setContent(std::string value);

    /**
     * @brief Getter for the x coordinate.
     * @return The x coordinate
     * @author Bryant Foster
     */
    double getX();

    /**
     * @brief Getter for the y coordinate.
     * @return The y coordinate
     * @author Bryant Foster
     */
    double getY();

    /**
     * @brief Getter for the width.
     * @return The button's width
     * @author Bryant Foster
     */
    double getW();

    /**
     * @brief Getter for the height.
     * @return The button's height
     * @author Bryant Foster
     */
    double getH();

    /**
     * @brief Getter for the text x coordinate.
     * @return The button's text x coordinate
     * @author Bryant Foster
     */
    double getTX();

    /**
     * @brief Getter for the text y coordinate.
     * @return The button's text y coordinate
     * @author Bryant Foster
     */
    double getTY();

    /**
     * @brief Getter for the text width.
     * @return The button's text width
     * @author Bryant Foster
     */
    double getTW();

    /**
     * @brief Getter for the disabled flag.
     * @return The button's disabled status
     * @author Bryant Foster
     */
    bool getDisabled();

    /**
     * @brief Setter for the disabled flag.
     * @param value Value to set the variable to
     * @author Bryant Foster
     */
    void setDisabled(bool value);

    /**
     * @brief Getter for the gui state dependency flag.
     * @return The button's gui state dependency status
     * @author Bryant Foster
     */
    bool getGuiStateDependent();

    /**
     * @brief Setter for the Gui State Dependency.
     * @param value Value to set the variable to
     * @author Bryant Foster
     */
    void setGuiStateDependent(bool value);

    /**
     * @brief Returns true/false if the button was clicked
     * @param mouseX The mouse's x coordinate
     * @param mouseY The mouse's y coordinate
     * @return True/False it was clicked or not
     * @author Bryant Foster
     */
    bool wasClicked(double mouseX, double mouseY);

    /**
     * @brief Updates the clickFrameCount and clicked as needed.
     * @author Bryant Foster
     */
    void updateClickData();

    /**
     * @brief Returns the color of the button based on if it is disabled or not, and if it is clicked or not.
     * @return The button's color
     * @author Bryant Foster
     */
    SDL_Color getColor();
};
