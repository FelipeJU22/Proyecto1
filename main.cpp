/**
 * Librerias externas utilizadas
 */
#include <iostream>
#include <raylib.h>
#include <math.h>
#include "pugixml-1.13/src/pugixml.cpp"


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
} Disparos;
/**
 * Variables generales para el funcionamiento del juego, como son la velocidad del jugador, la posición, las imagenes bases de este, etc.
 */
static int cadencia = 0;
static int rate = 1;
static int posJugx = 0;
static int posJugy = 0;
static Texture2D navecita;
static int velocity = 5;
static const char *nave = "imagenes/nave.png";
static const char *naveDown = "imagenes/naveDown.png";
static const char *naveUp = "imagenes/naveUp.png";
static int balasColl = 0;
static int balasRec;
static const int cantBalas1 = 20;
static int balas1 = 20;
static Disparos disparos1[cantBalas1] = {0 };
static const int cantBalas2 = 250;
static int balas2 = 250;
static Disparos disparos2[cantBalas2] = {0 };
static const int cantBalas3 = 125;
static int balas3 = 125;
static Disparos disparos3[cantBalas3] = {0 };
static Disparos recicladas[500] ={0};
static int dificultad = 1;
static int cantVidas = 3;
static Jugador jugador1 = { 0 };
static string respuesta = "";
static bool poderEsc = false;
static bool poderCad = false;
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
    int LargoLista(){
        int cont=0;

        Nodo * temp=  head;
        if(head = nullptr){
            return cont;
        }else{
            while(temp!=NULL){
                temp=temp->siguiente;
                cont++;
            }
            return cont;
            cout<< endl;
        }
    }
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
        //cout << "Se elimina ----> " << head <<endl;
        if (head == nullptr){}
            //cout << "No hay elementos en collector";
        else {
            //cout<< "Collector:   ";
            for (Nodo *temp = head; temp != nullptr; temp = temp->siguiente) {
                //cout << temp << " / ";
            }
            //cout << endl;
            //cout << endl;
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
    int LargoCollector(){
        return botadero->LargoLista();
    }
    void EnviarCollector(int data) {
        Encuentra(data);

        if(respuesta == "si") {
            if (data == cabeza->dato) {
                Nodo *temp = cabeza;
                cabeza = cabeza->siguiente;
                botadero->InsertarInicioC(temp);
                botadero->Mostrar();
                tamaño--;
                balasColl += 1;
                balasRec = balasColl;
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
    void EliminarR(int data) {
        Encuentra(data);
        if(respuesta == "si") {
            if (data == cabeza->dato) {
                Nodo *temp = cabeza;
                cabeza = cabeza->siguiente;
                tamaño--;
                balasRec-=1;
            } else {
                Nodo *buscador = cabeza;
                Nodo *prev;
                while (buscador->dato != data) {
                    prev = buscador;
                    buscador = buscador->siguiente;
                }
                prev->siguiente = buscador->siguiente; // prev->nextPtr = prev->nextPtr->nextPtr;
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
            cout << "No hay elementos en al lista " << endl;
        } else {
            temp = cabeza;
            cout << "Lista :   " << endl;
            while (temp) {
                cout <<  temp->dato << " <- " <<temp << endl;
                temp = temp->siguiente;
            }
            cout << endl;
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
static Lista *R =new Lista();
static Collector *C = new Collector();
/**
 *
 */
void juego(void){
    posJugx = 20;
    posJugy = 150;
    jugador1.velocidad.y = velocity;
    if(dificultad == 1) {
        for (int i = 0; i < cantBalas1; i++) {
            disparos1[i].rec.x = 85;
            disparos1[i].rec.y = posJugy + 30;
            disparos1[i].rec.width = 10;
            disparos1[i].rec.height = 5;
            disparos1[i].balavel.x = 7;
            disparos1[i].balavel.y = 0;
            disparos1[i].numero = i;
            disparos1[i].active = false;
            l->InsertarFinal(i);
        }
    }
    if(dificultad == 2) {
        for (int i = 0; i < cantBalas2; i++) {
            disparos2[i].rec.x = 85;
            disparos2[i].rec.y = posJugy + 30;
            disparos2[i].rec.width = 10;
            disparos2[i].rec.height = 5;
            disparos2[i].balavel.x = 7;
            disparos2[i].balavel.y = 0;
            disparos2[i].numero = i;
            disparos2[i].active = false;
            l->InsertarFinal(i);
        }
    }
    if(dificultad == 3) {
        for (int i = 0; i < cantBalas3; i++) {
            disparos3[i].rec.x = 85;
            disparos3[i].rec.y = posJugy + 30;
            disparos3[i].rec.width = 10;
            disparos3[i].rec.height = 5;
            disparos3[i].balavel.x = 7;
            disparos3[i].balavel.y = 0;
            disparos3[i].numero = i;
            disparos3[i].active = false;
            l->InsertarFinal(i);
        }
    }
    for (int i = 0; i < 500; i++) {
        recicladas[i].rec.x = 85;
        recicladas[i].rec.y = posJugy + 30;
        recicladas[i].rec.width = 10;
        recicladas[i].rec.height = 5;
        recicladas[i].balavel.x = 7;
        recicladas[i].balavel.y = 0;
        recicladas[i].numero = i;
        recicladas[i].active = false;
        R->InsertarFinal(i);
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
        pugi::xml_document doc;
        doc.load_file("Poder_Escudo.xml");
        pugi::xml_node Poderes = doc.child("Poderes");
        pugi::xml_node Poder = Poderes.child("Poder");
        string Vida = Poder.child("Vida").text().get();
        const char *Nave = Poder.child("Nave").text().get();
        const char *NaveDown = Poder.child("NaveDown").text().get();
        const char *NaveUp = Poder.child("NaveUp").text().get();
        cantVidas = stoi(Vida);
        nave = Nave;
        naveDown = NaveDown;
        naveUp = NaveUp;
    }
    if(poderCad && actPod){
        pugi::xml_document doc;
        doc.load_file("Poder_Cadencia.xml");
        pugi::xml_node Poderes = doc.child("Poderes");
        pugi::xml_node Poder = Poderes.child("Poder");
        const char *Nave = Poder.child("Nave").text().get();
        const char *NaveDown = Poder.child("NaveDown").text().get();
        const char *NaveUp = Poder.child("NaveUp").text().get();
        string Rate = Poder.child("Cadencia").text().get();
        nave = Nave;
        naveDown = NaveDown;
        naveUp = NaveUp;
        if(IsKeyPressed(KEY_G)){
            rate = stoi(Rate);
            cadencia = 0;
        }
    }
    if(poderVid && actPod){
        pugi::xml_document doc;
        doc.load_file("Poder_Alas.xml");
        pugi::xml_node Poderes = doc.child("Poderes");
        pugi::xml_node Poder = Poderes.child("Poder");
        string Vida = Poder.child("Vida").text().get();
        const char *Nave = Poder.child("Nave").text().get();
        const char *NaveDown = Poder.child("NaveDown").text().get();
        const char *NaveUp = Poder.child("NaveUp").text().get();
        cantVidas = stoi(Vida);
        nave = Nave;
        naveDown = NaveDown;
        naveUp = NaveUp;
    }


    if(posJugy>40){
        if (IsKeyDown(KEY_UP)) posJugy -= jugador1.velocidad.y;
        if (IsKeyDown(KEY_UP)) navecita = LoadTexture(naveUp);
        if (IsKeyReleased(KEY_UP)) navecita = LoadTexture(nave);
        cadencia += rate;
        if(dificultad == 1){
            for(int i=0; i<cantBalas1;i++) {
                if (!disparos1[i].active && cadencia % 48 == 0) {
                    disparos1[i].rec.x = 85;
                    disparos1[i].rec.y = posJugy + 30;
                    balas1 -= 1;
                    disparos1[i].active = true;
                    break;
                }
            }
        }
        if(dificultad == 2){
            for(int i=0; i<cantBalas2;i++) {
                if (!disparos2[i].active && cadencia % 48 == 0) {
                    disparos2[i].rec.x = 85;
                    disparos2[i].rec.y = posJugy + 30;
                    balas2 -= 1;
                    disparos2[i].active = true;
                    break;
                }
            }
        }
        if(dificultad == 3){
            for(int i=0; i<cantBalas3;i++) {
                if (!disparos3[i].active && cadencia % 48 == 0) {
                    disparos3[i].rec.x = 85;
                    disparos3[i].rec.y = posJugy + 30;
                    balas3 -= 1;
                    disparos3[i].active = true;
                    break;
                }
            }
        }
        if(balas1== 0){
            for(int i=0; i<balasColl;i++) {
                if (!recicladas[i].active && cadencia % 48 == 0) {
                    recicladas[i].rec.x = 85;
                    recicladas[i].rec.y = posJugy + 30;
                    recicladas[i].active = true;
                    break;
                }
            }
        }
        if(balas2== 0){
            for(int i=0; i<10;i++) {
                if (!recicladas[i].active && cadencia % 48 == 0) {
                    recicladas[i].rec.x = 85;
                    recicladas[i].rec.y = posJugy + 30;
                    recicladas[i].active = true;
                    break;
                }
            }
        }
        if(balas3== 0){
            for(int i=0; i<10;i++) {
                if (!recicladas[i].active && cadencia % 48 == 0) {
                    recicladas[i].rec.x = 85;
                    recicladas[i].rec.y = posJugy + 30;
                    recicladas[i].active = true;
                    break;
                }
            }
        }
    }


    if(posJugy<335){
        if(IsKeyDown(KEY_DOWN)) posJugy+= jugador1.velocidad.y;
        if (IsKeyDown(KEY_DOWN)) navecita = LoadTexture(naveDown);
        if (IsKeyReleased(KEY_DOWN)) navecita = LoadTexture(nave);
        cadencia += rate;
        if(dificultad == 1){
            for (int i = 0; i < cantBalas1; i++) {
                if (!disparos1[i].active && cadencia % 48 == 0) {
                    disparos1[i].rec.x = 85;
                    disparos1[i].rec.y = posJugy + 30;
                    balas1 -= 1;
                    disparos1[i].active = true;
                    break;
                }
            }
        }
        if(dificultad == 2){
            for (int i = 0; i < cantBalas2; i++) {
                if (!disparos2[i].active && cadencia % 48 == 0) {
                    disparos2[i].rec.x = 85;
                    disparos2[i].rec.y = posJugy + 30;
                    balas2 -= 1;
                    disparos2[i].active = true;
                    break;
                }
            }
        }
        if(dificultad == 3){
            for (int i = 0; i < cantBalas3; i++) {
                if (!disparos3[i].active && cadencia % 48 == 0) {
                    disparos3[i].rec.x = 85;
                    disparos3[i].rec.y = posJugy + 30;
                    balas3 -= 1;
                    disparos3[i].active = true;
                    break;
                }
            }
        }
        if(balas1== 0){
            for(int i=0; i<balasColl;i++) {
                if (!recicladas[i].active && cadencia % 48 == 0) {
                    recicladas[i].rec.x = 85;
                    recicladas[i].rec.y = posJugy + 30;
                    recicladas[i].active = true;
                    break;
                }
            }
        }
        if(balas2== 0){
            for(int i=0; i<balasColl;i++) {
                if (!recicladas[i].active && cadencia % 48 == 0) {
                    recicladas[i].rec.x = 85;
                    recicladas[i].rec.y = posJugy + 30;
                    recicladas[i].active = true;
                    break;
                }
            }
        }
        if(balas3== 0){
            for(int i=0; i<balasColl;i++) {
                if (!recicladas[i].active && cadencia % 48 == 0) {
                    recicladas[i].rec.x = 85;
                    recicladas[i].rec.y = posJugy + 30;
                    recicladas[i].active = true;
                    break;
                }
            }
        }
    }
    if(dificultad == 1){
        for(int i=0; i<cantBalas1;i++) {
            if (disparos1[i].active) {
                if (disparos1[i].rec.x <= 800) {
                    disparos1[i].rec.x += disparos1[i].balavel.x;
                    //l->MostrarLista();
                } else {
                    l->EnviarCollector(i);
                    //l->MostrarLista();
                }
            }
        }
    }
    if(dificultad == 2){
        for(int i=0; i<cantBalas2;i++){
            if(disparos2[i].active) {
                if (disparos2[i].rec.x <= 800) {
                    disparos2[i].rec.x += disparos2[i].balavel.x;
                } else {
                    l->EnviarCollector(i);
                    //l->MostrarLista();
                }
            }
        }
    }
    if(dificultad == 3){
        for(int i=0; i<cantBalas3;i++){
            if(disparos3[i].active) {
                if (disparos3[i].rec.x <= 800) {
                    disparos3[i].rec.x += disparos3[i].balavel.x;
                } else {
                    l->EnviarCollector(i);
                    //l->MostrarLista();
                }
            }
        }
    }
    for(int i=0; i<balasColl;i++){
        if(recicladas[i].active) {
            if (recicladas[i].rec.x <= 800) {
                recicladas[i].rec.x += recicladas[i].balavel.x;
            }else{
                R->EliminarR(i);
            }
        }
    }
}
/**
 * Función llamada cada frame, para revisar el funcionamiento del jugador
 */



void actualizar(void){
    actJuego();
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
    Texture2D icoVidas = LoadTexture("imagenes/vidas.png");
    Texture2D fondoI = LoadTexture("imagenes/fondo.png");
    Texture2D fondoS = LoadTexture("imagenes/fondostrat.png");
    Texture2D fondoG = LoadTexture("imagenes/fondoGame.png");
    Texture2D icoMun = LoadTexture("imagenes/municion.png");
    Texture2D munRec = LoadTexture("imagenes/municionRoja.png");

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
                    dificultad = 1;
                    currentScreen = ESTRATEGIAS;
                }
                if (CheckCollisionPointRec(GetMousePosition(), boton2) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    dificultad = 2;
                    currentScreen = ESTRATEGIAS;
                }
                if (CheckCollisionPointRec(GetMousePosition(), boton3) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    dificultad = 3;
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
                    pugi::xml_document doc;
                    doc.load_file("Poder_Velocidad.xml");
                    pugi::xml_node Poderes = doc.child("Poderes");
                    pugi::xml_node Poder = Poderes.child("Poder");
                    string Velocidad = Poder.child("Velocidad").text().get();
                    velocity = stoi(Velocidad);
                    juego();
                }
                if (CheckCollisionPointRec(GetMousePosition(), strat4) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    pugi::xml_document doc;
                    doc.load_file("Poder_Alas.xml");
                    pugi::xml_node Poderes = doc.child("Poderes");
                    pugi::xml_node Poder = Poderes.child("Poder");
                    string Velocidad = Poder.child("Velocidad").text().get();
                    currentScreen = GAMEPLAY;
                    velocity = stoi(Velocidad);
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
                nave = "imagenes/nave.png";
                naveDown = "imagenes/naveDown.png";
                naveUp = "imagenes/naveUp.png";
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
                DrawRectangle(0,0,screenWidth,screenHeight,BLACK);
                DrawTexture(fondoG, 0, 10, RAYWHITE);
                DrawTexture(navecita, posJugx, posJugy, RAYWHITE);
                DrawTexture(icoVidas,10,5,RAYWHITE);
                DrawTexture(icoMun, 630, 380, RAYWHITE);
                DrawTexture(munRec,520,382,RAYWHITE);
                DrawText("x",548,380,20,GRAY);
                if(balas1!=0 and balas2!=0 and balas3!=0) {
                    DrawText(TextFormat("%03i", balasColl), 565, 382, 20, GRAY);
                }else{
                    DrawText(TextFormat("%03i", balasRec),565,382,20,GRAY);
                }
                if(dificultad == 1) {
                    for (int i = 0; i < cantBalas1; i++) {
                        if (disparos1[i].active) DrawRectangleRec(disparos1[i].rec, YELLOW);
                    }
                    DrawText("x",658,380,20,GRAY);
                    DrawText(TextFormat("%04i", balas1), 673, 382, 20, GRAY);
                }
                if(dificultad == 2) {
                    for (int i = 0; i < cantBalas2; i++) {
                        if (disparos2[i].active) DrawRectangleRec(disparos2[i].rec, YELLOW);
                    }
                    DrawText("x",658,380,20,GRAY);
                    DrawText(TextFormat("%04i", balas2), 673, 382, 20, GRAY);
                }
                if(dificultad == 3) {
                    for (int i = 0; i < cantBalas3; i++) {
                        if (disparos3[i].active) DrawRectangleRec(disparos3[i].rec, YELLOW);
                    }
                    DrawText("x",658,380,20,GRAY);
                    DrawText(TextFormat("%04i", balas3), 673, 382, 20, GRAY);
                }
                if(balas1 == 0){
                    for (int i = 0; i < balasColl; i++) {
                        if (recicladas[i].active) DrawRectangleRec(recicladas[i].rec, RED);
                    }
                }
                if(balas2 == 0){
                    for (int i = 0; i < balasColl; i++) {
                        if (recicladas[i].active) DrawRectangleRec(recicladas[i].rec, RED);
                    }
                }
                if(balas3 == 0){
                    for (int i = 0; i < balasColl; i++) {
                        if (recicladas[i].active) DrawRectangleRec(recicladas[i].rec, RED);
                    }
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
