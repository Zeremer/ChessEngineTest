//
// Created by clint on 2/22/19.
//

#include "window.h"
#include "SDL2/SDL.h"
#include "debug.h"
#include "coutsync.h"
#include "console.h"
#include "thread.h"
#include <algorithm>

Window::~Window(){
    SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);
}

SDL_Renderer* Window::renderer(){
    return _renderer;
}

bool Window::init() {
    // Initialization flag
    bool success = true;

    // Initialize SDL
    if( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        success = false;
    } else {
        // Get current resolution
        SDL_DisplayMode userDisplay;
        if (SDL_GetCurrentDisplayMode(0, &userDisplay) == 0){
            SCREEN_WIDTH  = userDisplay.w;
            SCREEN_HEIGHT = userDisplay.h;

            // adjust scale if the user has a large screen
            if (SCREEN_HEIGHT > 2000){
                SCREEN_SCALE = SCREEN_HEIGHT / 500;
            }

            // get the user's refresh rate for render speed
            if (userDisplay.refresh_rate != 0) {
                _refresh = (uint32_t) userDisplay.refresh_rate;
            }
        }

        //Create window
        _window = SDL_CreateWindow("Chess Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if(_window == nullptr) {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            success = false;
        } else {
            _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
            SDL_RenderClear(_renderer);
            SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
            SDL_RenderFillRect(_renderer, nullptr);
            SDL_RenderPresent(_renderer);
        }
    }

    return success;
}

void Window::update() {
    uint32_t frameStart = SDL_GetTicks();

    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
    SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);
    SDL_RenderSetScale(_renderer, SCREEN_SCALE, SCREEN_SCALE);
    SDL_RenderClear(_renderer);

    // render things here and ONLY CALL RenderCopy in these functions, the order is important
    // and it will cause screen tearing if calls are out of order
    drawBoardBackground();
    _alternatePieces ? drawAlternatePieces() : drawPieces();
    drawMousePosition();
    drawSelectedSquareIndex();
    drawButtons();
    drawMessageBox();

    SDL_RenderPresent(_renderer);

    // framerate control
    _frameTime = SDL_GetTicks() - frameStart;
    if (_frameDelay > _frameTime){
        SDL_Delay(_frameDelay - _frameTime);
    }
}

void Window::initInterface() {
    _gfx = std::make_unique<Graphics>(_renderer);
    _guiHorizontalTextOffset = (int)(SCREEN_WIDTH / 2.5) / (int)SCREEN_SCALE;
    _guiVerticalTextOffset   = (int)(_gfx->TILE_HEIGHT*1.25);
}

void Window::initButtons() {
    initBtnTogglePieceSize();
    initBtnUndo();
    initBtnAlternatePieces();
    initBtnSuggestMove();
    initBtnReset();
}

void Window::initBtnTogglePieceSize() {
    double x = _gfx->TILE_WIDTH;
    double y = 11.5 * _gfx->TILE_HEIGHT;
    double w = 4.5 * _gfx->TILE_WIDTH;
    double h = _gfx->TILE_HEIGHT;

    double tx = x * 1.25;
    double ty = y * 1.021;
    double tw = w * 0.89;

    std::string content = "Toggle ";
    content += (_smallPieces ? "Large " : "Small ");
    content += "Pieces";

    _btnTogglePieceSize = Button(x, tx, y, ty, h, w, tw, content, SCREEN_SCALE);
}

void Window::initBtnUndo() {
    double x = 6.5 * _gfx->TILE_WIDTH;
    double y = 11.5 * _gfx->TILE_HEIGHT;
    double w = 4.5 * _gfx->TILE_WIDTH;
    double h = _gfx->TILE_HEIGHT;

    double tx = x * 1.25;
    double ty = y * 1.021;
    double tw = w * 0.89;

    _btnUndo = Button(x, tx, y, ty, h, w, tw, "Undo", SCREEN_SCALE);
    _btnUndo.setGuiStateDependent(true);
}

void Window::initBtnAlternatePieces() {
    double x = _gfx->TILE_WIDTH;
    double y = 13 * _gfx->TILE_HEIGHT;
    double w = 4.5 * _gfx->TILE_WIDTH;
    double h = _gfx->TILE_HEIGHT;

    double tx = x * 1.55;
    double ty = y * 1.021;
    double tw = w * 0.89;

    _btnAlternatePieces = Button(x, tx, y, ty, h, w, tw, "Alternate Pieces", SCREEN_SCALE);
}

void Window::initBtnSuggestMove() {
    double x = 6.5 * _gfx->TILE_WIDTH;
    double y = 13 * _gfx->TILE_HEIGHT;
    double w = 4.5 * _gfx->TILE_WIDTH;
    double h = _gfx->TILE_HEIGHT;

    double tx = x * 1.13;
    double ty = y * 1.021;
    double tw = w * 0.89;

    _btnSuggestMove = Button(x, tx, y, ty, h, w, tw, "Suggest Move", SCREEN_SCALE);
    _btnSuggestMove.setGuiStateDependent(true);
}

void Window::initBtnReset() {
    double x = 3.9 * _gfx->TILE_WIDTH;
    double y = 14.5 * _gfx->TILE_HEIGHT;
    double w = 4.5 * _gfx->TILE_WIDTH;
    double h = _gfx->TILE_HEIGHT;

    double tx = x * 1.43;
    double ty = y * 1.021;
    double tw = w * 0.89;

    _btnReset = Button(x, tx, y, ty, h, w, tw, "Reset", SCREEN_SCALE);
    _btnReset.setGuiStateDependent(true);
}

void Window::gameLoop(){
    // initial board setup
    _pos = std::make_shared<Position>(Position()); // Needed a class-level shared_ptr to reset the game on button click
//    _pos = std::make_shared<Position>(Position("8/8/4k3/8/5p2/8/4P3/4K3 w - - 0 1"));

    // Initialize the SDL window and interface
    init();
    initInterface();

    // Initialize the Button structs
    initButtons();

    // initialize the 2D array of piece positions
    Graphics::createDisplayBitboard(_pos, _displayBoard);

    //start loop
    bool gameRunning = true;
    while(gameRunning){
        if(_guiState == EnemyTurn){
            CheckEnemySearch(_pos);
        }
        gameRunning = handleEvents(_pos);
        update();
    }
}

bool Window::handleEvents(std::shared_ptr<Position> pos) {
    // event handler
    SDL_Event event;

    // If there are any events this will continue looping, otherwise it will break
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                // quit the game if Escape is pressed or the window X is pressed
                case SDLK_ESCAPE:
                    closeGame();
                    return false;
                default:
                    return true;
            }
        } else if (event.type == SDL_WINDOWEVENT){
            switch (event.window.event){
                case SDL_WINDOWEVENT_CLOSE:
                    closeGame();
                    return false;
                default:
                    return true;
            }
        } else if (event.type == SDL_MOUSEMOTION){
            // Get mouse position
            SDL_GetMouseState(&_mouseX, &_mouseY);
        } else if (event.type == SDL_MOUSEBUTTONDOWN) {
            clickBoardSquare(pos);
            checkButtonClicks(pos);
        } else if (event.type == SDL_FINGERMOTION){
            // Store touch position in mouse coordinates it will be handled the same
            // TODO: Test this on mobile/emulator
            _mouseX = (int)event.tfinger.x;
            _mouseY = (int)event.tfinger.y;
        }
    }
    return true;
}

void Window::closeGame(){
    if (Debug::getIsDebugLogEnabled()) {
        syncCout << Debug::LogType[0] << ": Closing GUI window" << syncEndl;
    }
    SDL_DestroyWindow(_window);
}

void Window::drawBoardBackground(){
    for (int row = 0; row < 11; row++) {
        for (int col = 0; col < 11; col++) {
            SDL_Rect tile;

            // set tile x and y position offset from 0,0 by one tile
            tile.x = col*_gfx->TILE_WIDTH + _gfx->TILE_WIDTH;
            tile.y = row*_gfx->TILE_HEIGHT + _gfx->TILE_HEIGHT;

            tile.w = _gfx->TILE_WIDTH;
            tile.h = _gfx->TILE_HEIGHT;

            if(row == 0) {
                if(col == 0) {
                    renderTintedTexture(_gfx->getBorderTileTopLeft(), tile, {133, 94, 66});
                } else if(col > 0 && col < 9) {
                    renderTintedTexture(_gfx->getBorderTileTopMiddle(), tile, {133, 94, 66});
                } else if(col == 9) {
                    renderTintedTexture(_gfx->getBorderTileTopRight(), tile, {133, 94, 66});
                }
            } else if(row > 0 && row < 9) {
                if(col == 0) {
                    renderTintedTexture(_gfx->getBorderTileLeftMiddle(), tile, {133, 94, 66});
                    renderIndexText(row, col, tile);
                } else if(col > 0 && col < 9) {
                    if ((row+col) % 2 == 0){
                        renderTexture(_gfx->getWhiteBoardTile(), tile);
                    } else {
                        renderTexture(_gfx->getBlackBoardTile(), tile);
                    }

                    int squareIndex = (row  * 8) + col - 9;// This is just because of how the board is displayed, currently
                    squareIndex = (squareIndex + 56) - ((squareIndex/8)*16);
                    if(squareIndex == _selectedSquareIndex) {
                        SDL_SetRenderDrawColor(_renderer, 0, 200, 0, 150); // Semi-transparent Green highlight color
                        SDL_RenderFillRect(_renderer, &tile);
                    }

                    // Render Highlighting when the user or the computer make a move
                    if(_selectedSquareIndex == -1) {
                        auto lastMove = _pos->getLastMove();
                        SDL_Color highlight;

                        if(_guiState == EnemyTurn) {
                            highlight = {0, 200, 0, 150};
                        } else if(_guiState == PlayerTurn) {
                            highlight = {178, 34, 34, 150};
                        }

                        if(squareIndex == lastMove.startSquare || squareIndex == lastMove.endSquare) {
                            SDL_SetRenderDrawColor(_renderer, highlight.r, highlight.g, highlight.b, highlight.a);
                            SDL_RenderFillRect(_renderer, &tile);
                        }
                    }
                } else if(col == 9) {
                    renderTintedTexture(_gfx->getBorderTileRightMiddle(), tile, {133, 94, 66});
                }
            } else if(row == 9) {
                if(col == 0) {
                    renderTintedTexture(_gfx->getBorderTileBottomLeft(), tile, {133, 94, 66});
                } else if(col > 0 && col < 9) {
                    renderTintedTexture(_gfx->getBorderTileBottomMiddle(), tile, {133, 94, 66});
                } else if(col == 9) {
                    renderTintedTexture(_gfx->getBorderTileBottomRight(), tile, {133, 94, 66});
                }
                renderIndexText(row, col, tile);
            }
        }
    }
}

void Window::drawPieces(){
    for (int row = 8; row >= 0; row--) { // In order to overlap the pieces correctly, this is reversed.
        if(row == 0) {continue;}
        for (int col = 0; col < 9; col++) {
            if(col == 0) {continue;}
            SDL_Rect sprite;

            char currentDBChar = _displayBoard[row - 1][col - 1];

            if(_smallPieces) {
                sprite.x = col*_gfx->TILE_WIDTH + (_gfx->TILE_WIDTH * 1.25);
                sprite.y = (9 - row)*_gfx->TILE_HEIGHT + (_gfx->TILE_HEIGHT);

                sprite.w = (_gfx->TILE_WIDTH * 0.5);
                sprite.h = _gfx->TILE_HEIGHT;

                if(currentDBChar == 'p' || currentDBChar == 'P') {
                    sprite.y -= _gfx->TILE_HEIGHT * 0.08;
                }
            } else {
                // piece y positions need to be offset from tiles by 32 pixels (1 tile)
                sprite.x = col*_gfx->TILE_WIDTH + _gfx->TILE_WIDTH;
                sprite.y = (9 - row)*_gfx->TILE_HEIGHT;

                sprite.w = _gfx->TILE_WIDTH;
                sprite.h = _gfx->TILE_HEIGHT * 2;
            }

            switch (currentDBChar){
                case 'p':
                    renderLighterTexture(_gfx->getBlackPawnSprite(), sprite, 1);
                    break;
                case 'P':
                    renderTintedTexture(_gfx->getWhitePawnSprite(), sprite, {255, 228, 196});
                    break;
                case 'r':
                    renderLighterTexture(_gfx->getBlackRookSprite(), sprite, 1);
                    break;
                case 'R':
                    renderTintedTexture(_gfx->getWhiteRookSprite(), sprite, {255, 228, 196});
                    break;
                case 'n':
                    renderLighterTexture(_gfx->getBlackKnightSprite(), sprite, 1);
                    break;
                case 'N':
                    renderTintedTexture(_gfx->getWhiteKnightSprite(), sprite, {255, 228, 196});
                    break;
                case 'b':
                    renderLighterTexture(_gfx->getBlackBishopSprite(), sprite, 1);
                    break;
                case 'B':
                    renderTintedTexture(_gfx->getWhiteBishopSprite(), sprite, {255, 228, 196});
                    break;
                case 'q':
                    renderLighterTexture(_gfx->getBlackQueenSprite(), sprite, 1);
                    break;
                case 'Q':
                    renderTintedTexture(_gfx->getWhiteQueenSprite(), sprite, {255, 228, 196});
                    break;
                case 'k':
                    renderLighterTexture(_gfx->getBlackKingSprite(), sprite, 1);
                    break;
                case 'K':
                    renderTintedTexture(_gfx->getWhiteKingSprite(), sprite, {255, 228, 196});
                    break;
                default:
                    break;
            }
        }
    }
}

void Window::drawAlternatePieces() {
    for(int row = 0; row < 9; row++) {
        if(row == 0) { continue; }
        for(int col = 0; col < 9; col++) {
            if(col == 0) { continue; }

            SDL_Rect sprite;
            sprite.x = col*_gfx->TILE_WIDTH + _gfx->TILE_WIDTH;
            sprite.y = (9 - row)*_gfx->TILE_HEIGHT + _gfx->TILE_HEIGHT;

            sprite.w = _gfx->TILE_WIDTH;
            sprite.h = _gfx->TILE_HEIGHT;

            switch(_displayBoard[row - 1][col - 1]) {
                case 'p':
                    renderTexture(_gfx->getAlternateBlackPawnSprite(), sprite);
                    break;
                case 'P':
                    renderTexture(_gfx->getAlternateWhitePawnSprite(), sprite);
                    break;
                case 'r':
                    renderTexture(_gfx->getAlternateBlackRookSprite(), sprite);
                    break;
                case 'R':
                    renderTexture(_gfx->getAlternateWhiteRookSprite(), sprite);
                    break;
                case 'n':
                    renderTexture(_gfx->getAlternateBlackKnightSprite(), sprite);
                    break;
                case 'N':
                    renderTexture(_gfx->getAlternateWhiteKnightSprite(), sprite);
                    break;
                case 'b':
                    renderTexture(_gfx->getAlternateBlackBishopSprite(), sprite);
                    break;
                case 'B':
                    renderTexture(_gfx->getAlternateWhiteBishopSprite(), sprite);
                    break;
                case 'q':
                    renderTexture(_gfx->getAlternateBlackQueenSprite(), sprite);
                    break;
                case 'Q':
                    renderTexture(_gfx->getAlternateWhiteQueenSprite(), sprite);
                    break;
                case 'k':
                    renderTexture(_gfx->getAlternateBlackKingSprite(), sprite);
                    break;
                case 'K':
                    renderTexture(_gfx->getAlternateWhiteKingSprite(), sprite);
                    break;
                default:
                    break;
            }
        }
    }
}

void Window::drawMousePosition(){
    mouseToBoardPositionText();
    SDL_Color white = {255, 255, 255};

    // create a rect to put the font message in
    SDL_Rect message_rect;
    message_rect.x = _guiHorizontalTextOffset; // x position on window
    message_rect.y = _guiVerticalTextOffset;   // y position on window, offset same as board
    message_rect.w = 100;
    message_rect.h = 20;

    SDL_Surface* messageSurf = TTF_RenderText_Blended(_gfx->getFont(), _mousePositionText.c_str(), white);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(_renderer, messageSurf);
    SDL_QueryTexture(tex, nullptr, nullptr, &message_rect.w, &message_rect.h);
    SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
    SDL_RenderCopy(_renderer, tex, nullptr, &message_rect);
    SDL_FreeSurface(messageSurf);
    SDL_DestroyTexture(tex);
}

void Window::drawButtons() {
    drawButton(_btnTogglePieceSize);
    drawButtonText(_btnTogglePieceSize);

    drawButton(_btnUndo);
    drawButtonText(_btnUndo);

    drawButton(_btnAlternatePieces);
    drawButtonText(_btnAlternatePieces);

    drawButton(_btnSuggestMove);
    drawButtonText(_btnSuggestMove);

    drawButton(_btnReset);
    drawButtonText(_btnReset);
}

void Window::drawButton(Button &btn) {
    btn.updateClickData();

    SDL_Rect b;
    b.x = btn.getX();
    b.y = btn.getY();
    b.w = btn.getW();
    b.h = btn.getH();

    if(btn.getGuiStateDependent()) { btn.setDisabled(_guiState == EnemyTurn); }

    SDL_Color c = btn.getColor();

    SDL_SetRenderDrawColor(_renderer, c.r, c.g, c.b, c.a);
    SDL_RenderFillRect(_renderer, &b);
}

void Window::drawButtonText(Button btn) {
    SDL_Rect btnText;
    btnText.x = btn.getTX();
    btnText.y = btn.getTY();
    btnText.w = btn.getTW();
    btnText.h = btn.getH();

    SDL_Surface* messageSurf = TTF_RenderText_Blended(_gfx->getFont(), btn.getContent().c_str(), {255, 255, 255});
    SDL_Texture* tex = SDL_CreateTextureFromSurface(_renderer, messageSurf);
    SDL_QueryTexture(tex, nullptr, nullptr, &btnText.w, &btnText.h);
    SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
    SDL_RenderCopy(_renderer, tex, nullptr, &btnText);
    SDL_FreeSurface(messageSurf);
    SDL_DestroyTexture(tex);
}

void Window::drawMessageBox() {
    // Basic dimensions of the message box
    SDL_Rect messageBox;
    messageBox.x = _guiHorizontalTextOffset;
    messageBox.y = _guiVerticalTextOffset + 150;
    messageBox.w = 350;
    messageBox.h = 250;

    // Draw the outer colored rectangle
    SDL_SetRenderDrawColor(_renderer, 20, 55, 55, 255);
    SDL_RenderFillRect(_renderer, &messageBox);

    messageBox.y += 10;
    messageBox.x += 10;

    // Write a line to the Message Box that just says "MESSAGES"; so that the user
    // knows what it is for.
    std::string title = "MESSAGES";
    SDL_Surface* mSurf = TTF_RenderText_Blended(_gfx->getFont(), title.c_str(), {255, 255, 255});
    SDL_Texture* tex = SDL_CreateTextureFromSurface(_renderer, mSurf);
    SDL_QueryTexture(tex, nullptr, nullptr, &messageBox.w, &messageBox.h);
    SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
    SDL_RenderCopy(_renderer, tex, nullptr, &messageBox);
    SDL_FreeSurface(mSurf);
    SDL_DestroyTexture(tex);

    messageBox.h = 20;

    // Creating a vector of strings in order to print multiple lines
    // to the MessageBox (there is not support for '\n' other than a '?'
    // char, normally.
    std::string temp = _loggedMessage;
    std::vector<std::string> strings;
    while(temp.find('\n') != std::string::npos) {
        strings.push_back(temp.substr(0, temp.find('\n')));
        temp = temp.substr(temp.find('\n') + 1);
    }
    strings.push_back(temp);

    // Draw each line onto the screen.
    for(auto s : strings) {
        messageBox.y += 20;
        mSurf = TTF_RenderText_Blended(_gfx->getFont(), s.c_str(), {255, 255, 255});
        tex = SDL_CreateTextureFromSurface(_renderer, mSurf);
        SDL_QueryTexture(tex, nullptr, nullptr, &messageBox.w, &messageBox.h);
        SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
        SDL_RenderCopy(_renderer, tex, nullptr, &messageBox);
        SDL_FreeSurface(mSurf);
        SDL_DestroyTexture(tex);
    }
}

void Window::mouseToBoardPositionText(){
    std::string mousePosition = "Mouse Position: ";

    if (isMouseOverBoard()){
        int arrayIndex = mouseToBoardPosition();
        mousePosition += squareIndexUppercase(arrayIndex);

        if (Debug::getIsDebugLogEnabled()) {
            mousePosition += " Index: " + std::to_string(arrayIndex);
        }

        _mousePositionText = mousePosition;
    }
}

std::string Window::squareIndexUppercase(int arrayIndex){
    std::string squareIndex = SquareIndexStrings[arrayIndex];
    std::string upperCased;
    upperCased += toupper(squareIndex[0]);
    upperCased += squareIndex[1];
    return upperCased;
}

int Window::mouseToBoardPosition() {
    int translatedMouseX = (_mouseX / _gfx->TILE_WIDTH / (int)SCREEN_SCALE) - 2;
    int translatedMouseY = (_mouseY / _gfx->TILE_HEIGHT / (int)SCREEN_SCALE) - 2;
    int unflippedPosition = (translatedMouseY*8) + translatedMouseX;
    return (unflippedPosition + 56) - ((unflippedPosition/8)*16);
}

int Window::mouseToBoardPositionForMove() {
    int translatedMouseX = (_mouseX / _gfx->TILE_WIDTH / (int)SCREEN_SCALE) - 2;
    int translatedMouseY = (_mouseY / _gfx->TILE_HEIGHT / (int)SCREEN_SCALE) - 2;
    return (7 - translatedMouseY) * 8 + translatedMouseX;
}

bool Window::isMouseOverBoard(){
    SDL_Rect board = getBoardDimensions();
    return _mouseX >= board.x && _mouseX < board.x + board.w && _mouseY >= board.y && _mouseY < board.y + board.h;
}

void Window::clickBoardSquare(std::shared_ptr<Position> pos){
    // piece selection and movement
    if (isMouseOverBoard()){
        if (_guiState != EnemyTurn && _guiState != SquareSelected) {
            _selectedSquareIndex = mouseToBoardPosition();
            _moveSquareIndex = mouseToBoardPositionForMove();
            _guiState = SquareSelected;

        // Player and Enemy turn code
        // Note: playerMove is in the conditional logic
        } else if (_guiState == SquareSelected && playerMove(pos)){
            // Enemy turn code
            // Set up other search
            _guiState = EnemyTurn;
            startEnemySearch(pos);
        }
    } else if (!isMouseOverBoard() && _guiState == SquareSelected){
        deselectSquare();
    }
}

void Window::checkButtonClicks(std::shared_ptr<Position> pos) {
    if(_btnTogglePieceSize.wasClicked(_mouseX, _mouseY) && !_btnTogglePieceSize.getDisabled()) {
        _smallPieces = !_smallPieces;
        std::string content = "Toggle ";
        content += _smallPieces ? "Large " : "Small ";
        content += "Pieces";
        _btnTogglePieceSize.setContent(content);
    } else if(_btnUndo.wasClicked(_mouseX, _mouseY) && !_btnUndo.getDisabled()) {
        pos->undoLastTwoMoves();
        Graphics::createDisplayBitboard(pos, _displayBoard);
    } else if(_btnAlternatePieces.wasClicked(_mouseX, _mouseY)) {
        _alternatePieces = !_alternatePieces;
        _btnTogglePieceSize.setDisabled(_alternatePieces);
    } else if(_btnSuggestMove.wasClicked(_mouseX, _mouseY) && !_btnSuggestMove.getDisabled()) {
        logMessage("Deciding on moves for the user to make...");
        // This loop is needed in order for the message to be displayed that the computer is searching for the user.
        // It also allows for the button animation before the search begins.
        int frameCount = 8;
        while(--frameCount >= 0) {
            update();
        }
        startUserSearch(pos);
    } else if(_btnReset.wasClicked(_mouseX, _mouseY) && !_btnReset.getDisabled()) {
        _pos = std::make_shared<Position>(Position());
        Graphics::createDisplayBitboard(_pos, _displayBoard);
        _guiState = PlayerTurn;
        _selectedSquareIndex = -1;
        _moveSquareIndex = -1;
    }
}

bool Window::playerMove(std::shared_ptr<Position> pos){
    clearMessageLog();

    std::unique_ptr<Move> move = std::make_unique<Move>();

    // set up move object
    SquareIndex start = (SquareIndex)(_moveSquareIndex);
    move->startSquare = start;
    move->pieceType = pos->getPieceTypeOfSquare(start);

    _selectedSquareIndex = mouseToBoardPosition();
    _moveSquareIndex = mouseToBoardPositionForMove();
    SquareIndex end = (SquareIndex)(_moveSquareIndex);
    move->endSquare = end;

    // print out move information and current bitboard positions to console
    if (Debug::getIsDebugLogEnabled()) {
        syncCout << "Move start: " << start << syncEndl;
        syncCout << "Piece Type: " << move->pieceType << syncEndl;
        syncCout << "Piece Color: " << pos->getCurrentMoveColor() << (pos->getCurrentMoveColor() == 1 ? " (Black)" : " (White)") << syncEndl;
        syncCout << "Move end: " << end << syncEndl;
    }

    // move the piece
    bool isValid = pos->playerMove(*move);
    deselectSquare();

    if (Debug::getIsDebugLogEnabled()){
        Console::printCombinedBitboards(pos);
    }

    // update the display board
    Graphics::createDisplayBitboard(pos, _displayBoard);

    return isValid;
}

void Window::deselectSquare(){
    // SquareSelected is a 'substate' of PlayerTurn, here we are unselecting a square by clicking
    // off the board so we'll set state back to PlayerTurn
    _guiState = PlayerTurn;
    _selectedSquareIndex = -1;
}

void Window::drawSelectedSquareIndex(){
    SDL_Color white = {255, 255, 255};

    // create a rect to put the font message in
    SDL_Rect message_rect;
    message_rect.x = _guiHorizontalTextOffset;  // x position on window
    message_rect.y = _guiVerticalTextOffset*2;  // y position on window, offset from Mouse Position display
    message_rect.w = 100;
    message_rect.h = 20;

    std::string selectedPosition = "Selected Position: " + (_selectedSquareIndex >= 0 ? squareIndexUppercase(_selectedSquareIndex) : "");
    SDL_Surface* messageSurf = TTF_RenderText_Blended(_gfx->getFont(), selectedPosition.c_str(), white);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(_renderer, messageSurf);
    SDL_QueryTexture(tex, nullptr, nullptr, &message_rect.w, &message_rect.h);
    SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
    SDL_RenderCopy(_renderer, tex, nullptr, &message_rect);
    SDL_FreeSurface(messageSurf);
    SDL_DestroyTexture(tex);
}

SDL_Rect Window::getBoardDimensions(){
    SDL_Rect board;
    board.x = (_gfx->TILE_WIDTH * (int)SCREEN_SCALE) * 2;
    board.y = (_gfx->TILE_HEIGHT * (int)SCREEN_SCALE) * 2;
    board.w = _gfx->TILE_WIDTH * 8 * (int)SCREEN_SCALE;
    board.h = _gfx->TILE_HEIGHT * 8 * (int)SCREEN_SCALE;

    return board;
}

void Window::renderTexture(SDL_Texture* texture, SDL_Rect tile) {
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    SDL_RenderCopy(_renderer, texture, nullptr, &tile);
}

void Window::renderTintedTexture(SDL_Texture *texture, SDL_Rect tile, SDL_Color tint) {
    // Tint the texture by the given RGB values
    SDL_SetTextureColorMod(texture, tint.r, tint.g, tint.b);
    renderTexture(texture, tile);
}

void Window::renderLighterTexture(SDL_Texture* texture, SDL_Rect tile, int timesLighter) {
    renderTexture(texture, tile);
    // Rendering the same texture on top of itself with SDL_BLENDMODE_ADD lightens the texture.
    // Source: https://forums.libsdl.org/viewtopic.php?p=51593
    // See David Olofson's post.
    for(int i = 0; i < timesLighter; i++) {
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_ADD);
        SDL_RenderCopy(_renderer, texture, nullptr, &tile);
    }
}

void Window::renderIndexText(int row, int col, SDL_Rect tile) {
    char label = '@'; // Junk char to check if we need to display it
    if(row == 9 && col > 0 && col < 9) {
        label = 'A' + col - 1;
        tile.x += 11;
        tile.y += 10;
    } else if(col == 0 && row > 0 && row < 9) {
        label = '9' - row;
        tile.x += 10;
        tile.y += 9;
    }
    if(label != '@') {
        std::string message = std::string() + label;
        SDL_Surface* messageSurf = TTF_RenderText_Blended(_gfx->getFont(), message.c_str(), {255, 255, 255});
        SDL_Texture* tex = SDL_CreateTextureFromSurface(_renderer, messageSurf);
        SDL_QueryTexture(tex, nullptr, nullptr, &tile.w, &tile.h);
        SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
        SDL_RenderCopy(_renderer, tex, nullptr, &tile);
        SDL_FreeSurface(messageSurf);
        SDL_DestroyTexture(tex);
    }
}

void Window::startEnemySearch(std::shared_ptr<Position> pos) {
    logMessage("The computer is deciding on a move to make...");
    SearchPool.done = false;
    SearchPool.startSearch(*pos, std::vector<Move>());
}

void Window::CheckEnemySearch(std::shared_ptr<Position> pos) {
    if(SearchPool.done){
        SearchOutput output = SearchPool.getOutput();
        if(output.node.nextNodes.empty()) return; // Checkmate/Stalemate
        SearchNode bestNode = output.node.nextNodes.front();
        bestNode.eval;
        for(auto nd : output.node.nextNodes){
            if(nd.eval < bestNode.eval){
                bestNode = nd;
            }
        }
        pos->move(bestNode.move);
        Graphics::createDisplayBitboard(pos, _displayBoard);
        _guiState = PlayerTurn;

        clearMessageLog();
    }
}

void Window::startUserSearch(std::shared_ptr<Position> pos) {
    SearchPool.done = false;
    SearchPool.startSearch(*pos, std::vector<Move>());

    // This loop is to prevent the user from doing anything while the computer searches for moves.
    bool flag = false;
    while(!flag) {
        flag = SearchPool.done;
    }

    SearchOutput output = SearchPool.getOutput();
    if(output.node.nextNodes.empty()) return;

    std::vector<SearchNode> nodes = output.node.nextNodes;
    // The nodes are sorted from smallest eval value to largest
    std::sort(nodes.begin(), nodes.end());

    // Create a string with the top 5 suggestions to be printed.
    std::string message = "Top 5 Suggestions: \n";
    int counter = nodes.size() < 5 ? nodes.size() : 5;
    for(int i = 0; i < counter; i++) {
        if(nodes.size() >= i) {
            message += "    " + std::to_string(i + 1);
            message += ". Move " + PieceTypeStrings[nodes[i].move.pieceType];
            message += " from " + SquareIndexStrings[nodes[i].move.startSquare] + " to ";
            message += SquareIndexStrings[nodes[i].move.endSquare] + ".\n";
        }
    }

    logMessage(message);
}

void Window::logMessage(std::string message) {
    _loggedMessage = std::move(message);
}

void Window::clearMessageLog() {
    _loggedMessage = "";
}
