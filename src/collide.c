#include "raylib.h"
#include "raymath.h"

#define SIZE 10
#define RADIUS 25

typedef struct Ball {
    Vector2 position;
    Vector2 velocity;

    float mass;
    float radius;
    Color look;
} Ball;

void PlayerMovment(Ball* PLAYER);
void updateCollision(Ball* ball1, Ball* ball2);
void updateBall(Ball* ball);


int main() {
    #define screenWidth 1280
    #define screenHeight 720

    //Creating window contexts
    InitWindow(screenWidth, screenHeight, "Collide");
    SetTargetFPS(60);

    //Initalize
    Ball balls[SIZE];
    Ball temp = { {screenWidth/2, screenHeight - RADIUS-1}, {0, 0}, 5, RADIUS, WHITE};
    balls[0] = temp;
    for(int i = 1; i < SIZE; i++) {
        Ball temp = { {Remap(i, 0, SIZE-1, RADIUS, screenWidth-RADIUS), screenHeight/2}, { 100 + i*25, 100 + i*25}, 5, RADIUS, ColorFromHSV(Remap(i, 0, SIZE-1, 100, 360), 1.0, 1.0) };
        balls[i] = temp;
    }


    //GameLoop
    while(!WindowShouldClose()) {
        //Update
        PlayerMovment(&balls[0]);
        for(int i = 0; i < SIZE; i++) 
        {
            for(int j = i+1; j < SIZE; j++) 
            {
                if(CheckCollisionCircles(balls[i].position, balls[i].radius, balls[j].position, balls[j].radius)) {
                    Ball temp = balls[i];
                    updateCollision(&balls[i], &balls[j]);
                    updateCollision(&balls[j], &temp);
                }
            }
        }
        for(int i = 0; i < SIZE; i++) {
            if(IsKeyDown(KEY_APOSTROPHE)) 
            {
               balls[i].position.x = Remap(i, 0, SIZE-1, RADIUS, screenWidth-RADIUS);
               balls[i].position.y = screenHeight/2;
            }
            else 
            {
                updateBall(&balls[i]);
            }
        }
        //Draw
        BeginDrawing();
            ClearBackground(BLACK);
            for(int i = 0; i < SIZE; i++) {
                DrawCircleV(balls[i].position, balls[i].radius, balls[i].look);
            }

        EndDrawing();
    }

    CloseWindow();
    return 1;
}

void PlayerMovment(Ball* PLAYER) {
    int speed = (IsKeyDown(KEY_LEFT_SHIFT) ? 30 : 20);

    PLAYER->velocity.x += (IsKeyDown(KEY_A) ? -1 * speed : 0) + (IsKeyDown(KEY_D) ? speed : 0);
    PLAYER->velocity.y += (IsKeyDown(KEY_W) ? -1 * speed : 0) + (IsKeyDown(KEY_S) ? speed : 0);

    PLAYER->velocity = Vector2Scale(PLAYER->velocity, 0.95);
}

void updateCollision(Ball* ball1, Ball* ball2) {
    //m1*v1 + m2*v2 = m1*v3 + m2*v4
    //m1*v1^2 + m2*v2^2 = m1*v3^2 + m2*v4^2
    float a = 2 * ball2->mass / (ball1->mass + ball2->mass);
    float b = Vector2DotProduct(Vector2Subtract(ball1->velocity, ball2->velocity), Vector2Subtract(ball1->position, ball2->position));
    float c = Vector2LengthSqr(Vector2Subtract(ball1->position, ball2->position));
    Vector2 d = Vector2Subtract(ball1->position, ball2->position);

    ball1->velocity = Vector2Subtract(ball1->velocity, Vector2Scale(d, a * b/c));
}

void updateBall(Ball* ball) {
    if(ball->position.x <= ball->radius) 
    {
        ball->position.x = ball->radius;
        ball->velocity.x *= -1;
    } 
    else if (ball->position.x >= screenWidth - ball->radius)
    {
        ball->position.x = screenWidth - ball->radius;
        ball->velocity.x *= -1;
    }

    if(ball->position.y <= ball->radius) 
    {
        ball->position.y = ball->radius;
        ball->velocity.y *= -1;
    } 
    else if (ball->position.y >= screenHeight - ball->radius)
    {
        ball->position.y = screenHeight - ball->radius;
        ball->velocity.y *= -1;
    }
    
    // xf = xi + v*dt
    ball->position = Vector2Add(ball->position, Vector2Scale(ball->velocity, GetFrameTime()));
}