#include <iostream>
#include <raylib.h>

using namespace std;

int ventanaMain(void){
    const int screenWidth = 800;
    const int screenHeight = 450;
    bool win1 = true;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    Rectangle boton = { 100, 100, 200, 50 }; // Coordenadas y dimensiones del botón
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(PINK);

        // Verificar si se hizo clic en el botón
        if (CheckCollisionPointRec(GetMousePosition(), boton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            break;
            cout << "Funciona :D" << endl;
        }

        // Dibujar botón
        BeginDrawing();
        DrawRectangleRec(boton, BLACK);
        DrawText("Haz clic aquí", boton.x + 10, boton.y + 15, 20, WHITE);

        EndDrawing();

        EndDrawing();
        }
    CloseWindow();
        //----------------------------------------------------------------------------------

    InitWindow(screenWidth, screenHeight, "ventana2");
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here

        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

int main(){
    ventanaMain();
    return 0;
}