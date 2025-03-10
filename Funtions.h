#ifndef FUNTIONS_H
#define FUNTIONS_H
#include <Arduino.h>

class Funtions
{
    private:
    
        //-2.1 ENTRADAS= Definicion de etiquetas para las Entradas.
            byte Zona_A_in=32;        // 32 Entrada de Zona 1
            byte Zona_B_in=33;        // 33 Entrada de Zona 2
            byte PB_ZA_in=38;        // Pulsador A= Resetea o reconoce "ACK" la Zona A
            byte PB_ZB_in=39;        // Pulsador B= Resetea o reconoce "ACK" la Zona B
            byte PB_ZC_in=0;         // Pulsador C= Resetea o reconoce "ACK" las Zona AB (Entrada de Pulsador por Defecto PB_ZC_in.)
            byte Fuente_in=22;

            byte Entrada_X1_in=9;         // Entrada Digital.
            byte in_12=12;
            byte in_13=13;       

        //-2.2 SALIDAS= Definicion de etiquetas para las Salidas.
            byte _LED_Azul=      21;
            byte out_rele_1=    12;
            byte out_rele_2=    13;
        //-2.3 Variables para interrupciones
            bool    falg_ISR_stringComplete=false;
            bool    flag_F_codified_funtion=true;
            char    function_Mode;
            char    function_Number;
            String  function_Parameter1;
            String  function_Parameter2;
            String  function_Parameter3;
            String  function_Parameter4;
            int     x1, x2, x3, x4;
    public:
        Funtions(bool LED);
        void A1();
        bool iniciar();
        void welcome();
        void Led_Monitor(byte repeticiones);
        void Decodificar_Solicitud(String rx_recive);
        void Ejecutar_Solicitud();
        // A
            void a1();
            void a2();
            void a3();
            void a4();
            void a5();
            void a6();
            void a7();
            void a8();
            void a9();
            void a0();
            void aA();
        // B
            void b1();
            void b2();
            void b3();
            void b4();
            void b5();
            void b6();
            void b7();
            void b8();
            void b9();
            void b0();
            void bA();

            void c1();
            void c2();
            void c3();
            void c4();
            void c5();
            void c6();
            void c7();
            void c8();
            void c9();
            void c0();
            void cA();

            void m1();
            void m2();
            void m3();
            void m4();
            void m5();
            void m6();
            void m7();
            void m8();
            void m9();
            void m0();
            void mA();

            void s1();
            void s2();
            void s3();
            void s4();
            void s5();
            void s6();
            void s7();
            void s8();
            void s9();
            void s0();
            void sA();

        // Variables
            bool firstScan=false;
};
#endif
