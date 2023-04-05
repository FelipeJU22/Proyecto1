#include <iostream>
#include <raylib.h>
#include <math.h>


#define cantidad_max_ene 50
#define cantidad_disparos 50
#define wave1 10
#define wave2 20
#define wave3 50

using namespace std;
typedef enum GameScreen {INICIO, ESTRATEGIAS, GAMEPLAY, ENDING } GameScreen;

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
static float exponent = 1.0f;                 // Audio exponentiation value
static float averageVolume[400] = { 0.0f };   // Average volume history
static int velocity = 5;
static const char *nave = "/home/felipe/Proyecto1_Felipe_Jimenez-Carlos_Segura/imagenes/nave.png";
static const char *naveDown = "/home/felipe/Proyecto1_Felipe_Jimenez-Carlos_Segura/imagenes/naveDown.png";
static const char *naveUp = "/home/felipe/Proyecto1_Felipe_Jimenez-Carlos_Segura/imagenes/naveUp.png";



static Waves waves1 = {};

void juego(void){
    posJugx = 20;
    posJugy = 50;
    jugador1.velocidad.y = velocity;
}

void actJuego(void){
    if(posJugy>5){
        if (IsKeyDown(KEY_UP)) posJugy -= jugador1.velocidad.y;
        if (IsKeyDown(KEY_UP)) navecita = LoadTexture(naveUp);
        if (IsKeyReleased(KEY_UP)) navecita = LoadTexture(nave);
    }

    if(posJugy<345){
        if(IsKeyDown(KEY_DOWN)) posJugy+= jugador1.velocidad.y;
        if (IsKeyDown(KEY_DOWN)) navecita = LoadTexture(naveDown);
        if (IsKeyReleased(KEY_DOWN)) navecita = LoadTexture(nave);
    }
}

void actualizar(void){
    actJuego();
    //actDraw();
}
/**void ProcessAudio(void *buffer, unsigned int frames)
{
    float *samples = (float *)buffer;   // Samples internally stored as <float>s
    float average = 0.0f;               // Temporary average volume

    for (unsigned int frame = 0; frame < frames; frame++)
    {
        float *left = &samples[frame * 2 + 0], *right = &samples[frame * 2 + 1];

        *left = powf(fabsf(*left), exponent) * ( (*left < 0.0f)? -1.0f : 1.0f );
        *right = powf(fabsf(*right), exponent) * ( (*right < 0.0f)? -1.0f : 1.0f );

        average += fabsf(*left) / frames;   // accumulating average volume
        average += fabsf(*right) / frames;
    }

    // Moving history to the left
    for (int i = 0; i < 399; i++) averageVolume[i] = averageVolume[i + 1];

    averageVolume[399] = average;         // Adding last average value
}*/

int main(){
    const int screenWidth = 732;
    const int screenHeight = 413;

    InitWindow(screenWidth, screenHeight, "Battlespace");
    GameScreen currentScreen = INICIO;

    /**InitAudioDevice();
    AttachAudioMixedProcessor(ProcessAudio);

    Music musica = LoadMusicStream("/home/felipe/raylib/examples/audio/resources/country.mp3");
    Sound sonidoButt = LoadSound("/home/felipe/raylib/examples/audio/resources/sound.wav");
    PlayMusicStream(musica);*/

    // TODO: Initialize all required variables and load all required data here!

    int framesCounter = 0;          // Useful to count frames
    Rectangle boton1 = {100, 150, 150, 50 }; // Coordenadas y dimensiones del botón
    Rectangle boton2 = {100, 225, 150, 50 }; // Coordenadas y dimensiones del botón
    Rectangle boton3 = {100, 300, 150, 50 }; // Coordenadas y dimensiones del botón

    Rectangle strat1 = {15, 150, 150, 50 }; // Coordenadas y dimensiones del botón
    Rectangle strat2 = {197, 150, 150, 50 }; // Coordenadas y dimensiones del botón
    Rectangle strat3 = {385, 150, 150, 50 }; // Coordenadas y dimensiones del botón
    Rectangle strat4 = {565, 150, 150, 50 }; // Coordenadas y dimensiones del botón

    navecita = LoadTexture(nave);
    Texture2D fondoI = LoadTexture("/home/felipe/Proyecto1_Felipe_Jimenez-Carlos_Segura/imagenes/fondo.png");
    Texture2D fondoS = LoadTexture("/home/felipe/Proyecto1_Felipe_Jimenez-Carlos_Segura/imagenes/fondostrat.png");

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
                if (CheckCollisionPointRec(GetMousePosition(), boton1) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    currentScreen = ESTRATEGIAS;
                }
                if (CheckCollisionPointRec(GetMousePosition(), boton2) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    currentScreen = ESTRATEGIAS;
                }
            } break;
            case ESTRATEGIAS:
            {
                if (CheckCollisionPointRec(GetMousePosition(), strat1) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    nave = "/home/felipe/Proyecto1_Felipe_Jimenez-Carlos_Segura/imagenes/naveEsc.png";
                    naveDown = "/home/felipe/Proyecto1_Felipe_Jimenez-Carlos_Segura/imagenes/naveEscDown.png";
                    naveUp = "/home/felipe/Proyecto1_Felipe_Jimenez-Carlos_Segura/imagenes/naveEscUp.png";
                    currentScreen = GAMEPLAY;
                    juego();
                }
                if (CheckCollisionPointRec(GetMousePosition(), strat2) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    currentScreen = GAMEPLAY;
                    juego();
                }
                if (CheckCollisionPointRec(GetMousePosition(), strat3) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    currentScreen = GAMEPLAY;
                    velocity = 15;
                    juego();
                }
                if (CheckCollisionPointRec(GetMousePosition(), strat4) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    currentScreen = GAMEPLAY;
                    juego();
                }
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
                velocity = 5;
                nave = "/home/felipe/Proyecto1_Felipe_Jimenez-Carlos_Segura/imagenes/nave.png";
                naveDown = "/home/felipe/Proyecto1_Felipe_Jimenez-Carlos_Segura/imagenes/naveDown.png";
                naveUp = "/home/felipe/Proyecto1_Felipe_Jimenez-Carlos_Segura/imagenes/naveUp.png";
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
                DrawTexture(fondoI, 0, 0, RAYWHITE);
                DrawRectangleRec(boton1, PURPLE);
                DrawText("Fácil", boton1.x + 50, boton1.y + 15, 20, WHITE);
                DrawRectangleRec(boton2, VIOLET);
                DrawText("Intermedio", boton2.x + 25, boton2.y + 15, 20, WHITE);
                DrawRectangleRec(boton3, DARKPURPLE);
                DrawText("Díficil", boton3.x + 50, boton3.y + 15, 20, WHITE);
            } break;
            case ESTRATEGIAS:
            {
                // TODO: Draw INICIO screen here!
                //DrawTexture(fondoI, 0, 0, RAYWHITE);
                DrawTexture(fondoS, 0, 0, RAYWHITE);
                DrawRectangleRec(strat1, WHITE);
                DrawText("Escudo", strat1.x + 37, strat1.y + 15, 20, BLACK);
                DrawRectangleRec(strat2, LIGHTGRAY);
                DrawText("Cadencia", strat2.x + 35, strat2.y + 15, 20, BLACK);
                DrawRectangleRec(strat3, GRAY);
                DrawText("Velocidad", strat3.x + 25, strat3.y + 15, 20, BLACK);
                DrawRectangleRec(strat4, DARKGRAY);
                DrawText("Vida extra", strat4.x + 20, strat4.y + 15, 20, BLACK);
            } break;

            case GAMEPLAY:
            {
                // TODO: Draw GAMEPLAY screen here!
                DrawRectangle(0, 0, screenWidth, screenHeight, BLACK);
                DrawTexture(navecita, posJugx, posJugy, RAYWHITE);
                DrawText("GAMEPLAY SCREEN", 20, 20, 40, MAROON);
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