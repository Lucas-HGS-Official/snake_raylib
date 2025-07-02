#define CLAY_IMPLEMENTATION
#include "clay.h"
#include "clay_renderer_raylib.c"

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

int init_game(void);
void game_loop(void);
int spawn_food(void);
int draw(void);
bool should_update(float seconds);
void get_input(void);
void update(void);
void check_collision(void);
void init_raylib_window(void);
void init_game_and_window(void);
void handle_clay_errors(Clay_ErrorData errorData);

double lastUpdate = 0;

int main(void) {
    init_game_and_window();

    game_loop();

    Clay_Raylib_Close();
    
    return 0;
}


void check_collision(void) {
    Vector2 head = state.snake[0];
    for(size_t i = 1; i < state.count; i++) {
        Vector2 currSnake = state.snake[i];
        if(Vector2Equals(currSnake, head)) {
            init_game();

            return;
        }
    }

    if(head.x < 0 || head.x > GRID_SIZE-1) {
        init_game();
        
        return;
    }

    if(head.y < 0 || head.y > GRID_SIZE-1) {
        init_game();

        return;
    }
}

void update(void) {
    Vector2 head = state.snake[0];

    if(Vector2Equals(head, state.food)) {
        for(size_t i = state.count; i > 0; i--) {
            state.snake[i] = state.snake[i -1];
        }
        state.count++;

        state.snake[0] = Vector2Add(head, state.direction);
        spawn_food();

        return;
    }

    for(size_t i = state.count -1; i > 0; i--) {
        state.snake[i] = state.snake[i -1];
    }

    state.snake[0] = Vector2Add(head, state.direction);

    check_collision();
}

int draw(void) {
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
        DrawRectangleV((Vector2) { currSnake.x * (GRID + GRID_SPACE) + MARGIN, currSnake.y * (GRID + GRID_SPACE) + MARGIN }, (Vector2) { GRID, GRID }, BLUE);
    }

    return 0;
}

bool should_update(float seconds) {
    double currTime = GetTime();
    if(currTime - lastUpdate > seconds) {
        lastUpdate = currTime;
        
        return true;
    }

    return false;
}

int spawn_food(void) {
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

int init_game(void) {
    state.food = (Vector2) { 0 };
    spawn_food();
    state.snake[0] = (Vector2) { 3, 3 };
    state.direction = (Vector2) { 1, 0 };
    state.count = 1;

    return 0;
}

void game_loop(void) {
    Font fonts = GetFontDefault();

    while (!WindowShouldClose()) {
        Clay_BeginLayout();

        CLAY({
            .id = CLAY_ID("Container"),
            .backgroundColor = (Clay_Color) { 245, 245, 245, 255 },
            .layout = {
                .sizing = { .width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_GROW(), },
                .childAlignment = { .x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER },
                .padding = CLAY_PADDING_ALL(8),
                .childGap = 8,
            },
        }) {
            for(int i=0; i<2;i++) {
                CLAY({
                    .backgroundColor = (Clay_Color) { 245, 0, 0, 255 },
                    .layout = {
                        .sizing = { .width = CLAY_SIZING_FIXED(GRID_SIZE), .height = CLAY_SIZING_FIXED(GRID_SIZE) },
                    },
                }) {}
            }
        }

        Clay_RenderCommandArray renderCommands = Clay_EndLayout();

        BeginDrawing();
        {
            if(should_update(0.3)) {
                update();
            }
            get_input();
            Clay_Raylib_Render(renderCommands, &fonts);
            draw();
        }
        EndDrawing();
    }
}

void get_input(void) {
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

void init_raylib_window(void) {
    Clay_Raylib_Initialize(SCREEN_WIDTH, SCREEN_HEIGHT, "raylib", FLAG_WINDOW_RESIZABLE);
    uint64_t clayRequiredMemory = Clay_MinMemorySize();
    Clay_Arena clayMemory = Clay_CreateArenaWithCapacityAndMemory(clayRequiredMemory, malloc(clayRequiredMemory));

    Clay_Initialize(clayMemory, (Clay_Dimensions) {
        .height = GetScreenHeight(),
        .width = GetScreenWidth(),
    }, (Clay_ErrorHandler) { handle_clay_errors });
    SetTargetFPS(60);
}

void init_game_and_window(void) {
    init_raylib_window();
    init_game();
}

void handle_clay_errors(Clay_ErrorData errorData) {
    printf("%s\n", errorData.errorText.chars);
}