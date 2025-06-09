#include "raylib.h"
#include "raymath.h"
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
    Vector2 head = state.snake[0];
    for(size_t i = state.count -1; i < 0; i++) {
        state.snake[i] = state.snake[i -1];
    }

    state.snake[0] = Vector2Add(head, state.direction);
}

int draw() {
    ClearBackground(RAYWHITE);

    for(size_t i = 0; i < GRID_SIZE; i++) {
        for(size_t j = 0; j < GRID_SIZE; j++) {
            DrawRectangleV((Vector2) { i * (GRID + GRID_SPACE) + MARGIN, j * (GRID + GRID_SPACE) + MARGIN }, (Vector2) { GRID, GRID }, GRAY);
        }
    }

    DrawRectangleV((Vector2) { state.food.x * (GRID + GRID_SPACE) + MARGIN, state.food.y * (GRID + GRID_SPACE) + MARGIN }, (Vector2) { GRID, GRID }, RED);

    for(size_t i = 0; i < state.count; i++) {
        Vector2 currSnake = state.snake[i];
        if(i == 0) {
            DrawRectangleV((Vector2) { currSnake.x * (GRID + GRID_SPACE) + MARGIN, currSnake.y * (GRID + GRID_SPACE) + MARGIN }, (Vector2) { GRID, GRID }, DARKBLUE);
            continue;
        }
        DrawRectangleV((Vector2) { currSnake.x * (GRID + GRID_SPACE) + MARGIN, currSnake.y * (GRID + GRID_SPACE) + MARGIN }, (Vector2) { GRID, GRID }, GRAY);
    }

    return 0;
}

double lastUpdate = 0;
bool should_update(float seconds) {
    double currTime = GetTime();
    if(currTime - lastUpdate > seconds) {
        lastUpdate = currTime;
        
        return true;
    }

    return false;
}

int spawn_food() {
    while(true) {
        float x = GetRandomValue(0, GRID_SIZE - 1);
        float y = GetRandomValue(0, GRID_SIZE - 1);
        state.food = (Vector2) { x, y };

        bool isFoodEaten = false;

        for(size_t i = 0; i < state.count; i++) {
            Vector2 currSnake = state.snake[i];
            if(Vector2Equals(currSnake, state.food)) {
                isFoodEaten = true;
                break;
            }
        }
        if(!isFoodEaten) {
            return 0;
        }
    }

    return 0;
}

int init_game() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "raylib");
    SetTargetFPS(60);

    state.food = (Vector2) { 0 };
    spawn_food();
    state.snake[0] = (Vector2) { 3, 3 };
    state.direction = (Vector2) { 1, 0 };
    state.count = 1;

    return 0;
}

void get_input() {
    if(IsKeyPressed(KEY_A) && state.direction.x != 1) {
        state.direction = (Vector2) { -1, 0 };
    }

    if(IsKeyPressed(KEY_D) && state.direction.x != -1) {
        state.direction = (Vector2) { 1, 0 };
    }

    if(IsKeyPressed(KEY_W) && state.direction.y != 1) {
        state.direction = (Vector2) { 0, -1 };
    }

    if(IsKeyPressed(KEY_S) && state.direction.y != -1) {
        state.direction = (Vector2) { 0, 1 };
    }
}

int main(void) {
    init_game();

    while(!WindowShouldClose()) {
        BeginDrawing();
        {
            if(should_update(0.5)) {
                update();
            }
            get_input();
            draw();
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
