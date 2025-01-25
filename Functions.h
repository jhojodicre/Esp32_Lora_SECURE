#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <Arduino.h>
class Functions{

    public:
        Functions();
        void A1();
        void Iniciar();
        void Led_Monitor(byte repeticiones);

    private:
        byte    _LED_Azul=35;

};
#endif
