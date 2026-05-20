#include <stdio.h>
#include <deque>

#include "config.h"

#define _GREEN			RGB(173, 204, 96)
#define _DARKGREEN		RGB(43, 51, 24)

#define CELL_SIZE		30
#define CELL_COUNT		25
#define OFFSET			75

double lastUpdateTime = 0;

bool eventTriggered(double interval)
{
	double currentTime = GetTime();
	if(currentTime - lastUpdateTime >= interval)
	{
		lastUpdateTime = currentTime;
		return true;
	}
	return false;
}

bool elementInDeque(Vector2 element, std::deque<Vector2> deque)
{
	for(unsigned int i = 0; i < deque.size(); i++)
	{
		if(Vector2Equals(deque[i], element))
			return true;
	}
	return false;
}

class Snake
{
public:
	std::deque<Vector2> body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
	Vector2 direction = {1, 0};
	bool addSegment = false;
	
	void Draw()
	{
		for(unsigned int i = 0; i < body.size(); i++)
		{
			float x = body[i].x;
			float y = body[i].y;
			Rectangle segment = {OFFSET + x * CELL_SIZE, OFFSET + y * CELL_SIZE, (float)CELL_SIZE, (float)CELL_SIZE};
			DrawRectangleRounded(segment, 0.5, 6, _DARKGREEN);
		}
	}
	
	void Update()
	{
		body.push_front(Vector2Add(body[0], direction));			
		if(addSegment)
		{
			addSegment = false;
		}
		else
		{
			body.pop_back();
		}
	}
	
	void Reset()
	{
		body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
		direction = {1, 0};
	}
	
protected:

private:

};

class Food
{
public:
	Vector2 position;
	Texture2D texture;
	
	Food(std::deque<Vector2> snakeBody)
	{
		Image image = LoadImage("./assets/sprites/food.png");
		texture = LoadTextureFromImage(image);
		UnloadImage(image);
		position = GenerateRandomPos(snakeBody);
	}
	
	~Food()
	{
		UnloadTexture(texture);
	}
	
	void Draw()
	{
		DrawTexture(texture, OFFSET + position.x * CELL_SIZE, OFFSET + position.y * CELL_SIZE, WHITE);
	}
	
	Vector2 GenerateRandomCell()
	{
		float x = GetRandomValue(0, CELL_COUNT - 1);
		float y = GetRandomValue(0, CELL_COUNT - 1);
		return Vector2{x, y};
	}
	
	Vector2 GenerateRandomPos(std::deque<Vector2> snakeBody)
	{
		Vector2 pos = GenerateRandomCell();
		while(elementInDeque(pos, snakeBody))
			pos = GenerateRandomCell();
		return pos;
	}
	
protected:

private:

};

class Game
{
public:
	Snake snake = Snake();
	Food food = Food(snake.body);
	bool running = true;
	int score = 0;
	Sound eatSound;
	Sound wallSound;
	
	Game()
	{
		InitAudioDevice();
		eatSound = LoadSound("./assets/sounds/eat.mp3");
		wallSound = LoadSound("./assets/sounds/wall.mp3");
	}
	
	~Game()
	{
		UnloadSound(eatSound);
		UnloadSound(wallSound);
		CloseAudioDevice();
	}
	
	void Draw()
	{
		food.Draw();
		snake.Draw();
	}
	
	void Update()
	{
		if(running)
		{
			snake.Update();
			CheckCollisionWithFood();
			CheckCollisionWithEdges();
			CheckCollisionWithTail();
		}
	}
	
	void CheckCollisionWithFood()
	{
		if(Vector2Equals(snake.body[0], food.position))
		{
			food.position = food.GenerateRandomPos(snake.body);
			snake.addSegment = true;
			score++;
			PlaySound(eatSound);
		}
	}
	
	void CheckCollisionWithEdges()
	{
		if(snake.body[0].x == CELL_COUNT || snake.body[0].x == -1)
			GameOver();
		if(snake.body[0].y == CELL_COUNT || snake.body[0].y == -1)
			GameOver();		
	}
	
	void CheckCollisionWithTail()
	{
		std::deque<Vector2> headlessBody = snake.body;
		headlessBody.pop_front();
		if(elementInDeque(snake.body[0], headlessBody))
			GameOver();
 	}
	
	void GameOver()
	{
		snake.Reset();
		food.position = food.GenerateRandomPos(snake.body);
		running = false;
		score = 0;
		PlaySound(wallSound);
	}

protected:

private:

};

int main(int argc, char** argv)
{
	/// Initialize
	InitWindow(2 * OFFSET + CELL_SIZE * CELL_COUNT, 2 * OFFSET + CELL_SIZE * CELL_COUNT, "Retro Snake");
	SetTargetFPS(CURRENT_FPS);
	
	Game game;
	
	while(!WindowShouldClose())
	{
		/************* Update *************/
		float deltaTime = GetFrameTime();
		if(eventTriggered(0.2))
		{
			game.Update();
		}
		
		if(IsKeyPressed(KEY_UP) && game.snake.direction.y != 1)
		{
			game.snake.direction = {0, -1};
			game.running = true;
		}
		if(IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1)
		{
			game.snake.direction = {0, 1};
			game.running = true;
		}
		if(IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1)
		{
			game.snake.direction = {-1, 0};
			game.running = true;
		}
		if(IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1)
		{
			game.snake.direction = {1, 0};
			game.running = true;
		}
		
		/************* Collision Detection *************/
		
		/************* Draw *************/
		BeginDrawing();		
			ClearBackground(_GREEN);
			
			DrawRectangleLinesEx(Rectangle{(float)OFFSET - 5, (float)OFFSET - 5, (float)CELL_SIZE * CELL_COUNT + 10, (float)CELL_SIZE * CELL_COUNT + 10}, 5, _DARKGREEN);
			DrawText("Retro Snake", OFFSET - 5, 20, 40, _DARKGREEN);
			DrawText(TextFormat("Score: %i", game.score), OFFSET - 5, OFFSET + CELL_SIZE * CELL_COUNT + 10, 40, _DARKGREEN);
			game.Draw();
			
		EndDrawing();
	}
	
	CloseWindow();
	
    return 0;
}