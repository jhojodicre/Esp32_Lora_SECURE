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
void Node::Update(int dato_actual_1, int dato_actual_2){
    int dato_recibido = dato_actual_1;
    // ZA
    if(bitRead(dato_recibido, 0)){
        Zone_A_ALR=true;
        Zone_A_ALR_str=ZONA_ACT;
    }
    if(!bitRead(dato_recibido, 0)){
        Zone_A_ALR=false;
        Zone_A_ALR_str=ZONA_DES;
    }
    // ZA FALLA
    if(bitRead(dato_recibido, 1)){
        Zone_A_FAL=true;
        Zone_A_FAL_str=ZONA_FA;
    }
    if(!bitRead(dato_recibido, 1)){
        Zone_A_FAL=false;
        Zone_A_FAL_str=ZONA_DES;
    }
    // ZB
    if(bitRead(dato_recibido, 2)){
        Zone_B_ALR=true;
        Zone_B_ALR_str=ZONA_ACT;
    }
    if(!bitRead(dato_recibido, 2)){
        Zone_B_ALR=false;
        Zone_B_ALR_str=ZONA_DES;
    }
    // ZB FALLA
    if(bitRead(dato_recibido, 3)){
        Zone_B_FAL=true;
        Zone_B_FAL_str=ZONA_FA;
    }
    if(!bitRead(dato_recibido, 3)){
        Zone_B_FAL=false;
        Zone_B_FAL_str=ZONA_DES;
    }
    // FUENTE
    if(bitRead(dato_recibido, 4)){
        Fuente=true;
        Fuente_str=FUENTE_OK;
    }
    if(!bitRead(dato_recibido, 4)){
        Fuente=false;
        Fuente_str=BATERIA_OK;
    }
    // timer
    if(bitRead(dato_recibido, 5)){
        Timer_ON=true;
        Timer_ON_str=Timer_ciclo_k;
    }
    if(!bitRead(dato_recibido, 5)){
        Timer_ON=false;
        Timer_ON_str=Timer_token_k;
    }
}
void Node::Estado(){
    Serial.println("SEC,"+Zone_A_ALR_str+","+Node_Number+",A");
    Serial.println("SEC,"+Zone_B_ALR_str+","+Node_Number+",B");
}
 