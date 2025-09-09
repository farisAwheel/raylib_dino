#include "raylib.h"

// this structure will represent our player
typedef struct Player {
	Vector2 position;
	Vector2 size;
	Vector2 speed;
	Rectangle bounds;
} Player;

// this structure will represent our obstacles
typedef struct Obstacle {
	Vector2 size;
	Vector2 position;
	Rectangle bounds;
	bool active;
} Obstacle;

typedef enum GameScreen { GAMEPLAY, ENDING } GameScreen; // this will help us keep track of what's happening in the game!

int main() {
	const int screenWidth = 800;
	const int screenHeight = 450;
	const int groundHeight = 400;
	const float gravity = 0.8f;
	const int numObstacles = 4;
	GameScreen screen = GAMEPLAY; // initalize the screen to gameplay
	int frameCounter = 0; // this counter will help us to spawn in objects

	// lets initalize the player for the game
	Player player;
	player.size = (Vector2){25, 50};
	const int playerGround = groundHeight-player.size.y; // this value is used to handle our movement logic
	player.position = (Vector2){50, playerGround};
	player.speed = (Vector2){5.0f, 0.0f};

	// lets initalize our obstacles
	Obstacle obstacles[4];
	for(int i=0; i<numObstacles; i++){
		obstacles[i].active = false;
		obstacles[i].size = (Vector2){GetRandomValue(25, 30), GetRandomValue(25, 60)};
		obstacles[i].position = (Vector2){0.0f, 0.0f};
	}
	int spawnNextObj = GetRandomValue(30, 60); // this random value will help us spawn objects

	InitWindow(screenWidth, screenHeight, "Window Title");
	SetTargetFPS(60);

	while (!WindowShouldClose()) {
		switch(screen) {
			case(GAMEPLAY): 
			{
				frameCounter++; // this counter will be used to help spawn objects
			
				// player movement
				player.position.y += player.speed.y; // at all times, we should adjust player position by player speed
				if(player.position.y < playerGround) player.speed.y += gravity; // if we're in the air, gravity applies
				if(player.position.y == playerGround && IsKeyDown(KEY_UP)) player.speed.y -= 12.0f; // if we're on the ground & we press UP, give player vertical speed
				// keep player from falling through the floor
				if(player.position.y > playerGround) {
					player.speed.y=0.0f;
					player.position.y = playerGround;
				}
				
				player.bounds = (Rectangle){player.position.x, player.position.y, player.size.x, player.size.y}; // this rectangle represents our player's hitbox

				// obstacle logic
				for(int i=0; i<numObstacles; i++) {
					obstacles[i].bounds = (Rectangle){obstacles[i].position.x, obstacles[i].position.y, obstacles[i].size.x, obstacles[i].size.y}; // this rectangle represents the object's hitbox
					if(obstacles[i].position.x < 0 - obstacles[i].size.x) obstacles[i].active = false; // free up obstacles to spawn if they're off screen
					if(obstacles[i].active && CheckCollisionRecs(player.bounds, obstacles[i].bounds)) screen = ENDING; // end game if the player is hitting an object
					if(obstacles[i].active) obstacles[i].position.x -= player.speed.x;
				}
				// obstacle spawning
				if(frameCounter == spawnNextObj) {
					frameCounter = 0;
					spawnNextObj = GetRandomValue(50, 200);	
					for(int i=0; i<numObstacles; i++){
						if(!obstacles[i].active) {
							obstacles[i].position.x = screenWidth + obstacles[i].size.x;
							obstacles[i].position.y = groundHeight - obstacles[i].size.y;
							obstacles[i].active = true;
							break;
						}
					}
				}
			} break;
			
			// this case will only trigger after the screen has been set to ending!
			case(ENDING):
			{
				// if the user presses enter, reinitalize everything to go back to the start of gameplay
				if(IsKeyPressed(KEY_ENTER)) {
					player.size = (Vector2){25, 50};
					player.position = (Vector2){50, playerGround};
					player.speed = (Vector2){5.0f, 0.0f};

					for(int i=0; i<numObstacles; i++) {
						obstacles[i].active = false;
						obstacles[i].size = (Vector2){GetRandomValue(25, 30), GetRandomValue(25, 60)};
						obstacles[i].position = (Vector2){0.0f, 0.0f};
					}

					screen = GAMEPLAY;
				}
			} break;

			
		}
		

		// this is us drawing the game for the player to see!
		BeginDrawing();
			ClearBackground(RAYWHITE);

			switch(screen){
				case(GAMEPLAY):
				{
					DrawRectangle(player.position.x, player.position.y, player.size.x, player.size.y, GREEN);   // this is our player
					for(int i=0; i<numObstacles; i++){
						if(obstacles[i].active) DrawRectangle(obstacles[i].position.x, obstacles[i].position.y, obstacles[i].size.x, obstacles[i].size.y, RED);
					}
					DrawLine(0, groundHeight, screenWidth, groundHeight, GRAY); // this is the line representing the ground
				} break;

				case(ENDING):
				{
					DrawText("\t\t Thanks for playing!\n Press [ENTER] to restart", 25, 300, 25, GREEN);
				} break;
			}

		EndDrawing();
	}

	CloseWindow();
	return 0;
}