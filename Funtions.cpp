#include <Arduino.h>
#include "heltec.h"
#include "Funtions.h"

Funtions::Funtions(bool LED){
   _LED_Azul= LED;
}
void Funtions::A1(){

}
bool Funtions::iniciar(){
    Led_Monitor(5);
    welcome();
    return true;
}
void Funtions::welcome(){
    Serial.println("SEC,MST,RST");
    Heltec.display->drawString(0, 20, "SEGURIDAD");
    Heltec.display->drawString(0, 30, "PERIMETRAL");
    Heltec.display->drawString(0, 40, "SECURE");
    Heltec.display->drawString(0, 50, "ALL");
    Heltec.display->display();
    delay(300);
}
void Funtions::Led_Monitor(byte repeticiones){
    for (byte repetir=0;repetir<repeticiones;++repetir){
        digitalWrite(_LED_Azul, HIGH);
        delay(500);
        digitalWrite(_LED_Azul, LOW);
        delay(500);
    }
}
void Funtions::Decodificar_Solicitud(String rx_recive){
    //Deshabilitamos Banderas
    falg_ISR_stringComplete=false;
    flag_F_codified_funtion=true;
    function_Mode=rx_recive.charAt(0);//(0,1);
    function_Number=rx_recive.charAt(1);
    function_Parameter1=rx_recive.substring(2,3);
    function_Parameter2=rx_recive.substring(3,4);
    function_Parameter3=rx_recive.substring(4,5);
    function_Parameter4=rx_recive.substring(5,6);
    Ejecutar_Solicitud();
   //  return true;
}
void Funtions::Ejecutar_Solicitud(){
    x1=function_Parameter1.toInt();
    x2=function_Parameter2.toInt();
    x3=function_Parameter3.toInt();
    x4=function_Parameter4.toInt();
    
    switch (function_Mode)
    {
        case 'A':
            switch (function_Number)
            {
                case '1':
                    a1();
                    break;
                 case '2':
                    a2();
                    break;
                 case '3':
                    a3();
                    break;
                 case '4':
                    a4();
                    break;
                 case '5':
                    a5();
                    break;
                 case '6':
                    a6();
                    break;
                 case '7':
                    a7();
                    break;
                 case '8':
                    a8();
                    break;
                 case '9':
                    a9();
                    break;
                 case '0':
                    a0();
                    break;
                
                default:
                    break;
            }
            break;
        case 'B':
            switch (function_Number)
            {
                case '1':
                    b1();
                    break;
                 case '2':
                    b2();
                    break;
                 case '3':
                    b3();
                    break;
                 case '4':
                    b4();
                    break;
                 case '5':
                    b5();
                    break;
                 case '6':
                    b6();
                    break;
                 case '7':
                    b7();
                    break;
                 case '8':
                    b8();
                    break;
                 case '9':
                    b9();
                    break;
                 case '0':
                    b0();
                    break;
                
                default:
                    break;
            }
            break;
        case 'C':
            switch (function_Number)
            {
                case '1':
                    c1();
                    break;
                 case '2':
                    c2();
                    break;
                 case '3':
                    c3();
                    break;
                 case '4':
                    c4();
                    break;
                 case '5':
                    c5();
                    break;
                 case '6':
                    c6();
                    break;
                 case '7':
                    c7();
                    break;
                 case '8':
                    c8();
                    break;
                 case '9':
                    c9();
                    break;
                 case '0':
                    c0();
                    break;
                
                default:
                    break;
            }
            break;
        case 'M':
            switch (function_Number)
            {
                case '1':
                    m1();
                    break;
                 case '2':
                    m2();
                    break;
                 case '3':
                    m3();
                    break;
                 case '4':
                    m4();
                    break;
                 case '5':
                    m5();
                    break;
                 case '6':
                    m6();
                    break;
                 case '7':
                    m7();
                    break;
                 case '8':
                    m8();
                    break;
                 case '9':
                    m9();
                    break;
                 case '0':
                    m0();
                    break;
                
                default:
                    break;
            }
            break;
        case 'S':
            switch (function_Number)
            {
                case '1':
                    s1();
                    break;
                 case '2':
                    s2();
                    break;
                 case '3':
                    s3();
                    break;
                 case '4':
                    s4();
                    break;
                 case '5':
                    s5();
                    break;
                 case '6':
                    s6();
                    break;
                 case '7':
                    s7();
                    break;
                 case '8':
                    s8();
                    break;
                 case '9':
                    s9();
                    break;
                 case '0':
                    s0();
                    break;
                
                default:
                    break;
            }
            break;
        default:
            break;
    }
}
// A - Funtions
void Funtions::a0(){

}
void Funtions::a1(){

}
void Funtions::a2(){
    
}
void Funtions::a3(){
    
}
void Funtions::a4(){
    
}
void Funtions::a5(){
    
}
void Funtions::a6(){
    
}
void Funtions::a7(){
    
}
void Funtions::a8(){
    
}
void Funtions::a9(){
    
}
void Funtions::aA(){
    
}

// B - Funtions
void Funtions::b0(){
    
}
void Funtions::b1(){

}
void Funtions::b2(){
    
}
void Funtions::b3(){
    
}
void Funtions::b4(){
    
}
void Funtions::b5(){
    
}
void Funtions::b6(){
    
}
void Funtions::b7(){
    
}
void Funtions::b8(){
    
}
void Funtions::b9(){
    
}
void Funtions::bA(){
}

// C Funtions 
void Funtions::c0(){
    
}
void Funtions::c1(){

}
void Funtions::c2(){
    
}
void Funtions::c3(){
    
}
void Funtions::c4(){
    
}
void Funtions::c5(){
    
}
void Funtions::c6(){
    
}
void Funtions::c7(){
    
}
void Funtions::c8(){
    
}
void Funtions::c9(){
    
}
void Funtions::cA(){
}

// M - Funtions
void Funtions::m0(){
    
}
void Funtions::m1(){

}
void Funtions::m2(){
    
}
void Funtions::m3(){
    
}
void Funtions::m4(){
    
}
void Funtions::m5(){
    
}
void Funtions::m6(){
    
}
void Funtions::m7(){
    
}
void Funtions::m8(){
    
}
void Funtions::m9(){
    
}
void Funtions::mA(){
}

// S Funtions
void Funtions::s0(){
    
}
void Funtions::s1(){

}
void Funtions::s2(){
    
}
void Funtions::s3(){
    
}
void Funtions::s4(){
    
}
void Funtions::s5(){
    
}
void Funtions::s6(){
    
}
void Funtions::s7(){
    
}
void Funtions::s8(){
    
}
void Funtions::s9(){
    
}
void Funtions::sA(){
}