#include <Arduino.h>

#include <SerialESP8266wifi.h>
#include <WiFiClient.h>
#include "index.h" // guarda o html

String s = MAIN_page; // guarda o codigo para ser enviado para o browser

//const char s = MAIN_page;

//Dados do router
const char* ssid;
const char* password;

#define RL 2//PT:Pino do LED vermelho; 
#define YeL 3//PT:Pino do LED amarelo;  
#define GL 4//PT:Pino do LED verde;  

#define B_IN 11//PT:Pino para o butão de entrada;  
#define B_OUT 6//PT:Pino para o butão de saída;  


float room_size; //PT:Dimensões da sala em m^2; 
float pm2; //PT:Número de pessoas por metro quadrado;  

//PT: Valores predifinidos para o pm2;
#define muito_afastado 0.05 
#define afastado 0.6
#define proximo 1


unsigned int room_capacity;
unsigned int n;
unsigned int max;
float perc;
float dist;


int time = millis();
int refresh_rate = 1000;


/*
PT:
Retorna o número máximo de pessoas que podem estar em uma sala.
Esta função leva em consideração as dimensões da sala em m^2 (room_size) e o número máximo de pessoas por metro quadrado (pm2).
Por razões práticas, o resultado é arredondado para baixo por padrão.
*/
int capacity(float room_size, float pm2) {return (int)room_size*pm2;}

void LEDS(float perc)
{
  if (perc < 33)
  {
    digitalWrite(YeL, LOW);
    digitalWrite(GL, HIGH);
  }
  else  if (perc < 66)
  {
    digitalWrite(RL, HIGH);
    digitalWrite(YeL, LOW);
    digitalWrite(GL, LOW);
  }
  else
  {
    digitalWrite(YeL, LOW);
    digitalWrite(GL, HIGH); 
  }
}

void GPIO_SETUP()
{
  pinMode(RL, OUTPUT);
  pinMode(YeL, OUTPUT);
  pinMode(GL, OUTPUT);
  
  pinMode(B_IN, INPUT_PULLUP);
  pinMode(B_OUT, INPUT_PULLUP);
  
  //teste de LEDS
  digitalWrite(RL, HIGH);
  digitalWrite(YeL, HIGH);
  digitalWrite(GL, HIGH);
  delay(1000);
  digitalWrite(RL, LOW);
  digitalWrite(YeL, LOW);
  digitalWrite(GL, LOW);
}

void htmlHandler() 
{
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", s); //Envia webpage para o cliente AJAX
}

void nHandler(int n) 
{
  server.send(200, "text/plane", String(n)); //Envia número de pessoas para o cliente AJAX
}

void webInputHandler(String query, float* value)
{
  String str_value = server.arg(query); //xhttp.open("GET", id +"?q=" + getValue(id), true);
  Serial.println(str_value);
  
  *value = (int)str_value;

  return value;
}

void SendToESP(String command, const int timeout, boolean debug)
{
  // Envio dos comandos AT para o modulo
  String resposta = "";
  Serial.print(command);
  //Serial1.print(command);
  long int time = millis();
  while ( (time + timeout) > millis())
  {
    while (Serial1.available())
    {
      char c = Serial1.read();
      resposta += c;
    }
  }
  if (debug) Serial.println(resposta);
}

void CheckESP()
{
  Serial.println("Verifica conexão");
  SendToESP("AT\r\n", 2000, 1);
  
  delay(1000);
  
  Serial.println("Versão Firmware");
  SendToESP("AT+GMR\r\n", 2000, 1);
  Serial.println();
}

void conectarWIFI(int tentativas)
{
  ESP8266WebServer server(80); //abre servidor na porta 80 (https)
  
  int n = 0;
  
  WiFi.begin(ssid, password);
  Serial.println("");
  
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
    if (n > tentativas) break;
    n++;
  }

  //Informações do IP da rede
  
  Serial.println("Rede: ");
  Serial.println(ssid);
  Serial.println("");
  Serial.println("");
  Serial.println("")("IP address: ");
  Serial.println(WiFi.localIP());
}

void IniciarServidor(int n, float *room_size, float *pm2)
{
  //Envia pedidos AJAX
  
  server.on("/", htmlHandler()); //envia pagina web page
  server.on("/numb", nHandler(n)); //envia dados do número de pessoas
  server.on("/tam", webInputHandler("TamSala", &room_size);  //recebe dados; SendValue('tam', 'TamSala')
  server.on("/pm2_max",  webInputHandler("np")); //recebe dados; SendValue('pm2_max', 'np')

  server.begin(); // inicializa o servidor
  Serial.println("HTTP server started");
}



void setup() 
{
  GPIO_SETUP();

  Serial.begin(9600); //comunicação UART com o computador
  Serial1.begin(115200); //comunicação UART com o ESP8266
  void CheckESP();

  conectarWIFI(10);
  IniciarServidor();

  max = 0;
  n  = 0;

  while(!max) //espera o input do utilizador no browser
  {
    server.handleClient(); //Cliente AJAX para o troca de informações
    max = capacity(room_size, pm2);
  }
}

void loop() 
{
  if (millis() - time > refresh_rate) 
  {
    Serial.println("Valor de n:");
    Serial.println(n);  
  
    Serial.println("Maximo de pessoas");
    Serial.println(max); 
    
    perc = ((float)n/(float)max)*100;
    Serial.println("Percentagem de ocupação:");
    Serial.println(perc);
    
    time += refresh_rate;
  }

  if (n < max && digitalRead(B_IN)) n++;
  if (n > 0 && digitalRead(B_OUT)) n--;
  
  server.handleClient(); //Cliente AJAX para o troca de informações

}
