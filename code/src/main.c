#include "raylib.h"
#include "stddef.h"

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 800
#define GRID_SIZE 10
#define GRID 40
#define GRID_SPACE 5
#define MARGIN 10

typedef struct {
    Vector2 direction;
    Vector2 snake[GRID_SIZE * GRID_SIZE];
    Vector2 food;
    size_t count;
} State;
State state = {0};

void update() {

}
int draw() {
    ClearBackground(RAYWHITE);

    for(size_t i = 0; i < GRID_SIZE; i++) {
        for(size_t j = 0; j < GRID_SIZE; j++) {
            DrawRectangleV((Vector2) { i * (GRID + GRID_SPACE) + MARGIN, j * (GRID + GRID_SPACE) + MARGIN }, (Vector2) { GRID, GRID }, GRAY);
        }
    }

    for(size_t i = 0; i < GRID_SIZE; i++) {
        
    }

    return 0;
}
int init_game() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "raylib");
    SetTargetFPS(60);

    state.food = (Vector2) { 0 };
    state.snake[0] = (Vector2) { 3, 3 };
    state.direction = (Vector2) { 1, 0 };

    return 0;
}

int main(void) {
    init_game();

    while(!WindowShouldClose()) {
        BeginDrawing();
        {
            update();
            draw();
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
