#include "raylib.h"
#include "stddef.h"

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 800
#define GRID_SIZE 10
#define GRID 40
#define GRID_SPACE 5

void update() {

}
void draw() {
    ClearBackground(RAYWHITE);

    for(size_t i = 0; i < GRID_SIZE; i++) {
        for(size_t j = 0; j < GRID_SIZE; j++) {
            DrawRectangleV((Vector2) { i * (GRID + GRID_SPACE), j * (GRID + GRID_SPACE) }, (Vector2) { GRID, GRID }, GRAY);
        }
    }
}

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "raylib");

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
