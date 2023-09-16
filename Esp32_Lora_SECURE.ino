//1. librerias.
  //- 1.1 Librerias****
    #include <Ticker.h>
    #include <SPI.h>
    #include <heltec.h>
    // #include "images.h"
    
//2. Definicion de Pinout.
  //  Las Etiquetas para los pinout son los que comienzan con GPIO
  //  Es decir, si queremos activar la salida 1, tenemos que buscar la referencia GPIO 1, Pero solomante Escribir 1 sin "GPIO"
  //  NO tomar como referencia las etiquetas D1, D2,D3, ....Dx.
  
  //-2.1 Definicion de etiquetas para las Entradas.
    #define Zona_A_in     32        // Entrada de Zona 1
    #define Zona_B_in     33        // Entrada de Zona 2
    #define Entrada_X1_in 9         // Entrada Digital.
  
    #define PB_zonas_in   0         // Entrada de Pulsador PB_zonas_in.
    #define PB_ZA_in      38        //
    #define PB_ZB_in      39        // 

  //-2.2 Definicion de etiquetas para las Salidas.
    #define LED_azul      2

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
    #define RFM95_FREQ 915E6  
  //-2.5 timer

//3. Variables Globales.
  //-3.1 Variables Interrupciones
    volatile bool flag_ISR_prueba=false;             // Flag: prueba para interrupcion serial.
    volatile bool falg_ISR_stringComplete=false;    // Flag: mensaje Serial Recibido completo.
    volatile bool flag_ISR_temporizador_1=false;
    volatile bool flag_ISR_temporizador_2=false;
    volatile bool flag_ISR_temporizador_3=false;        // pra actualizar los dato al servidor.
    volatile bool flag_ISR_temporizador_0=false;
  
    String        inputString;           // Buffer recepcion Serial.
    String        funtion_Mode;          // Tipo de funcion para ejecutar.
    String        funtion_Number;        // Numero de funcion a EJECUTAR.
    String        funtion_Parmeter1;     // Parametro 1 de la Funcion.
    String        funtion_Parmeter2;     // Parametro 2 de la Funcion.
    String        funtion_Parmeter3;     // Parametro 3 de la Funcion.
    String        funtion_Parmeter4;      // Parametro para las Funciones remotas.
    String        function_Remote;
    String        function_Enable;
    volatile int  x1=0;
    volatile int  x2=0;
    volatile int  x3=0;
    volatile int  x4=0;
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
    bool          flag_F_masterIniciado=false;
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
  
  //-3.3 Variables NODOS y ZONAS.
      int         Nodos = 3;           // Establece Cuantos Nodos Confirman La Red a6.
      byte        master=0xFF;
      byte        Nodo_siguiente=0;    // Direccion del Nodo que sigue para enviar mensaje
      byte        Nodo_anterior;
      byte        Nodo_actual=0;
      int         Nodo_ultimo;
      byte        Nodo_primero=1;
      byte        Nodo_destino;
      word        Nodos_LSB_ACK;
      int         Nodos_LSB_str;
      
      String      Nodo_Name;
      byte        nodos_LSB_MERGE;           // variable para igualar el dato de los nodos reportados
      byte        nodos_MSB_MERGE;

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
      bool        Zona_A_ST;
      bool        Zona_B_ST;
      String      Zona_A_str;
      String      Zona_B_str;
      bool        Zona_A_Aceptada;
      bool        Zona_B_Aceptada;
      bool        Zonas_Aceptadas;
      String      Zona_A_PB_str;
      String      Zona_B_PB_str;

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

      long        baseTime   = 1000;
      long        cycleTime = 1000;
      long        tokenTime  = 2000;
      long        updateTime = 2000;
      long        masterTime = 10000;
      float       wakeUpTime = 90.0;
      long        firstTime;
      long        tokenLast;
      long        waitTime   = 0;
      uint32_t    remainT1;
      uint32_t    remainT2;
      int         fastTime    =   1;
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
      byte        incoming_nodoInfo;
      byte        incoming_length;       // incoming_function msg length
      String      incoming_function = "";
      

    // Variable para Enviar.
      byte        destination   = 0x01; // destination to send to  0xFF;         a4      
      byte        localAddress  = 0x01; // address of this device           a3
      byte        nodoInfo;             // informacion particular que envia el nodo
      byte        zonesLSB;
      byte        zonesMSB;
      byte        nodosLSB;
      byte        nodosMSB;
      String      outgoing;             // outgoing message
      byte        msgNumber;            // en modo continuo este numero incrementa automaticamente.          // interval between sends.      
    
    // Otras.  
      String      codigo="";
      String      info_1="";

//4. Intancias.
  //-4.1 Temporizadores.
    Ticker temporizador_1;                // Tiempo de respuesta
    Ticker temporizador_2;                // Tiempo token.
    Ticker temporizador_3;                // Tiempo update Server.
    Ticker temporizador_0;
    // Ticker temporizador_4;                // Tiempo de respuesta de todas las placas.
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
    ICACHE_RAM_ATTR void ISR_0(){
      flag_ISR_prueba=true;
      Zonas=0;

    }
    ICACHE_RAM_ATTR void ISR_1(){
    }
    ICACHE_RAM_ATTR void ISR_2(){
    }
    ICACHE_RAM_ATTR void ISR_3(){
      bitClear(Zonas, Zona_A);
    }
    ICACHE_RAM_ATTR void ISR_4(){
      bitClear(Zonas, Zona_B);
    }
  //-5.3 Interrupciones por Timers.
    void ISR_temporizador_0(){
      // flag_ISR_temporizador_0=true;
      // if(flag_F_Nodos_Incompletos){
      //   analisar();
      // }
    }
    void ISR_temporizador_1(){
        beforeTime_1 = millis();
        if(flag_F_token){
          flag_F_token=false;
          return;
        }
        flag_ISR_temporizador_1=true;

    }
    void ISR_temporizador_2(){
      currentTime_2 = millis();
      flag_ISR_temporizador_2=true;
      flag_F_token=true;
      elapseTime_2=0;
      flag_F_T2_run=false;
    }
    void ISR_temporizador_3(){
      if(flag_F_Nodo_Iniciado) return;
      flag_ISR_temporizador_3=true;
      b6();
    }
void setup(){
  //1. Configuracion de Puertos.
    //1.1 Configuracion de Salidas:
    //1.2 Configuracion de Entradas
      pinMode(Zona_A_in, INPUT_PULLUP);
      pinMode(Zona_B_in, INPUT_PULLUP);
      pinMode(PB_ZA_in, INPUT);
      pinMode(PB_ZB_in, INPUT);
  //2. Condiciones Iniciales.
    //-2.1 Estado de Salidas.
    //-2.2 Valores y Espacios de Variables.
      Nodo_primero    = 1;
      Nodo_ultimo     = Nodos;
      Nodo_actual     = localAddress;
      Nodo_siguiente  = localAddress + 1;
      Nodo_anterior   = localAddress - 1;
      Zona_B          = localAddress + (localAddress - 1);
      Zona_A          = Zona_B - 1;
      incoming_zonesLSB  = 0x00;
      incoming_zonesMSB  = 0x00;
      
      if(localAddress==255){
        Nodo_Name="MASTER:";
      }
      else{
        Nodo_Name="NODO:";
      }
    //-2.3 Timer Answer.
      
      tokenTime       = 1000;
      baseTime        = 1000;
      updateTime      = 1000;
      wakeUpTime      = tokenTime*localAddress;
      cycleTime       = tokenTime*(Nodos+1);
      masterTime      = cycleTime*2;
      firstTime       = tokenTime*localAddress;     // El primer mensaje esta calculado en tiempo forma para cada nodo.
      wakeUpTime      = 30.0;
      if(localAddress==Nodo_primero){
        tokenTime =1000;
        Nodo_anterior=Nodo_ultimo;
      }
      if(localAddress==Nodo_ultimo){
        tokenTime =1000;
        flag_F_Nodo_Ultimo=true;
        Nodo_siguiente=Nodo_primero;
      }
      // Timer 3 Responde despues de Reiniciar sin Recibir respuesta.
      if(localAddress<255){
        temporizador_3.once(wakeUpTime, ISR_temporizador_3);
      }
      
    //-2.4 Mascara de Zonas.
      Zonas_Mascaras=65535;
      Zonas=0;
      bitClear(Zonas_Mascaras, Zona_A);
      bitClear(Zonas_Mascaras, Zona_B);
      Zonas_LSB_Mascara=lowByte(Zonas_Mascaras);
      Zonas_MSB_Mascara=highByte(Zonas_Mascaras);
  //3. Configuracion de Perifericos:
    //-3.1 Comunicacion Serial:
      Serial.begin(9600);
      delay(10);
    
    //-3.2 Interrupciones Habilitadas.
      //****************************
      // attachInterrupt (digitalPinToInterrupt (PB_zonas_in), ISR_0, FALLING);  // attach interrupt handler for D2
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
      delay(1000);

}
void loop(){
  //1. Bienvenida Funcion
    while (flag_F_inicio){
      welcome();        // Comprobamos el Sistema minimo de Funcionamiento.
      led_Monitor(3);
      if(localAddress==255){
        temporizador_1.attach_ms(masterTime, ISR_temporizador_1);
        beforeTime_1=millis();
        flag_F_T1_run=true;
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
        //MASTER
          // if(localAddress==255 && flag_F_Nodos_Completos==false){
          //   if(!flag_F_masterIniciado){
          //     b1();
          //   }
          //   else{
          //     b2();
          //   }
          //   flag_F_responder=true;
          //   flag_F_PAQUETE=false;
          //   codigo="TC";
          // }
        //NODOS.
        if(localAddress<255 && !flag_F_nodo_Anterior){
          b6();
          flag_F_responder=true;
          flag_F_PAQUETE=false;
          codigo="TC";
        }
      }
    //-4.3 F- Timer 2.
      if(flag_ISR_temporizador_2){
        beforeTime_2=0;

        if(flag_F_depurar){
          Serial.print("ET2: ");
          Serial.println(elapseTime_2);
          Serial.print("CT2: ");
          Serial.println(currentTime_2);
          Serial.print("BT2: ");
          Serial.println(beforeTime_2);
          beforeTime_2 = currentTime_2;
        }     
        flag_F_tokenTime=true;
        flag_F_responder=true;
        codigo="TK";
      }
    //-4.4 F- Timer 0.
      if(flag_ISR_temporizador_0){
        // analizar();
        // flag_F_responder=true;
      }
    //-4.5 F- Timer 3.
      if(flag_ISR_temporizador_3){
        flag_F_responder=true;
      }

    //-4.6 F- Server Update.
      if(flag_F_updateServer){
        serverUpdate();
      }
    //-4.7 F- Recepcion de Paquete.
      if(flag_F_PAQUETE){
        flag_F_PAQUETE=false;
        secuencia();
      }

  //5. RFM95 Funciones.
    //-5.1 RFM95 RESPONDER Si?
      if(flag_F_responder){
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
    delay(500);
  }
  void led_Monitor(int repeticiones){
    // Deshabilitamos Banderas
    int repetir=repeticiones;
    for (int encender=0; encender<repetir; ++encender){
      digitalWrite(LED_azul, LOW);   // Led ON.
      delay(500);                    // pausa 1 seg.
      digitalWrite(LED_azul, HIGH);    // Led OFF.
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
          Serial.println("funion A Nº001");
        }  
        a1_Nodo_Destellos(x1,x2);
      }
      if (funtion_Mode=="A" && funtion_Number=="2"){
        if(flag_F_depurar){
          Serial.println("funion A Nº2");
        }
        a2();
      }
      if (funtion_Mode=="A" && funtion_Number=="3"){
        // FUNCIONO A MEDIAS SOLO DIRECIONES BAJAS Y 255 falta acomodar un poco mas
        if(flag_F_depurar){
          Serial.println("funion A Nº3");
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
          Serial.println("funion A Nº4");
        }
        String Nodo_destino_aux = "";
        Nodo_destino_aux = funtion_Parmeter1+funtion_Parmeter2+funtion_Parmeter3;
        int Nodo_destino = Nodo_destino_aux.toInt();
        a4_Nodo_Direccion_Destino(Nodo_destino);
      }
      if (funtion_Mode=="A" && funtion_Number=="5"){
        if(flag_F_depurar){
          Serial.println("funion A Nº5: Modo Continuo");
          a5_Nodo_Modo_Continuo(x1);
        }
      }
      if (funtion_Mode=="A" && funtion_Number=="6"){
        if(flag_F_depurar){
          Serial.println("funion A Nº6: Numero de Nodos");
        }
        String Nodos_numeros_aux = funtion_Parmeter1+funtion_Parmeter2;
        int Nodos_numeros = Nodos_numeros_aux.toInt();
        a6_Nodo_Numeros(Nodos_numeros);
      }
      if (funtion_Mode=="A" && funtion_Number=="7"){
        if(flag_F_depurar){
          Serial.println("funion A Nº7 Modo Depurar");
        }
        a7(x1);
      }
      if (funtion_Mode=="A" && funtion_Number=="8"){
        Serial.println("funion A Nº8 Status");
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
        Serial.println(Zona_A_ST);
        Serial.print("Zona B Estado: ");
        Serial.println(Zona_B_ST);
        //6.
        Serial.print("Pulsadores: ");
        Serial.print(Zona_B_Aceptada);
        Serial.println(Zona_A_Aceptada);
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
          Serial.println("funion B Nº1: Quien envia?");
        }
        b1();
      }
      if (funtion_Mode=="B" && funtion_Number=="2"){
        if(flag_F_depurar){
          Serial.println("funion B Nº2: Preparo informacion propia");
        }
        b2();
      }
      if (funtion_Mode=="B" && funtion_Number=="3"){
        if(flag_F_depurar){
          Serial.println("funion B Nº3:  info recibida ");
        }
        b3();
      }
      if (funtion_Mode=="B" && funtion_Number=="4"){
        if(flag_F_depurar){
          Serial.println("funion B Nº4");
        }
        b4();
      }
      if (funtion_Mode=="B" && funtion_Number=="5"){
        if(flag_F_depurar){
          Serial.println("funion B Nº5");
        }
        b5();
      }
      if (funtion_Mode=="B" && funtion_Number=="6"){
        if(flag_F_depurar){
          Serial.println("funion B Nº6");
        }
        b6();
      }        
      if (funtion_Mode=="B" && funtion_Number=="7"){
        if(flag_F_depurar){
          Serial.println("funion B Nº7");
        }
        flag_F_masterNodo=true;
        flag_F_PAQUETE=true;
        function_Remote=inputString.substring(3);   // Extraemos la Funcion a Enviar de la Cadena.
        Nodo_destino=x1;
      }
      if (funtion_Mode=="B" && funtion_Number=="8"){
        if(flag_F_depurar){
          Serial.println("funion B Nº8");
        }
        flag_F_nodoRequest=true;
        flag_F_PAQUETE=true;
      }     
      if (funtion_Mode=="B" && funtion_Number=="9"){
        if(flag_F_depurar){
          Serial.println("funion B Nº9");
        }
        flag_F_masteRequest=true;
        codigo=inputString.substring(2);
        flag_F_PAQUETE=true;
      }     
      if (funtion_Mode=="B" && funtion_Number=="0"){
        if(flag_F_depurar){
          Serial.println("funion B Nº0");
        }
        b0();
      }                            
    // Function tipo C.
      if (funtion_Mode=="C" && funtion_Number=="1"){
        Serial.println("funion C Nº1: updateServer");
        c1(x2);
      }
      else{
        if(flag_F_depurar){
        Serial.println("Ninguna Funcion");
        }
      }
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
        digitalWrite(LED_azul, LOW);     // Led ON.
        delay(retardo);                  // pausa 1 seg.
        digitalWrite(LED_azul, HIGH);    // Led OFF.
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
        temporizador_1.detach();
        temporizador_2.detach();
        temporizador_3.detach();
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
    // Identifico quien Envia el Mensaje Byte
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
        codigo="R";
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
        destination=master;                           // Respondo a quien me escribe.
        // 2. Remitente.
        //localAddress=String(Nodo).toInt();            // Establecer direccion Local.
        // 3. Zonas Leidas 1.
        zonesMSB=Zonas_MSB_Estados;
        zonesLSB=Zonas_LSB_Estados;
        // 4. Nodos Leidas.
        nodosLSB=nodos_LSB_MERGE;
        // 5. Longitud de Bytes de la Cadena incoming_function
          // Este byte lo escribe antes de Enviar el mensaje
        // 6. Este byte contiene Informacion del Nodo
        // nodoInfo=String(msgNumber, HEX);
        // 7. Byte Escrito desde recepcion Serial o Predefinido.
        // 7. Byte Escrito desde recepcion Serial o Predefinido.
        codigo="B3";
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
        destination=Nodo_siguiente;                           // Respondo a quien me escribe.
        // 2. Remitente.
        //localAddress=String(Nodo).toInt();            // Establecer direccion Local.
        // 3. Nodos Leidos 1.
        
        zonesMSB=Zonas_MSB_Estados;
        zonesLSB=Zonas_LSB_Estados;
        // 5. Zonas Leidas.
        nodosLSB=nodos_LSB_MERGE;
        //6. Codigo.
      }
    // b7- Respuesta a Comandos del Maestro.  
      void b7 (){
        // Informacion Acerca de los nodos que pude LEER.
        // Si el mensaje viene del Maestro, preparar el mesaje para flag_F_responder al Maestro
        destination=Nodo_destino;                           // Respondo a quien me escribe.
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
    void c1(int modo_updateServer){
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


//4. Funciones UPDATE.
  //-4.1 Estados de Zonas.
    void reviso(){
      if(localAddress==255) return;
      Zona_A_Aceptada=digitalRead(PB_ZA_in);
      Zona_B_Aceptada=digitalRead(PB_ZB_in);

      Zona_A_ST=digitalRead(Zona_A_in);
      Zona_B_ST=digitalRead(Zona_B_in);

      Zonas_Aceptadas=digitalRead(PB_zonas_in);
      // Pulsadores
      if(!Zonas_Aceptadas){
        bitClear(Zonas, Zona_A);
        bitClear(Zonas, Zona_B);
      }
      if(!Zona_A_Aceptada){
        bitClear(Zonas, Zona_A);
      }
      if(!Zona_B_Aceptada){
        bitClear(Zonas, Zona_B);
      }

      // Zonas.
      if(!Zona_A_ST){
        bitSet(Zonas, Zona_A);
      }
      if(!Zona_B_ST){
        bitSet(Zonas, Zona_B);
      }
        Zona_A_str=String(Zona_A_ST, BIN);
        Zona_B_str=String(Zona_B_ST, BIN);

        Zona_A_PB_str=String(Zona_A_Aceptada, BIN);
        Zona_B_PB_str=String(Zona_B_Aceptada, BIN);
    }
  //-4.2 Secuencia.
    void secuencia(){
      //_____________Modo NODE_______________________________
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
      // Modo MASTER Broadcast.
      if(incoming_recipient==0              && incoming_sender==master){
        b6();
        flag_F_Nodo_Iniciado=true;
        temporizador_2.once_ms(firstTime, ISR_temporizador_2);
        temporizador_1.attach_ms(cycleTime, ISR_temporizador_1);
        beforeTime_2 = millis();  // despurar.
        beforeTime_1 = millis();  // despurar.
        flag_F_T2_run=true;
        flag_F_T1_run=true;
      }
      // Modo MASTER >> PARTICULAR si el master quiere saber: a quien puede escuchar.
      if(incoming_recipient==254            && incoming_sender==master && flag_F_Nodo_Iniciado==false){
        b6();
        temporizador_2.once_ms(wakeUpTime,ISR_temporizador_2);
        temporizador_1.attach_ms(cycleTime, ISR_temporizador_1);
      }
      // Modo NODO  >> MASTER.
      
      // Modo Particular
      if(incoming_recipient==localAddress   && incoming_sender==master){
        b3();
        temporizador_2.once_ms(fastTime, ISR_temporizador_2);
        flag_F_T2_run=true;
      }
      // Modo NODO  >> PRINCIPAL.
      if(localAddress==Nodo_actual          && flag_F_nodoRequest){
        b6();
        beforeTime_2 = millis();  // despurar.
        temporizador_2.once_ms(tokenTime, ISR_temporizador_2);
        flag_F_T2_run=true;
      } 
      // Modo NODOD >> BROADCAST CONTINUO (Prueba).
      if(flag_F_modo_Continuo               && flag_ISR_temporizador_1){
          b3();
      }
      
      
      //_____________Modo MASTER__________________________
      // Modo MASTRER Principal (INICA LA TRANSMISION)
      if(localAddress==master             && flag_F_masteRequest){
        b1();   //destination=0
        // beforeTime_2 = millis();  // despurar.
        // temporizador_0.attach_ms(masterTime, ISR_temporizador_0);
        temporizador_2.once_ms(tokenTime, ISR_temporizador_2);
        temporizador_1.attach_ms(cycleTime, ISR_temporizador_1);
        flag_F_T2_run=true;
        flag_F_T1_run=true;
      }
      if(localAddress==master             && flag_F_masterNodo){
        b7();
        temporizador_2.once_ms(fastTime, ISR_temporizador_2);
        flag_F_T2_run=true;
      }
      // if(localAddress==master             && flag_F_Nodos_Incompletos){
      //   b0();
      // }
      //
      if(Nodo_waiting && !flag_F_Nodo_Iniciado && localAddress < master){
        temporizador_1.attach(waitTime, ISR_temporizador_1);
        flag_F_T1_run=true;
        flag_F_Nodo_Iniciado=true;
      }
    }
  //-4.3 Sever Update.  
    void serverUpdate(){
      flag_F_updateServer=false;
      Serial.print("Nodo: ");
      Serial.print(incoming_sender);
      Serial.print(" ");
      Serial.println(incoming_function);

      if(Nodos_LSB_ACK==15){
        if(Nodos_LSB_ACK==0){
          Serial.println("SEC,ALL,0,0");
          return;
        }
      }
      // Nodo 1.
        if(bitRead(Nodos_LSB_ACK, 1)){
          if(bitRead(Zonas_LSB_Estados, P1ZA) && zona_1_err==false){
            Serial.println("SEC,NOK,1,A");
          }
          if(bitRead(Zonas_LSB_Estados, P1ZB) && zona_2_err==false){
            Serial.println("SEC,NOK,1,B");
          }


          if(bitRead(Zonas_LSB_Estados, P1ZA) && zona_1_err){
            Serial.println("SEC,ERR,1,A");
          }
          if(bitRead(Zonas_LSB_Estados, P1ZB) && zona_2_err){
            Serial.println("SEC,ERR,1,B");
          }


          if(!bitRead(Zonas_LSB_Estados, P1ZA)){
            Serial.println("SEC,BOK,1,A");
            zona_1_err=false;
          }
          if(!bitRead(Zonas_LSB_Estados, P1ZB)){
            Serial.println("SEC,BOK,1,B");
            zona_2_err=false;
          }
        }

      // Nodo 2.
        if(bitRead(Nodos_LSB_ACK, 2)){
          if(bitRead(Zonas_LSB_Estados, P2ZA)){
            Serial.println("SEC,NOK,2,A");
          }
          if(bitRead(Zonas_LSB_Estados, P2ZB)){
            Serial.println("SEC,NOK,2,B");
          }
          if(!bitRead(Zonas_LSB_Estados, P2ZA)){
            Serial.println("SEC,BOK,2,A");
          }
          if(!bitRead(Zonas_LSB_Estados, P2ZB)){
            Serial.println("SEC,BOK,2,B");
          }        
        }

      // Nodo 3.
        if(bitRead(Nodos_LSB_ACK, 3)){
          if(bitRead(Zonas_LSB_Estados, P3ZA)){
            Serial.println("SEC,NOK,3,A");
          }
          if(bitRead(Zonas_LSB_Estados, P3ZB)){
            Serial.println("SEC,NOK,3,B");
          }
          if(!bitRead(Zonas_LSB_Estados, P3ZA)){
            Serial.println("SEC,BOK,3,A");
          }
          if(!bitRead(Zonas_LSB_Estados, P3ZB)){
            Serial.println("SEC,BOK,3,B");
          }
        }

      // Nodo 4.
        if(bitRead(Nodos_LSB_ACK, 4)){
          if(bitRead(Zonas_LSB_Estados, P4ZA)){
            Serial.println("SEC,NOK,4,A");
          }
          if(bitRead(Zonas_LSB_Estados, P4ZB)){
            Serial.println("SEC,NOK,4,B");
          }
          if(!bitRead(Zonas_LSB_Estados, P4ZA)){
            Serial.println("SEC,BOK,4,A");
          }
          if(!bitRead(Zonas_LSB_Estados, P4ZB)){
            Serial.println("SEC,BOK,4,B");
          }
        }
    }
  //-4.4 Actualizar.  
    void actualizar(){
      //Procedimiento
        //1. Estado de Zonas Normalmente en Zero 0 y Activas en Uno 1
        //2. Borrar estados actuales de entradas ó Zonas Locales.
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

        //.1.5 Varibles para Pantalla OLED.
          Zonas_LSB_str=256;        // CON ESTA VARIABLE ACTIVO EL BIT 9 PARA PODER MOSTRAR LAS ZONAS ACTIVAS
          Zonas_LSB_str |=zonesLSB;
      
      //2. ESTADOS DE NODOS. 
        if(flag_F_PAQUETE){
        //-2.1 bit MAESTRO LEIDO Si el mensaje es del Maestro Activar el Bit 0.
          if(incoming_sender==master || localAddress==master){
            bitSet(Nodos_LSB_ACK, 0);
          }
        //-2.2 bit NODOS LEIDOS Si el mensaje es de Cualquier Nodo Activar el Bit Correspondiente.
          if(incoming_sender<master){
            if(incoming_sender>0){    // Si el Mensaje es para Todos el receptor es para todos=0
              bitSet(Nodos_LSB_ACK, incoming_sender);
            }
          }
        //-2.3 bit NODO LOCAL Activar propiamente el bit segun la direccion local.  
          if(localAddress<255){
            bitSet(Nodos_LSB_ACK, localAddress);
          }

        //-2.4 Nodos Para Mostrar en OLED.
          Nodos_LSB_str      = 256;
          Nodos_LSB_str |=Nodos_LSB_ACK;
      
        //-2.5 Sumatoria de Nodos Entrantes.
          nodos_LSB_MERGE = Nodos_LSB_ACK | incoming_nodosLSB;
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


      //3. Pantalla.
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
        // NODO INFORMACION
          Heltec.display->drawString(0, 0, Nodo_Name);
          Heltec.display->drawString(50, 0, String(localAddress, DEC));
          Heltec.display->drawString(70, 0, "RX:");
          Heltec.display->drawString(87, 0, String(incoming_sender, DEC));
        // CODIGO DE MENSAJE
          Heltec.display->drawString(107, 0, incoming_function);
        // ZONAS MENSAJE ENTRANTE
          Heltec.display->drawString(0, 10, "ZONAS:");
          Heltec.display->drawString(50, 10, "87654321");
          Heltec.display->drawString(0, 20, "SUMA:");
          Heltec.display->drawString(45, 20, String(Zonas_LSB_str, BIN));
          Heltec.display->drawString(45, 20, "#");
        // ZONA LOCAL TIMER 1 Y 2 TIEMPO TRANSCURRIDO
          // Heltec.display->drawString(0, 30, "LOCAL:");
          // Heltec.display->drawString(50,30, String(Zonas_LSB, BIN));
          Heltec.display->drawString(0, 30, "T1:");
          Heltec.display->drawString(17,30, String(elapseTime_1, DEC)); 
          Heltec.display->drawString(65,30, "T2:");
          Heltec.display->drawString(80,30, String(elapseTime_2, DEC));          
        // Error de Zona.
          // Heltec.display->drawString(65,30, String(zona_1, DEC));

        // NODOS COMPLETOS
          // Heltec.display->drawString(75,30, String("Nds: "));
          // Heltec.display->drawString(99,30, String(flag_F_Nodos_Completos, DEC));
        // Nodos Leidos.
          Heltec.display->drawString(0, 40, "NODOS:");
          Heltec.display->drawString(45,40, String(Nodos_LSB_str, BIN));
          Heltec.display->drawString(45,40, "#");
        // ZONA A
          if(localAddress<255){
            Heltec.display->drawString(0, 50, "ZA:");
            Heltec.display->drawString(16,50, Zona_A_str);
        // ZONA B
            Heltec.display->drawString(25, 50, "ZB:");
            Heltec.display->drawString(41, 50, Zona_B_str);
        // PULSADOR A
            Heltec.display->drawString(55, 50, "PA:");
            Heltec.display->drawString(72, 50, Zona_B_PB_str);
        // PULSADOR B
            Heltec.display->drawString(85, 50, "PB:");
            Heltec.display->drawString(101, 50, Zona_A_PB_str);
          }
          else{
        // Tiempo Transcurrido para el Master entre mensajes recibidos.    
            Heltec.display->drawString(0, 50, "TGM:");
            Heltec.display->drawString(25, 50, String(elapseTime_GAP, DEC));


        // Nodos reconocidos Propiamente.
            Heltec.display->drawString(60,50, String(Nodos_LSB_ACK, BIN));

          }
        // MOSTAR
          Heltec.display->display();
          delay(100);
    }
  //-4.5 Analizar.
    void analizar(){
      //1 Nodos Leidos
        if(Nodos_LSB_ACK>=14){
          flag_F_Nodos_Completos=true;
        }

      //2 Nodo Ultimo.
        if(incoming_sender==Nodos){
          flag_F_Nodo_Ultimo=true;
        }
      //3 RESETEO DE NODOS LEIDOS.  
        Nodos_LSB_ACK=0;
      //4 FALLA CONSTANTE.
        // NODO_1
          if(bitRead(Zonas_LSB_Estados, P1ZA)){
            if(!zona_1_err){
              ++ zona_1 ;
              if(zona_1==5){
                zona_1=0;
                zona_1_err=true;
              }
            }
          }
        // NODO_2.
          if(bitRead(Zonas_LSB_Estados, P1ZB)){
            if(!zona_1_err){
              ++ zona_2 ;
              if(zona_2==5){
                zona_2=0;
                zona_2_err=true;
              }
            }
          }
    }
//5. Funciones de Dispositivos Externos.
  //-5.1 RFM95 RECIBIR.
    void RFM95_recibir(int packetSize){
      if (packetSize == 0) return;        // if there's no packet, returnº1
      // read packet header bytes:
      incoming_recipient = LoRa.read();    // incoming_recipient address
      incoming_sender    = LoRa.read();    // incoming_sender address
      incoming_zonesLSB  = LoRa.read();    // incoming_function msg ID.
      incoming_zonesMSB  = LoRa.read();    // incoming_function msg ID.
      incoming_nodosLSB  = LoRa.read();    // incoming_function Nodos Reportados LSB.
      incoming_nodosMSB  = LoRa.read();    // incoming_function Nodos Reportados MSB.
      incoming_nodoInfo  = LoRa.read();    // incoming_function Informacion del Nodo Local.
      incoming_length    = LoRa.read();    // incoming_function msg length
      incoming_function           = "";
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
      // if (incoming_recipient != localAddress && incoming_recipient != 0xFF) {
      //   Serial.println("Sent to: 0x" + String(incoming_recipient, HEX));
      //   if(flag_F_depurar){
      //     Serial.println("This message is not for me.");
      //   }
      //   // return;                             // skip rest of function
      // }
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
      flag_F_updateServer=true;
      if(inputString.endsWith("R")){
        flag_F_responder=false;
      }
    }
  //-5.2 RFM95 ENVIAR.
    void RFM95_enviar(String outgoing){
      LoRa.beginPacket();             // start packet
      LoRa.write(destination);        // add destination address
      LoRa.write(localAddress);       // add incoming_sender address
      LoRa.write(zonesLSB);           // add message ID
      LoRa.write(zonesMSB);           // add message ID
      LoRa.write(nodosLSB);
      LoRa.write(nodosMSB);
      LoRa.write(nodoInfo);
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


      flag_F_tokenTime=false;
      flag_F_cycleTime=false;
      Nodo_waiting=false;             // El Nodo ya se inició.
      // eventos
        flag_F_Nodo_Iniciado=true;
        flag_F_respondido=true;
      
      incoming_sender=0;
      incoming_recipient=0;
      //Borrar datos leidos porcada ciclo de lectura de todos los nodos.
      // Nodos_LSB_ACK=0;
      // nodos_LSB_MERGE=0;
      // DEBUG
      if(flag_F_depurar){
        Serial.println(".");
      }
    }