#include <Arduino.h>
#include "Functions.h"

Functions::Functions(){

}
void Functions::A1(){

}

void Functions::Iniciar(){
    Led_Monitor(5);
}

void Functions::Led_Monitor(byte repeticiones){
    for (byte repetir=0;repetir<repeticiones;++repetir){
        digitalWrite(_LED_Azul, HIGH);
        delay(500);
        digitalWrite(_LED_Azul, LOW);
        delay(500);
    }
}
// void Functions::A2(){
    
// }
// void Functions::A3(){
    
// }
// void Functions::A4(){
    
// }
// void Functions::A5(){
    
// }
// void Functions::A6(){
    
// }
// void Functions::A7(){
    
// }
// void Functions::A8(){
    
// }
// void Functions::A9(){
    
// }
// void Functions::AA(){
    
// }