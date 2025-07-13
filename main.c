#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "tigr.h"
#include "background.h"
#include "obstacle.h"
#include "bird.h"

#define TIGR_IMPLEMENTATION

int main() {
    // Set screen size
    int screenWidth = 680, screenHeight = 720;
    Tigr* screen = tigrWindow(screenWidth, screenHeight, "Flappy Bird", TIGR_FIXED);
    if (!screen) return 1;

    // Load background
    Tigr* background = load_scaled_background("background2.png", screenWidth, screenHeight);
    if (!background) {
        background = tigrBitmap(screenWidth, screenHeight);
        tigrClear(background, tigrRGB(135, 206, 235)); // Fallback to sky blue
    }

    // Load bird sprite with proper scaling
    Tigr* originalBird = tigrLoadImage("bird-removebg.png");
    Tigr* birdSprite = NULL;
    int birdSize = 60;  // Resize the bird
    if (originalBird) {
        // Create bird with preserved transparency
        birdSprite = tigrBitmap(birdSize, birdSize);
        tigrClear(birdSprite, tigrRGBA(0, 0, 0, 0)); // Transparent background
        
        for (int y = 0; y < birdSize; y++) {
            for (int x = 0; x < birdSize; x++) {
                int srcX = (int)((float)x / birdSize * originalBird->w);
                int srcY = (int)((float)y / birdSize * originalBird->h);
                
                srcX = srcX < 0 ? 0 : (srcX >= originalBird->w ? originalBird->w - 1 : srcX);
                srcY = srcY < 0 ? 0 : (srcY >= originalBird->h ? originalBird->h - 1 : srcY);
                
                TPixel pixel = tigrGet(originalBird, srcX, srcY);
                // Only draw non-transparent pixels
                if (pixel.a > 0) {
                    tigrPlot(birdSprite, x, y, pixel);
                }
            }
        }
        tigrFree(originalBird);
    } else {
        // Fallback if image not found
        birdSprite = tigrBitmap(birdSize, birdSize);
        tigrClear(birdSprite, tigrRGBA(0, 0, 0, 0));
        tigrFill(birdSprite, 5, 5, birdSize-10, birdSize-10, tigrRGB(255, 255, 0));
        tigrFill(birdSprite, birdSize-15, 15, 10, 10, tigrRGB(0, 0, 0)); // Eye
        tigrFill(birdSprite, birdSize-20, 30, 15, 5, tigrRGB(255, 0, 0)); // Beak
    }

    // Initialize bird
    Bird bird;
    initBird(&bird, birdSprite, screenWidth * 0.3f, screenHeight * 0.3f);

    // Initialize pipes
    srand(time(NULL));
    #define MAX_PIPES 5
    Pipe pipes[MAX_PIPES];
    int pipeCount = 0;
    float pipeSpawnTimer = 0.0f;
    float pipeSpawnInterval = 2.0f;
    float pipeSpeed = 150.0f;
    const float pipeWidth = 80.0f;
    const float pipeGap = 220.0f;
    const int groundLevel = screenHeight - 100;
    
    // Game state
    int gameState = 0;  // 0 = waiting for start, 1 = game active, 2 = game over
    int score = 0;
    const float gravity = 800.0f;
    const float jumpForce = -350.0f;
    float last_time = tigrTime();
    float gameTime = 0.0f;
    float difficultyTimer = 0.0f;
    
    // Main game loop
    while (!tigrClosed(screen)) {
        // Calculate delta time
        float current_time = tigrTime();
        float dt = current_time - last_time;
        last_time = current_time;
        
        // Cap delta time
        if (dt > 0.1f) dt = 0.1f;

        // Handle input
        if (tigrKeyHeld(screen, TK_ESCAPE)) break;
        
        if (tigrKeyDown(screen, TK_SPACE)) {
            if (gameState == 0) {
                // Start game
                gameState = 1;
                jumpBird(&bird, jumpForce);
            } else if (gameState == 1 && bird.alive) {
                // Jump
                jumpBird(&bird, jumpForce);
            } else if (gameState == 2) {
                // Restart game
                gameState = 1;
                bird.alive = 1;
                bird.y = screenHeight * 0.3f;
                bird.velocity = 0;
                score = 0;
                pipeCount = 0;
                pipeSpawnTimer = 0.0f;
                pipeSpeed = 150.0f;
                gameTime = 0.0f;
            }
        }
        
        // Game logic
        if (gameState == 1) {
            gameTime += dt;
            difficultyTimer += dt;
            
            // Increase difficulty over time
            if (difficultyTimer > 5.0f) {
                pipeSpeed += 10.0f;
                difficultyTimer = 0.0f;
            }
            
            // Update bird physics
            updateBird(&bird, gravity, groundLevel, dt);
            
            // Spawn pipes
            pipeSpawnTimer += dt;
            if (pipeSpawnTimer >= pipeSpawnInterval && pipeCount < MAX_PIPES) {
                // Random gap position (ensuring it stays within screen bounds)
                float minGapY = 150 + pipeGap/2;
                float maxGapY = groundLevel - pipeGap/2 - 50;
                float gapY = minGapY + (rand() % (int)(maxGapY - minGapY));
                
                // Initialize pipe
                Pipe newPipe;
                newPipe.x = screenWidth;
                newPipe.gapY = gapY;
                newPipe.passed = 0;
                pipes[pipeCount] = newPipe;
                
                pipeCount++;
                pipeSpawnTimer = 0.0f;
            }
            
            // Update pipes and check collisions
            for (int i = 0; i < pipeCount; i++) {
                // Move pipe
                pipes[i].x -= pipeSpeed * dt;
                
                // Remove off-screen pipes
                if (pipes[i].x + pipeWidth < 0) {
                    // Shift remaining pipes
                    for (int j = i; j < pipeCount - 1; j++) {
                        pipes[j] = pipes[j+1];
                    }
                    pipeCount--;
                    i--;
                    continue;
                }
                
                // Check collision
                if (checkCollision(&bird, pipes[i].x, pipeWidth, pipes[i].gapY, pipeGap)) {
                    bird.alive = 0;
                    gameState = 2;
                }
                
                // Scoring
                if (!pipes[i].passed && pipes[i].x + pipeWidth < bird.x) {
                    pipes[i].passed = 1;
                    score++;
                }
            }
            
            // Ground collision
            if (bird.y + bird.height > groundLevel) {
                bird.y = groundLevel - bird.height;
                bird.alive = 0;
                gameState = 2;
            }
        }

        // Rendering
        tigrClear(screen, tigrRGB(20, 25, 35));
        tigrBlit(screen, background, 0, 0, 0, 0, background->w, background->h);
        
        // Draw pipes
        for (int i = 0; i < pipeCount; i++) {
            draw_pipe(screen, pipes[i].x, pipes[i].gapY, pipeWidth, pipeGap, groundLevel);
        }
        
        // Draw ground
        tigrFill(screen, 0, groundLevel, screenWidth, screenHeight - groundLevel, tigrRGB(80, 180, 60));
        
        // Draw bird with transparency
        tigrBlitAlpha(screen, bird.sprite, bird.x, bird.y, 0, 0, bird.width, bird.height, 1.0f);
        
        // Draw score
        char scoreText[50];
        snprintf(scoreText, sizeof(scoreText), "Score: %d", score);
        tigrPrint(screen, tfont, screenWidth/2 - 40, 20, tigrRGB(255, 255, 100), scoreText);
        
        // Draw speed indicator
        snprintf(scoreText, sizeof(scoreText), "Speed: %.0f", pipeSpeed);
        tigrPrint(screen, tfont, screenWidth - 150, 20, tigrRGB(200, 200, 255), scoreText);
        
        // Draw UI messages
        if (gameState == 0) {
            tigrPrint(screen, tfont, screenWidth/2 - 100, screenHeight/2 - 20, 
                      tigrRGB(255, 255, 255), "Press SPACE to start");
        } else if (gameState == 2) {
            tigrPrint(screen, tfont, screenWidth/2 - 80, screenHeight/2 - 40, 
                      tigrRGB(255, 100, 100), "GAME OVER");
            snprintf(scoreText, sizeof(scoreText), "Final Score: %d", score);
            tigrPrint(screen, tfont, screenWidth/2 - 100, screenHeight/2, 
                      tigrRGB(200, 255, 200), scoreText);
            tigrPrint(screen, tfont, screenWidth/2 - 150, screenHeight/2 + 40, 
                      tigrRGB(200, 255, 200), "Press SPACE to restart");
        }

        // Draw controls
        tigrPrint(screen, tfont, 20, 20, tigrRGB(200, 255, 200), "SPACE: Jump");
        tigrPrint(screen, tfont, screenWidth - 150, screenHeight - 20, tigrRGB(180, 220, 255), "ESC: Exit");
        
        // Update screen
        tigrUpdate(screen);
    }

    // Cleanup
    tigrFree(background);
    tigrFree(birdSprite);
    tigrFree(screen);
    return 0;
}