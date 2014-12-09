/*
/*
  Web Server Demo
  thrown together by Randy Sarafan
 
 Allows you to turn on and off an LED by entering different urls.
 
 To turn it on:
 http://your-IP-address/$1
 
 To turn it off:
 http://your-IP-address/$2
 
 Circuit:
 * Connect an LED to pin D2 and put it in series with a 220 ohm resistor to ground
 
 Based almost entirely upon Web Server by Tom Igoe and David Mellis
 
 */

#include <SPI.h>
#include <Ethernet.h>

boolean incoming = 0;   // Variable que nos dice si recibimos datos

// Introduce una direccion MAC e IP para tu ethernet Shield 
byte mac[] = { 0x90, 0xA2, 0xDA, 0x00, 0xFE, 0x0E };
// Para obtener IP DhcpPrinter
IPAddress ip(172,20,1,157); //<<< ENTER YOUR IP ADDRESS HERE!!!

//Inicializa la servidosr con el puerto 80
EthernetServer server(80);

void setup()
{
  pinMode(2, OUTPUT);   // Conectamos el LED al pin2D

  // Empieza la conexión Ethernet y el servidor
  start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.begin(9600);
}

void loop()
{
  // Esscucha de nuevas peticiones de clientes
  EthernetClient client = server.available();  //Creamos un cliente Web
  //Cuando detecte un cliente a través de una petición HTTP
  if (client) {
    //  Una peticion http finaliza con una linea en blanco
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();  //Leemos la petición HTTP carácter por carácter
        Serial.write(c);//Visualizamos la petición HTTP por el Monitor Serial
        
        // Si recibimos un final de linea y la nueva es blank
        // La peticion http ha finalizado y podemos enviar una respuesta
        // Leemos la cadena URL desde $ hasta espacio en blanco
        if(incoming && c == ' '){ 
          incoming = 0;
        }
        if(c == '$'){ 
          incoming = 1; 
        }
        
        //Revisa si el caracter es 1 o 2
        if(incoming == 1){
          Serial.println(c);
          
          if(c == '1'){
            Serial.println("ON");
            digitalWrite(2, HIGH);
          }
          if(c == '2'){
            Serial.println("OFF");
            digitalWrite(2, LOW);
          }
        
        }
        // Si el caracter es '/n' estas empezando una linea nueva
        if (c == '\n') {
          currentLineIsBlank = true;
        } 
        // Y si es es distinto de r es que tienes un caracter
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // Da tiempo al navegador par recibir los datos 
    delay(1);
    // Cierra la conexion
    client.stop();
  }
}


