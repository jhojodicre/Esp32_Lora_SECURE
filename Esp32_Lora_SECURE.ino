//1. librerias.
  //- 1.1 Librerias****
    #include <Ticker.h>
    #include <SPI.h>
    #include <heltec.h>
    #include "Node.h"
    // #include "images.h"
    
//2. PINOUT Definicion de ENTRADAS Y SALIDAS.
  //  Las Etiquetas para los pinout son los que comienzan con GPIO
  //  Es decir, si queremos activar la salida 1, tenemos que buscar la referencia GPIO 1, Pero solomante Escribir 1 sin "GPIO"
  //  NO tomar como referencia las etiquetas D1, D2,D3, ....Dx.
  
  //-2.1 ENTRADAS= Definicion de etiquetas para las Entradas.
    #define Zona_A_in     32        // 32 Entrada de Zona 1
    #define Zona_B_in     33        // 33 Entrada de Zona 2
    #define PB_ZA_in      38        // Pulsador A= Resetea o reconoce "ACK" la Zona A
    #define PB_ZB_in      39        // Pulsador B= Resetea o reconoce "ACK" la Zona B
    #define PB_ZC_in      0         // Pulsador C= Resetea o reconoce "ACK" las Zona AB (Entrada de Pulsador por Defecto PB_ZC_in.)
    #define Fuente_in     22

    #define Entrada_X1_in 9         // Entrada Digital.
    #define in_12         12
    #define in_13         13       

  //-2.2 SALIDAS= Definicion de etiquetas para las Salidas.
    #define LED_azul      21
    #define out_rele_1    12
    #define out_rele_2    13

  //-2.3 ZONAS
    #define P1ZA          0
    #define P1ZB          1
    #define P2ZA          2
    #define P2ZB          3
    #define P3ZA          4
    #define P3ZB          5
    #define P4ZA          6
    #define P4ZB          7

    #define P5ZA          0
    #define P5ZB          1
    #define P6ZA          2
    #define P6ZB          3
    #define P7ZA          4
    #define P7ZB          5
    #define P8ZA          6
    #define P8ZB          7

  //-2.4 Constantes.
    #define RFM95_FREQ    915E6  
    #define INDEPENDIENTE 1
    #define MASTER        2
  //-2.5 timer

//3. Variables Globales.
  //-3.1 Variables Interrupciones
    volatile bool flag_ISR_prueba=false;             // Flag: prueba para interrupcion serial.
    volatile bool falg_ISR_stringComplete=false;    // Flag: mensaje Serial Recibido completo.
    volatile bool flag_ISR_temporizador_1=false;
    volatile bool flag_ISR_temporizador_2=false;
    volatile bool flag_ISR_temporizador_3=false;        // pra actualizar los dato al servidor.
    volatile bool flag_ISR_temporizador_0=false;
    volatile bool flag_ISR_LORA=false;

    String        inputString;           // Buffer recepcion Serial.
    String        funtion_Mode;          // Tipo de funcion para ejecutar.
    String        funtion_Number;        // Numero de funcion a EJECUTAR.
    String        funtion_Parmeter1;     // Parametro 1 de la Funcion.
    String        funtion_Parmeter2;     // Parametro 2 de la Funcion.
    String        funtion_Parmeter3;     // Parametro 3 de la Funcion.
    String        funtion_Parmeter4;      // Parametro para las Funciones remotas.
    String        function_Remote;
    String        function_Enable;
    uint           x1=0;
    volatile uint  x2=0;
    volatile uint  x3=0;
    volatile uint  x4=0;
  //-3.2 Variables Banderas.
    bool          flag_F_codified_funtion=false;   // Notifica que la funcion ha sido codificada.
    bool          flag_F_Un_segundo=false;         // Se activa cuando Pasa un Segundo por Interrupcion.
    bool          flag_F_inicio=true;              // Habilitar mensaje de flag_F_inicio por unica vez
    bool          flag_F_responder=false;          // Se activa cuando recibe un mensaje para luego responder.
    bool          flag_F_modo_Continuo=false;
    bool          flag_F_depurar=false;
    bool          flag_F_once=true;
    bool          flag_F_updateServer=false;
    bool          flag_F_respondido=false;
    bool          flag_F_masteRequest=false;
    bool          flag_F_masterIniciado=false;      // Puede ocurrir durante un Reinicio
    bool          flag_F_nodoRequest=false;
    bool          flag_F_masterNodo=false;          // Habilitada para solicitar informacion a un Nodo Especifico    
    bool          flag_F_PAQUETE=false;
    bool          flag_F_tokenTime=false;
    bool          flag_F_cycleTime=false;
    bool          flag_F_T2_run=false;
    bool          flag_F_T1_run=false;
    bool          flag_F_Nodos_Completos=false;
    bool          flag_F_Nodos_Incompletos=false;
    bool          flag_F_Nodo_Iniciado=false;
    bool          flag_F_Nodo_Ultimo=false;
    bool          flag_F_nodo_Anterior=false;       // Indica cuando el nodo anterior se a comunicado con el nodo actual.
    bool          flag_F_token=false;               // Se habilita caundo el nodo responde por token
    bool          flag_F_analizar=false;
    bool          Nodo_waiting=false;
    bool          flag_F_Node_Enable=false;         // Se activa cuando localaddress es menor a 255 $>>-Setup 2.2
    bool          flag_F_totalTime=false;
    bool          flag_F_contar_tiempo=false;
    bool          flag_F_Master_Enable=false;       // Es Habilitado Solo cuando el esta en MODO Master
    bool          flag_F_Master_Esperando=true;        // Master espera esta activo cuando no recibe respuesta de ningun nodo. SE INICIALIZA EN VERDADERO

  
  //-3.3 Variables NODOS y ZONAS.
      // GENERALES
      int         MODE;
      //
      byte        master=0xFF;
      int         Nodos;//*****************// Establece Cuantos Nodos Confirman La Red a6.
      byte        Nodo_primero;
      int         Nodo_ultimo;
      byte        Nodo_anterior;
      byte        Nodo_siguiente;    // Direccion del Nodo que sigue para enviar mensaje
      byte        Nodo_destino;
      byte        Nodo_actual=0;
      byte        nodo_proximo;
      int         Nodo_Modo;
      int         Nodos_leidos;         // NODS Y MASTER -1
      float       Nodos_leidos_aux;
      word        Nodos_LSB_ACK;
      int         Nodos_LSB_str;
      int         Nodos_y_Master;
      String      Nodo_Name;
      byte        nodos_LSB_MERGE;           // variable para igualar el dato de los nodos reportados
      byte        nodos_MSB_MERGE;
      int         nodoCount;                  // Numero de veces que recibe el mensaje del Nodo
      bool        flag_ST_nodo1;
      int         nodo; // provicional para quitar el error
      byte        nodo1=0;
      byte        nodo2=0;
      byte        nodo3=0;
      byte        nodo4=0;
      byte        nodo5=0;
      byte        nodo6=0;
      byte        nodo7=0;
      byte        nodo8=0;
      byte        nodo9=0;
      byte        nodo10=0;
      byte        nodo11=0;
      byte        nodo12=0;
      byte        nodo13=0;
      byte        nodo14=0;
      byte        nodo15=0;
      byte        nodo16=0;
      byte        nodo_local=0;   // Info del nodo al final del mensaje.
      String      nodo_Status;
      byte        Nodo_Current;




      byte        Zonas_MSB=0;
      byte        Zonas_LSB=0;
      byte        Zonas_LSB_Estados=0;
      byte        Zonas_MSB_Estados=0;
      byte        Zonas_LSB_Mascara;
      byte        Zonas_MSB_Mascara;
      word        Zonas_Mascaras=65535;
      word        Zonas=65535;             // Estado de Zonas Activas.      
      int         Zonas_LSB_str=8;
      int         Zonas_MSB_str=8;


      int         Zona_A;
      int         Zona_B;
      bool        Zona_A_ax;
      bool        Zona_B_ax;
      bool        Zona_A_ACK;
      bool        Zona_B_ACK;
      bool        Zona_AB_ACK;
      bool        Zona_A_ST;
      bool        Zona_B_ST;
      String      Zona_A_str;
      String      Zona_B_str;
      String      Zona_A_ACK_str;
      String      Zona_B_ACK_str;
      byte        Zonas_Falla_Time;        // Tiempo para determinar cuando una zona esta en Falla. Numero de veces que Timer se ejecute
      word        Zonas_Fallan;
      byte        Zonas_Fallan_LSB;
      byte        Zonas_Fallan_MSB;
      
      byte        ZonasF_LSB_Estados;
      byte        ZonasF_MSB_Estados;

      int         ZonasF_LSB_str;
      char        Zona_A_F_str;
      char        Zona_B_F_str;
      // TIEMPO DE ZONA EN FALLA.
      int        zona_1;
      int        zona_2;
      int        zona_3;
      int        zona_4;
      int        zona_5;
      int        zona_6;
      int        zona_7;
      int        zona_8;

      bool        zona_1_err=false;
      bool        zona_2_err=false;
      bool        zona_3_err=false;
      bool        zona_4_err=false;
      bool        zona_5_err=false;
      bool        zona_6_err=false;
      bool        zona_7_err=false;
      bool        zona_8_err=false;

      bool        Fuente_in_ST=false;
      String      Fuente_in_str;
      bool        bat=false;
      bool        timer_nodo_ST;



      int         te_toca=1;           // Prueba para comunicacion continua con el servidor.      
  //-3.4 Variables TIME.
      long        initialTime= 0;

      long        currentTime_1 = 0;
      long        elapseTime_1 = 0;
      long        afterTime_1  = 0;
      long        beforeTime_1 = 0;
      long        beforeTime_GAP;         // Tiempo transcurrido entre mensajes entrantes para el MASTER.
      long        currentTime_GAP;
      long        elapseTime_GAP;

      long        currentTime_2 = 0;
      long        elapseTime_2 = 0;
      long        afterTime_2  = 0;
      long        beforeTime_2 = 0;

      long        chismeTime = 1000;
      long        baseTime   = 1000;
      long        cycleTime = 1000;
      long        tokenTime  = 2000;
      long        updateTime = 2000;
      long        masterTime = 10000;
      float       wakeUpTime = 90.0;
      long        firstTime;
      long        tokenLast;
      long        totalTime;
      long        waitTime   = 0;
      uint32_t    remainT1;
      uint32_t    remainT2;
      int         fastTime    =   1;
      String      temporizador_1_str;
      String      temporizador_2_str;

    // Alarmas
      int         Alarma_Zona_A_in=0;
      int         Alarma_Zona_B_in=0;
      int         Alarma_Entrada_X1_in=0;
    // Eventos
      
  //-3.5 Variables RFM95.
    // Variables para Recibir.
      byte        incoming_sender;               // incoming_sender address
      int         incoming_recipient;            // incoming_recipient address      
      byte        incoming_zonesLSB;     // incoming_function msg ID
      byte        incoming_zonesMSB;     // incoming_function msg ID
      byte        incoming_nodosLSB;
      byte        incoming_nodosMSB;
      byte        incoming_nodo_info;
      byte        incoming_zonaFLSB;
      byte        incoming_zonaFMSB;
      byte        incoming_nodoInfo;
      byte        incoming_length;       // incoming_function msg length
      String      incoming_function = "";

    // Variable para Enviar.
      byte        destination; // destination to send to  0xFF;         a4      
      byte        localAddress  = 0x06; // address of this device           a3
      byte        zonesLSB;
      byte        zonesMSB;

      byte        nodosLSB;
      byte        nodosMSB;
      byte        zonaFLSB;
      byte        zonaFMSB;
      byte        nodoInfo;             // informacion particular que envia el nodo
      String      outgoing;             // outgoing message
      byte        msgNumber;            // en modo continuo este numero incrementa automaticamente.          // interval between sends.      
    
    // Otras.  
      String      codigo="";
      String      info_1="";
      char        incomingFuntion;
//4. Intancias.
  //-4.1 Temporizadores.
    Ticker temporizador_1;                // Tiempo de Ciclo.
    Ticker temporizador_2;                // Tiempo token.
    Ticker temporizador_3;                // Tiempo update Server.
    Ticker temporizador_0;                // Tiempo de Master Request.
    // Ticker temporizador_4;             // Tiempo de respuesta de todas las placas.
  //-4.1 Node
    Node Node0(0);    // Nodo Local
    Node Node1(1);
    Node Node2(2);
    Node Node3(3);
    Node Node4(4);
    Node Node5(5);
    Node Node6(6);
    Node Node7(7);
    Node Node8(8);
    Node Node9(9);
    Node Node10(10);
    Node Node11(11);
    Node Node12(12);

//5. Funciones ISR.
  //-5.1 Serial Function.
    void serialEvent (){
      while (Serial.available()) {
        // get the new byte:
        char inChar = (char)Serial.read();
        // add it to the inputString:
        inputString += inChar;
        // if the incoming_function character is a newline, set a flag so the main loop can
        // do something about it:
        if (inChar == '\n') {
          falg_ISR_stringComplete = true;
          flag_F_codified_funtion=false;
        }
      }
    }
  //-5.2 Extern Function
    // ICACHE_RAM_ATTR void ISR_0(){
    //   flag_ISR_prueba=true;
    //   Zonas=0;

    // }
    // ICACHE_RAM_ATTR void ISR_1(){
    // }
    // ICACHE_RAM_ATTR void ISR_2(){
    // }
    // ICACHE_RAM_ATTR void ISR_3(){
    //   bitClear(Zonas, Zona_A);
    // }
    // ICACHE_RAM_ATTR void ISR_4(){
    //   bitClear(Zonas, Zona_B);
    // }
  //-5.3 Interrupciones por Timers.
    void ISR_temporizador_0(){
      flag_ISR_temporizador_0=true;
      // if(flag_F_Nodos_Incompletos){
      //   analisar();
      // }
    }
    void ISR_temporizador_1(){
        beforeTime_1 = millis();
        flag_ISR_temporizador_1=true;
        temporizador_1_str=String(flag_ISR_temporizador_1,BIN);
    }
    void ISR_temporizador_2(){
      currentTime_2 = millis();
      flag_F_token=true;
      elapseTime_2=0;
      flag_ISR_temporizador_2=true;
      temporizador_2_str=String(flag_ISR_temporizador_2,BIN);
      flag_F_T2_run=false;
    }
    void ISR_temporizador_3(){
      flag_ISR_temporizador_3=true;
    }
void setup(){
  //1. Configuracion de Puertos.
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
      
  //2. Condiciones Iniciales.
    //-2.1 Estado de Salidas.
      digitalWrite(out_rele_1, LOW);
      digitalWrite(out_rele_2, LOW);
    //-2.2 Valores y Espacios de Variables.
      localAddress    = 0xFF;
      Nodos           = 8;
      Nodo_primero    = 1;
      // Nodo_ultimo     = 3;
      Nodo_ultimo     = Nodos;
      Zonas_Falla_Time= 3;           // cuenta el Numero de veces que timer 1 se activa para dar zona falla constante.
      MODE            = MASTER;
      Nodo_Modo       = MASTER;
      Nodos_y_Master  = Nodos +1;    // Numero de Nodos + Master.
      Nodos_leidos    = Nodos_y_Master -1;
      Nodo_actual     = localAddress;
      Nodo_siguiente  = localAddress + 1;
      Nodo_anterior   = localAddress - 1;
      Zona_B          = localAddress + (localAddress - 1); // cada zona tomara su numero depende de la direccion local que tome el nodo. ORIGIBALMENTE= localAddress + (localAddress - 1)
      // Zona_B          = localAddress + (localAddress - 2); Esta linea puede ser activada cuando el numero de zonas son diferentes a la secuencia normal, ya que la placa 1 tiene las zonas 1 y 2, y asi sucesivamente.
      Zona_A          = Zona_B - 1;
      incoming_zonesLSB  = 0x00;
      incoming_zonesMSB  = 0x00;
      
      if(localAddress==255){
        Nodo_Name="MASTER:";
        flag_F_Master_Enable=true;
        flag_F_Node_Enable=false;
      }
      else{
        Nodo_Name="NODO:";
        flag_F_Master_Enable=false;
        flag_F_Node_Enable=true;
      }
      if(localAddress==Nodo_primero){
        Nodo_anterior=Nodo_ultimo;
      }
      if(localAddress==Nodo_ultimo){
        flag_F_Nodo_Ultimo=true;
        Nodo_siguiente=Nodo_primero;
      }
      // BANDERAS

    //-2.3 Timer Answer.
      tokenTime       = 700;
      baseTime        = 400;
      updateTime      = 400;
      wakeUpTime      = tokenTime*localAddress;
      cycleTime       = tokenTime*(Nodos+1);
      masterTime      = 2000;          // Cada 100 milisegundo el maetro le pregunta a cada esclavo, para pruebas ponemos 1 segundo igual a 1000.
      firstTime       = tokenTime*localAddress;     // El primer mensaje esta calculado en tiempo forma para cada nodo.
      wakeUpTime      = 30.0;         // Este temporizador es para rresponder despues que el nodo despierta despues de mucho tiempo sin encender y es el unico que esta activo.
      chismeTime      = (10*Nodo_actual)+100;
      fastTime        = 10;
      // Timer 3 Responde despues de Reiniciar sin Recibir respuesta.
      if(localAddress<255){
        temporizador_3.once(wakeUpTime, ISR_temporizador_3);
      }
      if(flag_F_Master_Enable){
        tokenTime       = 100;
      }
      
    //-2.4 Mascara de Zonas.
      Zonas_Mascaras=65535;
      Zonas=0;
      bitClear(Zonas_Mascaras, Zona_A);
      bitClear(Zonas_Mascaras, Zona_B);
      Zonas_LSB_Mascara=lowByte(Zonas_Mascaras);
      Zonas_MSB_Mascara=highByte(Zonas_Mascaras);
    //-2.5 Nodos a Leer.
      Nodos_leidos_aux = pow(2, Nodos_y_Master);
  //3. Configuracion de Perifericos:
    //-3.1 Comunicacion Serial:
      Serial.begin(115200);
      delay(10);
    
    //-3.2 Interrupciones Habilitadas.
      //****************************
      // attachInterrupt (digitalPinToInterrupt (PB_ZC_in), ISR_0, FALLING);  // attach interrupt handler for D2
      // attachInterrupt (digitalPinToInterrupt (Zona_A_in), ISR_1, FALLING);      // attach interrupt handler for D2
      // attachInterrupt (digitalPinToInterrupt (Zona_B_in), ISR_2, FALLING);      // attach interrupt handler for D2
      // attachInterrupt (digitalPinToInterrupt (PB_ZA_in), ISR_3, FALLING);      // attach interrupt handler for D2
      // attachInterrupt (digitalPinToInterrupt (PB_ZB_in), ISR_4, FALLING);      // attach interrupt handler for D2

      //interrupts ();

  //4. Prueba de Sitema Minimo Configurado.
    if(flag_F_depurar){
      Serial.println("Sistema Minimo Configurado");
    }
  //5. Configuracion de DEVICE externos.
    //-5.1 WIFI ESP32 LORA Configuracion.
      Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.LoRa Enable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, RFM95_FREQ /*long BAND*/);
    //-5.2 OLED Display.
      // Heltec.display->init();
      // Heltec.display->flipScreenVertically();        // Invierte el Orden de La LCD
      // Heltec.display->setFont(ArialMT_Plain_10);
      // delay(1500);
      // Heltec.display->clear();

      // Heltec.display->drawString(0, 0, "Heltec.LoRa Initial success!");
      Heltec.display->drawString(0, 10, "Wait for incoming_function data...");
      Heltec.display->display();
      delay(100);
  
}
void loop(){
  //1. Bienvenida Funcion
    while (flag_F_inicio){
      welcome();        // Comprobamos el Sistema minimo de Funcionamiento.
      led_Monitor(3);
      temporizador_1.attach_ms(1000, ISR_temporizador_1);
      beforeTime_1=millis();
      flag_F_T1_run=true;
      if(flag_F_Master_Enable){
        nodo_proximo=Nodo_primero-1;  
      }
      else{
        if(MODE==INDEPENDIENTE){
          b6();
          temporizador_2.once_ms(firstTime, ISR_temporizador_2);
          temporizador_1.attach_ms(cycleTime, ISR_temporizador_1);
          beforeTime_1=millis();
          flag_F_T1_run=true;
          flag_F_T2_run=true;
          flag_F_Nodo_Iniciado=true;
        }
      }
    }
  //2. Decodificar funcion serial
    if(falg_ISR_stringComplete){
      decodificar_solicitud();
    }
  //3. Ejecutar Funcion
      if(flag_F_codified_funtion){
        ejecutar_solicitud();
        flag_F_codified_funtion=false;
        inputString="";
      }
  //4. Atender Las fucniones activadas desde ISR FLAGS.
    //-4.0 Bandera de Prueba.
      if(flag_ISR_prueba){
      // flag_ISR_prueba=false;
        // a1_Nodo_Destellos(1,3);
      }
    //-4.1 EJ-  REVISO Y ACTUALIZO.
      reviso();
      actualizar();  

    //-4.2 F- Timer 1.
      if(flag_ISR_temporizador_1){
        analizar();
        //MASTER MODE.
          // RESPONDER.
          if(flag_F_Master_Enable && flag_F_Master_Esperando){
            Nodo_REQUEST();
            b7();
            flag_F_responder=true;
            flag_F_Master_Esperando=true; // Master indica que queda esperando un mensaje
          }
        //NODE MODE.
          // RESPONDER si NO hay TOKEN.
          if (flag_F_Node_Enable && !flag_F_token && MODE==INDEPENDIENTE){
            b6();
            flag_F_PAQUETE=false;
            flag_F_responder=true;
            timer_nodo_ST=true;
          }
        //FLAG UPDTAE.
          if (flag_F_token){
            flag_F_token=false;
          }
          flag_ISR_temporizador_1=false;
      }
    //-4.3 F- Timer 2.
      if(flag_ISR_temporizador_2){
        beforeTime_2=0;   
        if(flag_F_masterNodo){
          codigo=function_Remote;
        }
        // codigo="S2" + Fuente_in_str + temporizador_2_str;//"TK";
        if(flag_F_Master_Enable){
          Nodo_REQUEST();
          b7();
          flag_F_Master_Esperando=true; // Master indica que queda esperando un mensaje
        }
        flag_F_tokenTime=true;
        flag_F_responder=true;
        timer_nodo_ST=false;
      }
    //-4.4 F- Timer 0.
      if(flag_ISR_temporizador_0){
        // analizar();
        // flag_F_responder=true;
      }
    //-4.5 F- Timer 3.
      if(flag_ISR_temporizador_3 && !flag_F_Nodo_Iniciado){
        b6();
        flag_F_responder=true;
      }

    //-4.6 F- Server Update.
      if(flag_F_updateServer){
        serverUpdate();
      }
    //-4.7 F- Recepcion de Paquete.
      if(flag_F_PAQUETE){
        flag_F_PAQUETE=false;
        secuencia();              // Si recibo un paquete, voy a secuencia para preparar el mensaje que respondere dependiendo de lo que haya recibido
      }
  //5. RFM95 Funciones.
    //-5.1 RFM95 RESPONDER Si?
      if(flag_F_responder){
        // actualizar();
        RFM95_enviar(codigo);
      }
    //-5.2 RFM95 RECIBIR.
      RFM95_recibir(LoRa.parsePacket());
}
//1. Funciones de Logic interna del Micro.
  void welcome(){
    flag_F_inicio=false;
    Serial.println("SEC,MST,RST");
    if(flag_F_depurar){
      
      Serial.println("Sistema Iniciado");
      Serial.print("Direccion: ");
      Serial.println(localAddress);
    }
    info_1=String(Zonas, BIN);
    Heltec.display->drawString(0, 20, "SEGURIDAD");
    Heltec.display->drawString(0, 30, "PERIMETRAL");
    Heltec.display->drawString(0, 40, "SECURE");
    Heltec.display->drawString(0, 50, "ALL");
    Heltec.display->display();
    delay(300);
  }
  void led_Monitor(int repeticiones){
    // Deshabilitamos Banderas
    int repetir=repeticiones;
    for (int encender=0; encender<repetir; ++encender){
      digitalWrite(LED_azul, HIGH);  // Led ON.
      delay(500);                    // pausa 1 seg.
      digitalWrite(LED_azul, LOW);   // Led OFF.
      delay(500);                    // pausa 1 seg.
    }
  }
//2. Gestiona las funciones a Ejecutar.
  void decodificar_solicitud(){
    //Deshabilitamos Banderas
    falg_ISR_stringComplete=false;
    flag_F_codified_funtion=true;
    funtion_Mode=inputString.substring(0,1);
    funtion_Number=inputString.substring(1,2);
    funtion_Parmeter1=inputString.substring(2,3);
    funtion_Parmeter2=inputString.substring(3,4);
    funtion_Parmeter3=inputString.substring(4,5);
    funtion_Parmeter4=inputString.substring(5,6);

    if(flag_F_depurar){
      Serial.println(inputString);         // Pureba de Comunicacion Serial.
      Serial.println("funcion: " + funtion_Mode);
      Serial.println("Numero: " + funtion_Number);
      Serial.println("Parametro1: " + funtion_Parmeter1);
      Serial.println("Parametro2: " + funtion_Parmeter2);
      Serial.println("Parametro3: " + funtion_Parmeter3+ "\n");
    }
  }
  void ejecutar_solicitud(){
    x1=funtion_Parmeter1.toInt();
    x2=funtion_Parmeter2.toInt();
    x3=funtion_Parmeter3.toInt();
    x4=funtion_Parmeter4.toInt();
    // Function Tipo A
      if (funtion_Mode=="A" && funtion_Number=="1"){
        if(flag_F_depurar){
          Serial.println("funion A N?�001");
        }  
        a1_Nodo_Destellos(x1,x2);
      }
      if (funtion_Mode=="A" && funtion_Number=="2"){
        if(flag_F_depurar){
          Serial.println("funion A N?�2");
        }
        a2();
      }
      if (funtion_Mode=="A" && funtion_Number=="3"){
        // FUNCIONO A MEDIAS SOLO DIRECIONES BAJAS Y 255 falta acomodar un poco mas
        if(flag_F_depurar){
          Serial.println("funion A N?�3");
        }
        String Nodo_direccion_aux = "";
        Nodo_direccion_aux = funtion_Parmeter1 + funtion_Parmeter2 + funtion_Parmeter3;
        //Serial.println(Nodo_direccion_aux);
        int Nodo_direccion = Nodo_direccion_aux.toInt();
        //Serial.println(Nodo_direccion);
        a3_Nodo_Direccion_Local(Nodo_direccion);
      }
      if (funtion_Mode=="A" && funtion_Number=="4"){
        if(flag_F_depurar){
          Serial.println("funion A N?�4");
        }
        String Nodo_destino_aux = "";
        Nodo_destino_aux = funtion_Parmeter1+funtion_Parmeter2+funtion_Parmeter3;
        int Nodo_destino = Nodo_destino_aux.toInt();
        a4_Nodo_Direccion_Destino(Nodo_destino);
      }
      if (funtion_Mode=="A" && funtion_Number=="5"){
        if(flag_F_depurar){
          Serial.println("funion A N?�5: Modo Continuo");
        }
        a5_Nodo_Modo_Continuo(x1);
      }
      if (funtion_Mode=="A" && funtion_Number=="6"){
        if(flag_F_depurar){
          Serial.println("funion A N?�6: Numero de Nodos");
        }
        String Nodos_numeros_aux = funtion_Parmeter1+funtion_Parmeter2;
        int Nodos_numeros = Nodos_numeros_aux.toInt();
        a6_Nodo_Numeros(Nodos_numeros);
      }
      if (funtion_Mode=="A" && funtion_Number=="7"){
        if(flag_F_depurar){
          Serial.println("funion A N?�7 Modo Depurar");
        }
        a7(x1);
      }
      if (funtion_Mode=="A" && funtion_Number=="8"){
        Serial.println("funion A N?�8 Status");
        //1.
        Serial.print("Direccion Local: ");
        Serial.println(localAddress);
        //2.
        Serial.print("Direccion Destino: ");
        Serial.println(destination);
        //3.
        Serial.print("Modo Continuo: ");
        Serial.println(flag_F_modo_Continuo);
        //4.
        Serial.print("Entradas: ");
        Serial.println(info_1=String(Zonas, BIN));
        //5.
        Serial.print("Zona A= bit: ");
        Serial.println(Zona_A);
        Serial.print("Zona B= bit: ");
        Serial.println(Zona_B);
        Serial.print("Zona A Estado: ");
        Serial.println(Zona_A_ax);
        Serial.print("Zona B Estado: ");
        Serial.println(Zona_B_ax);
        //6.
        Serial.print("Pulsadores: ");
        Serial.print(Zona_B_ACK);
        Serial.println(Zona_A_ACK);
        //7.
        Serial.print("Codigo: ");
        Serial.println(codigo);
        //8.
        Serial.print("tokenTime: ");
        Serial.println(tokenTime);

        //9.
        Serial.print("CycleTime: ");
        Serial.println(cycleTime);

        //10.
        Serial.print("masterTime: ");
        Serial.println(masterTime);
        //11.
        Serial.print("Nodos Leidos: ");
        Serial.println(Nodos_leidos_aux);        
      }
      if (funtion_Mode=="A" && funtion_Number=="9"){
        ESP.restart();
      }
      if (funtion_Mode=="A" && funtion_Number=="0"){
        // sIN PROGRAMAR.
      }
    // Function Tipo B
      //
      if (funtion_Mode=="B" && funtion_Number=="1"){
        if(flag_F_depurar){
          Serial.println("funion B N?�1: Quien envia?");
        }
        b1();
      }
      if (funtion_Mode=="B" && funtion_Number=="2"){
        if(flag_F_depurar){
          Serial.println("funion B N?�2: Preparo informacion propia");
        }
        b2();
      }
      if (funtion_Mode=="B" && funtion_Number=="3"){
        if(flag_F_depurar){
          Serial.println("funion B N?�3:  info recibida ");
        }
        b3();
      }
      if (funtion_Mode=="B" && funtion_Number=="4"){
        if(flag_F_depurar){
          Serial.println("funion B N?�4");
        }
        b4();
      }
      if (funtion_Mode=="B" && funtion_Number=="5"){
        if(flag_F_depurar){
          Serial.println("funion B N?�5");
        }
        b5();
      }
      if (funtion_Mode=="B" && funtion_Number=="6"){
        if(flag_F_depurar){
          Serial.println("funion B N?�6");
        }
        b6();
      }
      if (funtion_Mode=="B" && funtion_Number=="7"){
        if(flag_F_depurar){
          Serial.println("funion B N?�7");
        }
      }
      if (funtion_Mode=="B" && funtion_Number=="8"){
        if(flag_F_depurar){
          Serial.println("funion B N?�8");
        }
        flag_F_nodoRequest=true;
        flag_F_PAQUETE=true;
      }     
      if (funtion_Mode=="B" && funtion_Number=="9"){
        if(flag_F_depurar){
          Serial.println("funion B N?�9");
        }
      }     
      if (funtion_Mode=="B" && funtion_Number=="0"){
        if(flag_F_depurar){
          Serial.println("funion B N?�0");
        }
        b0();
      }                            
    // Function tipo C.
      if (funtion_Mode=="C" && funtion_Number=="0"){
        incomingFuntion='0';
      }
      if(funtion_Mode=="C" && funtion_Number=="1"){
        Serial.println("Funcion C1");
        if(flag_F_depurar){
          Serial.println("Funcion C1 EJECUTANDOSE");
        }
        c1(x1);
      }
      if(funtion_Mode=="C" && funtion_Number=="2"){
        Serial.println("Funcion C2");
        if(flag_F_depurar){
          Serial.println("Funcion C2 EJECUTANDOSE");
        }
        c2(x1);
      }
      if (funtion_Mode=="C" && funtion_Number=="3"){
        // Serial.println("funion C N?�1: updateServer");
        // c1(x2);
        incomingFuntion='3';  // anterior,ente tenia '1' no se porque
      }
    // Function tipo M.
      // MAESTRO Envia a Un Nodo Para Ejecutar Una Funcion.
        if (funtion_Mode=="M" && funtion_Number=="1"){
          if(flag_F_depurar){
            Serial.println("funion M1");
          }
          function_Remote=inputString.substring(3);   // Extraemos la Funcion a Enviar de la Cadena.
          m1(x1,function_Remote);

        }
      // MAESTRO Inicia La Comunicacion.
        if (funtion_Mode=="M" && funtion_Number=="2"){
          if(flag_F_depurar){
          }
          codigo=inputString.substring(2);
          Serial.print("Function Code: ");
          Serial.println(codigo);
          flag_F_PAQUETE=true;
          flag_F_masteRequest=true;
        }
    // Function Tipo S: Funcion de Estado de Los Nodos.
        if (funtion_Mode=="S" && funtion_Number=="0"){
          if(flag_F_depurar){
            Serial.println("funion S N0");
          }
          // s1(x1);
        }
        if (funtion_Mode=="S" && funtion_Number=="1"){
          if(flag_F_depurar){
            Serial.println("funion S N1");
          }
          s1(incoming_nodo_info,x2);
        }
        // if (funtion_Mode=="S" && funtion_Number=="2"){
        //   if(flag_F_depurar){
        //     Serial.println("funion S N2");
        //   }
        //   s2(x1);
        // }
  }

//3. Funciones Seleccionadas para Ejecutar.
  //-3.1 Funciones tipo A.
    void a1_Nodo_Destellos (int repeticiones, int tiempo){
      // FUNCION PROBADA CORRECTAMENTE
      int veces=repeticiones;
      int retardo=tiempo*100;
      if(flag_F_depurar){
        Serial.println("Ejecutando F1.. \n");
      }
      for(int repetir=0; repetir<veces; ++repetir){
        delay(retardo);                  // pausa 1 seg.
        digitalWrite(LED_azul, HIGH);     // Led ON.
        delay(retardo);                  // pausa 1 seg.
        digitalWrite(LED_azul, LOW);    // Led OFF.
      }
    }
    void a2(){
      // FUNCION PROBADA CORRECTAMENTE
      codigo=inputString.substring(2);
      Serial.println("codigo:" + codigo);         // Pureba de Comunicacion Serial.
    }
    void a3_Nodo_Direccion_Local(int paramatro_1){
      if(flag_F_depurar){
        Serial.println("Ejecutando F3.. \n");
      }
      localAddress    = paramatro_1;
      Nodo_actual     = localAddress;
      Nodo_siguiente  = localAddress + 1;
      Nodo_anterior   = localAddress - 1;

      Zona_B          = localAddress + (localAddress - 1);
      Zona_A          = Zona_B - 1;
      // cycleTime      = localAddress * 20;
      if(localAddress==master){
        // temporizador_1.detach();
        // temporizador_2.detach();
        // temporizador_3.detach();
      }
      Serial.print("Address Local: ");
      Serial.println(localAddress);
    }
    void a4_Nodo_Direccion_Destino(int direccion_aux){
      Serial.println("Ejecutando F4.. \n");
      destination = direccion_aux;
      Serial.print("Drireccion Destino: ");
      Serial.println(destination);
      
    }
    void a5_Nodo_Modo_Continuo(int tipo_Modo){      
      int a=tipo_Modo;
      if(a==1){
        beforeTime_1 = millis();
        flag_F_modo_Continuo=true;
        temporizador_1.attach_ms(500, ISR_temporizador_1);
        flag_F_PAQUETE=true;
      }
      if(a==0){
        flag_F_modo_Continuo=false;
        flag_F_responder=false;
        temporizador_1.detach();
      }
    }
    void a6_Nodo_Numeros(int parametro_1){
      Nodos=parametro_1;  
    }
    void a7(int tipo_Depurar){
      
      int b=tipo_Depurar;

      if(b==1){
        flag_F_depurar=1;
      }
      if(b==0){
        flag_F_depurar=0;
      }
    }
    void a10(int a1, int a2){
      int aa=a1;
      int aa2=a2;
      bitSet(zonesLSB, incoming_sender);
      zonesMSB=0;
    }
  //-3.2 Funciones tipo B.
    // b0- Identifico quien Envia el Mensaje Byte
      void b0 (){
        // Informacion Acerca de los nodos que pude LEER.
        // Si el mensaje viene del Maestro, preparar el mesaje para flag_F_responder al Maestro
        destination=254;
                                  // Respondo a quien me escribe.
        // 2. Remitente.
        //localAddress=String(Nodo).toInt();            // Establecer direccion Local.
        // 3. Zonas Leidas 1.
        zonesMSB=Zonas_MSB_Estados;
        zonesLSB=Zonas_LSB_Estados;

        // 5. Longitud de Bytes de la Cadena incoming_function
          // Este byte lo escribe antes de Enviar el mensaje
        // 6. Este byte contiene Informacion del Nodo
        // nodoInfo=String(msgNumber, HEX);
        // 7. Byte Escrito desde recepcion Serial o Predefinido.
        // 7. Byte Escrito desde recepcion Serial o Predefinido.
        codigo="S1";
      }
    // b1- Respuesta Automatica todos los NODOS.
      void b1(){
        // 1. Destinatario.
        destination=0;                           // Respondo a quien me escribe.
        // 2. Remitente.
        //localAddress=String(Nodo).toInt();            // Establecer direccion Local.
        // 3. Zonas Leidas 1.
        zonesMSB=Zonas_MSB_Estados;
        zonesLSB=Zonas_LSB_Estados;
        // 4. Nodos Leidos.
        nodosLSB=nodos_LSB_MERGE;
        // 5. Longitud de Bytes de la Cadena incoming_function.
        // Este byte lo escribe antes de Enviar el mensaje.
        // 6. Este byte contiene Informacion del Nodo.
        // nodoInfo=String(nodoInfo, HEX);
        // 7. Byte Escrito desde recepcion Serial o Predefinido.
        // 7. Byte Escrito desde recepcion Serial o Predefinido.
        codigo="YA";
      }
    // b2- Maestro Reinicia La Comunicacion.
      void b2 (){
        // 1. Destinatario.
        destination=0;                           // Respondo a quien me escribe.
        // 2. Remitente.
        //localAddress=String(Nodo).toInt();            // Establecer direccion Local.
        
        // 4. Zonas Leidas 2.
        zonesMSB=Zonas_MSB_Estados;
        zonesLSB=Zonas_LSB_Estados;
        // 5. Nodos Leidos.
        nodosLSB=nodos_LSB_MERGE;
        // 5. Longitud de Bytes de la Cadena incoming_function.
        // Este byte lo escribe antes de Enviar el mensaje.
        // 6. Este byte contiene Informacion del Nodo.
        codigo=String("RST");
        // 7. Byte Escrito desde recepcion Serial o Predefinido.
      }
    // b3- Respuesta al Maestro b3, b4, b5
      void b3 (){
        // Informacion Acerca de los nodos que pude LEER.
        // Si el mensaje viene del Maestro, preparar el mesaje para flag_F_responder al Maestro
        destination=0XFF;                           // Respondo aL maestro.
        // 2. Remitente.
        //localAddress=String(Nodo).toInt();            // Establecer direccion Local.
        // 3. Zonas Leidas 1.
        zonesMSB=Zonas_MSB_Estados;
        // zonesLSB=Zonas_LSB_Estados;
        // 4. Nodos Leidas.
        nodosLSB=nodos_LSB_MERGE;
        // 5. Longitud de Bytes de la Cadena incoming_function
          // Este byte lo escribe antes de Enviar el mensaje
        // 6. Este byte contiene Informacion del Nodo
        // nodoInfo=String(msgNumber, HEX);
        // 7. Byte Escrito desde recepcion Serial o Predefinido.
        // 7. Byte Escrito desde recepcion Serial o Predefinido.
        codigo="S1";
        codigo+=nodo_Status;
      }
      void b4 (){
        // MASTER BROADCAST
        // Informacion Acerca de los nodos que pude LEER.
        // Si el mensaje viene del Maestro, preparar el mesaje para flag_F_responder al Maestro
        destination=master;                           // Respondo a quien me escribe.
        // 2. Remitente.
        //localAddress=String(Nodo).toInt();            // Establecer direccion Local.
        // 3. Zonas Leidas 1.
        zonesMSB=Zonas_MSB_Estados;
        zonesLSB=Zonas_LSB_Estados;
        // 4. Nodos Leidos.
        nodosLSB=nodos_LSB_MERGE;
        // 5. Longitud de Bytes de la Cadena incoming_function
          // Este byte lo escribe antes de Enviar el mensaje
        // 6. Este byte contiene Informacion del Nodo
        // nodoInfo=String(msgNumber, HEX);
        // 7. Byte Escrito desde recepcion Serial o Predefinido.
        // 7. Byte Escrito desde recepcion Serial o Predefinido.
        codigo="ReadyR";
      }
      void b5 (){
        // MASTER CODE 254
        // Informacion Acerca de los nodos que pude LEER.
        // Si el mensaje viene del Maestro, preparar el mesaje para flag_F_responder al Maestro
        destination=master;                         // Respondo a quien me escribe.
        // 2. Remitente.
        //localAddress=String(Nodo).toInt();            // Establecer direccion Local.
        // 3. Zonas Leidas 1.
        zonesMSB=Zonas_MSB_Estados;
        zonesLSB=Zonas_LSB_Estados;
        // 5. Nodos Leidos.
        nodosLSB=nodos_LSB_MERGE;
        // 5. Longitud de Bytes de la Cadena incoming_function
          // Este byte lo escribe antes de Enviar el mensaje
        // 6. Este byte contiene Informacion del Nodo
        // nodoInfo=String(msgNumber, HEX);
        // 7. Byte Escrito desde recepcion Serial o Predefinido.
        // 7. Byte Escrito desde recepcion Serial o Predefinido.
        codigo="OK";
      }
    // b6- Respueta Nodo Siguiente b6
      void b6 (){
        // Informacion Acerca de los nodos que pude LEER.
        // Si el mensaje viene del Maestro, preparar el mesaje para flag_F_responder al Maestro
        //1-
        destination=Nodo_siguiente;                           // Respondo a quien me escribe.
        // 2. Remitente.
        //localAddress=String(Nodo).toInt();            // Establecer direccion Local.
        // 3. Nodos Leidos 1.
        //-3- -4. Zonas Leidas.
        zonesMSB=Zonas_MSB_Estados;
        zonesLSB=Zonas_LSB_Estados;
        //-5. -6. Nodos Leidos.
        nodosLSB=nodos_LSB_MERGE;
        //-7. Nodo_Info.
        nodoInfo = Zonas_Fallan;
        codigo="S1" + Fuente_in_str;
      }
    // b7- MASTER > REQUEST NODE Comandos del Maestro.  
      void b7 (){
        // Informacion Acerca de los nodos que pude LEER.
        // Si el mensaje viene del Maestro, preparar el mesaje para flag_F_responder al Maestro
        destination=nodo_proximo;                           // Respondo a quien me escribe.
        // 2. Remitente.
        //localAddress=String(Nodo).toInt();            // Establecer direccion Local.
        // 3. Nodos Leidos 1.
        zonesMSB=Zonas_MSB_Estados;
        zonesLSB=Zonas_LSB_Estados;                           // ANTERIORMENTE incoming_zonesLSB;
        // 4. Zonas Leidas 2.
        nodosLSB=nodos_LSB_MERGE;       
        // 5. Longitud de Bytes de la Cadena incoming_function
          // Este byte lo escribe antes de Enviar el mensaje
        // 6. Este byte contiene Informacion del Nodo
        // nodoInfo=String(msgNumber, HEX);
        // 7. Byte Escrito desde recepcion Serial o Predefinido.
        // 7. Byte Escrito desde recepcion Serial o Predefinido.
        codigo=function_Remote;   //Question solicitd.
      }
      void b8 (int a1, int a2){
        int aa=a1;
        int aa2=a2;
      }
      void b9 (int a1, int a2){
        int aa=a1;
        int aa2=a2;
      }
  //-3.3 Funciones tipo C.
    void c1(int salida_RELE){
      int a = salida_RELE;
      if(a==1){
        digitalWrite(out_rele_1, HIGH);
        Serial.println(a);
      }
      if(a==0){
        digitalWrite(out_rele_1, LOW);
        Serial.println(a);
      }
      Serial.println("ejecutada");

    }
    void c2(int salida_RELE){
      int a = salida_RELE;
      if(a==1){
        digitalWrite(out_rele_2, HIGH);
        Serial.println(a);
      }
      if(a==0){
        digitalWrite(out_rele_2, LOW);
        Serial.println(a);
      }
      Serial.println("ejecutada");

    }
    void c3(int modo_updateServer){
      int a=modo_updateServer;
      if(a==1){
        flag_F_updateServer=true;
        temporizador_1.attach_ms(updateTime, ISR_temporizador_3);
      }
      if(a==0){
        flag_F_updateServer=false;
        temporizador_3.detach();
      }
    }
  //-3.4 Funciones tipo S.
    void s1(byte data_in_1, int data_in_2){
      byte Dato_Nuevo_1 = data_in_1;   // Estado del nodo
      int Dato_Nuevo_2 = data_in_2;   
      switch(incoming_sender){
        case 1:
          Node1.Update(Dato_Nuevo_1, Dato_Nuevo_2);
          break;
        case 2:
          Node2.Update(Dato_Nuevo_1, Dato_Nuevo_2);
          break;
        case 3:
          Node3.Update(Dato_Nuevo_1, Dato_Nuevo_2);
          break;
        case 4:
          Node4.Update(Dato_Nuevo_1, Dato_Nuevo_2);
          break;
        case 5:
          Node5.Update(Dato_Nuevo_1, Dato_Nuevo_2);
          break;
        case 6:
          Node6.Update(Dato_Nuevo_1, Dato_Nuevo_2);
          break;
        case 7:
          Node7.Update(Dato_Nuevo_1, Dato_Nuevo_2);
          break;
        case 8:
          Node8.Update(Dato_Nuevo_1, Dato_Nuevo_2);
          break;
        case 9:
          Node9.Update(Dato_Nuevo_1, Dato_Nuevo_2);
          break;
        default:
          break;
      }
    }
  //-3.5 Funciones Tipo M.
    void m1(int nodo_num_request, String request_kind){
      Serial.print("Function Code: ");
      Serial.println(request_kind);
      Nodo_destino=nodo_num_request;
      codigo=request_kind;
      flag_F_masterNodo=true;
      flag_F_PAQUETE=true;
    }
//4. Funciones UPDATE.
  //-4.1 Estados de Entradas.
    void reviso(){
      if(localAddress==255) return;
      // 1. Pulsadores A y B Lectura.
        Zona_A_ACK=digitalRead(PB_ZA_in);       // pulsador A. PB_ZA_in
        Zona_B_ACK=digitalRead(PB_ZB_in);       // pulsador B.
        Zona_AB_ACK=digitalRead(PB_ZC_in);      // pulsador C. Pulsador por defecto PRG.
      // 2. Zona A y Zona B Lectura.
        Zona_A_ax=digitalRead(Zona_A_in);
        Zona_B_ax=digitalRead(Zona_B_in);
      // 3. Bateria o Fuente Lectura.
        Fuente_in_ST=digitalRead(Fuente_in);

      // 4. ZONAS A y B RESET.
        // Pulsador C = reconocimiento de Ambas Zonas A y B. Reset de Ambas Zonas y Fallas.
        if(!Zona_AB_ACK){
          bitClear(Zonas, Zona_A);      // ZONA A Reset.
          bitClear(Zonas, Zona_B);      // ZONA B Reset.

          bitClear(Zonas_Fallan, Zona_A);     // ZONA A FALLA Reset.
          bitClear(Zonas_Fallan, Zona_B);     // ZONA B FALLA Reset.

          Zona_A_F_str='.';
          Zona_B_F_str='.';

          zona_1_err=false;
          zona_2_err=false;
        }
      // 5. ZONA A RESET= Zona A aceptada desde el pulsador activo en bajo "0"
        if(!Zona_A_ACK){
          bitClear(Zonas, Zona_A);
          bitClear(Zonas_Fallan, Zona_A);
          zona_1_err=false;
          Zona_A_F_str='.';
          Zona_A_ST=false;
        }
      // 6. ZONA B RESET= Zona B aceptada desde el pulsador activo en bajo "0"
        if(!Zona_B_ACK){
          bitClear(Zonas, Zona_B);
          bitClear(Zonas_Fallan, Zona_B);
          zona_2_err=false;
          Zona_B_F_str='.';        
          Zona_B_ST=false;
        }

      // 7. ZONA A ACTIVA.
        if(!Zona_A_ax){
          bitSet(Zonas, Zona_A);
          Zona_A_ST=true;
        }
      // 8. ZONA B ACTIVA.
          if(!Zona_B_ax){
            bitSet(Zonas, Zona_B);
            Zona_B_ST=true;
          }
      // 10 ZONAS para mostrar en Pantalla  OLED
        Zona_A_str=String(Zona_A_ST, BIN);
        Zona_B_str=String(Zona_B_ST, BIN);

        Zona_A_ACK_str=String(!Zona_A_ACK, BIN);
        Zona_B_ACK_str=String(!Zona_B_ACK, BIN);

        Fuente_in_str=String(Fuente_in_ST, BIN);
    }
  //-4.2 Secuencia.
    void secuencia(){
      //1. _____________Modo NODE_______________________________
        //1.1 Modo Nodo >> Nodo Siguiente Responde
          if(MODE==INDEPENDIENTE){
            if(incoming_recipient==localAddress   && incoming_sender==Nodo_anterior){
              b6();
              temporizador_2.once_ms(tokenTime, ISR_temporizador_2);
              temporizador_1.attach_ms(cycleTime, ISR_temporizador_1); // CADA VEZ QUE ME LLEGA UN MENSAJE DEL NODO ANTERIOR CONFIGURO EL CYCLE TIME PARA ESTAR SINCRONIZADO
              beforeTime_2 = millis();  // despurar.
              beforeTime_1 = millis();  // despurar.
              flag_F_T2_run=true;
              flag_F_T1_run=true;
              flag_F_Nodo_Iniciado=true;
              Nodo_waiting=false;
            }
          }
        //1.2 Modo Nodo >> MASTER Broadcast.
          if(incoming_recipient==0              && incoming_sender==master){
            // b6();
            // flag_F_Nodo_Iniciado=true;
            // temporizador_2.once_ms(firstTime, ISR_temporizador_2);
            // temporizador_1.attach_ms(cycleTime, ISR_temporizador_1);
            // beforeTime_2 = millis();  // despurar.
            // beforeTime_1 = millis();  // despurar.
            // flag_F_T2_run=true;
            // flag_F_T1_run=true;
          }
        //1.3 Modo Nodo >> Funcion si el master quiere saber: a quien puede escuchar.
          if(incoming_recipient==254            && incoming_sender==master && flag_F_Nodo_Iniciado==false){
            // b6();
            // temporizador_2.once_ms(wakeUpTime,ISR_temporizador_2);
            // temporizador_1.attach_ms(cycleTime, ISR_temporizador_1);
          }
   
        //1.5 Modo Nodo >> Master.
          if(incoming_recipient==localAddress   && incoming_sender==master){
            b3();
            temporizador_2.once_ms(fastTime, ISR_temporizador_2);
            flag_F_T2_run=true;
          }
        //1.6 Modo NODO >> PRINCIPAL.
          if(localAddress==Nodo_actual          && flag_F_nodoRequest){
            // b6();
            // beforeTime_2 = millis();  // despurar.
            // temporizador_2.once_ms(tokenTime, ISR_temporizador_2);
            // flag_F_T2_run=true;
          } 
        //1.7 Modo NODO >> BROADCAST CONTINUO (Prueba).
          if(flag_F_modo_Continuo               && flag_ISR_temporizador_1){
              b3();
          }
        //1.8 Modo NODO Inicio Automatico
          if(Nodo_waiting && !flag_F_Nodo_Iniciado && localAddress < master && MODE==INDEPENDIENTE){
            temporizador_1.attach(waitTime, ISR_temporizador_1);
            flag_F_T1_run=true;
            flag_F_Nodo_Iniciado=true;
          }
      //2. _____________Modo MASTER__________________________
        //-2.1 Modo MASTER Principal (INICA LA TRANSMISION) Ejecutado desde M2.
          if(localAddress==master             && flag_F_masteRequest){
            b1();   //destination=0
            // beforeTime_2 = millis();  // despurar.
            // temporizador_0.attach_ms(masterTime, ISR_temporizador_0);
            temporizador_2.once_ms(tokenTime, ISR_temporizador_2);
            temporizador_1.attach_ms(cycleTime, ISR_temporizador_1);
            flag_F_T2_run=true;
            flag_F_T1_run=true;
          }
        //-2.2 Modo MASTER Nodo (Solicitud de Funcion) Ejecutado desde M1.
          if(localAddress==master             && flag_F_masterNodo){
            b7();
            temporizador_2.once_ms(fastTime, ISR_temporizador_2);
            flag_F_T2_run=true;
          }
        //-2.3 Modo MASTER Recepciona Mensaje del NODO.
          if(incoming_recipient==master             && incoming_sender==destination){
            temporizador_2.once_ms(tokenTime, ISR_temporizador_2);
            temporizador_1.attach_ms(masterTime, ISR_temporizador_1); // CADA VEZ QUE ME LLEGA UN MENSAJE DEL NODO ANTERIOR CONFIGURO EL CYCLE TIME PARA ESTAR SINCRONIZADO
            flag_F_Master_Esperando=false;       // DESPUES QUE EL MAESTRO RECIBE EL MENSAJE DEL NODO ACTUALIYA LA BANDERA mMASTER ESPERA A FALSE
            // flag_F_T2_run=true;
          }

    }
  //-4.3 Sever Update.  
    void serverUpdate(){
      flag_F_updateServer=false;
      switch(incoming_sender){
        case 1:
          Node1.Estado();
          Serial.println(incoming_nodo_info, BIN);
          break;
        case 2:
          Node2.Estado();
          Serial.println(incoming_nodo_info, BIN);
          break;
        case 3:
          Node3.Estado();
          Serial.println(incoming_nodo_info, BIN);
          break;
        case 4:
          Node4.Estado();
          Serial.println(incoming_nodo_info, BIN);
          break;
        case 5:
          Node5.Estado();
          break;
        case 6:
          Node6.Estado();

          break;
        case 7:
          Node8.Estado();        
          break;
        case 8:
          Node8.Estado();
          break;
      }
    }
  //-4.4 Actualizar.  
    void actualizar(){
      //0. Procedimiento
        //1. Estado de Zonas Normalmente en Zero 0 y Activas en Uno 1
        //2. Borrar estados actuales de entradas ?? Zonas Locales.
        //3. Conservar estados de Entradas de los demas Nodos Aplicando Una and.
        //4. Actualizo estados Propios en el mensaje de Salida con una OR.
      
      //1. ESTADOS DE ZONAS.
        //-1.1 ZONAS LOCALES.
          Zonas_MSB=highByte(Zonas);
          Zonas_LSB=lowByte(Zonas);

        //-1.2 Aplico la Mascara. Mascara: Todos los bits en 1 Uno y Las Zonas locales en Cero 0.
          Zonas_LSB_Estados=Zonas_LSB_Mascara & incoming_zonesLSB;
          Zonas_MSB_Estados=Zonas_MSB_Mascara & incoming_zonesMSB;

        //-1.3 INTEGRO ZONAS LOCALES CON ZONAS ENTRANTES.
          Zonas_LSB_Estados |= Zonas_LSB;
          Zonas_MSB_Estados |= Zonas_MSB;

        //-1.4 PREPARAMOS LA VARIABLE A SER ENVIADA.
          zonesLSB=Zonas_LSB_Estados;
          zonesMSB=Zonas_MSB_Estados;

        //-1.5 Varibles para Pantalla OLED.
          Zonas_LSB_str=256;        // CON ESTA VARIABLE ACTIVO EL BIT 9 PARA PODER MOSTRAR LAS ZONAS ACTIVAS
          Zonas_LSB_str |=zonesLSB;
      //2. Estados de Entradas.
        if(localAddress<255){
          bitWrite(nodo_local,1, Zona_A_ST);
          bitWrite(nodo_local,2, zona_1_err);
          bitWrite(nodo_local,3, Zona_B_ST);
          bitWrite(nodo_local,4, zona_2_err);
          bitWrite(nodo_local,5, Fuente_in_ST);
          bitWrite(nodo_local,6, timer_nodo_ST);
          nodo_Status=String(nodo_local,HEX);
        }
      //3. ESTADOS DE NODOS. 
        if(flag_F_PAQUETE){
        //-2.1 bit MAESTRO LEIDO Si el mensaje es del Maestro, Activar el Bit 0.
          if(incoming_sender==master || localAddress==master){
            bitSet(Nodos_LSB_ACK, 0);
          }
        //-2.2 bit NODOS LEIDOS Si el mensaje es de Cualquier Nodo, Activar el Bit Correspondiente.
          if(incoming_sender<master){
            if(incoming_sender>0){    // Si el Mensaje es para Todos el receptor es para todos=0
              bitSet(Nodos_LSB_ACK, incoming_sender);
            }
          }
        //-2.3 bit NODO LOCAL Activar propiamente el bit Local, segun la direccion local.  
          if(localAddress<255){
            bitSet(Nodos_LSB_ACK, localAddress);
          }

        //-2.4 Nodos Para Mostrar en OLED.
          Nodos_LSB_str = 256;
          Nodos_LSB_str |=Nodos_LSB_ACK;
        //-2.5 Sumatoria de Nodos Entrantes.
          nodos_LSB_MERGE = Nodos_LSB_ACK;
          nodos_LSB_MERGE |= incoming_nodosLSB;
        //-2.6 Start Nodo waiting. funciona solo para los nodos.
          if(!flag_F_Nodo_Iniciado){
            Nodo_waiting=true;
            waitTime=(Nodos+(Nodo_actual-incoming_sender))*baseTime;
          }
        //-2.7 FLAG Nodo Anterior.
          if(incoming_sender==Nodo_anterior){
            flag_F_nodo_Anterior=true;
          }
        }
      //4. Zonas en Falla.
        Zonas_Fallan_LSB=lowByte(Zonas_Fallan);
        Zonas_Fallan_MSB=highByte(Zonas_Fallan);
        
        // Aplico la Mascara.
        ZonasF_LSB_Estados=Zonas_LSB_Mascara & incoming_zonaFLSB;
        ZonasF_MSB_Estados=Zonas_MSB_Mascara & incoming_zonaFMSB;

        // Integro las zonas fallas Locales.

        ZonasF_LSB_Estados |=Zonas_Fallan_LSB;
        ZonasF_MSB_Estados |=Zonas_Fallan_MSB;

        // Preparamos los datos a ser enviados.
        zonaFLSB = ZonasF_LSB_Estados;
        zonaFMSB = ZonasF_MSB_Estados;

        // depurar en OLED.
        // Se suma 256 para Mostrar los estados de todas las zonas
        ZonasF_LSB_str = 256;
        ZonasF_LSB_str |= zonaFLSB;


      //5. Pantalla. (Cada espacio equivale a 8)
        // Tiempo transcurrido para Timer 1
          if(flag_F_T1_run){
            currentTime_1=millis();
            elapseTime_1=currentTime_1-beforeTime_1;
          }
        // Tiempo Transcurrido para Timer 2.  
          if(flag_F_T2_run){
            currentTime_2=millis();
            elapseTime_2=currentTime_2-beforeTime_2;
          }
         // Tiempo Transcurrido ENTRE MENSAJES RECIBIDOS GAP DEL MASTER.  
          currentTime_GAP=millis();
          
        // LIMPIO PANTALLA
          Heltec.display->clear();
        //0 NODO INFORMACION
          Heltec.display->drawString(0, 0, Nodo_Name);
          Heltec.display->drawString(50, 0, String(localAddress, DEC));
          Heltec.display->drawString(70, 0, "RX:");
          Heltec.display->drawString(87, 0, String(incoming_sender, DEC));
          // CODIGO DE MENSAJE
          Heltec.display->drawString(107, 0, incoming_function);
        //1 ZONAS MENSAJE ENTRANTE
          Heltec.display->drawString(0, 10, "ZONAS:");
          Heltec.display->drawString(50, 10, "87654321");
          Heltec.display->drawString(100, 10, String(nodoCount, DEC));
          Heltec.display->drawString(108, 10, String(Fuente_in_str));     // Bit uqe nos indica el estado de la fuente si es on ò off.
        //2  
          Heltec.display->drawString(0, 20, "SUMA:");
          Heltec.display->drawString(45, 20, "#");
          Heltec.display->drawString(45, 20, String(Zonas_LSB_str, BIN));
          Heltec.display->drawString(108, 20, String(Node1.GetAckNum(), DEC));
        //3 ZONA LOCAL TIMER 1 Y 2 TIEMPO TRANSCURRIDO
          // Heltec.display->drawString(0, 30, "LOCAL:");
          // Heltec.display->drawString(50,30, String(Zonas_LSB, BIN));

          Heltec.display->drawString(0, 30, "T1:");
          Heltec.display->drawString(17,30, String(elapseTime_1, DEC)); 
          Heltec.display->drawString(45,30, "T2:");
          Heltec.display->drawString(62,30, String(elapseTime_2, DEC));          
          
          //MASTER: Tiempo Transcurrido para el Master entre mensajes recibidos.    
            Heltec.display->drawString(85, 30, "TG:");
            Heltec.display->drawString(100, 30, String(elapseTime_GAP, DEC));

          // NODOS COMPLETOS
          // Heltec.display->drawString(75,30, String("Nds: "));
          // Heltec.display->drawString(99,30, String(flag_F_Nodos_Completos, DEC));
        
        //4 Nodos Leidos.
          // Heltec.display->drawString(0, 40, "NODOS:");
          // Heltec.display->drawString(45,40, String(Nodos_LSB_str, BIN));
          // Heltec.display->drawString(45,40, "#");
          
          
          Heltec.display->drawString(0, 40, String(ZonasF_LSB_Estados, BIN));
          Heltec.display->drawString(70, 40, String(ZonasF_LSB_str, BIN));
          Heltec.display->drawString(70, 40, "#");
          
          
          
        //5
          // ZONA A
            if(localAddress<255){
              Heltec.display->drawString(0, 50, "ZA:");
              Heltec.display->drawString(16,50, Zona_A_str);
              // Error de Zona A.
              if(!zona_1_err){
                Heltec.display->drawString(23,50, String(zona_1, DEC));
              }
              else{
                Heltec.display->drawString(23, 50, String(Zona_A_F_str));
              }
          // ZONA B
              Heltec.display->drawString(35, 50, "ZB:");
              Heltec.display->drawString(53, 50, Zona_B_str);
              // Error de Zona B.
              if(!zona_2_err){
                Heltec.display->drawString(61,50, String(zona_2, DEC));
              }
              else{
                Heltec.display->drawString(61,50, String(Zona_B_F_str));
              }
          // PULSADOR A
              Heltec.display->drawString(75, 50, "PA:");
              Heltec.display->drawString(93, 50, Zona_A_ACK_str);
          // PULSADOR B
              Heltec.display->drawString(102, 50, "PB:");
              Heltec.display->drawString(119, 50, Zona_B_ACK_str);
            }
            else{
              // Nodos reconocidos Propiamente.
              Heltec.display->drawString(0,50, String(Nodos_LSB_str, BIN));
              Heltec.display->drawString(70,50, String(nodos_LSB_MERGE, BIN));
              Heltec.display->drawString(0, 50, "#");
          }
        // MOSTAR
          Heltec.display->display();
      //DEPURAR
          
    }
  //-4.5 Analizar.
    void analizar(){
      //1 Nodos Leidos
        if(Nodos_LSB_ACK>=Nodos_leidos){
          flag_F_Nodos_Completos=true;
        }
      //2 Nodo Ultimo..
        if(incoming_sender==Nodos){
          flag_F_Nodo_Ultimo=true;
        }
      //3 RESETEO DE NODOS LEIDOS.  
        Nodos_LSB_ACK=0;
      //4 FALLA CONSTANTE.
        //-4.1 FALLA ZONA_A
          if(!Zona_A_ax && !zona_1_err){
            ++ zona_1;
            if(zona_1==Zonas_Falla_Time){
              zona_1_err=true;
              zona_1=0;
            }
          }
          if(zona_1_err==true){
            bitSet(Zonas_Fallan, Zona_A);
            Zona_A_F_str='X';
          }
        //-4.2 FALLA ZONA_B.
          if(!Zona_B_ax && !zona_2_err){
             ++ zona_2;
             if(zona_2==Zonas_Falla_Time){
               zona_2=0;
               zona_2_err=true;
               Serial.println(zona_2_err);
             }
          }
          if(zona_2_err==true){
            bitSet(Zonas_Fallan, Zona_B);
            Zona_B_F_str='X';
          }
        //-4.3 Reset contador
          // si el contador no llega al set point y se reestableció la zona
          if(Zona_A_ax && zona_1>0) zona_1=0;
          if(Zona_B_ax && zona_2>0) zona_2=0;
      //5 Flag clear Timer 1 and 2.
        // flag_ISR_temporizador_1=false;    // Cuando el master reconoce todos los Nodos el Flag del T1 no se resetea en RFM95 ENVIAR, SINO AQUI.
      //6 Mensajes recibidos
        // Node1.GetAckNum();
    }
  //-4.6 PROXIMO NODO.
    void Nodo_REQUEST(){
      if(nodo_proximo==Nodo_ultimo){
        nodo_proximo=Nodo_primero-1;
      }
      if(nodo_proximo<=Nodo_ultimo) {
        ++ nodo_proximo;
      }
    }
//5. Funciones de Dispositivos Externos.
  //-5.1 RFM95 RECIBIR.
    void RFM95_recibir(int packetSize){
      if (packetSize == 0) return;        // if there's no packet, return?�1
      // read packet header bytes:
      incoming_recipient = LoRa.read();    // incoming_recipient address
      incoming_sender    = LoRa.read();    // incoming_sender address
      // incoming_zonesLSB  = LoRa.read();    // incoming_function msg ID.
      // incoming_zonesMSB  = LoRa.read();    // incoming_function msg ID.
      // incoming_nodosLSB  = LoRa.read();    // incoming_function Nodos Reportados LSB.
      // incoming_nodosMSB  = LoRa.read();    // incoming_function Nodos Reportados MSB.
      // incoming_zonaFLSB  = LoRa.read();    // incoming_function Informacion del Nodo Local.
      // incoming_zonaFMSB  = LoRa.read();    // incoming_function Informacion del Nodo Local.
      incoming_nodo_info = LoRa.read();
      incoming_length    = LoRa.read();    // incoming_function msg length
      incoming_function  = "";
      while (LoRa.available()){
        incoming_function += (char)LoRa.read();
      }
      if (incoming_length != incoming_function.length()) {   // check length for error
        Serial.println("error: message length does not match length");
        return;                             // skip rest of function
      }
      elapseTime_GAP=currentTime_GAP-beforeTime_GAP;
      beforeTime_GAP=millis();
      

      // if the incoming_recipient isn't this device or broadcast,
      if (incoming_recipient != localAddress && incoming_recipient > 0 && incoming_sender == 0xFF) {
        Serial.println("Sent to: 0x" + String(incoming_recipient, HEX));
        if(flag_F_depurar){
          Serial.println("This message is not for me.");
        }
        return;                             // skip rest of function
      }
      // if message is for this device, or broadcast, print details:
      if(flag_F_depurar){
        Serial.println("Received from: 0x" + String(incoming_sender, HEX));
        Serial.println("Sent to: 0x" + String(incoming_recipient, HEX));
        Serial.println("Message ID1: " + String(incoming_zonesLSB));
        Serial.println("Message ID2: " + String(incoming_zonesMSB));
        Serial.println("Message length: " + String(incoming_length));
        Serial.println("Message: " + incoming_function);
        Serial.println("RSSI: " + String(LoRa.packetRssi()));
        Serial.println("Snr: " + String(LoRa.packetSnr()));
        Serial.println();
      }
      
      inputString=incoming_function;
      falg_ISR_stringComplete=true;
      flag_F_PAQUETE=true;
      
      if(inputString.endsWith("R")){
        flag_F_responder=false;
      }
      if(localAddress==255){
        flag_F_updateServer=true;
      }
    }
  //-5.2 RFM95 ENVIAR.
    void RFM95_enviar(String outgoing){
      LoRa.beginPacket();             // start packet
      LoRa.write(destination);        // add destination address
      LoRa.write(localAddress);       // add incoming_sender address
      // LoRa.write(zonesLSB);           // add message ID
      // LoRa.write(zonesMSB);           // add message ID
      // LoRa.write(nodosLSB);
      // LoRa.write(nodosMSB);
      // LoRa.write(zonaFLSB);
      // LoRa.write(zonaFMSB);
      
      LoRa.write(nodo_local);
      LoRa.write(outgoing.length());  // add payload length
      LoRa.print(outgoing);           // add payload
      LoRa.endPacket();               // finish packet and send it
      
      flag_F_responder=false;
      
      // FLAG ACKNOWLEDGE
      flag_ISR_temporizador_0=false;
      flag_ISR_temporizador_1=false;
      flag_ISR_temporizador_2=false;  // se habilita en el ISR del temporiador 2
      flag_ISR_temporizador_3=false;
      
      flag_F_masterIniciado=true;
      flag_F_masterNodo=false;
      flag_F_masteRequest=false;
      flag_F_nodoRequest=false;
      flag_F_nodo_Anterior=false;

      flag_F_Master_Esperando=true;      // Despues que el maestro envia un mensaje que esperadno a que el Nodo Responda.

      flag_F_tokenTime=false;
      flag_F_cycleTime=false;
      Nodo_waiting=false;             // El Nodo ya se inici??.
      // eventos
        // flag_F_Nodo_Iniciado=true; //
        flag_F_respondido=true;
      
      incoming_sender=0;
      incoming_recipient=0;
      //NODO+-+-+-+-
      //Borrar datos leidos porcada ciclo de lectura de todos los nodos.
      if(localAddress<255){
        Nodos_LSB_ACK=0;
        nodos_LSB_MERGE=0;
        nodo_Status="";
        Serial.println(nodo_local, BIN);
        Serial.println(codigo);
        
      }

      // DEBUG
      if(flag_F_depurar){
        Serial.println(".");
        Serial.println("Sent to: 0x" + String(destination, HEX));
      }
    }


//https://resource.heltec.cn/download/package_heltec_esp32_index.json