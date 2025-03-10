#include "General.h"
#include <Arduino.h>

General::General(bool general){
    // Inicialización adicional si es necesario
    _general = general;
}
void General::gestiones() {
    // Implementación de la función gestiones
}

void General::configuracion() {
    // Implementación de la función configuracion
    //1.1 Configuracion de Salidas:
      // pinMode(in_36, OUTPUT);
      pinMode(out_rele_1, OUTPUT);
      pinMode(out_rele_2, OUTPUT);
      pinMode(LED_azul, OUTPUT);
    //1.2 Configuracion de Entradas
      pinMode(Zona_A_in, INPUT);
      pinMode(Zona_B_in, INPUT);

      pinMode(PB_ZA_in, INPUT);
      pinMode(PB_ZB_in, INPUT);
      pinMode(PB_ZC_in, INPUT);

      pinMode(Fuente_in, INPUT);

      // pinMode(in_12, INPUT);
      // pinMode(in_13, INPUT);      
      
}