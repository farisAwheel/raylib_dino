#include "raylib.h"

int main() {
	const int screenWidth = 800;
	const int screenHeight = 450;

	InitWindow(screenWidth, screenHeight, "Window Title");
	SetTargetFPS(60);

	while (!WindowShouldClose()) {
		BeginDrawing();
			ClearBackground(RAYWHITE);
			DrawText("Simple Window!", 25, 300, 50, GRAY);
		EndDrawing();
	}

	CloseWindow();
	return 0;
}