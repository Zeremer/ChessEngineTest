//
// Created by clint on 2/24/19.
//

#include "graphics.h"
#include "coutsync.h"

Graphics::Graphics(SDL_Renderer* renderer) {
    // load full image for board tiles
    std::string programPath = SDL_GetBasePath();
    std::string filePath = programPath.append("board.png");
    _boardImage = IMG_LoadTexture(renderer, filePath.c_str());

    // splice tiles for chessboard out of image
    SDL_Rect tile;
    tile.x = 0;
    tile.y = 0;
    tile.w = TILE_WIDTH;
    tile.h = TILE_HEIGHT;

    // white tile is at origin no need to move x,y
    _whiteBoardTile = getTileFromTexture(tile, renderer, _boardImage);

    // move down on board image and get the black tile
    tile.y += TILE_HEIGHT;
    _blackBoardTile = getTileFromTexture(tile, renderer, _boardImage);

    // Load Border Tiles
    tile.x = TILE_WIDTH;
    tile.y = 0;
    _boardBorderTopLeftTile = getTileFromTexture(tile, renderer, _boardImage);
    tile.y = TILE_HEIGHT;
    _boardBorderLeftMiddleTile = getTileFromTexture(tile, renderer, _boardImage);
    tile.y += TILE_HEIGHT;
    _boardBorderBottomLeftTile = getTileFromTexture(tile, renderer, _boardImage);
    tile.x += TILE_WIDTH;
    _boardBorderBottomMiddleTile = getTileFromTexture(tile, renderer, _boardImage);
    tile.x += TILE_WIDTH;
    _boardBorderBottomRightTile = getTileFromTexture(tile, renderer, _boardImage);
    tile.y -= TILE_HEIGHT;
    _boardBorderRightMiddleTile = getTileFromTexture(tile, renderer, _boardImage);
    tile.y -= TILE_HEIGHT;
    _boardBorderTopRightTile = getTileFromTexture(tile, renderer, _boardImage);
    tile.x -= TILE_WIDTH;
    _boardBorderTopMiddleTile = getTileFromTexture(tile, renderer, _boardImage);

    // load full image for chess pieces - probably should have used an array and looped this is a mess
    programPath = SDL_GetBasePath();
    filePath = programPath.append("pieces.png");
    _piecesImage = IMG_LoadTexture(renderer, filePath.c_str());
    tile.h = TILE_HEIGHT*2; // the sprites are twice the height of the tiles
    tile.x = 0;
    tile.y = 0;

    // white pieces
    _whitePawnSprite = getTileFromTexture(tile, renderer, _piecesImage);
    tile.x += TILE_WIDTH;
    _whiteRookSprite = getTileFromTexture(tile, renderer, _piecesImage);
    tile.x += TILE_WIDTH;
    _whiteKnightSprite = getTileFromTexture(tile, renderer, _piecesImage);
    tile.x += TILE_WIDTH;
    _whiteBishopSprite = getTileFromTexture(tile, renderer, _piecesImage);
    tile.x += TILE_WIDTH;
    _whiteQueenSprite = getTileFromTexture(tile, renderer, _piecesImage);
    tile.x += TILE_WIDTH;
    _whiteKingSprite = getTileFromTexture(tile, renderer, _piecesImage);

    // black pieces
    tile.x = 0;
    tile.y += tile.h;
    _blackPawnSprite = getTileFromTexture(tile, renderer, _piecesImage);
    tile.x += TILE_WIDTH;
    _blackRookSprite = getTileFromTexture(tile, renderer, _piecesImage);
    tile.x += TILE_WIDTH;
    _blackKnightSprite = getTileFromTexture(tile, renderer, _piecesImage);
    tile.x += TILE_WIDTH;
    _blackBishopSprite = getTileFromTexture(tile, renderer, _piecesImage);
    tile.x += TILE_WIDTH;
    _blackQueenSprite = getTileFromTexture(tile, renderer, _piecesImage);
    tile.x += TILE_WIDTH;
    _blackKingSprite = getTileFromTexture(tile, renderer, _piecesImage);

    programPath = SDL_GetBasePath();
    filePath = programPath.append("altPieces.png");
    _altPiecesImage = IMG_LoadTexture(renderer, filePath.c_str());
    tile.x = 0;
    tile.y = 0;
    tile.w = 128;
    tile.h = 128;

    // black alternate pieces
    _altBlackPawnSprite = getTileFromTexture(tile, renderer, _altPiecesImage);
    tile.x += 128;
    _altBlackKnightSprite = getTileFromTexture(tile, renderer, _altPiecesImage);
    tile.x += 128;
    _altBlackBishopSprite = getTileFromTexture(tile, renderer, _altPiecesImage);
    tile.x += 128;
    _altBlackRookSprite = getTileFromTexture(tile, renderer, _altPiecesImage);
    tile.x += 128;
    _altBlackQueenSprite = getTileFromTexture(tile, renderer, _altPiecesImage);
    tile.x += 128;
    _altBlackKingSprite = getTileFromTexture(tile, renderer, _altPiecesImage);

    // white alternate pieces
    tile.x = 0;
    tile.y += tile.h;
    _altWhitePawnSprite = getTileFromTexture(tile, renderer, _altPiecesImage);
    tile.x += 128;
    _altWhiteKnightSprite = getTileFromTexture(tile, renderer, _altPiecesImage);
    tile.x += 128;
    _altWhiteBishopSprite = getTileFromTexture(tile, renderer, _altPiecesImage);
    tile.x += 128;
    _altWhiteRookSprite = getTileFromTexture(tile, renderer, _altPiecesImage);
    tile.x += 128;
    _altWhiteQueenSprite = getTileFromTexture(tile, renderer, _altPiecesImage);
    tile.x += 128;
    _altWhiteKingSprite = getTileFromTexture(tile, renderer, _altPiecesImage);

    // Initialize TTF font support
    TTF_Init();

    // load font
    programPath = SDL_GetBasePath();
    filePath = programPath.append("Carlito-Regular.ttf");
    _font = TTF_OpenFont(filePath.c_str(), 16);
}

Graphics::~Graphics(){
    SDL_DestroyTexture(_boardImage);
    SDL_DestroyTexture(_piecesImage);
    SDL_DestroyTexture(_whiteBoardTile);
    SDL_DestroyTexture(_blackBoardTile);
    SDL_DestroyTexture(_blackPawnSprite);
    SDL_DestroyTexture(_whitePawnSprite);
    SDL_DestroyTexture(_blackRookSprite);
    SDL_DestroyTexture(_whiteRookSprite);
    SDL_DestroyTexture(_blackKnightSprite);
    SDL_DestroyTexture(_whiteKnightSprite);
    SDL_DestroyTexture(_blackBishopSprite);
    SDL_DestroyTexture(_whiteBishopSprite);
    SDL_DestroyTexture(_blackQueenSprite);
    SDL_DestroyTexture(_whiteQueenSprite);
    SDL_DestroyTexture(_blackKingSprite);
    SDL_DestroyTexture(_whiteKingSprite);
    SDL_DestroyTexture(_altPiecesImage);
    SDL_DestroyTexture(_altBlackPawnSprite);
    SDL_DestroyTexture(_altWhitePawnSprite);
    SDL_DestroyTexture(_altBlackRookSprite);
    SDL_DestroyTexture(_altWhiteRookSprite);
    SDL_DestroyTexture(_altBlackKnightSprite);
    SDL_DestroyTexture(_altWhiteKnightSprite);
    SDL_DestroyTexture(_altBlackBishopSprite);
    SDL_DestroyTexture(_altWhiteBishopSprite);
    SDL_DestroyTexture(_altBlackQueenSprite);
    SDL_DestroyTexture(_altWhiteQueenSprite);
    SDL_DestroyTexture(_altBlackKingSprite);
    SDL_DestroyTexture(_altWhiteKingSprite);
    SDL_DestroyTexture(_boardBorderBottomLeftTile);
    SDL_DestroyTexture(_boardBorderBottomMiddleTile);
    SDL_DestroyTexture(_boardBorderBottomRightTile);
    SDL_DestroyTexture(_boardBorderLeftMiddleTile);
    SDL_DestroyTexture(_boardBorderRightMiddleTile);
    SDL_DestroyTexture(_boardBorderTopLeftTile);
    SDL_DestroyTexture(_boardBorderTopMiddleTile);
    SDL_DestroyTexture(_boardBorderTopRightTile);
    TTF_CloseFont(_font);
    TTF_Quit();
}

SDL_Texture* Graphics::getTileFromTexture(SDL_Rect tile, SDL_Renderer* renderer, SDL_Texture* source){
    SDL_Texture* result(SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, tile.w, tile.h));
    SDL_SetRenderTarget(renderer, result);
    SDL_RenderCopy(renderer, source, &tile, nullptr);
    // reset target to default (the screen)
    SDL_SetRenderTarget(renderer, nullptr);

    return result;
}

void Graphics::bitboardTo2DCharArray(Bitboard bb, char char2D[8][8], char replaceWith){
    for (int row = 7; row >= 0; row--) {
        for (int col = 0; col < 8; col++) {
            if (((bb >> (8 * row + col)) & 1) == 1) {
                char2D[row][col] = replaceWith;
            }
        }
    }
}

void Graphics::createDisplayBitboard(std::shared_ptr<Position> board, char displayBoard[8][8]){
    // initialize displayBoard values to make sure we don't have any weird symbols in the array
    for (int row = 7; row >= 0; row--) {
        for (int col = 0; col < 8; col++) {
            displayBoard[row][col] = '.';
        }
    }

    // create bitboards for white pieces
    bitboardTo2DCharArray(board->getPiecePositions(White, Pawn), displayBoard, 'P');
    bitboardTo2DCharArray(board->getPiecePositions(White, Rook), displayBoard, 'R');
    bitboardTo2DCharArray(board->getPiecePositions(White, Knight), displayBoard, 'N');
    bitboardTo2DCharArray(board->getPiecePositions(White, Bishop), displayBoard, 'B');
    bitboardTo2DCharArray(board->getPiecePositions(White, Queen), displayBoard, 'Q');
    bitboardTo2DCharArray(board->getPiecePositions(White, King), displayBoard, 'K');

    // create bitboards for black pieces
    bitboardTo2DCharArray(board->getPiecePositions(Black, Pawn), displayBoard, 'p');
    bitboardTo2DCharArray(board->getPiecePositions(Black, Rook), displayBoard, 'r');
    bitboardTo2DCharArray(board->getPiecePositions(Black, Knight), displayBoard, 'n');
    bitboardTo2DCharArray(board->getPiecePositions(Black, Bishop), displayBoard, 'b');
    bitboardTo2DCharArray(board->getPiecePositions(Black, Queen), displayBoard, 'q');
    bitboardTo2DCharArray(board->getPiecePositions(Black, King), displayBoard, 'k');
}

SDL_Texture* Graphics::getWhiteBoardTile(){
    return _whiteBoardTile;
}
SDL_Texture* Graphics::getBlackBoardTile(){
    return _blackBoardTile;
}

SDL_Texture* Graphics::getBorderTileTopLeft() {
    return _boardBorderTopLeftTile;
}
SDL_Texture* Graphics::getBorderTileTopMiddle() {
    return _boardBorderTopMiddleTile;
}
SDL_Texture* Graphics::getBorderTileTopRight() {
    return _boardBorderTopRightTile;
}
SDL_Texture* Graphics::getBorderTileLeftMiddle() {
    return _boardBorderLeftMiddleTile;
}
SDL_Texture* Graphics::getBorderTileRightMiddle() {
    return _boardBorderRightMiddleTile;
}
SDL_Texture* Graphics::getBorderTileBottomLeft() {
    return _boardBorderBottomLeftTile;
}
SDL_Texture* Graphics::getBorderTileBottomMiddle() {
    return _boardBorderBottomMiddleTile;
}
SDL_Texture* Graphics::getBorderTileBottomRight() {
    return _boardBorderBottomRightTile;
}

SDL_Texture* Graphics::getBlackPawnSprite(){
    return _blackPawnSprite;
}

SDL_Texture* Graphics::getWhitePawnSprite(){
    return _whitePawnSprite;
}

SDL_Texture* Graphics::getBlackRookSprite(){
    return _blackRookSprite;
}

SDL_Texture* Graphics::getWhiteRookSprite(){
    return _whiteRookSprite;
}

SDL_Texture* Graphics::getBlackKnightSprite(){
    return _blackKnightSprite;
}

SDL_Texture* Graphics::getWhiteKnightSprite(){
    return _whiteKnightSprite;
}

SDL_Texture* Graphics::getBlackBishopSprite(){
    return _blackBishopSprite;
}

SDL_Texture* Graphics::getWhiteBishopSprite(){
    return _whiteBishopSprite;
}

SDL_Texture* Graphics::getBlackQueenSprite(){
    return _blackQueenSprite;
}

SDL_Texture* Graphics::getWhiteQueenSprite(){
    return _whiteQueenSprite;
}

SDL_Texture* Graphics::getBlackKingSprite(){
    return _blackKingSprite;
}

SDL_Texture* Graphics::getWhiteKingSprite(){
    return _whiteKingSprite;
}

SDL_Texture* Graphics::getAlternateBlackPawnSprite(){
    return _altBlackPawnSprite;
}

SDL_Texture* Graphics::getAlternateWhitePawnSprite(){
    return _altWhitePawnSprite;
}

SDL_Texture* Graphics::getAlternateBlackRookSprite(){
    return _altBlackRookSprite;
}

SDL_Texture* Graphics::getAlternateWhiteRookSprite(){
    return _altWhiteRookSprite;
}

SDL_Texture* Graphics::getAlternateBlackKnightSprite(){
    return _altBlackKnightSprite;
}

SDL_Texture* Graphics::getAlternateWhiteKnightSprite(){
    return _altWhiteKnightSprite;
}

SDL_Texture* Graphics::getAlternateBlackBishopSprite(){
    return _altBlackBishopSprite;
}

SDL_Texture* Graphics::getAlternateWhiteBishopSprite(){
    return _altWhiteBishopSprite;
}

SDL_Texture* Graphics::getAlternateBlackQueenSprite(){
    return _altBlackQueenSprite;
}

SDL_Texture* Graphics::getAlternateWhiteQueenSprite(){
    return _altWhiteQueenSprite;
}

SDL_Texture* Graphics::getAlternateBlackKingSprite(){
    return _altBlackKingSprite;
}

SDL_Texture* Graphics::getAlternateWhiteKingSprite(){
    return _altWhiteKingSprite;
}

TTF_Font* Graphics::getFont(){
    return _font;
}