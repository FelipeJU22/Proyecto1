#include <iostream>
#include <raylib.h>


#define cantidad_max_ene 50
#define cantidad_disparos 50
#define wave1 10
#define wave2 20
#define wave3 50

using namespace std;
typedef enum GameScreen {INICIO, GAMEPLAY, ENDING } GameScreen;

typedef enum { FIRST = 0, SECOND, THIRD } Waves;

typedef struct Jugador {
    Vector2 velocidad;
}Jugador;

typedef struct Enemigos {
    Image enemigos;
    Vector2 velocidad;
    bool active;
}Enemigos;

static Jugador jugador1 = { 0 };
static Enemigos enemigos1[cantidad_max_ene] = { 0 };
static int cadencia = 0;
static int enemigosActivos = 0;
static int posJugx = 0;
static int posJugy = 0;
static Texture2D navecita;


static Waves waves1 = {};

void juego(void){
    posJugx = 20;
    posJugy = 50;
    jugador1.velocidad.y = 5;
}

void actJuego(void){
    if(posJugy>5){
        if (IsKeyDown(KEY_UP)) posJugy -= jugador1.velocidad.y;
        if (IsKeyDown(KEY_UP)) navecita = LoadTexture("/home/felipe/Proyecto1_Felipe_Jimenez-Carlos_Segura/imagenes/naveUp.png");
        if (IsKeyReleased(KEY_UP)) navecita = LoadTexture("/home/felipe/Proyecto1_Felipe_Jimenez-Carlos_Segura/imagenes/nave.png");
    }

    if(posJugy<385){
        if(IsKeyDown(KEY_DOWN)) posJugy+= jugador1.velocidad.y;
        if (IsKeyDown(KEY_DOWN)) navecita = LoadTexture("/home/felipe/Proyecto1_Felipe_Jimenez-Carlos_Segura/imagenes/naveDown.png");
        if (IsKeyReleased(KEY_DOWN)) navecita = LoadTexture("/home/felipe/Proyecto1_Felipe_Jimenez-Carlos_Segura/imagenes/nave.png");
    }
}

/**void actDraw(void){
    BeginDrawing();
    DrawTexture(navecita, posJugx, posJugy, RAYWHITE);
    if(posJugy>5){
        if (IsKeyDown(KEY_UP)) DrawTexture(navecitaAr, posJugx, posJugy, RAYWHITE);

    }
    if(posJugy<385){
        if (IsKeyDown(KEY_UP)) DrawTexture(navecitaAb, posJugx, posJugy, RAYWHITE);
    }

    EndDrawing();
}
*/

void actualizar(void){
    actJuego();
    //actDraw();
}

int main(){
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Battlespace");
    GameScreen currentScreen = INICIO;

    // TODO: Initialize all required variables and load all required data here!

    int framesCounter = 0;          // Useful to count frames
    Rectangle boton = { 100, 100, 200, 50 }; // Coordenadas y dimensiones del botón

    navecita = LoadTexture("/home/felipe/Proyecto1_Felipe_Jimenez-Carlos_Segura/imagenes/nave.png");

    SetTargetFPS(60);               // Set desired framerate (frames-per-second)
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key

    {
        // Update
        //----------------------------------------------------------------------------------
        switch(currentScreen)
        {
            case INICIO:
            {
                // TODO: Update INICIO screen variables here!

                BeginDrawing();


                // Verificar si se hizo clic en el botón
                if (CheckCollisionPointRec(GetMousePosition(), boton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    currentScreen = GAMEPLAY;
                    juego();
                    cout << "Funciona :D" << endl;
                }

                // Dibujar botón

                // Press enter to change to GAMEPLAY screen

            } break;
            case GAMEPLAY:
            {
                // TODO: Update GAMEPLAY screen variables here!

                actualizar();
                // Press enter to change to ENDING screen
                if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
                {
                    currentScreen = ENDING;
                }
            } break;
            case ENDING:
            {
                // TODO: Update ENDING screen variables here!

                // Press enter to return to INICIO screen
                if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
                {
                    currentScreen = INICIO;
                }
            } break;
            default: break;
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        switch(currentScreen)
        {
            case INICIO:
            {
                // TODO: Draw INICIO screen here!
                DrawRectangle(0, 0, screenWidth, screenHeight, GREEN);
                DrawRectangleRec(boton, BLACK);
                DrawText("Haz clic aquí", boton.x + 10, boton.y + 15, 20, WHITE);
                DrawText("INICIO SCREEN", 20, 20, 40, DARKGREEN);
                DrawText("PRESS ENTER or TAP to JUMP to GAMEPLAY SCREEN", 120, 220, 20, DARKGREEN);

            } break;

            case GAMEPLAY:
            {
                // TODO: Draw GAMEPLAY screen here!
                DrawTexture(navecita, posJugx, posJugy, RAYWHITE);
                DrawText("GAMEPLAY SCREEN", 20, 20, 40, MAROON);
                DrawText("PRESS ENTER or TAP to JUMP to ENDING SCREEN", 130, 220, 20, MAROON);

            } break;
            case ENDING:
            {
                // TODO: Draw ENDING screen here!
                DrawRectangle(0, 0, screenWidth, screenHeight, BLUE);
                DrawText("ENDING SCREEN", 20, 20, 40, DARKBLUE);
                DrawText("PRESS ENTER or TAP to RETURN to INICIO SCREEN", 120, 220, 20, DARKBLUE);

            } break;
            default: break;
        }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------

    // TODO: Unload all loaded data (textures, fonts, audio) here!
    UnloadTexture(navecita);
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}