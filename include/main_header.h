#include<stdio.h>
#include<stdlib.h>
#include "macros.h"
#include "raylib.h"
#include <pthread.h>
#include <stdbool.h>

#define INIT_WIDTH 1000
#define INIT_HEIGHT 600
#define INIT_FPS 165
#define player1_x 30
#define player2_x INIT_WIDTH - player1_x
#define player_length 100
#define player_width 10
#define ball_pos_x INIT_WIDTH / 2
#define ball_pos_y INIT_HEIGHT / 2

// ------------------------ Structures ------------------------------------

typedef struct
{
	Vector2 pos;
	Vector2 size;
	Color color;
	u8 font_size;
	char* text;
}Button;

typedef struct 
{
	Button** array;
	size_t size;
}Button_array;

typedef struct
{
	Vector2 start;
	Vector2 end;
	float thickness;
	i16 up_control;
	i16 down_control;
}Player;

typedef struct
{
	Vector2 pos;
	u8 radius;
}Ball;

// ----------------------- void Functions ----------------------------------
void createWindow();
void mainLoop();
void NewButton(Button_array** b_array, Vector2 pos, Vector2 size, Color color, char* text, u8 font_size);
void checkForExit();
void RenderButton(Button* data);
void updatePlayers(Player* player1, Player* player2);
void updateBall(Ball* ball);
void renderPlayers(Player* player1, Player* player2);
void renderBall(Ball* ball);
void updatePlayerInput(Player* player);

// ----------------------- struct Functions -----------------------------

Button_array* InitButtonArray();
Player* createPlayer(Vector2 start, Vector2 end, float thickness, i16 up_control, i16 down_control);
Ball* createBall(Vector2 pos, u8 radius);

// ----------------------- variable Functions ------------------------------------

u8 hoverMouse(Button* button);
float updateDT();

// ----------------------- definitions -------------------------------------

void createWindow()
{
	InitWindow(INIT_WIDTH, INIT_HEIGHT, "Test");

	SetTargetFPS(INIT_FPS);
}

void mainLoop()
{
	createWindow();
	
	Button_array* b_array = InitButtonArray();
	NewButton(&b_array, (Vector2){100, 100}, (Vector2){100, 50}, WHITE, "nigger", 10);

	Player* player1 = createPlayer((Vector2){player1_x, 250}, (Vector2){player1_x, 350}, player_width, KEY_W, KEY_S);
	Player* player2 = createPlayer((Vector2){player2_x, 250}, (Vector2){player2_x, 350}, player_width, KEY_UP, KEY_DOWN);

	Ball* ball = createBall((Vector2){ball_pos_x, ball_pos_y}, 5);

	while(!WindowShouldClose())
	{
		checkForExit();

		BeginDrawing();

		ClearBackground(BLACK);

		updateBall(ball);
		updatePlayers(player1, player2);

		//RenderButton(b_array->array[0]);

		EndDrawing();
	}
}

void NewButton(Button_array** b_array, Vector2 pos, Vector2 size, Color color, char* text, u8 font_size)
{
	Button* button = (Button*)malloc(sizeof(Button));
	button->pos = pos;
	button->size = size;
	button->color = color;
	button->text = text;
	button->font_size = font_size;

	(*b_array)->size++;
	(*b_array)->array = (Button**)realloc((*b_array)->array, sizeof(Button) * (*b_array)->size);
	(*b_array)->array[(*b_array)->size - 1] = button;
}

Button_array* InitButtonArray()
{
	Button_array* b_array = (Button_array*)malloc(sizeof(Button_array));
	b_array->array = (Button**)malloc(sizeof(Button*));
	b_array->size = 0;
	return b_array;
}

void checkForExit()
{
	if(IsKeyPressed(KEY_Q))
		CloseWindow();
}

u8 hoverMouse(Button* button)
{
	Vector2 current_mouse_pos = GetMousePosition();

	if(current_mouse_pos.x > button->pos.x && current_mouse_pos.x < button->pos.x + button->size.x &&
		current_mouse_pos.y > button->pos.y && current_mouse_pos.y < button->pos.x + button->size.y)
		return 1;
	return 0;
}

void RenderButton(Button* data)
{
	static i8  counter = 0;
	i16 text_pos_x = (data->pos.x + data->size.x / 2) - (MeasureText(data->text, data->font_size) / 2);
	i16 text_pos_y = (data->pos.y + data->size.y / 2) - (data->font_size / 2);

	if(hoverMouse(data) == 1)
	{
		DrawRectangleV((Vector2){data->pos.x - 1, data->pos.y - 1}, (Vector2){data->size.x + 2, data->size.y + 2}, BLUE);
	}

	DrawRectangleV(data->pos, data->size, data->color);
	DrawText(data->text, text_pos_x, text_pos_y, data->font_size, RED);
}

Player* createPlayer(Vector2 start, Vector2 end, float thickness, i16 up_control, i16 down_control)
{
	Player* player = (Player*)malloc(sizeof(Player));
	player->thickness = thickness;
	player->start = start;
	player->end = end;
	player->up_control = up_control;
	player->down_control = down_control;

	return player;
}

Ball* createBall(Vector2 pos, u8 radius)
{
	Ball* ball = (Ball*)malloc(sizeof(Ball));
	ball->pos = pos;
	ball->radius = radius;

	return ball;
}

void updatePlayerInput(Player* player)
{
	float dt = GetFrameTime();

	if(IsKeyDown(player->up_control))
	{
		if(player->start.y <= 1)
			player->start.y == 1;
		else
		{
			player->start.y -= 200 * dt;
			player->end.y -= 200  * dt;
		}
	}

	if(IsKeyDown(player->down_control))
	{
		if(player->end.y >= INIT_HEIGHT - 1)
			player->end.y == INIT_HEIGHT - 1;
		else
		{
			player->start.y += 200 * dt;
			player->end.y += 200 * dt;
		}
	}
}

void updatePlayers(Player* player1, Player* player2)
{
	updatePlayerInput(player1);
	updatePlayerInput(player2);

	renderPlayers(player1, player2);
}

void updateBall(Ball* ball)
{
	renderBall(ball);
}

void renderPlayers(Player* player1, Player* player2)
{
	DrawLineEx(player1->start, player1->end, player1->thickness, WHITE);
	DrawLineEx(player2->start, player2->end, player2->thickness, WHITE);
}

void renderBall(Ball* ball)
{
	DrawCircleV(ball->pos, ball->radius, WHITE);
}