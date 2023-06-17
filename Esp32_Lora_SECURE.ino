//1. librerias.
  //- 1.1 Librerias
    //****************************
    #include <Ticker.h>
    #include <SPI.h>
    #include "heltec.h"
    
//2. Defflag_F_inicion de Pinout.
  //  Las Etiquetas para los pinout son los que comienzan con GPIO
  //  Es decir, si queremos activar la salida 1, tenemos que buscar la referencia GPIO 1, Pero solomante Escribir 1 sin "GPIO"
  //  NO tomar como referencia las etiquetas D1, D2,D3, ....Dx.
  
  //-2.1 Defflag_F_inicion de etiquetas para las Entradas.
    #define in_Zona_1     32        // Entrada de Zona 1
    #define in_Zona_2     33        // Entrada de Zona 2
    #define in_Zona_3     9         // Entrada de Zona 3
    #define in_PB_Aceptar 0         // Entrada de Pulsador in_PB_Aceptar 
  //-2.2 Defflag_F_inicion de etiquetas para las Salidas.
    //********************************************************
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

  //-2.4. Constantes
    //********************************************************
    #define RFM95_FREQ 915E6  
  //-2.5 timer

//3. Variables Globales.
  //-3.1 Variables para las Interrupciones
    String        inputString;                       // Buffer recepcion Serial.
    volatile bool falg_ISR_stringComplete= false;    // Flag: mensaje Serial Recibido completo.
    volatile bool flag_ISR_prueba=false;             // Flag: prueba para interrupcion serial.
    volatile bool flag_ISR_temporizador_1=false;
    volatile bool flag_ISR_temporizador_2=false;
    volatile bool flag_ISR_temporizador_3=false;        // pra actualizar los dato al servidor.
    
  //-3.2 Variables Globales para Las Funciones.
    //********************************************************
    String        funtion_Mode;          // Tipo de funcion para ejecutar.
    String        funtion_Number;        // Numero de funcion a EJECUTAR.
    String        funtion_Parmeter1;     // Parametro 1 de la Funcion.
    String        funtion_Parmeter2;     // Parametro 2 de la Funcion.
    String        funtion_Parmeter3;     // Parametro 3 de la Funcion.
    
    volatile int  x1=0;
    volatile int  x2=0;
    volatile int  x3=0;

    bool          flag_F_codified_funtion=false;   // Notifica que la funcion ha sido codificada.
    bool          flag_F_Un_segundo=false;         // Se activa cuando Pasa un Segundo por Interrupcion.
    bool          flag_F_inicio=true;              // Habilitar mensaje de flag_F_inicio por unica vez
    bool          flag_F_responder=false;          // Se activa cuando recibe un mensaje para luego responder.
    bool          flag_F_modo_Continuo;
    bool          flag_depurar=true;
    bool          flag_F_once=true;
    bool          flag_F_updateServer=false;
    bool          flag_F_respondido=false;
    bool          flag_F_masteRequest=false;
    bool          flag_F_nodoRequest=false;
    bool          flag_F_Nodo_iniciado=false;       // Status
    bool          flag_F_PAQUETE=false;
// Variables para Logica interna.

      byte        master=0xFF;
      byte        Nodo_siguiente=0;    // Direccion del Nodo que sigue para enviar mensaje
      byte        Nodo_anterior;
      byte        Nodo_actual=0;
      word        Zonas=0;             // Estado de Zonas Activas.
      int         Nodos = 2;           // Establece Cuantos Nodos Conforman La Red a6.
      int         Zona_A;
      int         Zona_B;

      //************************
      // String Compañeros="0";
      // String Nodo ="1";
      // String Nodo_cercano=""; // Guardo la direccion del Nodo que escribe
      // bool Nodo_Reconocido=0;
      // unsigned long tiempo1;
      // unsigned long tiempo2;
      String      Nodo_info="";
      String      letras="";
      String      info_1="";
      int         te_toca=1;           // Prueba para comunicacion continua con el servidor.      
      //************************  
      long        initialTime= 0;

      long        actualTime_1 = 0;
      long        elapseTime_1 = 0;
      long        afterTime_1  = 0;
      long        beforeTime_1 = 0;
      
      long        actualTime_2 = 0;
      long        elapseTime_2 = 0;
      long        afterTime_2  = 0;
      long        beforeTime_2 = 0;

      long        baseTime   = 1000;
      long        answerTime = 7000;
      long        tokenTime  ;
      long        updateTime = 2000;
    // Alarmas
      int         Alarma_in_Zona_1=0;
      int         Alarma_in_Zona_2=0;
      int         Alarma_in_Zona_3=0;
    // Eventos
      
  //-3.3 RFM95 Variables.
      byte        msg1_Write = 0;       // Habilito bandera del Nodo que envia 
      byte        msg2_Write = 0;       // Habilito bandera del Nodo que envia
      byte        localAddress = 0x01;  // address of this device           a3
      byte        destination = 0xFF;   // destination to send to           a4
      // long lastSendTime = 0;        // last send time
      // int interval = 2000;
      byte        msgNumber;            
      // en modo continuo este numero incrementa automaticamente.          // interval between sends.
      byte        sender;               // sender address
      byte        incomingMsgId1;       // incoming msg ID
      byte        incomingMsgId2;       // incoming msg ID
      byte        nodo_informa;         // informacion particular que envia el nodo
      byte        incomingLength;       // incoming msg length
      String      incoming = "";
      String      outgoing;             // outgoing message
      int         recipient;            // recipient address

//4. Intancias.
  //********************************************************
  Ticker temporizador_1;
  Ticker temporizador_2;
  Ticker temporizador_3;
//5. Funciones ISR.
  //-5.1 Serial Function.
    void serialEvent (){
      while (Serial.available()) {
        // get the new byte:
        char inChar = (char)Serial.read();
        // add it to the inputString:
        inputString += inChar;
        // if the incoming character is a newline, set a flag so the main loop can
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
      bitSet(Zonas, Zona_A);
    }
    ICACHE_RAM_ATTR void ISR_2(){
      bitSet(Zonas, Zona_B);
    }
    ICACHE_RAM_ATTR void ISR_3(){
      flag_ISR_prueba=true;
    }
    // -5.3 Interrupciones por Timer 1.
    void ISR_temporizador_1(){
      if(flag_F_modo_Continuo){
        flag_F_responder=true;
      }
        actualTime_1 = millis();
        flag_ISR_temporizador_1=true;
        flag_F_responder=true;
        // if(!flag_ISR_temporizador_2){
        //   flag_F_responder=true;
        // }
    }
    void ISR_temporizador_2(){
      actualTime_2 = millis();
      flag_F_responder=true;
      flag_ISR_temporizador_2=true;
    }
    void ISR_temporizador_3(){
      flag_ISR_temporizador_3=true;
      ++te_toca;
    }
void setup(){
  //1. Configuracion de Puertos.
    //****************************
    //1.1 Configuracion de Salidas:
    //1.2 Configuracion de Entradas
      pinMode(in_Zona_1, INPUT_PULLUP);
      pinMode(in_Zona_2, INPUT_PULLUP);
      // pinMode(in_Zona_3, INPUT_PULLUP);
      // pinMode(in_PB_Aceptar, INPUT_PULLUP);
  //2. Condiciones Iniciales.
    //-2.1 Estado de Salidas.
    //-2.2 Valores y Espacios de Variables.
      Nodo_actual     = localAddress;
      Nodo_siguiente  = localAddress + 1;
      Nodo_anterior   = localAddress - 1;
      Zona_B          = localAddress + (localAddress - 1);
      Zona_A          = Zona_B - 1;
      // Original para deployment
      // answerTime      = localAddress * 20;
      answerTime      = 6000;
      tokenTime       = 2000;
      updateTime      = 2500;
  //3. Configuracion de Perifericos:
    //-3.1 Comunicacion Serial:
      Serial.begin(9600);
      delay(10);
    //-3.2 Temporizador.
    //-3.2 Interrupciones Habilitadas.
      //****************************
      attachInterrupt (digitalPinToInterrupt (in_PB_Aceptar), ISR_0, FALLING);  // attach interrupt handler for D2
      attachInterrupt (digitalPinToInterrupt (in_Zona_1), ISR_1, FALLING);      // attach interrupt handler for D2
      attachInterrupt (digitalPinToInterrupt (in_Zona_2), ISR_2, FALLING);      // attach interrupt handler for D2
      attachInterrupt (digitalPinToInterrupt (in_Zona_3), ISR_3, FALLING);      // attach interrupt handler for D2
      //interrupts ();
  //5. Configuracion de DEVICE externos.
    //-5.1 WIFI ESP32 LORA Configuracion.
      Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.LoRa Enable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, RFM95_FREQ /*long BAND*/);
      //****************************
  //4. Prueba de Sitema Minimo Configurado.
    //****************************
    if(flag_depurar){
      Serial.println("Sistema Minimo Configurado");
    }    
}
void loop(){
  //1. Bienvenida Funcion 
    //+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    while (flag_F_inicio){
      welcome();        // Comprobamos el Sistema minimo de Funcionamiento.
      led_Monitor(3);
    }
  //2. Decodificar funcion serial
    //+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    if(falg_ISR_stringComplete){
      decodificar_solicitud();
    }
  //3. Ejecutar Funcion
    //-3.1 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      if(flag_F_codified_funtion){
        ejecutar_solicitud();
        // 3.1 Desactivar Banderas.
        flag_F_codified_funtion=false;
        inputString="";
      }
    //-3.2 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      reviso();
  //4. Atender Las fucniones activadas desde ISR.
    if(flag_ISR_prueba){
      // flag_ISR_prueba=false;
      // a1_Nodo_Destellos(1,3);
    }
  if(flag_ISR_temporizador_1){
    elapseTime_1 = actualTime_1 - beforeTime_1;
    Serial.println(elapseTime_1);
    // Serial.println(actualTime_1);
    // Serial.println(beforeTime_1);
    beforeTime_1 = actualTime_1;
    // flag_ISR_temporizador_1=false;
  }
  if(flag_ISR_temporizador_2){
    elapseTime_2 = actualTime_2 - beforeTime_2;
    Serial.println(elapseTime_2);
    // Serial.println(actualTime_1);
    // Serial.println(beforeTime_1);
    beforeTime_2 = actualTime_2;
    // flag_ISR_temporizador_1=false;
  }
  if(flag_F_updateServer){
      serverUpdate();
    }
  //5. RFM95 Funciones.
    //-5.1 RFM95 RESPONDER Si?
      if(flag_F_responder){
        RFM95_enviar(Nodo_info+letras);
      }
    //-5.2 RFM95 RECIBIR.
      RFM95_recibir(LoRa.parsePacket());
      if(flag_F_PAQUETE){
        flag_F_PAQUETE=false;
        serverUpdate();
      }
      
      secuencia();
}
//1. Funciones de Logic interna del Micro.
  void welcome(){
  // Deshabilitamos Banderas
    flag_F_inicio=false;
    if(flag_depurar){
      Serial.println("Sistema Iniciado");
      Serial.println("Direccion: ");
      Serial.println(localAddress);
    }
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
  void decodificar_solicitud(){
    //Deshabilitamos Banderas
    falg_ISR_stringComplete=false;
    flag_F_codified_funtion=true;
    funtion_Mode=inputString.substring(0,1);
    funtion_Number=inputString.substring(1,2);
    funtion_Parmeter1=inputString.substring(2,3);
    funtion_Parmeter2=inputString.substring(3,4);
    funtion_Parmeter3=inputString.substring(4,5);

    if(flag_depurar){
      Serial.println(inputString);         // Pureba de Comunicacion Serial.
      Serial.println("funcion: " + funtion_Mode);
      Serial.println("Numero: " + funtion_Number);
      Serial.println("Parametro1: " + funtion_Parmeter1);
      Serial.println("Parametro2: " + funtion_Parmeter2);
      Serial.println("Parametro3: " + funtion_Parmeter3+ "\n");
    }
  }
//2. Funciones Seleccionadas para Ejecutar.
  //-2.1 Funciones Tipo A.
    void a1_Nodo_Destellos (int repeticiones, int tiempo){
      // FUNCION PROBADA CORRECTAMENTE
      int veces=repeticiones;
      int retardo=tiempo*100;
      Serial.println("Ejecutando F1.. \n");
      for(int repetir=0; repetir<veces; ++repetir){
        delay(retardo);                  // pausa 1 seg.
        digitalWrite(LED_azul, LOW);     // Led ON.
        delay(retardo);                  // pausa 1 seg.
        digitalWrite(LED_azul, HIGH);    // Led OFF.
      }
    }
    void a2(){
      // FUNCION PROBADA CORRECTAMENTE
      letras=inputString.substring(2);
      Serial.println("letras:" + letras);         // Pureba de Comunicacion Serial.
    }
    void a3_Nodo_Direccion_Local(int paramatro_1){
      Serial.println("Ejecutando F3.. \n");
      localAddress    = paramatro_1;
      Nodo_actual     = localAddress;
      Nodo_siguiente  = localAddress + 1;
      Nodo_anterior   = localAddress - 1;

      Zona_B          = localAddress + (localAddress - 1);
      Zona_A          = Zona_B - 1;
      // answerTime      = localAddress * 20;
      if(localAddress==master){
        temporizador_1.detach();
        temporizador_2.detach();
        temporizador_3.detach();
      }
      Serial.print("Drireccion Local: ");
      Serial.println(localAddress);
    }
    void a4_Nodo_Direccion_Destino(int direccion_aux){
      Serial.println("Ejecutando F4.. \n");
      destination = direccion_aux;
      Serial.print("Drireccion Destino: ");
      Serial.println(destination);
      
    }
    void a5_Nodo_Mensaje_ID(){      
      msgNumber++;                           // increment message ID.
    }
    void a6_Nodo_Numeros(int parametro_1){
      Nodos=parametro_1;  
    }
    void a7(int tipo_Modo, int tipo_Depurar){
      int a=tipo_Modo;
      int b=tipo_Depurar;
      if(a==1){
        beforeTime_1 = millis();
        flag_F_modo_Continuo=true;
        temporizador_1.attach_ms(answerTime, ISR_temporizador_1);
      }
      if(a==0){
        flag_F_modo_Continuo=false;
        flag_F_responder=false;
        temporizador_1.detach();
      }
      if(b==1){
        flag_depurar=1;
      }
      if(b==0){
        flag_depurar=0;
      }
    }
    void a10(int a1, int a2){
      int aa=a1;
      int aa2=a2;
      bitSet(msg1_Write, sender);
      msg2_Write=0;
    }

  //-2.2 Funciones tipo B.
    // Identifico quien Envia el Mensaje Byte
    void b1 (){
      // 1. Destinatario.
      destination=sender;                           // Respondo a quien me escribe.
      // 2. Remitente.
      //localAddress=String(Nodo).toInt();            // Establecer direccion Local.
      // 3. Nodos Leidos 1.
      msg1_Write=lowByte(Zonas);
      // 4. Nodos Leidos 2.
      msg2_Write=highByte(Zonas);
      // 5. Longitud de Bytes de la Cadena incoming.
      // Este byte lo escribe antes de Enviar el mensaje.
      // 6. Este byte contiene Informacion del Nodo.
      Nodo_info=String(nodo_informa, HEX);
      // 7. Byte Escrito desde recepcion Serial o Predefinido.
      // 7. Byte Escrito desde recepcion Serial o Predefinido.
      letras="R";
    }
    void b2 (){
      // 1. Destinatario.
      destination=0;                           // Respondo a quien me escribe.
      // 2. Remitente.
      //localAddress=String(Nodo).toInt();            // Establecer direccion Local.
      // 3. Nodos Leidos 1.
      msg1_Write=lowByte(Zonas);
      // 4. Nodos Leidos 2.
      msg2_Write=highByte(Zonas);
      // 5. Longitud de Bytes de la Cadena incoming.
      // Este byte lo escribe antes de Enviar el mensaje.
      // 6. Este byte contiene Informacion del Nodo.
      Nodo_info=String(Alarma_in_Zona_1+Alarma_in_Zona_2, HEX);
      // 7. Byte Escrito desde recepcion Serial o Predefinido.
    }
    void b3 (){
      // Informacion Acerca de los nodos que pude LEER.
      // Si el mensaje viene del Maestro, preparar el mesaje para flag_F_responder al Maestro
      destination=master;                           // Respondo a quien me escribe.
      // 2. Remitente.
      //localAddress=String(Nodo).toInt();            // Establecer direccion Local.
      // 3. Nodos Leidos 1.
      msg1_Write=lowByte(Zonas);                            // ANTERIORMENTE incomingMsgId1;
      // 4. Nodos Leidos 2.
      msg2_Write=highByte(Zonas);
      // 5. Longitud de Bytes de la Cadena incoming
        // Este byte lo escribe antes de Enviar el mensaje
      // 6. Este byte contiene Informacion del Nodo
      // Nodo_info=String(msgNumber, HEX);
      // 7. Byte Escrito desde recepcion Serial o Predefinido.
      // 7. Byte Escrito desde recepcion Serial o Predefinido.
      letras="R";
    }
    void b4 (){
      // MASTER BROADCAST
      // Informacion Acerca de los nodos que pude LEER.
      // Si el mensaje viene del Maestro, preparar el mesaje para flag_F_responder al Maestro
      destination=master;                           // Respondo a quien me escribe.
      // 2. Remitente.
      //localAddress=String(Nodo).toInt();            // Establecer direccion Local.
      // 3. Nodos Leidos 1.
      msg1_Write=lowByte(Zonas);                            // ANTERIORMENTE incomingMsgId1;
      // 4. Nodos Leidos 2.
      msg2_Write=highByte(Zonas);
      // 5. Longitud de Bytes de la Cadena incoming
        // Este byte lo escribe antes de Enviar el mensaje
      // 6. Este byte contiene Informacion del Nodo
      // Nodo_info=String(msgNumber, HEX);
      // 7. Byte Escrito desde recepcion Serial o Predefinido.
      // 7. Byte Escrito desde recepcion Serial o Predefinido.
      letras="ReadyR";
    }
    void b5 (){
      // MASTER CODE 254
      // Informacion Acerca de los nodos que pude LEER.
      // Si el mensaje viene del Maestro, preparar el mesaje para flag_F_responder al Maestro
      destination=master;                           // Respondo a quien me escribe.
      // 2. Remitente.
      //localAddress=String(Nodo).toInt();            // Establecer direccion Local.
      // 3. Nodos Leidos 1.
      msg1_Write=lowByte(Zonas);                            // ANTERIORMENTE incomingMsgId1;
      // 4. Nodos Leidos 2.
      msg2_Write=highByte(Zonas);
      // 5. Longitud de Bytes de la Cadena incoming
        // Este byte lo escribe antes de Enviar el mensaje
      // 6. Este byte contiene Informacion del Nodo
      // Nodo_info=String(msgNumber, HEX);
      // 7. Byte Escrito desde recepcion Serial o Predefinido.
      // 7. Byte Escrito desde recepcion Serial o Predefinido.
      letras="OK";
    }
    void b6 (){
      // Informacion Acerca de los nodos que pude LEER.
      // Si el mensaje viene del Maestro, preparar el mesaje para flag_F_responder al Maestro
      destination=Nodo_siguiente;                           // Respondo a quien me escribe.
      // 2. Remitente.
      //localAddress=String(Nodo).toInt();            // Establecer direccion Local.
      // 3. Nodos Leidos 1.
      msg1_Write=lowByte(Zonas);                            // ANTERIORMENTE incomingMsgId1;
      // 4. Nodos Leidos 2.
      msg2_Write=highByte(Zonas);
      // 5. Longitud de Bytes de la Cadena incoming
        // Este byte lo escribe antes de Enviar el mensaje
      // 6. Este byte contiene Informacion del Nodo
      // Nodo_info=String(msgNumber, HEX);
      // 7. Byte Escrito desde recepcion Serial o Predefinido.
      // 7. Byte Escrito desde recepcion Serial o Predefinido.
      letras="Q";   //Question solicitd.
    }
    void b7 (int a1, int a2){
      int aa=a1;
      int aa2=a2;
    }
    void b8 (int a1, int a2){
      int aa=a1;
      int aa2=a2;
    }
    void b9 (int a1, int a2){
      int aa=a1;
      int aa2=a2;
    }
    void b0 (){
      int aa=1;
    }
  //-2.3 Funciones tipo C.
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
//3. Gestiona las funciones a Ejecutar.
  void ejecutar_solicitud(){
    // Deshabilitamos Banderas
      x1=funtion_Parmeter1.toInt();
      x2=funtion_Parmeter2.toInt();
      x3=funtion_Parmeter3.toInt();
    // Function Tipo A
      if (funtion_Mode=="A" && funtion_Number=="1"){
        Serial.println("funion A Nº001");
        a1_Nodo_Destellos(x1,x2);
      }
      if (funtion_Mode=="A" && funtion_Number=="2"){
        Serial.println("funion A Nº2");
        a2();
      }
      if (funtion_Mode=="A" && funtion_Number=="3"){
        // FUNCIONO A MEDIAS SOLO DIRECIONES BAJAS Y 255 falta acomodar un poco mas
        Serial.println("funion A Nº3");
        String Nodo_direccion_aux = "";
        Nodo_direccion_aux = funtion_Parmeter1 + funtion_Parmeter2 + funtion_Parmeter3;
        //Serial.println(Nodo_direccion_aux);
        int Nodo_direccion = Nodo_direccion_aux.toInt();
        //Serial.println(Nodo_direccion);
        a3_Nodo_Direccion_Local(Nodo_direccion);
      }
      if (funtion_Mode=="A" && funtion_Number=="4"){
        Serial.println("funion A Nº4");
        String Nodo_destino_aux = "";
        Nodo_destino_aux = funtion_Parmeter1+funtion_Parmeter2+funtion_Parmeter3;
        int Nodo_destino = Nodo_destino_aux.toInt();
        a4_Nodo_Direccion_Destino(Nodo_destino);
      }
      if (funtion_Mode=="A" && funtion_Number=="5"){
        Serial.println("funion A Nº5");
      }
      if (funtion_Mode=="A" && funtion_Number=="6"){
        Serial.println("funion A Nº6: Numero de Nodos");
        String Nodos_numeros_aux = funtion_Parmeter1+funtion_Parmeter2;
        int Nodos_numeros = Nodos_numeros_aux.toInt();
        a6_Nodo_Numeros(Nodos_numeros);
      }
      if (funtion_Mode=="A" && funtion_Number=="7"){
        Serial.println("funion A Nº7");
        a7(x1,x2);
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
        Serial.print("Letras: ");
        Serial.println(letras);
        // //6.
        Serial.print("Tiempo de Respuesta: ");
        Serial.println(answerTime);
      }
      if (funtion_Mode=="A" && funtion_Number=="9"){
        Serial.println("funion A Nº9");
        flag_F_masteRequest=true;
        letras=inputString.substring(2);
        // Serial.println(letras);
      }
      if (funtion_Mode=="A" && funtion_Number=="0"){
        Serial.println("funion A Nº0");
        flag_F_nodoRequest=true;
      }
    // Function Tipo B
      //
      if (funtion_Mode=="B" && funtion_Number=="1"){
        Serial.println("funion B Nº1: Quien envia?");
        b1();
      }
      if (funtion_Mode=="B" && funtion_Number=="2"){
        Serial.println("funion B Nº2: Preparo informacion propia");
        b2();
      }
      if (funtion_Mode=="B" && funtion_Number=="3"){
        Serial.println("funion B Nº3:  info recibida ");
        b3();
      }
      if (funtion_Mode=="B" && funtion_Number=="4"){
        Serial.println("funion B Nº4");
        b4();
      }
      if (funtion_Mode=="B" && funtion_Number=="5"){
        Serial.println("funion B Nº5");
        b5();
      }
      if (funtion_Mode=="B" && funtion_Number=="6"){
        Serial.println("funion B Nº6");
        b6();
      }        
      if (funtion_Mode=="B" && funtion_Number=="7"){
        Serial.println("funion B Nº7");
        b7(1,1);
      }
      if (funtion_Mode=="B" && funtion_Number=="8"){
        Serial.println("funion B Nº8");
        b8(1,1);
      }     
      if (funtion_Mode=="B" && funtion_Number=="9"){
        Serial.println("SEC,NOK,0,A");
      }     
      if (funtion_Mode=="B" && funtion_Number=="0"){
        Serial.println("funion B Nº0");
        b0();
      }                            
    // Function tipo C
      if (funtion_Mode=="C" && funtion_Number=="1"){
        Serial.println("funion C Nº1: updateServer");
        c1(x2);
      }
      else{
      Serial.println("Ninguna Funcion");
      }
      
  }
//4. Funcion que Revisa estados a ser enviados.
  //-4.1 Estados de Zonas.
    void reviso(){      
      if(!digitalRead(in_PB_Aceptar)){
        Alarma_in_Zona_1=0;
        Alarma_in_Zona_2=0;
        Alarma_in_Zona_3=0;
        Nodo_info="0";
      }
    }
    void secuencia(){
      // Solo se cumple si es el Primer Nodo.
      if(sender==master && recipient==localAddress){
        flag_F_responder=true;
        b3();
      }
      if(sender==Nodo_anterior && recipient==localAddress){
        b6();
        beforeTime_2 = millis();
        temporizador_2.once_ms(tokenTime, ISR_temporizador_2);
        if(flag_F_once){
          temporizador_1.attach_ms(answerTime, ISR_temporizador_1);
          flag_F_once=false;
        }
      }
      // Broadcast.
      if(sender==master && recipient==0){
        b4();
        temporizador_1.attach_ms(answerTime, ISR_temporizador_1);
      }
      // si el master quiere saber: a quien puede escuchar.
      if(sender==master && recipient==254){
        temporizador_2.once_ms(answerTime,ISR_temporizador_2);
      }
      // Modo Prueba.
      if(flag_F_modo_Continuo && flag_ISR_temporizador_1){
          a5_Nodo_Mensaje_ID();
          // b3();
          flag_F_responder=true;
      }
      // Modo Maestro.
      if(localAddress==master && flag_F_masteRequest){
          flag_F_responder=true;
      }
      if(localAddress==Nodo_actual && flag_F_nodoRequest){
        flag_F_responder=true;
      }
    }
    void serverUpdate(){
      // flag_ISR_temporizador_3=false;
      // if(te_toca==1){
      //   Serial.println("SEC,NOK,1,A");
      // }
      // if(te_toca==2){
      //   Serial.println("SEC,NOK,2,B");
      // }
      // if(te_toca==3){
      //   Serial.println("SEC,NOK,3,A");
      // }
      // if(te_toca==4){
      //   Serial.println("SEC,NOK,3,B");
      // }
      // if(te_toca==5){
      //   te_toca=0;
      //   Serial.println("SEC,ALL,0,0");
      // }
      if(bitRead(incomingMsgId1, P1ZA)){
        Serial.println("SEC,NOK,1,A");
      }
      if(bitRead(incomingMsgId1, P1ZB)){
        Serial.println("SEC,NOK,1,B");
      }
      if(!bitRead(incomingMsgId1, P4ZA)){
        Serial.println("SEC,BOK,1,A");
      }
      if(!bitRead(incomingMsgId1, P4ZB)){
        Serial.println("SEC,BOK,1,B");
      }


      if(bitRead(incomingMsgId1, P2ZA)){
        Serial.println("SEC,NOK,2,A");
      }
      if(bitRead(incomingMsgId1, P2ZB)){
        Serial.println("SEC,NOK,2,B");
      }
      if(!bitRead(incomingMsgId1, P4ZA)){
        Serial.println("SEC,BOK,3,A");
      }
      if(!bitRead(incomingMsgId1, P4ZB)){
        Serial.println("SEC,BOK,3,B");
      }


      if(bitRead(incomingMsgId1, P3ZA)){
        Serial.println("SEC,NOK,3,A");
      }
      if(bitRead(incomingMsgId1, P3ZB)){
        Serial.println("SEC,NOK,3,B");
      }
      if(!bitRead(incomingMsgId1, P4ZA)){
        Serial.println("SEC,BOK,3,A");
      }
      if(!bitRead(incomingMsgId1, P4ZB)){
        Serial.println("SEC,BOK,3,B");
      }


      if(bitRead(incomingMsgId1, P4ZA)){
        Serial.println("SEC,NOK,4,A");
      }
      if(bitRead(incomingMsgId1, P4ZB)){
        Serial.println("SEC,NOK,4,B");
      }
      if(!bitRead(incomingMsgId1, P4ZA)){
        Serial.println("SEC,BOK,4,A");
      }
      if(!bitRead(incomingMsgId1, P4ZB)){
        Serial.println("SEC,BOK,4,B");
      }
      
    }
//5. Funciones de Dispositivos Externos.
  //-5.1 RFM95 RECIBIR.
    void RFM95_recibir(int packetSize){
      if (packetSize == 0) return;        // if there's no packet, returnº1
      // read packet header bytes:
      recipient       = LoRa.read();      // recipient address
      sender          = LoRa.read();      // sender address
      incomingMsgId1  = LoRa.read();      // incoming msg ID
      incomingMsgId2  = LoRa.read();      // incoming msg ID
      incomingLength  = LoRa.read();      // incoming msg length
      incoming = "";
      while (LoRa.available()){
        incoming += (char)LoRa.read();
      }
      if (incomingLength != incoming.length()) {   // check length for error
        Serial.println("error: message length does not match length");
        return;                             // skip rest of function
      }
      // if the recipient isn't this device or broadcast,
      if (recipient != localAddress && recipient != 0xFF) {
        Serial.println("Sent to: 0x" + String(recipient, HEX));
        Serial.println("This message is not for me.");
        return;                             // skip rest of function
      }
      // if message is for this device, or broadcast, print details:
      if(flag_depurar){
        Serial.println("Received from: 0x" + String(sender, HEX));
        Serial.println("Sent to: 0x" + String(recipient, HEX));
        Serial.println("Message ID1: " + String(incomingMsgId1));
        Serial.println("Message ID2: " + String(incomingMsgId2));
        Serial.println("Message length: " + String(incomingLength));
        Serial.println("Message: " + incoming);
        Serial.println("RSSI: " + String(LoRa.packetRssi()));
        Serial.println("Snr: " + String(LoRa.packetSnr()));
        Serial.println();
      }
      
      inputString=incoming;
      falg_ISR_stringComplete=true;
      flag_F_PAQUETE=true;
      if(inputString.endsWith("R")){
        flag_F_responder=false;
      }
    }
  //-5.2 RFM95 ENVIAR.
    void RFM95_enviar(String outgoing){
      Serial.println(".");
      LoRa.beginPacket();                   // start packet
      LoRa.write(destination);              // add destination address
      LoRa.write(localAddress);             // add sender address
      LoRa.write(msg1_Write);               // add message ID
      LoRa.write(msg2_Write);               // add message ID
      LoRa.write(outgoing.length());        // add payload length
      LoRa.print(outgoing);                 // add payload
      LoRa.endPacket();                     // finish packet and send it
      
      flag_F_responder=false;
      flag_ISR_temporizador_2=false;        // se habilita en el ISR del temporiador 2
      flag_ISR_temporizador_1=false;
      flag_F_respondido=true;
      flag_F_masteRequest=false;
      flag_F_nodoRequest=false;

      sender=0;
      recipient=0;
    }
    // Arregalr el mensaje que se envia en la cadena letras, que sea de 3 letras mas la R al final    
    // Mejorar el mensaje de las entradas digitales, 