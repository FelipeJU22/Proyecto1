/**
 * Librerias externas utilizadas
 */
#include <iostream>
#include <raylib.h>
#include <math.h>


#define cantidad_max_ene 50
/**
 * Facilitar prints
 */
using namespace std;

/**
 * Se setean las distintas ventanas que se van a utilizar en el juego
 */
typedef enum GameScreen {INICIO, ESTRATEGIAS, GAMEPLAY, ENDING } GameScreen;

/**
 *
 */
typedef enum { FIRST = 0, SECOND, THIRD } Waves;

/**
 * Estructura utilizada para el jugador, se le da velocidad
 */
typedef struct Jugador {
    Vector2 velocidad;
}Jugador;
/**
 * Estructura utilizada para los enemigos,
 */
typedef struct Enemigos {
    Image enemigos;
    Vector2 velocidad;
    bool active;
}Enemigos;

/**
 * Estructura utilizada para los proyectiles aliados
 */
typedef struct Disparos{
    Rectangle rec;
    Vector2 balavel;
    bool active;
    int numero;
} ;
/**
 * Variables generales para el funcionamiento del juego, como son la velocidad del jugador, la posición, las imagenes bases de este, etc.
 */
static int cadencia = 0;
static int rate = 1;
static int posJugx = 0;
static int posJugy = 0;
static Texture2D navecita;
static int velocity = 5;
static const char *nave = "/home/felipe/Proyecto1_Felipe_Jimenez-Carlos_Segura/imagenes/nave.png";
static const char *naveDown = "/home/felipe/Proyecto1_Felipe_Jimenez-Carlos_Segura/imagenes/naveDown.png";
static const char *naveUp = "/home/felipe/Proyecto1_Felipe_Jimenez-Carlos_Segura/imagenes/naveUp.png";
static const int cantBalas = 1000;
static int cantVidas = 3;
static Jugador jugador1 = { 0 };
static Disparos disparos[cantBalas] = { 0 };
static string respuesta = "";
static bool poderEsc = false;
static bool poderCad = false;
static bool poderVel = false;
static bool poderVid = false;
static bool actPod = false;



/**
 * Clase nodo, utilizada para almacenar información
 * Con sus respectivos atributos y métodos
 * En este caso, almacenaran punteros
 */
class Nodo
{
public:
    int dato;
    Nodo *siguiente;
    Nodo(){
        dato = 0;
        siguiente = nullptr;
    }
     /**
     * Método constructor para nodos enteros
     * @param dato
     */
    Nodo(int dato)
    {
        this->dato = dato;
        this->siguiente = NULL;
    }
    /**
     * Método constructor para nodos y puntero hacia el siguiente nodo
     * @param dato
     * @param signodo
     */
    Nodo(int dato, Nodo *signodo)
    {
        this->dato = dato;
        this->siguiente = signodo;
    }
    /**
     * setear del dato entero
     * @param x
     */
    void setDato(int x);
    /**
     * Setear el siguiente valor
     * @param x
     */
    void setSiguiente(Nodo* x);
    friend class Collector;
};

void Nodo::setDato(int x){
    dato = x;
}
void Nodo::setSiguiente(Nodo* x){
    siguiente = x;
}



//-----------------------------------------------------------------------------
/**
 * Clase Collector, utilizada para reservar nodos
 * Si el jugador falla un proyectil, se "irán" a collector.
 */
class Collector {
public:
    Nodo *head;
    /**
     * Insertar al inicio de collector
     * @param puntero
     */
    void InsertarInicioC(Nodo *puntero) {
        if (head == nullptr) {
            head = puntero;
            //head->setDato(0);
            head->siguiente = nullptr;
        } else {
            puntero->setSiguiente(head);
            head = puntero;
            head->setDato(0);
        }
    }
    /**
     * Mostrar collector
     */
    void Mostrar() {
        if (head == nullptr)
            cout << "No hay elementos en collector";
        else {
            for (Nodo *temp = head; temp != nullptr; temp = temp->siguiente) {
            }
        }
    }
    /**
     * Función para verificar si hay algo en collector
     * @return
     */
    bool HayEspacios() {
        if (this->head == nullptr) {
            return false;
        } else {
            return true;
        }
    }
    /**
     * Función para eliminar de collector
     * @return
     */
    Nodo *EliminarDeColl() {
        if (this->head->siguiente == nullptr) {
            Nodo *tmp = head;
            head = nullptr;

            return tmp;
        } else {
            Nodo *tmp = head;
            head = head->siguiente;
            return tmp;
        }
    };
};


//-----------------------------------------------------------------------------
/**
 * Clase Lista enlazada, utilizada para reservar nodos de información general
 */
class Lista {
public:
    Nodo *cabeza;
    int tamaño;
    Collector *botadero;
    /**
     * Atributos de la lista simple enlazada
     */
    Lista() {
        cabeza = nullptr;
        tamaño = 0;
        botadero = new Collector();
    }
    /**
     * Función para insertar al final de la lista
     * @param data
     */
    void InsertarFinal(int data) {
        if (botadero->HayEspacios() != false) {
            Nodo *newPtr = botadero->EliminarDeColl();
            if (cabeza == nullptr) {
                cabeza = newPtr;
                cabeza->setDato(data);
                cabeza->setSiguiente(nullptr);
                tamaño++;
            } else {
                Nodo *temp = cabeza;
                while (temp->siguiente != nullptr) {
                    temp = temp->siguiente;
                }
                temp->setSiguiente(newPtr);
                newPtr->setSiguiente(nullptr);
                newPtr->setDato(data);
                tamaño++;
            }
        } else {
            Nodo *newPtr = new Nodo(data);
            if (cabeza == nullptr) {
                cabeza = newPtr;
                tamaño++;
            } else {
                Nodo *temp = cabeza;
                while (temp->siguiente != nullptr) {
                    temp = temp->siguiente;
                }
                temp->setSiguiente(newPtr);
                tamaño++;
            }
        }
    }
    /**
     * Función para insertar al inicio de la lista enlazada
     * @param data
     */
    void InsertarInicio(int data) {

        if (botadero->HayEspacios() != false) {
            Nodo *newPtr = botadero->EliminarDeColl();
            if (cabeza == nullptr) {
                cabeza = newPtr;
                cabeza->setDato(data);
                cabeza->setSiguiente(nullptr);
                tamaño++;
            } else {
                newPtr->setSiguiente(cabeza);
                cabeza = newPtr;
                cabeza->setDato(data);
                tamaño++;
            }
        } else {
            if (cabeza == nullptr) {
                Nodo *newPtr = new Nodo(data);
                cabeza = newPtr;
                tamaño++;
            } else {
                Nodo *newPtr = new Nodo(data, cabeza);
                cabeza = newPtr;
                tamaño++;
            }
        }

    }
    /**
     * Función para eliminar dato de la lista enlazada
     * @param data
     */

    void EliminarDato(int data) {
        Encuentra(data);
        if(respuesta == "si") {
            if (data == cabeza->dato) {
                Nodo *temp = cabeza;
                cabeza = cabeza->siguiente;
                botadero->InsertarInicioC(temp);
                botadero->Mostrar();
                tamaño--;


            } else {
                Nodo *buscador = cabeza;
                Nodo *prev;
                while (buscador->dato != data) {
                    prev = buscador;
                    buscador = buscador->siguiente;
                }

                prev->siguiente = buscador->siguiente; // prev->nextPtr = prev->nextPtr->nextPtr;
                botadero->InsertarInicioC(buscador);
                botadero->Mostrar();
                tamaño--;
            }
        } else{
        }
    }
    /**
     * Función para mostrar la lista enlazada actual
     */
    void MostrarLista() {
        Nodo *temp;
        if (cabeza == nullptr) {
        } else {
            temp = cabeza;
            while (temp) {
                temp = temp->siguiente;
            }
        }
    }
    void Encuentra(int data){
        Nodo *temp;
        if (cabeza == nullptr) {
            respuesta = "no";
        } else{
            int f = 0;
            temp = cabeza;
            do {
                if (temp->dato == data) {
                    f = 1;
                    respuesta = "si";
                    break;
                }
                if(temp->siguiente == nullptr){
                    break;
                }
                else{
                    temp = temp->siguiente;
                }

            } while (temp != cabeza);
            if (f == 0) {
                respuesta = "no";
            }
        }
    }
};

static Lista *l = new Lista();
static Collector *C = new Collector();
/**
 *
 */
void juego(void){
    posJugx = 20;
    posJugy = 150;
    jugador1.velocidad.y = velocity;
    for(int i=0; i<cantBalas;i++){
        disparos[i].rec.x = 85;
        disparos[i].rec.y = posJugy+30;
        disparos[i].rec.width = 10;
        disparos[i].rec.height = 5;
        disparos[i].balavel.x = 7;
        disparos[i].balavel.y = 0;
        disparos[i].numero = i;
        disparos[i].active = false;
        l->InsertarFinal(i);
    }
}
/**
 * Función la cual le da al jugador los límites de la pantalla, su cambio de sprites, la posibilidad de disparar a
 * cierta cadencia, además de configurarla dependiendo de las teclas que presiona el jugador y tener cierta munición.
 */

void actJuego(void){
    if(IsKeyPressed(KEY_A)) {
        rate = 1 ;
        cadencia = 0;
    }

    if(IsKeyPressed(KEY_S)) {
        rate = 2;
        cadencia = 0;
    }

    if(IsKeyPressed(KEY_D)) {
        rate = 3;
        cadencia = 0;
    }

    if(IsKeyPressed(KEY_F)) {
        rate = 4;
        cadencia = 0;
    }
    if(poderEsc && actPod){
        nave = "/home/felipe/Proyecto1_Felipe_Jimenez-Carlos_Segura/imagenes/naveEsc.png";
        naveDown = "/home/felipe/Proyecto1_Felipe_Jimenez-Carlos_Segura/imagenes/naveEscDown.png";
        naveUp = "/home/felipe/Proyecto1_Felipe_Jimenez-Carlos_Segura/imagenes/naveEscUp.png";
    }

    if(poderCad && actPod){
        nave = "/home/felipe/Proyecto1_Felipe_Jimenez-Carlos_Segura/imagenes/naveVel.png";
        naveDown = "/home/felipe/Proyecto1_Felipe_Jimenez-Carlos_Segura/imagenes/naveDVel.png";
        naveUp = "/home/felipe/Proyecto1_Felipe_Jimenez-Carlos_Segura/imagenes/naveUVel.png";
        if(IsKeyPressed(KEY_G)){
            rate = 6;
            cadencia = 0;
        }
    }


    if(poderVid && actPod){
        nave = "/home/felipe/Proyecto1_Felipe_Jimenez-Carlos_Segura/imagenes/naveAlas.png";
        naveDown = "/home/felipe/Proyecto1_Felipe_Jimenez-Carlos_Segura/imagenes/naveAlasD.png";
        naveUp = "/home/felipe/Proyecto1_Felipe_Jimenez-Carlos_Segura/imagenes/naveAlasU.png";
    }


    if(posJugy>40){
        if (IsKeyDown(KEY_UP)) posJugy -= jugador1.velocidad.y;
        if (IsKeyDown(KEY_UP)) navecita = LoadTexture(naveUp);
        if (IsKeyReleased(KEY_UP)) navecita = LoadTexture(nave);
        cadencia += rate;
        for(int i=0; i<cantBalas;i++){
            if(!disparos[i].active && cadencia %48 == 0){
                disparos[i].rec.x = 85;
                disparos[i].rec.y = posJugy+30;
                disparos[i].active = true;
                break;
            }

        }
    }

    if(posJugy<345){
        if(IsKeyDown(KEY_DOWN)) posJugy+= jugador1.velocidad.y;
        if (IsKeyDown(KEY_DOWN)) navecita = LoadTexture(naveDown);
        if (IsKeyReleased(KEY_DOWN)) navecita = LoadTexture(nave);
        cadencia += rate;
        for (int i = 0; i < cantBalas; i++) {
            if (!disparos[i].active && cadencia %48 == 0) {
                disparos[i].rec.x = 85;
                disparos[i].rec.y = posJugy + 30;
                disparos[i].active = true;
                break;
            }

    }
    }
    for(int i=0; i<cantBalas;i++){
        if(disparos[i].active){
            if(disparos[i].rec.x<=800){
                disparos[i].rec.x += disparos[i].balavel.x;
            } else {
                l->EliminarDato(i);
                l->MostrarLista();
            }

        }
    }
}
/**
 * Función llamada cada frame, para revisar el funcionamiento del jugador
 */



void actualizar(void){
    actJuego();
    //actDraw();
}
/**
 * Función Main, utilizada para iniciar el código, abrir la ventana inicial del juego, dibujar texturas, actualizar y
 * llevar la comunicación de ventanas, actualización de sprites, etc.
 * @return
 */
int main(){
    const int screenWidth = 732;
    const int screenHeight = 413;

    InitWindow(screenWidth, screenHeight, "Battlespace");
    GameScreen currentScreen = INICIO;



    // TODO: Initialize all required variables and load all required data here!

    int framesCounter = 0;          // Useful to count frames
    Rectangle boton1 = {100, 150, 150, 50 }; // Coordenadas y dimensiones del botón
    Rectangle boton2 = {100, 225, 150, 50 }; // Coordenadas y dimensiones del botón
    Rectangle boton3 = {100, 300, 150, 50 }; // Coordenadas y dimensiones del botón

    Rectangle strat1 = {15, 150, 150, 50 }; // Coordenadas y dimensiones del botón
    Rectangle strat2 = {197, 150, 150, 50 }; // Coordenadas y dimensiones del botón
    Rectangle strat3 = {385, 150, 150, 50 }; // Coordenadas y dimensiones del botón
    Rectangle strat4 = {565, 150, 150, 50 }; // Coordenadas y dimensiones del botón

    Rectangle activarPoder = {450,5,150,30};

    navecita = LoadTexture(nave);
    Texture2D icoVidas = LoadTexture("/home/felipe/Proyecto1_Felipe_Jimenez-Carlos_Segura/imagenes/vidas.png");
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
                    poderEsc = true;
                    currentScreen = GAMEPLAY;
                    juego();
                }
                if (CheckCollisionPointRec(GetMousePosition(), strat2) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    currentScreen = GAMEPLAY;
                    poderCad = true;
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
                    velocity = 10;
                    poderVid = true;
                    juego();
                }
            } break;
            case GAMEPLAY:
            {
                // TODO: Update GAMEPLAY screen variables here!

                actualizar();
                if (IsKeyPressed(KEY_Z)){
                    actPod = true;
                }

                    // Press enter to change to ENDING screen
                //if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
                //{
                //    currentScreen = ENDING;
                //}

            } break;
            case ENDING:
            {
                // TODO: Update ENDING screen variables here!
                velocity = 5;
                nave = "/home/felipe/Proyecto1_Felipe_Jimenez-Carlos_Segura/imagenes/nave.png";
                naveDown = "/home/felipe/Proyecto1_Felipe_Jimenez-Carlos_Segura/imagenes/naveDown.png";
                naveUp = "/home/felipe/Proyecto1_Felipe_Jimenez-Carlos_Segura/imagenes/naveUp.png";
                rate = 1;
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
                DrawTexture(icoVidas,10,5,RAYWHITE);
                for(int i=0; i<cantBalas;i++){
                    if(disparos[i].active) DrawRectangleRec(disparos[i].rec, YELLOW);
                }
                DrawText("Para activar el poder presione [z]", activarPoder.x + 10, activarPoder.y + 5, 15, DARKGRAY);
                DrawText("x", 50, 18, 20, GRAY);
                DrawText(TextFormat("%02i", cantVidas), 65, 20, 20, GRAY);
                DrawText("Wave", 310, 10, 30, GRAY);
                DrawText(TextFormat("%01i", cantVidas), 400, 10, 30, GRAY);

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
    UnloadTexture(fondoI);
    UnloadTexture(fondoS);
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
