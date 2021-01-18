#include <Arduino.h>

#define RL 3//PT:Pino do LED vermelho;  EN:Red LED pin;
#define YL 2//PT:Pino do LED amarelo;  EN:Yellow LED pin;
#define GL 1//PT:Pino do LED verde;  EN:Green LED pin;

#define B_IN 6//PT:Pino para o butão de entrada;  EN: Entry room button pin;
#define B_OUT 5//PT:Pino para o butão de saída;  EN:Leave room button pin;


float room_size; //PT:Dimensões da sala em m^2;  EN:Dimensions of the room in m^2;
float pm2; //PT:Número de pessoas por metro quadrado;  EN:Number of people per square meter;

//PT: Valores predifinidos para o pm2; EN:pm2 defalt values
#define muito_afastado 0.05 
#define afastado 0.6
#define proximo 1

//(Ponto n.º 4 do Despacho n.º 8998-C/2020, de 18 de setembro e artigos 7.º  n.º 1 a) e b) da RCM 70-A/2020):

unsigned int room_capacity;
unsigned int n;
unsigned int max;
unsigned float perc;
unsigned float dist;


/*
PT:
Retorna o número máximo de pessoas que podem estar em uma sala.
Esta função leva em consideração as dimensões da sala em m^2 (room_size) e o número máximo de pessoas por metro quadrado (pm2).
Por razões práticas, o resultado é arredondado para baixo por padrão.

EN:
Returns the maximum number of people that can be in a room.
This function takes into account the dimensions of the room in m^2 (room_size) and the maximum number of people per square meter (pm2).
For practical reasons the result is rounded down by default.
*/
int capacity(float room_size, float pm2) {return (int)room_size*pm2;}

void LEDS(float perc)
{
  if (perc < 33)
  {
    digitalWrite(YL, LOW);
    digitalWrite(GL, HIGH);
  }
  else  if (perc < 66)
  {
    digitalWrite(RL, HIGH);
    digitalWrite(YL, LOW);
    digitalWrite(GL, LOW);
  }
  else
  {
    digitalWrite(YL, LOW);
    digitalWrite(GL, HIGH); 
  }
}

void nHandler(int n) 
{
  server.send(200, "text/plane", String(n)); //Send ADC value only to client ajax request
}

float webInputHandler(String query)
{
  String str_value = server.arg(query); //Refer  xhttp.open("GET", id +"?q=" + getValue(id), true);
  Serial.println(str_value);
  if(t_state == "1")
}

void setup() 
{
  pinMode(RL, OUTPUT);
  pinMode(YL, OUTPUT);
  pinMode(GL, OUTPUT);
  
  pinMode(B_IN, INPUT_PULLUP);
  pinMode(B_OUT, INPUT_PULLUP);

  digitalWrite(RL, LOW);
  digitalWrite(YL, LOW);
  digitalWrite(GL, LOW);

  
  
  pm2 = muito_afastado;
  max = 0;
  n  = 0;

}

void loop() 
{
  room_size = 55; //REQUEST AO ESP
  pm2 = 1; //REQUEST AO ESP entre valores costumizados e predefinidos

  if(!max) max = capacity(room_size, pm2);

  if (n < max && digitalRead(B_IN)) n++;
  if (n > 0 && digitalRead(B_OUT)) n--;

  perc = (float)(n/max)*100;
  dist = n/room_size;

  LEDS(perc);

  server.on("/numb", percHandler(n));
}
