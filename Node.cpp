#include <Arduino.h>
#include <heltec.h>
#include "Node.h"

Node::Node(int numero_nodo){
    Node_Number = numero_nodo;
    _led = 21;
    _rele1 = 12;

    pinMode(_led, INPUT);
    pinMode(Zone_A_in, INPUT);
    pinMode(Zone_B_in, INPUT);
    pinMode(PB_ZA_in, INPUT);
    pinMode(PB_ZB_in, INPUT);
    pinMode(PB_ZC_in, INPUT);
    pinMode(Fuente_in, INPUT);
    pinMode(Entrada_X1_in, INPUT);
    pinMode(in_12, INPUT);
    pinMode(in_13, INPUT);

    digitalWrite(_led, HIGH);
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
    // bit 0= Modo Continuo.
        if(bitRead(dato_recibido,0))
        {
            MODE_CONTINIUS = true;
        }
        else{
            MODE_CONTINIUS = false;
        }
    // bit 1=ZA 
        // FALLA
            if(bitRead(dato_recibido, 1) && bitRead(dato_recibido, 2)){
                Zone_A_FAL=true;
                Zone_A_ST_str=ZONA_FA;
            }
        // ACTIVADA
            if(bitRead(dato_recibido,1)&& !bitRead(dato_recibido, 2))
            {
                Zone_A_ALR = true;
                Zone_A_ST_str = ZONA_ACT;
            }
        // DESACTIVADA
        if(!bitRead(dato_recibido,1)&& !bitRead(dato_recibido, 2))
        {
            Zone_A_ALR=false;
            Zone_A_ST_str = ZONA_DES;
        }
    // bit 3=ZB 
        // FALLA
            if(bitRead(dato_recibido,3)&& bitRead(dato_recibido, 4)){
                Zone_B_FAL=true;
                Zone_B_ST_str=ZONA_FA;
            }

        // ACTIVADA
            if(bitRead(dato_recibido, 3)&& !bitRead(dato_recibido, 4)){
                Zone_B_ALR=true;
                Zone_B_ST_str=ZONA_ACT;
            }
        // DESACTIVADA
        if(!bitRead(dato_recibido, 3)&& !bitRead(dato_recibido, 4)){
            Zone_B_ALR=false;
            Zone_B_ST_str=ZONA_DES;
        }
    // bit 5=NODE SOURCE
        if(bitRead(dato_recibido, 5)){
            Fuente=false;
            Batery=true;
            Node_Source_Mode_str=FUENTE_EN;
        }
        else{
            Batery=false;
            Fuente=true;
            Node_Source_Mode_str=BATERIA_EN;
        }
    // bit 6=TIMER
        if(bitRead(dato_recibido, 6)){
            Timer_ON=true;
            Timer_ON_str=Timer_ciclo_k;
        }
        else{
            Timer_ON=false;
            Timer_ON_str=Timer_token_k;
        }
    // bit 7=NODO SIN COMUNICACION
        if(bitRead(dato_recibido, 7)){
            Zone_A_ST_str=NODO_PERDIDO;
            Zone_B_ST_str=NODO_PERDIDO;
        }
        // else{
        //     Zone_A_ALR=false;
        //     Zone_A_ST_str="";
        // }
}
void Node::Estado(){
    if(MODE_CONTINIUS){
        Serial.println("SEC,"+Node_Number+MSG_NUM);
    }
    else{
        Serial.println("SEC,"+Zone_A_ST_str+","+Node_Number+",A"+","+Node_Source_Mode_str);
        Serial.println("SEC,"+Zone_B_ST_str+","+Node_Number+",B"+","+Node_Source_Mode_str);
    }
}
void Node::CONTINIUS(byte data_in){
    MODE_CONTINIUS = true;
    MSG_NUM = data_in;
}
void Node::A01(){
    for(int encender = 0; encender<5; ++encender){
        delay(1000);
        digitalWrite(_led, HIGH);
        delay(1000);
        digitalWrite(_led, LOW);
    }
}
void Node::revisar(){
      // 1. Pulsadores A y B Lectura.
        Zone_A_ACK=digitalRead(PB_ZA_in);       // pulsador A. PB_ZA_in
        Zone_B_ACK=digitalRead(PB_ZB_in);       // pulsador B.
        Zone_AB_ACK=digitalRead(PB_ZC_in);      // pulsador C. Pulsador por defecto PRG.
      // 2. Zona A y Zona B Lectura.
        Zone_A=digitalRead(Zone_A_in);
        Zone_B=digitalRead(Zone_B_in);
      // 3. Bateria o Fuente Lectura.
        Fuente=digitalRead(Fuente_in);

      // 4. ZONAS A y B RESET.
        // Pulsador C = reconocimiento de Ambas Zonas A y B. Reset de Ambas Zonas y Fallas.
        if(!Zone_AB_ACK){
          bitClear(Zonas, Zone_A);      // ZONA A Reset.
          bitClear(Zonas, Zone_B);      // ZONA B Reset.

          bitClear(Zonas_Fallan, Zone_A);     // ZONA A FALLA Reset.
          bitClear(Zonas_Fallan, Zone_B);     // ZONA B FALLA Reset.

          Zone_A_FAL_str='.';
          Zone_B_FAL_str='.';

          Zone_A_ERR=false;
          Zone_B_ERR=false;
        }
      // 5. ZONA A RESET= Zona A aceptada desde el pulsador activo en bajo "0"
        if(!Zone_A_ACK){
          bitClear(Zonas, Zone_A);
          bitClear(Zonas_Fallan, Zone_A);
          Zone_A_ERR=false;
          Zone_A_FAL_str='.';
          Zone_A_ST=false;
        }
      // 6. ZONA B RESET= Zona B aceptada desde el pulsador activo en bajo "0"
        if(!Zone_B_ACK){
          bitClear(Zonas, Zone_B);
          bitClear(Zonas_Fallan, Zone_B);
          Zone_A_ERR=false;
          Zone_B_FAL_str='.';        
          Zone_B_ST=false;
        }

      // 7. ZONA A ACTIVA.
        if(!Zone_A){
          bitSet(Zonas, Zone_A);
          Zone_A_ST=true;
        }
      // 8. ZONA B ACTIVA.
          if(!Zone_B){
            bitSet(Zonas, Zone_B);
            Zone_B_ST=true;
          }
      // 9. ZONAS ACTIVAS.
        if(Zonas>0){
          Zones_Enables=true;
        }
      // 11 ZONAS para mostrar en Pantalla  OLED
        Zone_A_ST_str=String(Zone_A_ST, BIN);
        Zone_B_ST_str=String(Zone_B_ST, BIN);

        Zone_A_ACK_str=String(!Zone_A_ACK, BIN);
        Zone_B_ACK_str=String(!Zone_B_ACK, BIN);

        Fuente_in_str=String(Fuente, BIN);        
}
void Node::a1_Nodo_Destellos (int repeticiones, int tiempo){
        int retardo=tiempo*100;

    for(int repetir=0; repetir<repeticiones; ++repetir){
        delay(retardo);                  // pausa 1 seg.
        digitalWrite(_led, HIGH);     // Led ON.
        delay(retardo);                  // pausa 1 seg.
        digitalWrite(_led, LOW);    // Led OFF.
    }
}
