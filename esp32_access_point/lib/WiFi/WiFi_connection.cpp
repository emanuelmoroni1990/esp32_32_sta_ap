#include <WiFi_connection.h>

unsigned char contador_segundos = 0;
bool flag_conectado;

bool chequeo_conexion (void)
{
  /*
   * Permanezco en un while hasta alcanzar la condición de estar conectado a la red o que se cumpla el timeout
   */
  while((WiFi.status() != WL_CONNECTED) && (!flag_conectado)){
    delay(1000);
    Serial.println(contador_segundos);
    contador_segundos ++;
    if (contador_segundos == 60){flag_conectado = 1;} // Coloco esta condición como timeout = 2s, para que en caso de que no se pueda conectar pueda salir del lazo del while.
  }
  /*
   * A este if ingresaré solo si ya está conectado a la red wifi
   */
  if(WiFi.status() == WL_CONNECTED){
    Serial.print("ESTADO: ");
    Serial.println(WiFi.status());
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    return (true);
  }
  else{
    return (false);
  }
}