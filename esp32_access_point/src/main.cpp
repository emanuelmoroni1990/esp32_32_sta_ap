/*
 * Links de interés;
 * https://randomnerdtutorials.com/esp32-access-point-ap-web-server/
 * https://randomnerdtutorials.com/esp32-esp8266-input-data-html-form/
 * https://randomnerdtutorials.com/esp32-useful-wi-fi-functions-arduino/
 * https://www.esploradores.com/wp-content/uploads/2018/06/Modos-de-conexi%C3%B3n-WiFi-3.pdf
 * Puede venir por acá...
 * https://stackoverflow.com/questions/63098345/trying-to-read-form-data-from-esp8266-webserver-and-printing-it-on-serial-monito
 */

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

#define LED 2
#define REDESMAX 10

// Credenciales que voy a definir para mi access point.
const char* ssid     = "ESP32-Access-Point";
const char* password = "123456789";

// Puerto 80; Profundizar sobre este concepto.
WiFiServer server(80);

// Variable para almacenar al solicitud HTTP
String header;

// Variables auxiliares para uno de los primeros ejemplos desarrollados
String output26State = "off";
String output27State = "off";

// Declaración de los valores referidos a los GPIO
const int output26 = 26;
const int output27 = 27;

bool flag_listado = true;
int cantidadRedes = 0;

String listado_redes [50];

void setup() {
  Serial.begin(9600);
  // Initialize the output variables as outputs
  pinMode(LED     , OUTPUT);
  pinMode(output26, OUTPUT);
  pinMode(output27, OUTPUT);
  // Set outputs to LOW
  digitalWrite(output26, LOW);
  digitalWrite(output27, LOW);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open.
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  server.begin();
}

void loop(){

  if (flag_listado)
  {
    Serial.println("scan start");

    // WiFi.scanNetworks will return the number of networks found
    cantidadRedes = WiFi.scanNetworks();
    Serial.println("scan done");
    if (!cantidadRedes) {
        Serial.println("no networks found");
    } else {
      Serial.print(cantidadRedes);
      Serial.println(" networks found");
      for (int i = 0; i < cantidadRedes; i++) {
        // Print SSID and RSSI for each network found
        Serial.print(i + 1);
        Serial.print(": ");
        Serial.print(WiFi.SSID(i));
        Serial.println("");
        listado_redes[i] = WiFi.SSID(i);

        /*Serial.print(" (");
        Serial.print(WiFi.RSSI(i));
        Serial.print(")");
        Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");*/
        delay(10);
      }
    }
    Serial.println("");

    flag_listado = false;
  }

  /*
   * ----------------------------------------------
   */

  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("Nuevo STA / cliente.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // turns the GPIOs on and off
            for(int i = 0; i < REDESMAX; i ++)
            {
              if (header.indexOf("GET /redes/" + String(i)) >= 0) {
                Serial.print("Red Seleccionada: ");
                Serial.println(listado_redes[i]);
              }
            }
            
            /* 
             * Acá comienzo a desarrollar la página web que el STA podrá ver al acceder al AP.
             */
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>");
            client.println("html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; width: 100%; height: 100px; padding: 0; text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".psswrd { width: 100%; height: 100px; padding: 0; text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println("</style>");
            client.println("</head>");
            
            // Web Page Heading
            client.println("<body><h2>ESP32 Web Server</h2><hr>");
            
            client.println("<h2>Listado de redes</h2>");

            for(int i = 0; i < REDESMAX; i ++)
            {
              //Serial.println("<a href=\"/redes/" + String(i) + "\"><button class=\"button\">Red: " + listado_redes [i] + "</button></a>");
              client.print("<a href=\"/redes/" + String(i) + "\"><button class=\"button\">Red: " + listado_redes [i] + "</button></a>");
              client.println("<a href=\"/boton\"><input class=\"psswrd\" type=\"password\" class=\"form-control\" id=\"passwordId" + String(i) + "\"></a>");
              /*client.println("<script>");
              client.println("var referencia" + String (i) + "= document.getElementById(\"passwordId" + String(i) +"\");");
              client.println("referencia.addEventListener.(\"click\", logIn, false);");
              client.println("</script>");*/
              client.print("<hr><br>");
              //client.println("<input type=\"password\" class=\"form-control\" id=\"passwordId\">");    
            }

            //client.println("<a href=\"/redes/0\"><button class=\"button\">" + listado_redes [0] + "</a></button>");
            //client.println("<input type=\"password\" class=\"form-control\" id=\"passwordId\">");
            //Acá ya lo tengo, tengo que embeber el script para levantar el pass y listo.
               
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}