/*
 * Microcontrolador ESP32 - Empleado como access point ; Emanuel Moroni. ETRR 2021
 * Links de interés;
 * https://randomnerdtutorials.com/esp32-access-point-ap-web-server/
 * https://randomnerdtutorials.com/esp32-esp8266-input-data-html-form/
 * https://randomnerdtutorials.com/esp32-useful-wi-fi-functions-arduino/
 * https://www.esploradores.com/wp-content/uploads/2018/06/Modos-de-conexi%C3%B3n-WiFi-3.pdf
 * https://github.com/zhouhan0126/WebServer-esp32/blob/master/examples/SimpleAuthentification/SimpleAuthentification.ino
 * https://stackoverflow.com/questions/63098345/trying-to-read-form-data-from-esp8266-webserver-and-printing-it-on-serial-monito
 */

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

// Archivos .h de librerías desarrolladas
#include <mainPage.h>
#include <WiFi_connection.h>

#define LED 2

// Prototipos de las funciones desarrolladas.
void mainPage      (void);
void listarRedWiFi (void);
void cargarRedWiFi (void);

// Credenciales SSID y pass necesarias para definir al access point.
const char* ssid     = "ESP32-Access-Point";
const char* password = "123456789";

// Declaración de un objeto llamado servidor de la clase WebServer.
WebServer servidor (80);
// Declaración de un array de objetos llamado listado_redes de la clase String.
String listado_redes [50];

// Declaración de variables para emplear a lo largo del programa.
bool infoRed = false;  // Será empleada para verificar que toda la red WiFi haya sido cargada junto con la contraseña
int cantidadRedes = 0; // Será empleada para contar la cantidad de redes detectadas.

const char * nombreRedFinal;
const char * passwordFinal;

void setup() {
  
  // Inicializo la comunicación serie con la PC para debugear cada etapa de interes. 
  Serial.begin(9600);
  
  // Definición del modo de operación de los pines digitales que se utilizarán en el desarrollo
  pinMode(LED     , OUTPUT);
  
  Serial.print("ESP32 emplado en modo de access point");
  WiFi.mode(WIFI_AP_STA);       // Estoy empleando el microcontrolador ESP32 en modo AP y STA.
  WiFi.softAP(ssid, password);  // Se realiza el setup del AP con el ssid y pass antes definidos.

  IPAddress IP = WiFi.softAPIP();
  Serial.print("Dirección IP del Access Point: ");
  Serial.println(IP);

  servidor.begin();
  // Utilización del método on asociado a objetos de la clase WebServer
  servidor.on("/", mainPage);
  servidor.on("/listarRedes", listarRedWiFi);
  servidor.on("/cargarRedes", cargarRedWiFi);
}

void loop(){
  servidor.handleClient();

  if(infoRed){
    Serial.println("Todo Cargado");
    WiFi.begin(nombreRedFinal, passwordFinal);

    bool estadoConexion = chequeo_conexion();
    Serial.print("Conexión: "));
    Serial.println(estadoConexion);

    digitalWrite(LED, estadoConexion);
    servidor.send(200, "text.html", webcode(listado_redes, 0));

    infoRed = false;
  }
}

void mainPage (void){
  // El servidor envía al cliente el código de estado, el tipo y el contenido en si mismo.
  servidor.send(200, "text.html", webcode(listado_redes, 0));
}

void listarRedWiFi (void){
  Serial.println("Comenzando búsqueda de Redes WiFi...");

  // WiFi.scanNetworks es un método que retorna el número de redes encontradas.
  cantidadRedes = WiFi.scanNetworks();
  
  if (!cantidadRedes) {
    Serial.println("No se encontraron redes WiFi.");
  } 
  else {
    Serial.print(cantidadRedes);
    Serial.println(" redes encontradas.");
    for (int i = 0; i < cantidadRedes; i++) {
      // Se realiza un impresión por consola de las redes WiFi detectadas.
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.println("");
      listado_redes[i] = WiFi.SSID(i);

      delay(100);
    }
  }

  // Ocurre lo mismo que en el caso antes desarrollado pero ahora se envía como argumento el listado de redes WiFi.
  servidor.send(200, "text.html", webcode(listado_redes, 10));
}

void cargarRedWiFi (void){
  Serial.println("Levantando la información de los campos.");
  if(servidor.hasArg("redWiFi") && servidor.hasArg("password")){

    String auxRed  = servidor.arg("redWiFi");
    String auxPass = servidor.arg("password");

    Serial.println(auxRed);
    Serial.println(auxPass);

    nombreRedFinal = auxRed.c_str();
    passwordFinal  = auxPass.c_str();

    Serial.print("Red: ");
    Serial.println(nombreRedFinal);
    Serial.print("Password: ");
    Serial.println(passwordFinal);

    servidor.send(200, "text.html", mensajeCarga());

    infoRed = true;
  }
}