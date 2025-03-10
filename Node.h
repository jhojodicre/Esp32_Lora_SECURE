#ifndef NODE_H  // si no esta definida la libreria
#define NODE_H  // la definimos
#include <Arduino.h>

class Node{
    private:
        int node=0;
        bool zoneA=0;
        bool zoneB=0;
        int nodeACK=0;             // Numero de Veces que el nodo a Sido Reconocido por el Maestro.
        bool Zone_Check;
        byte _led;
        byte _rele1;
        

    public:
        // Funciones
        Node(int nodo_numero);
        void Ack(char functionCode);
        int GetAckNum();
        void Coming (char nodeCode);
        void Update(byte Nodo_Source_1, int Nodo_Source_2);
        void Estado();
        void CONTINIUS(byte data_in);
        void A01();
        void welcome();
        void a1_Nodo_Destellos(int a, int b);
        void revisar();
        //Varibales
        bool Zone_A_in;
        bool Zone_B_in;
        bool PB_ZA_in;
        bool PB_ZB_in;
        bool PB_ZC_in;
        bool Fuente_in;
        bool Entrada_X1_in;
        bool in_12;
        bool in_13;

        byte nodo_local;
        byte nodo_Status;
        int Zone_A_Num;
        int Zone_B_Num;
        int Zonas;
        int Zonas_Fallan;

        bool Zone_A;
        bool Zone_B;

        bool Zone_A_ACK;
        bool Zone_B_ACK;
        bool Zone_AB_ACK;

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
        byte Zonas_MSB;
        byte Zonas_LSB;
        byte Zonas_LSB_Estados;
        byte Zonas_MSB_Estados;
        byte Zonas_LSB_Mascara;
        byte Zonas_MSB_Mascara;
        word Zonas_Mascaras;
        int Zonas_LSB_str;
        int Zonas_MSB_str;


        String Zone_A_ACK_str;
        String Zone_B_ACK_str;

        bool Zone_A_ST;
        bool Zone_B_ST;

        bool Zones_Enables;
        
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

        String  Fuente_in_str;
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