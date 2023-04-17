/**
 * Librerias externas utilizadas
 */
#include <iostream>
#include <raylib.h>
#include <math.h>
#include "pugixml-1.13/src/pugixml.cpp"
#include <boost/asio.hpp>


#define cantidad_max_ene 50
/**
 * Facilitar prints
 */
using namespace std;

#define oleada1 4
#define oleada2 6
#define oleada3 10
#define oleada4 13
#define oleada5 17
#define oleada6 5
#define oleada7 10
#define oleada8 15
#define oleada9 20
#define oleada10 25
#define enemigosMaximos 125


/**
 * Se setean las distintas ventanas que se van a utilizar en el juego
 */
typedef enum GameScreen {INICIO, ESTRATEGIAS, GAMEPLAY, GANAR, PERDER} GameScreen;

/**
 *
 */
typedef enum { FIRST = 0, SECOND, THIRD, FOURTH, FIFTH, SIXTH, SEVENTH, EIGHTH, NINETH,TENTH} EnemyWave;;

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
    Rectangle rec;
    int posEnex;
    int posEney;
    Vector2 velocidad;
    bool active;
    Color color;
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
 * Variables generales para el funcionamiento del juego, como son la velocidad del jugador, la posiciÃ³n, las imagenes bases de este, etc.
 */
static int cadencia = 0;
static int rate = 1;
static int velocity = 5;
static int cantVidas = 3;

static int posJugx = 0;
static int posJugy = 0;
static int posEnex = 0;
static int posEney = 0;

static Texture2D navecita;
static const char *nave = "imagenes/nave.png";
static const char *naveDown = "imagenes/naveDown.png";
static const char *naveUp = "imagenes/naveUp.png";

static int balasColl = 0;
static int balasRec;
static int oleada = 1;

static const int cantBalas1 = 10;
static int balas1 = 10;
static Disparos disparos1[cantBalas1] = {0 };

static const int cantBalas2 = 500;
static int balas2 = 500;
static Disparos disparos2[cantBalas2] = {0 };

static const int cantBalas3 = 250;
static int balas3 = 250;
static Disparos disparos3[cantBalas3] = {0 };

static Disparos recicladas[500] ={0};

static int dificultad = 1;
static Jugador jugador1 = { 0 };

static string respuesta = "";
static string respuestaE = "";

static bool poderEsc = false;
static bool poderCad = false;
static bool poderVid = false;
static bool actPod = false;
static string Vida = "3";

static int enemigosActivos = 0;
static Enemigos enemigos[enemigosMaximos] ={0};
static EnemyWave wave = {static_cast<EnemyWave>(0)};
static Enemigos asteroides[5] = {0};
static int velEne = 2;
static int navesFuera = 0;
static bool fase2 = false;
static bool startFase2 = false;
static int eliminaciones1 = 0;
static int eliminaciones2 = 0;
static bool ganar = false;

static const int screenWidth = 732;
static const int screenHeight = 413;

boost::asio::io_service io;
boost::asio::serial_port port = boost::asio::serial_port(io, "/dev/ttyACM0");
boost::asio::streambuf buffer;
istream input_stream(&buffer);

static string message;

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
//        int cont=0;
//
//        Nodo * temp=  head;
//        if(head = nullptr){
//            return cont;
//        }else{
//            while(temp!=NULL){
//                temp=temp->siguiente;
//                cont++;
//            }
//            return cont;
//        }
        if (head == nullptr) {
            balasColl = 0;
            cout << "balasColl "<< balasColl<< endl;
            return balasColl;
        }else {
            for (Nodo *temp = head; temp != nullptr; temp = temp->siguiente) {
                balasColl ++;
            }
            cout << "balasColl "<< balasColl<< endl;
            return balasColl;
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
            cout<< "Collector:   ";
            for (Nodo *temp = head; temp != nullptr; temp = temp->siguiente) {
                cout << temp << " / ";
            }
            cout << endl;
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
    void EnviarCollector(int data) {
        Encuentra(data);

        if(respuesta == "si") {
            if (data == cabeza->dato) {
                Nodo *temp = cabeza;
                cabeza = cabeza->siguiente;
                botadero->InsertarInicioC(temp);
                //botadero->Mostrar();
                tamaño--;
                balasColl ++;
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
                //botadero->Mostrar();
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
    void EliminarE(int data) {
        EncuentraE(data);
        if(respuestaE == "si") {
            if (data == cabeza->dato) {

                Nodo *temp = cabeza;
                cabeza = cabeza->siguiente;
                tamaño--;
                cantVidas--;

            } else {
                cantVidas--;
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
    void borrar(){
        while (cabeza->siguiente != nullptr) {
            cabeza = cabeza->siguiente;
        }
        cabeza = nullptr;
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
    void EncuentraE(int data){
        Nodo *temp;
        if (cabeza == nullptr) {
            respuestaE = "no";
        } else{
            int f = 0;
            temp = cabeza;

            do {
                if (temp->dato == data) {
                    f = 1;
                    respuestaE = "si";
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
                respuestaE = "no";
            }
        }
    }
};

static Lista *l = new Lista();
static Lista *R = new Lista();
static Lista *E = new Lista();
static Lista *A = new Lista();
/**
 *
 */

//void arduino(void){
//    while(message!="a"){
//
//    }
//}

void enviar_mensaje_arduino(int dato){
    boost::asio::write(port, boost::asio::buffer(&dato, sizeof(dato)));
}

void reset(void){
    cadencia = 0;
    rate = 1;
    velocity = 5;
    cantVidas = 3;
    posJugx = 0;
    posJugy = 0;
    posEnex = 0;
    posEney = 0;

    Texture2D navecita;
    nave = "imagenes/nave.png";
    naveDown = "imagenes/naveDown.png";
    naveUp = "imagenes/naveUp.png";

    balasColl = 0;
    balasRec;
    oleada = 1;
    balas1 = 1000;
    Disparos disparos1[cantBalas1] = {0 };

    balas2 = 500;
    Disparos disparos2[cantBalas2] = {0 };

    balas3 = 250;
    Disparos disparos3[cantBalas3] = {0 };

    Disparos recicladas[500] ={0};

    dificultad = 1;
    Jugador jugador1 = { 0 };

    string respuesta = "";
    string respuestaE = "";

    poderEsc = false;
    poderCad = false;
    poderVid = false;
    actPod = false;

    enemigosActivos = 0;
    Enemigos enemigos[enemigosMaximos] ={0};
    EnemyWave wave = {static_cast<EnemyWave>(0)};
    Enemigos asteroides[5] = {0};
    velEne = 2;
    navesFuera = 0;
    fase2 = false;
    startFase2 = false;
    eliminaciones1 = 0;
    eliminaciones2 = 0;
    ganar = false;
//
//    l->borrar();
//    R->borrar();
//    E->borrar();
//    A->borrar();
}
void juego(void){
//    arduino();
    enemigosActivos = oleada1;
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
    for(int i = 0; i < enemigosMaximos; i++){
        enemigos[i].posEnex = GetRandomValue(screenWidth, screenWidth+1000);
        enemigos[i].posEney = GetRandomValue(50, 350);
        enemigos[i].velocidad.x = velEne;
        enemigos[i].velocidad.y = 5;
        enemigos[i].active = true;
        E->InsertarFinal(i);
    }
    for(int i = 0; i < 5; i++) {
        asteroides[i].posEnex = GetRandomValue(screenWidth, screenWidth+1000);
        asteroides[i].posEney = GetRandomValue(50, 330);
        asteroides[i].velocidad.x = 1;
        asteroides[i].velocidad.y = 1;
        asteroides[i].active = true;
        A->InsertarFinal(i);
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
        Vida = Poder.child("Vida").text().get();
        const char *Nave = Poder.child("Nave").text().get();
        const char *NaveDown = Poder.child("NaveDown").text().get();
        const char *NaveUp = Poder.child("NaveUp").text().get();
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
        Vida = Poder.child("Vida").text().get();
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
        if(balas1 == 0){
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
                //cout<<disparos1[i].rec.x<<endl;
                if (0<=disparos1[i].rec.x and disparos1[i].rec.x<= 800) {
                    disparos1[i].rec.x += disparos1[i].balavel.x;

                    for(int j=0; j<enemigosActivos;j++){
                        if(enemigos[j].active){
                            if(abs(disparos1[i].rec.x+5-enemigos[j].posEnex-10)<=10 and abs(disparos1[i].rec.y-enemigos[j].posEney-17)<=17) {
                                disparos1[i].rec.x = -1000;
                                navesFuera++;
                                if(!fase2)eliminaciones1++;
                                else{
                                    eliminaciones2++;
                                }
                                enemigos[j].posEnex = GetRandomValue(screenWidth, screenWidth + 3000);
                                enemigos[j].posEney = GetRandomValue(50, 350);
                                //enemigos[j].active = false;
                            }
                        }
                    }
                    for(int x=0; x<5;x++){
                        if(abs(disparos1[i].rec.x+10-asteroides[x].posEnex-19)<=12 and abs(disparos1[i].rec.y+2-asteroides[x].posEney-17)<=12){
                            disparos1[i].rec.x = -1000;
                            asteroides[x].posEnex = GetRandomValue(screenWidth, screenWidth + 3000);
                            asteroides[x].posEney = GetRandomValue(50, 330);
                        }
                    }
                    //l->MostrarLista();
                }
                else {
                    l->EnviarCollector(i);
                    //l->botadero->Mostrar();
                }
            }
        }
    }
    if(dificultad == 2){
        for(int i=0; i<cantBalas2;i++){
            if(disparos2[i].active) {
                if (0<=disparos2[i].rec.x and disparos2[i].rec.x <= 800) {
                    disparos2[i].rec.x += disparos2[i].balavel.x;
                    for(int j=0; j<enemigosActivos;j++){
                        if(enemigos[j].active) {
                            if (abs(disparos2[i].rec.x + 5 - enemigos[j].posEnex - 10) <= 10 and
                                abs(disparos2[i].rec.y + 5 - enemigos[j].posEney - 17) <= 17) {
                                disparos2[i].rec.x = -1000;
                                navesFuera++;
                                if (!fase2)eliminaciones1++;
                                else {
                                    eliminaciones2++;
                                }
                                enemigos[j].posEnex = GetRandomValue(screenWidth, screenWidth + 3000);
                                enemigos[j].posEney = GetRandomValue(50, 350);
                            }
                        }
                    }
                    for(int x=0; x<5;x++){
                        if(abs(disparos1[i].rec.x+10-asteroides[x].posEnex-19)<=12 and abs(disparos1[i].rec.y+2-asteroides[x].posEney-17)<=12){
                            disparos2[i].rec.x = -1000;
                            asteroides[x].posEnex = GetRandomValue(screenWidth, screenWidth + 3000);
                            asteroides[x].posEney = GetRandomValue(50, 330);
                        }
                    }
                } else{
                    l->EnviarCollector(i);
                    //l->MostrarLista();
                }
            }
        }
    }
    if(dificultad == 3){
        for(int i=0; i<cantBalas3;i++){
            if(disparos3[i].active) {
                if (0<=disparos3[i].rec.x and disparos3[i].rec.x <= 800) {
                    disparos3[i].rec.x += disparos3[i].balavel.x;
                    for(int j=0; j<enemigosActivos;j++){
                        if(enemigos[j].active) {
                            if (abs(disparos3[i].rec.x + 5 - enemigos[j].posEnex - 10) <= 10 and
                                abs(disparos3[i].rec.y + 5 - enemigos[j].posEney - 17) <= 17) {
                                disparos3[i].rec.x = -1000;
                                navesFuera++;
                                if (!fase2)eliminaciones1++;
                                else {
                                    eliminaciones2++;
                                }
                                enemigos[j].posEnex = GetRandomValue(screenWidth, screenWidth + 3000);
                                enemigos[j].posEney = GetRandomValue(50, 350);
                            }
                        }
                    }
                    for(int x=0; x<5;x++){
                        if(abs(disparos1[i].rec.x+10-asteroides[x].posEnex-19)<=12 and abs(disparos1[i].rec.y+2-asteroides[x].posEney-17)<=12){
                            disparos3[i].rec.x = -1000;
                            asteroides[x].posEnex = GetRandomValue(screenWidth, screenWidth + 3000);
                            asteroides[x].posEney = GetRandomValue(50, 330);
                        }
                    }
                } else{
                    l->EnviarCollector(i);
                    //l->MostrarLista();
                }
            }
        }
    }
    for(int i=0; i<balasColl;i++){
        if(recicladas[i].active) {
            if (0<=recicladas[i].rec.x and recicladas[i].rec.x <= 800) {
                recicladas[i].rec.x += recicladas[i].balavel.x;
                for(int j=0; j<enemigosActivos;j++){
                    if(enemigos[j].active){
                        if(abs(recicladas[i].rec.x+5-enemigos[j].posEnex-10)<=10 and abs(recicladas[i].rec.y+5-enemigos[j].posEney-10)<=20) {
                            recicladas[i].rec.x = -100;
                            navesFuera++;
                            if(!fase2)eliminaciones1++;
                            else{
                                eliminaciones2++;
                            }
                            enemigos[j].posEnex = GetRandomValue(screenWidth, screenWidth + 3000);
                            enemigos[j].posEney = GetRandomValue(50, 350);
                        }
                    }
                }
            }else{
                R->EliminarR(i);
            }
        }
    }
/**
 * Código oleadas
 */
    switch (wave) {
        case FIRST: {
            oleada = 1;
            if (navesFuera == enemigosActivos){
                navesFuera = 0;
                for(int i = 0; i < 5; i++) {
                    if(!asteroides[i].active) asteroides[i].active = true;
                }
                for (int i = 0; i < enemigosActivos; i++) {
                    if (!enemigos[i].active) enemigos[i].active = true;
                }
                enemigosActivos = oleada2;
                oleada = 2;
                wave = SECOND;
            }
            break;
        }
        case SECOND: {
            if (navesFuera == enemigosActivos) {
                navesFuera = 0;
                for(int i = 0; i < 5; i++) {
                    if(!asteroides[i].active) asteroides[i].active = true;
                }
                oleada = 2;
                for (int i = 0; i < enemigosActivos; i++) {
                    if (!enemigos[i].active) enemigos[i].active = true;
                }
                enemigosActivos = oleada3;
                oleada = 3;
                wave = THIRD;
            }
            break;
        }
        case THIRD: {
            if (navesFuera == enemigosActivos) {
                navesFuera = 0;
                for(int i = 0; i < 5; i++) {
                    if(!asteroides[i].active) asteroides[i].active = true;
                }
                oleada = 3;
                for (int i = 0; i < enemigosActivos; i++) {
                    if (!enemigos[i].active) enemigos[i].active = true;
                }
                enemigosActivos = oleada4;
                oleada = 4;
                wave = FOURTH;
            }
            break;
        }
        case FOURTH: {
            if (navesFuera == enemigosActivos) {
                navesFuera = 0;
                for(int i = 0; i < 5; i++) {
                    if(!asteroides[i].active) asteroides[i].active = true;
                }
                oleada = 4;
                for (int i = 0; i < enemigosActivos; i++) {
                    if (!enemigos[i].active) enemigos[i].active = true;
                }
                enemigosActivos = oleada5;
                oleada = 5;
                wave = FIFTH;
            }
            break;
        }
        case FIFTH: {
            if (navesFuera == enemigosActivos) {
                navesFuera = 0;
                for(int i = 0; i < 5; i++) {
                    if(!asteroides[i].active) asteroides[i].active = true;
                }
                oleada = 5;
                for (int i = 0; i < enemigosActivos; i++) {
                    if (!enemigos[i].active) enemigos[i].active = true;
                }
                if(!fase2)fase2=true;
                if(!startFase2)startFase2=true;
                enemigosActivos = oleada6;
                oleada = 1;
                wave = SIXTH;
            }
            break;
        }
        case SIXTH: {
            if (navesFuera == enemigosActivos) {
                navesFuera = 0;
                for(int i = 0; i < 5; i++) {
                    if(!asteroides[i].active) asteroides[i].active = true;
                }
                oleada = 6;
                for (int i = 0; i < enemigosActivos; i++) {
                    if (!enemigos[i].active) enemigos[i].active = true;
                }
                enemigosActivos = oleada7;
                oleada = 2;
                wave = SEVENTH;
            }
            break;
        }
        case SEVENTH: {
            if (navesFuera == enemigosActivos) {
                navesFuera = 0;
                for(int i = 0; i < 5; i++) {
                    if(!asteroides[i].active) asteroides[i].active = true;
                }
                oleada = 3;
                for (int i = 0; i < enemigosActivos; i++) {
                    if (!enemigos[i].active) enemigos[i].active = true;
                }
                enemigosActivos = oleada8;
                oleada = 8;
                wave = EIGHTH;
            }
            break;
        }
        case EIGHTH: {
            if (navesFuera == enemigosActivos) {
                navesFuera = 0;
                for(int i = 0; i < 5; i++) {
                    if(!asteroides[i].active) asteroides[i].active = true;
                }
                oleada = 8;
                for (int i = 0; i < enemigosActivos; i++) {
                    if (!enemigos[i].active) enemigos[i].active = true;
                }
                enemigosActivos = oleada9;
                oleada = 4;
                wave = NINETH;
            }
            break;
        }
        case NINETH: {
            if (navesFuera == enemigosActivos) {
                navesFuera = 0;
                for(int i = 0; i < 5; i++) {
                    if(!asteroides[i].active) asteroides[i].active = true;
                }
                oleada = 9;
                for (int i = 0; i < enemigosActivos; i++) {
                    if (!enemigos[i].active) enemigos[i].active = true;
                }
                enemigosActivos = oleada10;
                oleada = 5;
                wave = TENTH;
            }
            break;
        }
        case TENTH: {
            if (navesFuera == enemigosActivos) {
                ganar = true;
            }
            break;
        }
        default:
            break;
    }
/**
 * Termina oleadas
 */
    for (int i = 0; i < enemigosActivos; i++) {
        if (abs((posJugy + 30) - (enemigos[i].posEney + 10)) <= 40 and
            abs(posJugx + 25 - enemigos[i].posEnex) <= 40) {
            enemigos[i].posEnex = GetRandomValue(500, 700);
            //enemigos[i].posEnex = GetRandomValue(screenWidth, screenWidth + 3000);
            enemigos[i].posEney = GetRandomValue(50, 350);
            E->EliminarE(i);
        }
    }
    for (int i = 0; i < enemigosActivos; i++) {
        if (enemigos[i].active) {
            enemigos[i].posEnex -= enemigos[i].velocidad.x;
            if (enemigos[i].posEnex < -20) {
                enviar_mensaje_arduino(1);
                enemigos[i].posEnex = GetRandomValue(screenWidth, screenWidth + 3000);
                enemigos[i].posEney = GetRandomValue(50, 350);
                //enemigos[i].active = false;
            }
        }
    }
    for(int i = 0; i < 5; i++) {
        if(asteroides[i].active){
            asteroides[i].posEnex -= asteroides[i].velocidad.x;
            asteroides[i].posEney += asteroides[i].velocidad.y;
            if(asteroides[i].posEnex < -20){
                asteroides[i].active = false;
            }
            if(50 <= asteroides[i].posEney){
                asteroides[i].velocidad.y *=-1;
            }
            if(asteroides[i].posEney <= 340){
                asteroides[i].velocidad.y *=-1;
            }
        }
        if (abs((posJugy + 30) - (asteroides[i].posEney + 5)) <= 20 and
            abs(posJugx + 25 - asteroides[i].posEnex) <= 25) {
            asteroides[i].active = false;
            A->EliminarE(i);
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
int main() {
    const int screenWidth = 732;
    const int screenHeight = 413;

    InitWindow(screenWidth, screenHeight, "Battlespace");
    GameScreen currentScreen = INICIO;


    // TODO: Initialize all required variables and load all required data here!

    int framesCounter = 0;          // Useful to count frames
    Rectangle boton1 = {100, 150, 150, 50}; // Coordenadas y dimensiones del botón
    Rectangle boton2 = {100, 225, 150, 50}; // Coordenadas y dimensiones del botón
    Rectangle boton3 = {100, 300, 150, 50}; // Coordenadas y dimensiones del botón

    Rectangle strat1 = {15, 150, 150, 50}; // Coordenadas y dimensiones del botón
    Rectangle strat2 = {197, 150, 150, 50}; // Coordenadas y dimensiones del botón
    Rectangle strat3 = {385, 150, 150, 50}; // Coordenadas y dimensiones del botón
    Rectangle strat4 = {565, 150, 150, 50}; // Coordenadas y dimensiones del botón

    Rectangle activarPoder = {450, 5, 150, 30};

    navecita = LoadTexture(nave);
    Texture2D icoVidas = LoadTexture("imagenes/vidas.png");
    Texture2D fondoI = LoadTexture("imagenes/fondo.png");
    Texture2D fondoS = LoadTexture("imagenes/fondostrat.png");
    Texture2D fondoG = LoadTexture("imagenes/fondoGame.png");
    Texture2D icoMun = LoadTexture("imagenes/municion.png");
    Texture2D munRec = LoadTexture("imagenes/municionRoja.png");
    Texture2D enemigos1 = LoadTexture("imagenes/enemigo1.png");
    Texture2D enemigos2 = LoadTexture("imagenes/enemigo2.png");
    Texture2D eli1 = LoadTexture("imagenes/eli1.png");
    Texture2D eli2 = LoadTexture("imagenes/eli2.png");
    Texture2D asteroide = LoadTexture("imagenes/asteroide.png");
    Texture2D win = LoadTexture("imagenes/win.png");
    Texture2D lose = LoadTexture("imagenes/lose.png");

    ///////////////////////////////////////////////////////////////////////////////////

    InitAudioDevice();  // Initialize audio device

    // Load music file
    Music music = LoadMusicStream("music/menu.mp3");
    // Play music
    PlayMusicStream(music);


    Texture2D frames[24]; // Array de texturas para almacenar cada frame
    int currentFrame = 0; // Indice del frame actual
    float frameCounter = 0; // Contador para controlar la velocidad de la animacion
    float frameSpeed = 0.1f; // Velocidad de la animacion (cuanto menor, mas rapida)

    // Cargar los frames de la animacion
    for (int i = 0; i < 24; i++)
    {
        char fileName[64];
        printf("a");
        sprintf(fileName, "gif/frame_%02d.png", i);
        frames[i] = LoadTexture(fileName);

    }
////////////////////////////////////////////////////////////////
    SetTargetFPS(60);               // Set desired framerate (frames-per-second)
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        UpdateMusicStream(music);
        frameCounter += GetFrameTime();
        if (frameCounter >= frameSpeed)
        {
            frameCounter = 0;
            currentFrame++;
            if (currentFrame > 23) currentFrame = 0;
        }
        // Update
        //----------------------------------------------------------------------------------
        switch(currentScreen)
        {
            case INICIO:
            {
                if (CheckCollisionPointRec(GetMousePosition(), boton1) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    dificultad = 1;
                    velEne = 2;
                    currentScreen = ESTRATEGIAS;
                }
                if (CheckCollisionPointRec(GetMousePosition(), boton2) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    dificultad = 2;
                    velEne = 4;
                    currentScreen = ESTRATEGIAS;
                }
                if (CheckCollisionPointRec(GetMousePosition(), boton3) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    dificultad = 3;
                    velEne = 6;
                    currentScreen = ESTRATEGIAS;
                }
            } break;
            case ESTRATEGIAS:
            {
                if (CheckCollisionPointRec(GetMousePosition(), strat1) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    poderEsc = true;
                    currentScreen = GAMEPLAY;

                    StopMusicStream(music);
                    // Load new music file
                    music = LoadMusicStream("music/gameplay.mp3");

                    // Play new music
                    PlayMusicStream(music);

                    juego();
                }
                if (CheckCollisionPointRec(GetMousePosition(), strat2) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    currentScreen = GAMEPLAY;
                    poderCad = true;
                    StopMusicStream(music);
                    // Load new music file
                    music = LoadMusicStream("music/gameplay.mp3");

                    // Play new music
                    PlayMusicStream(music);

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

                    StopMusicStream(music);
                    // Load new music file
                    music = LoadMusicStream("music/gameplay.mp3");

                    // Play new music
                    PlayMusicStream(music);

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


                    StopMusicStream(music);
                    // Load new music file
                    music = LoadMusicStream("music/gameplay.mp3");

                    // Play new music
                    PlayMusicStream(music);

                    juego();
                }
            } break;
            case GAMEPLAY:
            {
                actualizar();
//                boost::asio::read_until(port, buffer, "\n");
//                getline(input_stream, message);
//                cout<<"mensaje de arduino: "<<message<<endl;
                if (IsKeyPressed(KEY_Z)){
                    actPod = true;
                    if(poderEsc){
                        cantVidas = stoi(Vida);
                    }
                }
                if(ganar == true){
                    StopMusicStream(music);
                    music = LoadMusicStream("music/win.mp3");
                    PlayMusicStream(music);
                    currentScreen = GANAR;
                }
                if(cantVidas == 0){
                    StopMusicStream(music);
                    currentScreen = PERDER;
                }

            } break;
            case GANAR:
            {

                reset();
                // Press enter to return to INICIO screen
                if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
                {
                    currentScreen = INICIO;

                }
            } break;
            case PERDER:
            {
                reset();
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
                DrawRectangle(0,0,screenWidth,screenHeight,BLACK);

                DrawTexture(frames[currentFrame], 0, 50, RAYWHITE);
//                DrawTexture(fondoG, 0, 10, RAYWHITE);
                DrawTexture(navecita, posJugx, posJugy, RAYWHITE);
                DrawTexture(icoVidas,10,5,RAYWHITE);
                DrawTexture(eli1, 180,380,RAYWHITE);
                DrawTexture(eli2, 240,380,RAYWHITE);
                DrawText(TextFormat("%02i",eliminaciones1),210,383,20,GRAY);
                DrawText(TextFormat("%02i",eliminaciones2),270,383,20,GRAY);
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
                        if (disparos1[i].active) {
                            DrawRectangleRec(disparos1[i].rec, YELLOW);
                        } else{
                            //break;
                        }
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
                if(fase2){
                    for (int i = 0; i < enemigosMaximos; i++) {
                        if (enemigos[i].active) {
                            //DrawRectangle(enemigos[i].posEnex, enemigos[i].posEney+2,35,35,RED); //HITBOX ENEMIGO
                            DrawTexture(enemigos2, enemigos[i].posEnex, enemigos[i].posEney, RAYWHITE);
                        }
                    }
                }
                else {
                    for (int i = 0; i < enemigosMaximos; i++) {
                        if (enemigos[i].active) {
                            //DrawRectangle(enemigos[i].posEnex, enemigos[i].posEney+2,35,35,RED); //HITBOX ENEMIGO
                            DrawTexture(enemigos1, enemigos[i].posEnex, enemigos[i].posEney, RAYWHITE);
                        }
                    }
                }
                for(int i = 0; i < 5; i++) {
                    if(asteroides[i].active){
                        //DrawRectangle(asteroides[i].posEnex+7,asteroides[i].posEney+5, 24, 24 ,RED); //HITBOX ASTEROIDES
                        DrawTexture(asteroide,asteroides[i].posEnex, asteroides[i].posEney, RAYWHITE);
                    }
                }
                if(startFase2){
                    DrawText(" FASE 2 ",30,380,30,GRAY);
                } else{
                    DrawText(" FASE 1 ",30,380,30,GRAY);
                }
                DrawText("Para activar el poder presione [z]", activarPoder.x + 10, activarPoder.y + 5, 15, DARKGRAY);
                DrawText("x", 50, 18, 20, GRAY);
                DrawText(TextFormat("%02i", cantVidas), 65, 20, 20, GRAY);
                DrawText("Wave", 310, 10, 30, GRAY);
                DrawText(TextFormat("%01i", oleada), 400, 10, 30, GRAY);

            } break;
            case GANAR:
            {
                DrawRectangle(0,0,screenWidth,screenHeight,BLACK);
                DrawTexture(win, 0, 0, RAYWHITE);

            } break;
            case PERDER:
            {
                DrawRectangle(0,0,screenWidth,screenHeight,BLACK);
                DrawTexture(lose, 0, 0, RAYWHITE);

            } break;
            default: break;
        }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }


    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(icoVidas);
    UnloadTexture(navecita);
    UnloadTexture(fondoI);
    UnloadTexture(fondoS);
    UnloadTexture(fondoG);
    UnloadTexture(icoMun);
    UnloadTexture(munRec);
    UnloadTexture(enemigos1);
    UnloadTexture(enemigos2);
    UnloadTexture(eli1);
    UnloadTexture(eli2);
    UnloadTexture(asteroide);
    UnloadTexture(win);
    UnloadTexture(lose);

    StopMusicStream(music);
    UnloadMusicStream(music);


    // Liberar la memoria de las texturas
    for (int i = 0; i < 24; i++)
    {
        UnloadTexture(frames[i]);
    }

    CloseWindow();
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
