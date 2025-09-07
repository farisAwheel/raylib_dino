#include "raylib.h"

// this structure will represent our player
typedef struct Player {
	Vector2 position;
	Vector2 size;
	Vector2 speed;
	Rectangle bounds;
} Player;

int main() {
	const int screenWidth = 800;
	const int screenHeight = 450;
	const int groundHeight = 400;
	const float gravity = 1.2f;

	// lets initalize the player for the game
	Player player;
	player.size = (Vector2){25, 50};
	const int playerGround = groundHeight-player.size.y; // this value is used to handle our movement logic
	player.position = (Vector2){50, playerGround};
	player.speed = (Vector2){2.5f, 0.0f};

	InitWindow(screenWidth, screenHeight, "Window Title");
	SetTargetFPS(60);

	while (!WindowShouldClose()) {
		// player movement
		player.position.y += player.speed.y; // at all times, we should adjust player position by player speed
		if(player.position.y < playerGround) player.speed.y += gravity; // if we're in the air, gravity applies
		if(player.position.y == playerGround && IsKeyDown(KEY_UP)) player.speed.y -= 15.0f; // if we're on the ground & we press UP, give player vertical speed
		// keep player from falling through the floor
		if(player.position.y > playerGround) {
			player.speed.y=0.0f;
			player.position.y = playerGround;
		}
		
		player.bounds = (Rectangle){player.position.x, player.position.y, player.size.x, player.size.y}; // this rectangle represents our player's hitbox

		// this is us drawing the game for the player to see!
		BeginDrawing();
			ClearBackground(RAYWHITE);
			DrawRectangle(player.position.x, player.position.y, player.size.x, player.size.y, GREEN);   // this is our player
			DrawLine(0, groundHeight, screenWidth, groundHeight, GRAY); // this is the line representing the ground
		EndDrawing();
	}

	CloseWindow();
	return 0;
}