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
        Node(int nodo_numero);
        void Ack(char functionCode);
        int GetAckNum();
        void Coming (char nodeCode);
        void Update(byte Nodo_Source_1, int Nodo_Source_2);
        void Estado();
        void CONTINIUS(byte data_in);
        
        int Zone_A_Num;
        int Zone_B_Num;

        bool Zone_A;
        bool Zone_B;

        bool Zone_A_ACK;
        bool Zone_B_ACK;

        bool Zone_A_ERR;
        bool Zone_B_ERR;

        bool Zone_A_FAL;
        bool Zone_B_FAL;

        String Zone_A_FAL_str;
        String Zone_B_FAL_str;

        bool Zone_A_ALR;
        bool Zone_B_ALR;

        String Zone_A_ST_str;
        String Zone_B_ST_str;

        String Node_Source_Mode_str;
        
        bool Timer_ON;
        String Timer_ON_str;
        int  Node_Number;

        #define  Timer_token_k  "2"
        #define  Timer_ciclo_k  "1"
        #define  NODO_PERDIDO   "NOC"
        
        String  ZONA_ACT ="NOK";
        String  ZONA_DES ="BOK";
        String  ZONA_FA  ="ERR";


        String  FUENTE_EN = "0";
        String  BATERIA_EN= "1";


        bool Zone_A_OK;
        bool Zone_B_OK;

        char Funcion_Tipo;
        char Funcion_Num;

        bool Fuente;
        bool Batery;
        String Fuente_str;
        int Timero;

        byte MSG_NUM;

        String Actual;

        bool MODE_CONTINIUS;
};
#endif   // fin de la definicion.