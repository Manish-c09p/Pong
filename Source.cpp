#include <iostream>
#include <raylib.h>

using namespace std;
int MineScore = 0;
int CPUScore = 0;

Color BackGround = Color{ 63, 193, 186, 255 };
Color Strip = Color{ 54, 91, 109, 255 };
Color Oblong = Color{ 13, 135, 129, 255 };
Color Pong = Color{ 186, 255, 252, 255 };
Color Rec = Color{ 5, 71, 68, 255 };

class Ball {
public:
	float x, y;
	float radius;
	float SpeedX, SpeedY;

	void draw() {
		DrawCircle(x, y, radius, Pong);
	}

	void update() {
		x += SpeedX;
		y -= SpeedY;
		if (x - radius < 0) {
			CPUScore++;
			ResetBall();
			// OR For simple continuation
			//SpeedX *= -1;
		}
		if (x + radius > GetScreenWidth()) {
			MineScore++;
			ResetBall();
			// OR For simple continuation
			//SpeedY *= -1;
		}
		if (y - radius < 0 || y + radius > GetScreenHeight()) {
			SpeedY *= -1;
		}
	}

	void ResetBall() {
		x = GetScreenWidth() / 2.0f;
		y = GetScreenHeight() / 2.0f;

		int SpeedChoices[2] = { -1, 1 };
		SpeedX *= SpeedChoices[GetRandomValue(0, 1)];
		SpeedY *= SpeedChoices[GetRandomValue(0, 1)];
	}
};

class Paddle {
protected:
	void LimitMovement(){
		if (y <= 0) {
			y = 0;
		}
		if (y + height >= GetScreenHeight()) {
			y = GetScreenHeight() - height;
		}
	}
	
public:
	float x, y;
	float width, height;
	int speed;

	void draw() {
		DrawRectangleRounded(Rectangle{ x, y, width, height }, 0.8, 0, Rec);
	}

	void update() {
		if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
			y -= speed;
		}
		if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
			y += speed;
		}
		LimitMovement();
	}
};

class CPUpaddle : public Paddle {
public:
	void update(int ballY) {
		if (y + height >= ballY) {
			y -= speed;
		}
		if (y + height <= ballY) {
			y += speed;
		}
		LimitMovement();
	}
};

Ball ball;
Paddle Me;
CPUpaddle cpu;

int main() {
	float ScreenWidth = 1200;
	float ScreenHeight = 800;
	InitWindow(ScreenWidth, ScreenHeight, "Pong Game!!");
	//SetTargetFPS(90);
	SetWindowState(FLAG_VSYNC_HINT); // BOTH ARE CORRECT CHOOAE ACCORDING TO YOU REQUIREMENT.

	ball.x = ScreenWidth / 2;
	ball.y = ScreenHeight / 2;
	ball.radius = 15;
	ball.SpeedX = 5;
	ball.SpeedY = 5;

	Me.width = 25;
	Me.height = 150;
	Me.x = 20;
	Me.y = ScreenHeight / 2 - Me.height / 2;
	Me.speed = 5;

	cpu.width = 25;
	cpu.height = 150;
	cpu.x = ScreenWidth - cpu.width - 20;
	cpu.y = ScreenHeight / 2 - cpu.height / 2;
	cpu.speed = 5;

	float OlongWidth = 90;
	float OlongHeight = 180;
	float OlongX = ScreenWidth / 2 - 15 - 30; // Measured according to the initial position of the olong....
	float OlongY = ScreenHeight / 2 - OlongHeight / 2;

	while (!WindowShouldClose()) {
		BeginDrawing();

		ball.update();
		Me.update();
		cpu.update(ball.y);

		// For Collision Checking 
		if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{Me.x, Me.y, Me.width, Me.height})) {
			ball.SpeedX *= -1;
		}
		if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{cpu.x, cpu.y, cpu.width, cpu.height})) {
			ball.SpeedX *= -1;
		}
		
		ClearBackground(BackGround);

		ball.draw();
		Me.draw();
		cpu.draw();

		// Regaring the design of the whole game:
		DrawRectangleRounded(Rectangle{ OlongX, OlongY, OlongWidth, OlongHeight }, 1.1, 0, Oblong);
		DrawRectangle(ScreenWidth / 2, 0 , 15, ScreenHeight, WHITE);
		DrawRectangle(ScreenWidth / 2 - 15, 0, 15, ScreenHeight, Strip);

        // Regarding Score:
		DrawText(TextFormat("%i", MineScore), ScreenWidth / 4 - 20, 20, 80, WHITE);
		DrawText(TextFormat("%i", CPUScore), 3 * ScreenWidth / 4 - 20, 20, 80, WHITE);

		DrawFPS(10, 10);
		EndDrawing();
	}

	CloseWindow();
	return 0;
}
