@echo off
gcc main.c bird.c obstacle.c tigr.c -o tigr-app.exe -lgdi32 -lopengl32
if exist tigr-app.exe (
    tigr-app.exe
)
pause