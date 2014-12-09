/*
  Web Server
 
 A simple web server that shows the value of the analog input pins.
 using an Arduino Wiznet Ethernet shield. 
 
 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 * Analog inputs attached to pins A0 through A5 (optional)
 
 created 18 Dec 2009
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe
 
 Modified by Jose Pujol
 
 */

// Pines que vamos a usar
int ldrPin=A0;    // Pin al que conectamos LDR
int lm35Pin=A1;   // Pin al que conectamos LM35

// Valores de los sensores
int ldrValue=0;    // Valor LDR
int lm35Value=0;     // Valor LM35

int temperatura;

#include <SPI.h>
#include <Ethernet.h>

// Introduce una direccion MAC e IP para tu ethernet Shield 
byte mac[] = { 
  0x90, 0xA2, 0xDA, 0x00, 0xFE, 0x0E
   };
IPAddress ip(172,20,1,157);

//Inicializa la biblioteca Ethernet servidor con el puerto 80
EthernetServer server(80);

void setup() {
  // Abre la comunicacion serie y espera 
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  // Empieza la conexión Ethernet y el servidor
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}


void loop() {
  // Escucha de nuevas peticiones de clientes
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    //  Una peticion http finaliza con una linea en blanco
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        
        // Si recibimos un final de linea y la nueva es blank
        // La peticion http ha finalizado y podemos enviar una respuesta
        // Leemos la cadena URL desde $ hasta espacio en blanco
        if (c == '\n' && currentLineIsBlank) {
          // Envia un enacbezado de respuesta estandar
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
         
          // Lectura de valores de los pines analógicos
          ldrValue=analogRead(ldrPin);
          lm35Value=analogRead(lm35Pin);
          temperatura= lm35Value*500/1024;    // Convertimos 10mv/ºC en una escala de 2xy 10 bits
         
          // Si el valor de la ldr es menor que 200 decir que es de noche
          if (ldrValue<200){
            client.print("Es de noche ");
            client.println("<br />"); 
          }
          // Sino de dia
          else{
            client.print("Es de dia ");
            client.println("<br />"); 
          }
          // Imprimimos el valor de la LDR
          client.print("LDR=");
          client.print(ldrValue);
          client.println("<br />"); 
          // Imprimimos el valor de la Temperatura
          client.print("Temperatura=");
          client.print(temperatura);
          client.println("<br />"); 
          client.println("</html>");
          break;
        }
        // Si el caracter es '/n' estas empezando una linea nueva
        if (c == '\n') {
          currentLineIsBlank = true;
        } 
        // you've gotten a character on the current line
        else if (c != '\r') {
          currentLineIsBlank = false;
        }
      }
    }
    delay(1);      // Da tiempo al navegador par recibir los datos 
    client.stop();      // Cierra la conexion
    Serial.println("client disonnected");
  }
}





