#include <mainPage.h>

String webcode(String red [], int counter)
{
    String page = "<!DOCTYPE html><html><head>";
    page +="<title>Local network</title>";

    page +="<style>";
    page +="body {margin-top:50px; font-family:Arial;";
    page +="font-size:20px; text-align:center}";
    page +=".button {background-color: #4CAF50; border: none; color: white; width: 100%; height: 100px; padding: 0; text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}";
    page +=".entradas {width: 100%; height: 100px; padding: 0; font-size: 50px}";
    page +=".formulario {font-size: 50px}";
    page +="p {font-size:40px; margin-top:50px;";
    page += "margin-bottom:5px;}";
    page +="</style></head>";

    page +="<body>";
    page +="<h1>Red Local ESP32</h1><br>";
    page +="<form action='/cargarRedes' method='POST'>Ingrese la red y password a la que desea conectarse...<br><br>";
    page +="Red: <input class=\"entradas\" type='text' name='redWiFi' placeholder='user name'><hr>";
    page +="Password: <input class=\"entradas\" type='password' name='password' placeholder='password'><hr>";
    page +="<button class=\"button\" type='submit'>Cargar Datos</button></form><hr>";

    page += "<a href=\"/listarRedes\"><button class=\"button\">Listar Redes</button></a><hr>";  
    
    for (int i = 0; i < counter; i ++)
    {
        page += "<p>" + red [i] + "</p>";
    }

    page +="</body></html>";
    return page;
}

String mensajeCarga (void)
{
    String page = "<!DOCTYPE html><html><head>";
    page +="<title>Local network</title>";

    page +="<style>";
    page +="body {margin-top:50px; font-family:Arial;";
    page +="font-size:20px; text-align:center}";
    page +=".btn {display:block; width:220px;";
    page += "margin:auto; padding:30px}";
    page +=".button {background-color: #4CAF50; border: none; color: white; width: 100%; height: 100px; padding: 0; text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}";
    page +=".on {background-color:SkyBlue}";
    page +=".off {background-color:LightSteelBlue}";
    page +=".zero {background-color:Thistle}";
    page +="td {font-size:30px; margin-top:50px;";
    page += "margin-bottom:5px}";
    page +="p {font-size:30px; margin-top:50px;";
    page += "margin-bottom:5px}";
    page +="</style></head>";

    page +="<body>";
    page +="<h1>Información actualizada... Aguarde.</h1><br>";

    page +="</body></html>";
    return page;
}