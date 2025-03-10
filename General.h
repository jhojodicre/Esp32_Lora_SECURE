#ifndef GENERAL_H
#define GENERAL_H

class General {
public:
    General(bool general);

    void gestiones();
    void configuracion();
    bool out_rele_1;
    bool out_rele_2;
    bool LED_azul;
    bool Zona_A_in;
    bool Zona_B_in;
    bool PB_ZA_in;
    bool PB_ZB_in;
    bool PB_ZC_in;
    bool Fuente_in;

private:
    bool _general;
};;

#endif // GENERAL_H