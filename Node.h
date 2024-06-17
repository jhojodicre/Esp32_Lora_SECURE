#ifndef NODE_H  // si no esta definida la libreria la defino
#define NODE_H  // la defino
#include <Arduino.h>

class Node{
    private:
        int node=0;
        bool zoneA=0;
        bool zoneB=0;
        int nodeACK=0;             // Numero de Veces que el nodo a Sido Reconocido por el Maestro.
        


        bool Zone_Check;
    public:
        Node();
        void Ack(char functionCode);
        int GetAckNum();
        void Coming (char nodeCode);
        
        int Zone_A;
        int Zone_B;

        bool Zone_A_ACK;
        bool Zone_B_ACK;

        bool Zone_A_ERR;
        bool Zone_B_ERR;

        bool Zone_A_FAL;
        bool Zone_B_FAL;

        bool Zone_A_ALR;
        bool Zone_B_ALR;

        bool Zone_A_OK;
        bool Zone_B_OK;
};
#endif   // fin de la definicion.