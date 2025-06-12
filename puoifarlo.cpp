#include <iostream>
#include <cstdlib>
#include <ctime>
#include <random>
#include <algorithm>
#include <vector>
#include <queue> //pal BFS
using namespace std;


// variables en todo el codigo
struct coordenadas {
    int x, y;

    // Es donde se crea y almacena constantemente las coordenadas
    coordenadas(int _x = 0, int _y = 0) : x(_x), y(_y) {}
    

    // revisar
    // Permite coomparar dos coordenadas de detectar cuando llega a la salida
    bool operator==(const coordenadas& otro) const {
        return x == otro.x && y == otro.y;
    }
};

// lista de lista donde el elemneto va a ser un character.
vector<vector<char>> laberinto;

// sirve para marcar celdas ya visitadas
vector<vector<bool>> visitado;

// sirve para reconstruir el camino de la salida
vector<vector<coordenadas>> padre;


// Variables que no se cambian
const int movimientoX[4] = {0, 1, 0, -1}, movimientoY[4] = {-1, 0, 1, 0};


// Funcion donde Comienza la creacion del laberinto, define variables filas y columnas, y llenamos de '#'.        revisar resize() y el assign().
void startLaberinto(int dimension) {
    int tamanhoFilas = 2 * dimension + 1, tamanhoColumnas = 2 * dimension +1;
    // creacion de la matriz, lista de listas, se puede iterar tambien hasta que "i" alcanze filas y colocando '#' con emplace_back o push_back
    laberinto = vector<vector<char>>(tamanhoFilas, vector<char>(tamanhoColumnas, '#'));
}


// funcion de impresion de laberinto
void printLaberinto() {
    for (size_t i = 0; i < laberinto.size(); i++)
    {
        for (size_t j = 0; j < laberinto[i].size(); j++)
        {
            cout << laberinto[i][j] << " ";
        }
        cout << endl;
    }
}

// Funcion para mezclar direcciones con algoritmo fisher-yates
vector<int> mezclarDirecciones() {
    vector<int> direcciones = {0, 1, 2, 3};
    for (int i = 0; i < 4; i++)
    {
        swap(direcciones[i], direcciones[rand() % 4]);
    }
    return direcciones;
}

// Funcion para crear calcular siguientes movimientos con la posicion actual y las direcciones.
coordenadas calcularPosicion(coordenadas actual, int direcciones, int distancia) {
    coordenadas siguiente;
    siguiente.x = actual.x + distancia * movimientoX[direcciones];
    siguiente.y = actual.y + distancia * movimientoY[direcciones];
    return siguiente;
}

// Funcion que verifica si es un posible movimiento o no True or False
bool posicionValida(coordenadas valida) {
    return valida.x > 0 && valida.x < laberinto[0].size() - 1 && valida.y > 0 && valida.y < laberinto.size() -1;
}

// Funcion que convierte la celda de contigua a un pasillo.
void eliminarpared(coordenadas actual, coordenadas siguiente, int direc) {
   laberinto[actual.y + movimientoY[direc]][actual.x + movimientoX[direc]] = ' '; 
   laberinto[siguiente.y][siguiente.x] = ' ';
}




                // No es DFS porque no explora en el arbol de posibilidades. directamente busca una posicion valida.
// el primer pez gordo. backtracking crudo no DFS supuestamente funcion de donde junta todas las demas funciones para el laberinto 
void backtracking(int dimension) {
    startLaberinto(dimension);


    // Guarda el historial de celdas visitadas para poder retroceder, como una "pila", es una lista "vector"
    vector<coordenadas> pila;


    coordenadas inicio (1, 1);

    coordenadas salida(laberinto.size() - 2, laberinto[0].size() - 2);
    

    // Guarda de donde comienza y termina el algoritmo
    pila.push_back(inicio);
    laberinto[inicio.y][inicio.x] = 'S';
    laberinto[salida.y][salida.x] = 'E';
    laberinto[salida.y + 1][salida.x] = 'E';
    laberinto[salida.y -1][salida.x] = ' ';
    

    
    while (!pila.empty())
    {
        // Guarda la ultima posicion en la lista "vector" pila
        coordenadas actual = pila.back();
        // guarda en variable el retorno de las direcciones mezcladas
        vector<int> direcciones = mezclarDirecciones();
        
        // movimiento valido declarado como falso para despues validar
        bool movimientoValido = false;
        

        for (int direc : direcciones)
        {
            // Guarda la posible siguiente posicion ... Aca usamos recursividad en siiguiente
            coordenadas siguiente = calcularPosicion(actual, direc, 2);
            
            // Condicional si la posicion es valida en la posible siguiente posicion y comprueba si la celda destino contiene una pared.
            if (posicionValida(siguiente) && laberinto[siguiente.y][siguiente.x] == '#') 
            {
                // elimina la pared contigua
                eliminarpared(actual, siguiente, direc);
                
                // agrega nuevamente a la pila o stack el siguiente movimiento
                pila.push_back(siguiente);
                movimientoValido = true;
                break;
            }
            
        }
        // Si el movimientoValido es falso quita el ultimo movimiento de la pila o stack
        if (!movimientoValido)
        {
            pila.pop_back();
        }
        

    }

}
    


            //      ACA EL MANGURUYU   BFS
            // BFS


// Busca las coordenadas de los simbolos "S" y "E"
coordenadas buscarSimbolo(char simbolo) {

    for (size_t i = 0; i < laberinto.size(); i++)
    {
        for (size_t j = 0; j < laberinto[1].size(); j++)
        {
            if (laberinto[i][j] == simbolo)
            {
                return coordenadas(j,i);
            }
            
        }
        
    }
    return coordenadas(-1, -1);
}

// Crea 2 matrices visitados y padre parasaber que celdas ya se procesaron y para poder reconstruir el camino al final

void crearMatrices() {
    int alto = laberinto.size(), ancho = laberinto[1].size();
    visitado = vector<vector<bool>>(alto, vector<bool>(ancho, false));
    padre = vector<vector<coordenadas>>(alto, vector<coordenadas>(ancho, coordenadas(-1, -1)));
}

// Aca comienza el algoritmo donde llena las matrices de visitado y padre para despues reconstruir el camino
void bfs(coordenadas inicio, coordenadas salida) {
    
    // se usa para controlar cual celda se tiene que explorar despues
    queue<coordenadas> cola;

    // agregamos la 'S' en la cola
    cola.push(inicio);

    // Marcamos esa celda como visitada para que no cambie el simbolo
    visitado[inicio.y][inicio.x] = true;

    // mientras exista celda no explorada....
    while (!cola.empty())
    {   
        // definimos actual con la celda mas cercana aun no procesada
        coordenadas actual = cola.front();

        // le sacamos de la cola
                    // se puede hacer tambien con pila o otra estructura?
        cola.pop();

        // Si llega a la celda de salida para
        if (actual == salida) break;
        {
            // Se explora en las 4 direcciones
            for (int i = 0; i < 4; i++)
            {   // calcula la celda vecina
                coordenadas siguiente(actual.x + movimientoX[i], actual.y + movimientoY[i]);

                // si las 3 condiciones son true (esta dentro del laberinto), (no es una pared) y (aun no visitado) hacer ...
                if (posicionValida(siguiente) && laberinto[siguiente.y][siguiente.x] != '#' && !visitado[siguiente.y][siguiente.x]) {
                    // Marcamos como visitada         , guardamos el padre de donde viene y se agrega a la cola para explorar despues.
                    visitado[siguiente.y][siguiente.x] = true;
                    padre[siguiente.y][siguiente.x] = actual;
                    cola.push(siguiente);
                }

            }
            
        }
        
    }
    

}

// Recorre la matriz padre para ir marcando el camino practicar explicacion
void senhalarCamino(coordenadas inicio, coordenadas salida) {

    // si el algoritmo llego a la salida es true y actual es coordenada de la salida
    if (visitado[salida.y][salida.x]) {
        coordenadas actual = salida;

        // mientras coordenada actual sea igual a inicio retornar false
        while (!(actual == inicio))
        {
            //  si lo mismo condicional
            if (!(actual == salida))
            {   
                // senhala el con *
                laberinto[actual.y][actual.x] = '*';
                
            }
            actual = padre[actual.y][actual.x];
        }
        
    }

}

// funcion completa de BFS
void solucionLaberinto() {
    coordenadas inicio = buscarSimbolo('S');
    coordenadas salida = buscarSimbolo('E');


    crearMatrices();
    bfs(inicio, salida);
    senhalarCamino(inicio, salida);
}


int main() {
    srand(time(nullptr));

    int dimension;
    cout << "Ingresa la dimension del laberinto: ";
    cin >> dimension;


    backtracking(dimension);
    cout << "\n Laberinto Comienzaaaa \n";
    printLaberinto();

    solucionLaberinto();
    cout << "\n Laberinto resuelto \n";
    printLaberinto();
    
    return 0;
}
