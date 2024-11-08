#ifndef MASTER_H
#define MASTER_H
#include <Arduino.h>

class Master

{

public:
    Master(byte nodes_number);
    void Node_Request();
    void Node_Break();
    void XXXX();
    

private:
    byte _Master;         // Direccion del Master.
    byte _Nodes;          // Numero de Nodos a ser consultados.
    byte _Node_first;     // Primer Nodo.
    byte _Node_last;      // Ultimo Nodo.
    byte _Node_next;      // Proximo Nodo.
    byte _Node_requested; // Nodo Consultado.
};
#endif
