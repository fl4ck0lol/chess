#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include "raylib.h"
#include "math.h"

#define null NULL
#define ui8 unsigned char 
#define tileSize 64
#define windowSize (tileSize * 9)
#define BLACKCOLOR (Color){185, 206, 172, 255}
#define WHITECOLOR (Color){242, 238, 231, 255}
#define SELECTORCOLOR RED
#define MOVECOLOR (Color){100, 100, 100, 100}

typedef enum {_WHITE, _BLACK}PlayerColor;
typedef enum {PAWN, KING, QUEEN, ROOK, KNIGHT, BISHOP}PieceType;

typedef struct ui8Vec
{
	ui8 x, y;
}ui8Vec;

typedef struct Piece
{
	bool hasMoved, hasLeaped;
	PieceType pieceType;
	PlayerColor color;
	Texture2D texture;
	ui8Vec* moves;
	int movesCount;
}Piece;

typedef struct Tile
{
	ui8 x, y, size;
	Color color;
	Piece* piece;
}Tile;

typedef struct Board
{
	Tile* allTiles[8][8];
	PlayerColor turn;
}Board;

typedef struct Selector
{
	ui8 x, y;
	bool selected, hasMove;
	Piece* selectedPiece;
	Color outlineColor;
	Color moveColor;
}Selector;

void initBoard(Board* b);
void drawBoard(Board* b);
void initPiece(Board* b, size_t x, size_t y, PieceType p);
void arrangeBoard(Board* b);
void initSelector(Selector* s);
void drawSelector(Selector* s);
void addMove(Piece* p, size_t x, size_t y);
void updateInput(Board* b, Selector* s);
void updateMoves(Board* b, Selector* s);
void drawPieces(Board* b);

void initBoard(Board* b)
{	
	for(size_t i = 0; i < 8; i++)
	{
		for(size_t j = 0; j < 8; j++)
		{
			b->allTiles[i][j] = (Tile*)malloc(sizeof(Tile));

			b->allTiles[i][j]->x = i;
			b->allTiles[i][j]->y = j;
			b->allTiles[i][j]->size = tileSize;

			if(i % 2 == 0 && j % 2 == 0)
				b->allTiles[i][j]->color = WHITECOLOR;
			if(i % 2 == 1 && j % 2 == 1)
				b->allTiles[i][j]->color = WHITECOLOR;

			if(i % 2 == 1 && j % 2 == 0)
				b->allTiles[i][j]->color = BLACKCOLOR;
			if(i % 2 == 0 && j % 2 == 1)
				b->allTiles[i][j]->color = BLACKCOLOR;

			b->allTiles[i][j]->piece = null;
		}
	}
}

void drawBoard(Board* b)
{	
	for(size_t i = 0; i < 8; i++)
	{
		for(size_t j = 0; j < 8; j++)
		{
			DrawRectangle(
				(b->allTiles[i][j]->x + 1) * b->allTiles[i][j]->size, 
				b->allTiles[i][j]->y * b->allTiles[i][j]->size, 
				b->allTiles[i][j]->size, 
				b->allTiles[i][j]->size, 
				b->allTiles[i][j]->color
				); 
		}
	}

	char buff[2];

	for(size_t i = 8; i > 0; i--)
	{
		buff[0] = '0' + (9 - i);
		buff[1] = '\0';

		DrawText(buff, tileSize / 2 - MeasureText(buff, tileSize) / 2, (i - 1) * tileSize, tileSize, RAYWHITE);
	}

	for(size_t i = 0; i < 8; i++)
	{
		buff[0] = 'A' + i;
		buff[1] = '\0';

		DrawText(buff, (i + 1) * tileSize + tileSize / 2 - MeasureText(buff, tileSize) / 2, tileSize * 8, tileSize, RAYWHITE);
	}
}

void initPiece(Board* b, size_t x, size_t y, PieceType p)
{
	b->allTiles[x][y]->piece = (Piece*)malloc(sizeof(Piece));

	b->allTiles[x][y]->piece->hasMoved = 0;
	b->allTiles[x][y]->piece->hasLeaped = 0;
	b->allTiles[x][y]->piece->pieceType = p;
	b->allTiles[x][y]->piece->moves = null;
	b->allTiles[x][y]->piece->movesCount = 0;

	if(y == 1 || y == 6)
	{
		if(y == 6)
		{
			b->allTiles[x][y]->piece->color =_WHITE;
			b->allTiles[x][y]->piece->texture = LoadTexture("resources/piece_textures/pawn/white_pawn.png");
		}
		else
		{
			b->allTiles[x][y]->piece->color =_BLACK;
			b->allTiles[x][y]->piece->texture = LoadTexture("resources/piece_textures/pawn/black_pawn.png");
		}
		return;
	}

	switch(x)
	{
		case 0:
			if(y == 7)
			{
				b->allTiles[x][y]->piece->color =_WHITE;
				b->allTiles[x][y]->piece->texture = LoadTexture("resources/piece_textures/rook/white_rook.png");
			}
			else
			{
				b->allTiles[x][y]->piece->color =_BLACK;
				b->allTiles[x][y]->piece->texture = LoadTexture("resources/piece_textures/rook/black_rook.png");
			}
			break;
		case 7:
			if(y == 7)
			{
				b->allTiles[x][y]->piece->color =_WHITE;
				b->allTiles[x][y]->piece->texture = LoadTexture("resources/piece_textures/rook/white_rook.png");
			}
			else
			{
				b->allTiles[x][y]->piece->color =_BLACK;
				b->allTiles[x][y]->piece->texture = LoadTexture("resources/piece_textures/rook/black_rook.png");
			}
			break;
		case 1:
			if(y == 7)
			{
				b->allTiles[x][y]->piece->color =_WHITE;
				b->allTiles[x][y]->piece->texture = LoadTexture("resources/piece_textures/knight/white_knight.png");
			}
			else
			{
				b->allTiles[x][y]->piece->color =_BLACK;
				b->allTiles[x][y]->piece->texture = LoadTexture("resources/piece_textures/knight/black_knight.png");
			}
			break;
		case 6:
			if(y == 7)
			{
				b->allTiles[x][y]->piece->color =_WHITE;
				b->allTiles[x][y]->piece->texture = LoadTexture("resources/piece_textures/knight/white_knight.png");
			}
			else
			{
				b->allTiles[x][y]->piece->color =_BLACK;
				b->allTiles[x][y]->piece->texture = LoadTexture("resources/piece_textures/knight/black_knight.png");
			}
			break;
		case 2:
			if(y == 7)
			{
				b->allTiles[x][y]->piece->color =_WHITE;
				b->allTiles[x][y]->piece->texture = LoadTexture("resources/piece_textures/bishop/white_bishop.png");
			}
			else
			{
				b->allTiles[x][y]->piece->color =_BLACK;
				b->allTiles[x][y]->piece->texture = LoadTexture("resources/piece_textures/bishop/black_bishop.png");
			}
			break;
		case 5:
			if(y == 7)
			{
				b->allTiles[x][y]->piece->color =_WHITE;
				b->allTiles[x][y]->piece->texture = LoadTexture("resources/piece_textures/bishop/white_bishop.png");
			}
			else
			{
				b->allTiles[x][y]->piece->color =_BLACK;
				b->allTiles[x][y]->piece->texture = LoadTexture("resources/piece_textures/bishop/black_bishop.png");
			}
			break;
		case 3:
			if(y == 7)
			{
				b->allTiles[x][y]->piece->color =_WHITE;
				b->allTiles[x][y]->piece->texture = LoadTexture("resources/piece_textures/queen/white_queen.png");
			}
			else
			{
				b->allTiles[x][y]->piece->color =_BLACK;
				b->allTiles[x][y]->piece->texture = LoadTexture("resources/piece_textures/king/black_king.png");
			}
			break;
		case 4:
			if(y == 7)
			{
				b->allTiles[x][y]->piece->color =_WHITE;
				b->allTiles[x][y]->piece->texture = LoadTexture("resources/piece_textures/king/white_king.png");
			}
			else
			{
				b->allTiles[x][y]->piece->color =_BLACK;
				b->allTiles[x][y]->piece->texture = LoadTexture("resources/piece_textures/queen/black_queen.png");
			}
			break;
		default:
		break; 
	}
}

void arrangeBoard(Board* b)
{
	for(size_t i = 0; i < 8; i++)
		for(size_t j = 0; j < 8; j++)
			b->allTiles[i][j]->piece = null;

	for(size_t i = 0; i < 8; i++)
	{
		initPiece(b, i, 1, PAWN);
		initPiece(b, i, 6, PAWN);
	}

	initPiece(b, 0, 0, ROOK);
	initPiece(b, 0, 7, ROOK);
	initPiece(b, 7, 0, ROOK);
	initPiece(b, 7, 7, ROOK);

	initPiece(b, 1, 0, KNIGHT);
	initPiece(b, 1, 7, KNIGHT);
	initPiece(b, 6, 0, KNIGHT);
	initPiece(b, 6, 7, KNIGHT);

	initPiece(b, 2, 0, BISHOP);
	initPiece(b, 2, 7, BISHOP);
	initPiece(b, 5, 0, BISHOP);
	initPiece(b, 5, 7, BISHOP);

	initPiece(b, 3, 0, BISHOP);
	initPiece(b, 3, 7, BISHOP);
	initPiece(b, 4, 0, BISHOP);
	initPiece(b, 4, 7, BISHOP);

	b->turn = _WHITE;
}

void initSelector(Selector* s)
{
	s->x = 0;
	s->y = 0;
	s->selected = 0;
	s->hasMove = 0;
	s->selectedPiece = null;
	s->outlineColor = SELECTORCOLOR;
	s->moveColor = MOVECOLOR;
}

void drawSelector(Selector* s)
{
	if(s->selected)
	{
		DrawRectangleLines((s->x + 1) * tileSize, s->y * tileSize, tileSize, tileSize, s->outlineColor);
		if(s->selectedPiece != null)
			for(size_t i = 0; i < s->selectedPiece->movesCount; i++)
				DrawCircle((s->selectedPiece->moves[i].x + 1) * tileSize + tileSize / 2, 
					s->selectedPiece->moves[i].y * tileSize + tileSize / 2, 20, s->moveColor);
	}
}

void addMove(Piece* p, size_t x, size_t y)
{
	if(p->movesCount == 0)
	{
		p->movesCount++;
		p->moves = (ui8Vec*)malloc(sizeof(ui8Vec));
		p->moves[p->movesCount - 1] = (ui8Vec){x, y};
		printf("[%d][%d]", p->moves[p->movesCount - 1].x, p->moves[p->movesCount - 1].y);
		return;
	}

	p->movesCount++;
	p->moves = (ui8Vec*)realloc(p->moves, sizeof(ui8Vec) * p->movesCount);
	p->moves[p->movesCount - 1] = (ui8Vec){x, y};
	printf("[%d][%d]", p->moves[p->movesCount - 1].x, p->moves[p->movesCount - 1].y);
}

void updateInput(Board* b, Selector* s)
{
	int x, y;

	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
	{
    	x = GetMousePosition().x;
    	y = GetMousePosition().y;

    	if(y <= tileSize * 8 && x >= tileSize && b->allTiles[(x / tileSize) - 1][y / tileSize]->piece != null)
    	{
    		s->selected = 1;
    		s->x = (x / tileSize) - 1;
    		s->y = (y / tileSize);
    		s->selectedPiece = b->allTiles[s->x][s->y]->piece;
    		
    		updateMoves(b, s);
    	}
	}
}

void updateMoves(Board* b, Selector* s)
{
	switch(b->allTiles[s->x][s->y]->piece->pieceType)
	{
		case PAWN:
			if(!b->allTiles[s->x][s->y]->piece->hasMoved 
			&& b->allTiles[s->x][s->y - 1]->piece == null
			&& b->allTiles[s->x][s->y - 2]->piece == null)
			{
				addMove(b->allTiles[s->x][s->y]->piece, s->x, s->y - 1);
				addMove(b->allTiles[s->x][s->y]->piece, s->x, s->y - 2);
			}
			break;

		default:
			break;
	}
}

void drawPieces(Board* b)
{
	for(size_t i = 0; i < 8; i++)
	{
		for(size_t j = 0; j < 8; j++)
		{
			if(b->allTiles[i][j]->piece != null && b->allTiles[i][j]->piece->texture.id != 0)
				DrawTexture(b->allTiles[i][j]->piece->texture, (b->allTiles[i][j]->x + 1) * 64, b->allTiles[i][j]->y * 64, WHITE);
		}
	}
}

int main(void)
{
	InitWindow(windowSize, windowSize, "chess");

	Board board;
	initBoard(&board);
	arrangeBoard(&board);

	Selector selector;
	initSelector(&selector);

	while(!WindowShouldClose())
	{
		updateInput(&board, &selector);

		BeginDrawing();

		ClearBackground(BLACK);

		drawBoard(&board);

		drawPieces(&board);

		drawSelector(&selector);

		EndDrawing();
	}
	return 0;
}