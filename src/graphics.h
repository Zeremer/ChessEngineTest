//
// Created by Clinton Brown on 2/24/19.
//

#pragma once

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "types.h"
#include "position.h"

class Graphics {
    public:
    int TILE_WIDTH = 32;
    int TILE_HEIGHT = 32;

    explicit Graphics(SDL_Renderer*);

    ~Graphics();

    /**
     * @brief Translates bitboard locations onto a 2D char array for display in the UI
     * @author Clinton Brown
     * @param Position object with bitboard position information
     * @param char[8][8] array to put translated positions into
     */
    static void createDisplayBitboard(std::shared_ptr<Position>, char[8][8]);

    /**
     * @brief Gets the white checkerboard tile texture
     * @author Clinton Brown
     * @return The white checkerboard tile texture
     */
    SDL_Texture* getWhiteBoardTile();

    /**
     * @brief Gets the black checkerboard tile texture
     * @author Clinton Brown
     * @return The black checkerboard tile texture
     */
    SDL_Texture* getBlackBoardTile();

    /**
     * @brief Getters for the border tiles.
     * @author Bryant Foster
     */
    SDL_Texture* getBorderTileTopLeft();
    SDL_Texture* getBorderTileTopMiddle();
    SDL_Texture* getBorderTileTopRight();
    SDL_Texture* getBorderTileLeftMiddle();
    SDL_Texture* getBorderTileRightMiddle();
    SDL_Texture* getBorderTileBottomLeft();
    SDL_Texture* getBorderTileBottomMiddle();
    SDL_Texture* getBorderTileBottomRight();

    // Getters for individual sprites for pieces
    // author Clinton Brown
    SDL_Texture* getBlackPawnSprite();
    SDL_Texture* getWhitePawnSprite();
    SDL_Texture* getBlackRookSprite();
    SDL_Texture* getWhiteRookSprite();
    SDL_Texture* getBlackKnightSprite();
    SDL_Texture* getWhiteKnightSprite();
    SDL_Texture* getBlackBishopSprite();
    SDL_Texture* getWhiteBishopSprite();
    SDL_Texture* getBlackQueenSprite();
    SDL_Texture* getWhiteQueenSprite();
    SDL_Texture* getBlackKingSprite();
    SDL_Texture* getWhiteKingSprite();

    // Getters for individual alternate sprites for pieces
    // author Bryant Foster
    SDL_Texture* getAlternateBlackPawnSprite();
    SDL_Texture* getAlternateWhitePawnSprite();
    SDL_Texture* getAlternateBlackRookSprite();
    SDL_Texture* getAlternateWhiteRookSprite();
    SDL_Texture* getAlternateBlackKnightSprite();
    SDL_Texture* getAlternateWhiteKnightSprite();
    SDL_Texture* getAlternateBlackBishopSprite();
    SDL_Texture* getAlternateWhiteBishopSprite();
    SDL_Texture* getAlternateBlackQueenSprite();
    SDL_Texture* getAlternateWhiteQueenSprite();
    SDL_Texture* getAlternateBlackKingSprite();
    SDL_Texture* getAlternateWhiteKingSprite();

    // Getters for text textures
    // author Clinton Brown
    SDL_Texture* getMousePositionTexture();

    /**@brief Get font for GUI display
     * @author Clinton Brown
     */
     TTF_Font* getFont();

    private:
    /**
     * @brief Translates bitboard locations onto a 2D char array for display in the UI
     * @author Clinton Brown
     * @param Bitboard bitboard to translate
     * @param char[8][8] array to put translated positions into
     * @param char character to replace the bitboard 1s with
     */
    static void bitboardTo2DCharArray(Bitboard, char[8][8], char);

    /**
     * @brief Gets a tile from an image with multiple tiles
     * @author Clinton Brown
     * @param SDL_Rect A rectangle representing a portion of the image
     * @param SDL_Renderer The renderer for the SDL window
     * @param SDL_Texture The source image for the tile to be taken from
     */
    SDL_Texture* getTileFromTexture(SDL_Rect, SDL_Renderer*, SDL_Texture*);

    /**@brief GUI image containing board tiles
     * @author Clinton Brown
     */
    SDL_Texture* _boardImage;

    /**@brief Image containing sprites for the chess pieces
     * @author Clinton Brown
     */
    SDL_Texture* _piecesImage;

    /**
     * @brief Image containing the alternate sprites for the chess pieces.
     * @author Bryant Foster
     */
    SDL_Texture* _altPiecesImage;

    /**@brief GUI display tile for white board spaces
     * @author Clinton Brown
     */
    SDL_Texture* _whiteBoardTile;

    /**@brief GUI display tile for black board spaces
     * @author Clinton Brown
     */
    SDL_Texture* _blackBoardTile;

    /**
     * @brief Individual border tiles
     * @author Bryant Foster
     */
    SDL_Texture* _boardBorderTopLeftTile;
    SDL_Texture* _boardBorderTopRightTile;
    SDL_Texture* _boardBorderTopMiddleTile;
    SDL_Texture* _boardBorderLeftMiddleTile;
    SDL_Texture* _boardBorderRightMiddleTile;
    SDL_Texture* _boardBorderBottomLeftTile;
    SDL_Texture* _boardBorderBottomRightTile;
    SDL_Texture* _boardBorderBottomMiddleTile;

    // Individual sprites for pieces
    // author Clinton Brown
    SDL_Texture* _blackPawnSprite;
    SDL_Texture* _whitePawnSprite;
    SDL_Texture* _blackRookSprite;
    SDL_Texture* _whiteRookSprite;
    SDL_Texture* _blackKnightSprite;
    SDL_Texture* _whiteKnightSprite;
    SDL_Texture* _blackBishopSprite;
    SDL_Texture* _whiteBishopSprite;
    SDL_Texture* _blackQueenSprite;
    SDL_Texture* _whiteQueenSprite;
    SDL_Texture* _blackKingSprite;
    SDL_Texture* _whiteKingSprite;

    // Individual sprites for the alternate pieces
    // author Bryant Foster
    SDL_Texture* _altBlackPawnSprite;
    SDL_Texture* _altWhitePawnSprite;
    SDL_Texture* _altBlackRookSprite;
    SDL_Texture* _altWhiteRookSprite;
    SDL_Texture* _altBlackKnightSprite;
    SDL_Texture* _altWhiteKnightSprite;
    SDL_Texture* _altBlackBishopSprite;
    SDL_Texture* _altWhiteBishopSprite;
    SDL_Texture* _altBlackQueenSprite;
    SDL_Texture* _altWhiteQueenSprite;
    SDL_Texture* _altBlackKingSprite;
    SDL_Texture* _altWhiteKingSprite;

    /**@brief Font for GUI display
     * @author Clinton Brown
     */
    TTF_Font* _font;
};
