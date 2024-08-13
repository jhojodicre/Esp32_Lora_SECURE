#include <Arduino.h>
#include "Node.h"

Node::Node(int numero_nodo){
    Node_Number = numero_nodo;
}
void Node::Ack(char functionCode){
    nodeACK ++;
    if(nodeACK == 3) nodeACK = 0;
    switch(functionCode){
        case '0':
            Zone_A_FAL = true;
            break;
        case '1':
            Zone_A_FAL = false;
            break;
        case '2':
            Zone_B_FAL = true;
            break;
        default:
            break;
    }
}
int Node::GetAckNum(){
    return nodeACK;
}
void Node::Coming(char nodeCode){
    switch(nodeCode){
        case '1':
            Zone_A_FAL = true;
            break;
        case '2':
            Zone_B_FAL = true;
            break;
        case '3':
            break;
        default:
            break;
    }


}
void Node::Update(byte dato_actual_1, int dato_actual_2){
    byte dato_recibido = dato_actual_1;
   
    // bit 1=ZA
        //
        if(bitRead(dato_recibido, 1)){
            Zone_A_ALR=true;
            Zone_A_ST_str=ZONA_ACT;
        }
        if(!bitRead(dato_recibido, 1)){
            Zone_A_ALR=false;
            Zone_A_ST_str=ZONA_DES;
        }
    // bit 2=ZA FALLA
        if(bitRead(dato_recibido, 2)){
            Zone_A_FAL=true;
            Zone_A_ST_str=ZONA_FA;
        }
        if(!bitRead(dato_recibido, 2)){
            Zone_A_FAL=false;
            Zone_A_ST_str=ZONA_DES;
        }
    // bit 3=ZB
        if(bitRead(dato_recibido, 3)){
            Zone_B_ALR=true;
            Zone_B_ST_str=ZONA_ACT;
        }
        if(!bitRead(dato_recibido, 3)){
            Zone_B_ALR=false;
            Zone_B_ST_str=ZONA_DES;
        }
    // bit 4=ZB FALLA
        if(bitRead(dato_recibido, 4)){
            Zone_B_FAL=true;
            Zone_B_ST_str=ZONA_FA;
        }
        if(!bitRead(dato_recibido, 4)){
            Zone_B_FAL=false;
            Zone_B_FAL_str=ZONA_DES;
        }
    // bit 5=NODE SOURCE
        if(bitRead(dato_recibido, 5)){
            Fuente=false;
            Batery=true;
            Node_Source_Mode_str=BATERIA_EN;
            // Zone_A_ST_str="BAT";
            // Zone_B_ST_str="BAT";
        }
        if(!bitRead(dato_recibido, 5)){
            Batery=false;
            Fuente=true;
            Node_Source_Mode_str=FUENTE_EN;
        }
    // bit 6=TIMER
        if(bitRead(dato_recibido, 6)){
            Timer_ON=true;
            Timer_ON_str=Timer_ciclo_k;
        }
        if(!bitRead(dato_recibido, 6)){
            Timer_ON=false;
            Timer_ON_str=Timer_token_k;
        }
    // bit 7=NODO SIN COMUNICACION
        //
        if(bitRead(dato_recibido, 7)){
            Zone_A_ST_str=NODO_PERDIDO;
            Zone_B_ST_str=NODO_PERDIDO;
        }
        // if(!bitRead(dato_recibido, 7)){
        //     Zone_A_ALR=false;
        //     Zone_A_ST_str="";
        // }
}
void Node::Estado(){
    Serial.println("SEC,"+Zone_A_ST_str+","+Node_Number+",A"+","+Node_Source_Mode_str);
    Serial.println("SEC,"+Zone_B_ST_str+","+Node_Number+",B"+","+Node_Source_Mode_str);
}