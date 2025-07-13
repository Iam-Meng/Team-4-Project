@echo off
gcc main.c bird.c obstacle.c background.c tigr.c -o flappy.exe -lgdi32 -lopengl32
if exist flappy.exe (
    flappy.exe
)
pause