//
// Created by clint on 2/22/19.
//

#pragma once

#include <SDL2/SDL_video.h>
#include <SDL2/SDL_render.h>
#include "graphics.h"
#include "button.h"

class Window {
    public:
    /**
     * @brief Where all looping gameflow happens for the GUI.
     * @author Clinton Brown
     */
    void gameLoop();

    ~Window();
    SDL_Renderer* renderer();

    private:
    /**
     * @brief initial setup for SDL window
     * @author Clinton Brown
     */
    bool init();

    /**
     * @brief Updates the contents of the SDL window
     * @author Clinton Brown
     */
    void update();

    void initInterface();

    /**
     * @brief Calls the methods needed to initialize the button structs.
     * @author Bryant Foster
     */
    void initButtons();

    /**
     * @brief Initializes the Toggle Piece Size Button.
     * @author Bryant Foster
     */
    void initBtnTogglePieceSize();

    /**
     * @brief Initializes the Undo Button.
     * @author Bryant Foster
     */
    void initBtnUndo();

    /**
     * @brief Initializes the Alternate Pieces Button.
     * @author Bryant Foster
     */
    void initBtnAlternatePieces();

    /**
     * @brief Initializes the Suggest Move Button.
     * @author Bryant Foster
     */
    void initBtnSuggestMove();

    /**
     * @brief Initializes the Reset Button.
     * @author Bryant Foster
     */
    void initBtnReset();

    /**
     * @brief Tracks the current state of the GUI, determines how the player can interact with the interface.
     * @author Clinton Brown
     */
    enum GuiState { PlayerTurn, EnemyTurn, SquareSelected };

    /**
     * @brief Check for any SDL events and process them.
     * @author Clinton Brown
     * @return boolean to indicate whether the game loop should continue
     */
    bool handleEvents(std::shared_ptr<Position>);

    /**
     * @brief Closes the window and game.
     * @author Clinton Brown
     */
    void closeGame();

    /**
     * @brief Draws the checkerboard background.
     * @author Clinton Brown
     */
    void drawBoardBackground();

    /**
     * @brief Draws the chess pieces in their current positions.
     * @author Clinton Brown
     */
    void drawPieces();

    /**
     * @brief Draws the alternate chess pieces in their current positions.
     * @author Bryant Foster
     */
    void drawAlternatePieces();

    /**
     * @brief Draws the position of the mouse cursor as a board position.
     * @author Clinton Brown
     */
    void drawMousePosition();

    /**
     * @brief Calls the button drawing methods.
     * @author Bryant Foster
     */
    void drawButtons();

    /**
     * @brief Draws a given button to the screen.
     * @param btn The button to draw
     * @author Bryant Foster
     */
    void drawButton(Button &btn);

    /**
     * @brief Draws the given button's text on top of the button.
     * @param btn The button whose text is to be drawn
     * @author Bryant Foster
     */
    void drawButtonText(Button btn);

    /**
     * @brief Draws the Message Box to the screen.
     * @author Bryant Foster
     */
    void drawMessageBox();

    /**
     * @brief Calls the methods to check for button "clicks".
     * @author Bryant Foster
     */
    void checkButtonClicks(std::shared_ptr<Position>);

    /**
     * @brief Converts the x and y position of the mouse cursor to a board position string
     * formatted as "Mouse Position: XX" and stores it in _mousePositionText
     * @author Clinton Brown
     */
    void mouseToBoardPositionText();

    /**
     * @brief Converts the x and y position of the mouse cursor to a display board index.
     * @author Clinton Brown
     * @return Integer index of board position
     */
    int mouseToBoardPosition();

    /**
     * @brief Converts the x and y position of the mouse cursor for moving a piece, uses unflipped index
     * @author Clinton Brown
     * @return Integer index of board position unflipped
     */
    int mouseToBoardPositionForMove();

    /**
     * @brief Takes an index and converts it to an uppercase string from the square index array.
     * @author Clinton Brown
     * @param arrayIndex The index to use for the square index array.
     * @return The square index as an uppercase string.
     */
    std::string squareIndexUppercase(int);

    /**
     * @brief Selects the board square that was clicked on, updates the selected board square
     * index and the GUI state.
     * @author Clinton Brown
     */
    void clickBoardSquare(std::shared_ptr<Position>);

    /**
     * @brief Deselects any currently selected square on the GUI.
     * @author Clinton Brown
     */
    void deselectSquare();

    /**
     * @brief Move the player from the previously selected position to the clicked position.
     * @author Clinton Brown
     * @return Boolean indicating if a valid move was done
     */
    bool playerMove(std::shared_ptr<Position>);

    /**
     * @brief Draws the currently selected square index to the GUI.
     * @author Clinton Brown
     */
    void drawSelectedSquareIndex();

    /**
     * @brief Gets a rect with the dimensions and position of the chess board on the screen
     * @author Clinton Brown
     * @return Rect with dimensions and position of the chess board
     */
    SDL_Rect getBoardDimensions();

    /**
     * @brief Checks if the mouse position is currently over the chess board.
     * @author Clinton Brown
     */
    bool isMouseOverBoard();

    /**
     * @brief Renders a given texture with a given tint color.
     * @param texture texture to render
     * @param tile tile for the texture to be rendered on
     * @author Bryant Foster
     */
    void renderTexture(SDL_Texture* texture, SDL_Rect tile);

    /**
     * @brief Sets a color to tint the texture and calls renderTexture() to render.
     * @param texture texture to render
     * @param tile tile for the texture to be rendered on
     * @param tint color to tint the texture with
     * @author Bryant Foster
     */
    void renderTintedTexture(SDL_Texture* texture, SDL_Rect tile, SDL_Color tint);

    /**
     * @brief Renders a given texture lighter (brighter) than normal by the given factor.
     * @param texture texture to render
     * @param tile tile for the texture to be rendered on
     * @param timesLighter number of times to lighten the texture
     * @author Bryant Foster
     */
    void renderLighterTexture(SDL_Texture* texture, SDL_Rect tile, int timesLighter);

    /**
     * @brief Renders index text onto the game board.
     * @param row current row
     * @param col current col
     * @param tile tile for the text to be rendered on
     * @author Bryant Foster
     */
    void renderIndexText(int row, int col, SDL_Rect tile);

    /**
     * @brief Starts searching for the enemy position.
     * @param pos
     */
    void startEnemySearch(std::shared_ptr<Position>);

    void CheckEnemySearch(std::shared_ptr<Position>);

    /**
     * @brief Starts searching for suggestions for moves for the User to play.
     * @author Bryant Foster
     */
    void startUserSearch(std::shared_ptr<Position>);

    /**
     * @brief Sets the _loggedMessage variable to the given string.
     * @param message The string to be printed to the message box
     * @author Bryant Foster
     */
    void logMessage(std::string message);

    /**
     * @brief Clears the _loggedMessage variable to "".
     * @author Bryant Foster
     */
    void clearMessageLog();

    GuiState _guiState = PlayerTurn;
    SDL_Window* _window;
    SDL_Renderer* _renderer;
    std::unique_ptr<Graphics> _gfx;
    std::shared_ptr<Position> _pos;
    char _displayBoard[8][8];
    std::string _mousePositionText = "Mouse Position:";
    uint32_t _frameTime;
    uint32_t _refresh = 60;
    uint32_t _frameDelay = 1000 / _refresh;
    int SCREEN_WIDTH = 1280;
    int SCREEN_HEIGHT = 720;
    float SCREEN_SCALE = 2;
    int _mouseX = 0;
    int _mouseY = 0;
    int _selectedSquareIndex = -1; // -1 represents no cell selected
    int _moveSquareIndex = -1;
    int _guiHorizontalTextOffset = 0;
    int _guiVerticalTextOffset = 0;
    bool _smallPieces = true;
    bool _alternatePieces = false;
    Button _btnTogglePieceSize;
    Button _btnUndo;
    Button _btnAlternatePieces;
    Button _btnSuggestMove;
    Button _btnReset;
    std::string _loggedMessage = "";
};
