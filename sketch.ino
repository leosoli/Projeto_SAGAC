#include "RTClib.h"
#include <Servo.h>

RTC_DS1307 rtc;

char diasSemana[7][15] = {"Domingo", "Segunda-feira", "Terça-feira", "Quarta-feira", "Quinta-feira", "Sexta-feira", "Sabado"};


//Criação do objeto do tipo SERVO
Servo servo_Motor; 

//LEDs externos ILUMINAÇÃO
const int pinoLedsIluminacao = 12;

//Sensor de luz (LDR)
const int pinoSensorLuz = 7;
int ausenciaLuzAmbienteExterna = 0; // Luz ambiente externa disponível

// Botão de "MANUAL"
const int pinoBotaoManual = 8;
int estadoManual = 0;

// LED Manual acionado
const int pinoLedManual = 9;              
int LedManual = 0;

// Botão de "MANUAL ABRIR CLARABOIA"
const int pinoBotaoManualClaraboia = 5;
int estadoManualAbrirClaraboia = 0;

// Botão de "MANUAL LIGAR LEDS"
const int pinoBotaoManualLEDs = 6;
int estadoManualLigarLEDs = 0;

void setup() {
  Serial.begin(115200);
  
  pinMode(pinoBotaoManual, INPUT);
  pinMode(pinoBotaoManualClaraboia, INPUT);
  pinMode(pinoBotaoManualLEDs, INPUT);
  
  pinMode(pinoSensorLuz, INPUT);
  
  pinMode(pinoLedsIluminacao, OUTPUT);
  pinMode(pinoLedManual, OUTPUT);
  
  servo_Motor.attach(10); //PINO DE CONTROLE DO SERVO MOTOR
  setClaboiaAberta(false); //ESTADO INICIAL DA CLARABOIA (ABERTA)

  if (! rtc.begin()) {
    Serial.println("RTC nao foi encontrado.");
    Serial.flush();
    abort();
  }
  
}



void loop()
{
  DateTime now = rtc.now();

  //Execução de leituras
  ausenciaLuzAmbienteExterna = digitalRead(pinoSensorLuz);
  estadoManual = digitalRead(pinoBotaoManual);
  estadoManualAbrirClaraboia = digitalRead(pinoBotaoManualClaraboia);
  estadoManualLigarLEDs = digitalRead(pinoBotaoManualLEDs);
  
  Serial.print("Hora atual: ");
  Serial.print(now.day(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.year(), DEC);
  Serial.print(" (");
  Serial.print(diasSemana[now.dayOfTheWeek()]);
  Serial.print(") ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();


  Serial.print(F("Ausencia luz ambiente externa: "));
  formatPrintBoolean(ausenciaLuzAmbienteExterna);
  
  Serial.print(F("Estado Botao Manual: "));
  formatPrintBoolean(estadoManual);
  
  Serial.print(F("Estado Botao Manual Abrir Claraboia: "));
  formatPrintBoolean(estadoManualAbrirClaraboia);
  
  Serial.print(F("Estado Botao Manual Ligar Iluminacao: "));
  formatPrintBoolean(estadoManualLigarLEDs);
  
  Serial.println("");
  Serial.println(F("****************************"));
  Serial.println("");
  
  // Acionamento LED Manual ativado
  if(estadoManual){
    digitalWrite(pinoLedManual, HIGH); // Led Manual ligado
  } else {
    digitalWrite(pinoLedManual, LOW); // Led Manual Desligado
  }
  
  // Acionamento Claraboia
  if( (estadoManual && estadoManualAbrirClaraboia) || (!ausenciaLuzAmbienteExterna && !estadoManual) ){
    setClaboiaAberta(true);
  } else{
    setClaboiaAberta(false);
  }
  
  // Acionamento lampadas LED
  if( (ausenciaLuzAmbienteExterna && !estadoManual) || (estadoManual && estadoManualLigarLEDs) ){
    acionarLampadasLED(true);
  } else {
    acionarLampadasLED(false);
  }
  
  delay(100);
}


void acionarLampadasLED(int ligarLED){ // Acionamento das lâmpadas LED - iluminação
    if(ligarLED){
    digitalWrite(pinoLedsIluminacao, HIGH); // Leds Iluminação ligados
  } else {
    digitalWrite(pinoLedsIluminacao, LOW); // Led Iluminação Desligados
  }
}

void setClaboiaAberta(int aberta){ // Ajustando o estado da Clarabóia
  if (aberta){
      digitalWrite(LED_BUILTIN, HIGH);
      servo_Motor.write(45); // Claraboia aberta
  }
  else{
      digitalWrite(LED_BUILTIN, LOW);// LED VERMELHO APAGA
      servo_Motor.write(0); // Claraboia fechada
  }
}

void formatPrintBoolean(int estado){
  if(estado){
    Serial.println(F("Sim"));
  } else {
    Serial.println(F("Nao"));
  }
}
