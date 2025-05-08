#include "LiquidCrystal_I2C.h"
#include "Button.h"
#include "FiniteStateMachine.h"
#include "Adafruit_NeoPixel.h"
#include "math.h"

#define painelAut_DIN 32
#define painelPas_DIN 33
#define num_pixels 64
#define buzzer 16

int vermelho_array[] = { 2,3,4,5,9,10,11,12,13,14,16,17,18,21,22,23,24,25,30,31,32,33,38,39,40,41,42,45,46,47,49,50,51,52,53,54,58,59,60,61 };
int amarelo_array[] = { 1,2,3,4,5,6,9,14,18,21,27,28,35,36,42,45,49,54,57,58,59,60,61,62 };
int verde_array[] = { 0,1,6,7,9,10,13,14,18,19,20,21,24,27,28,31,32,33,38,39,41,42,45,46,50,51,52,53,59,60 };

Adafruit_NeoPixel painelAut(num_pixels, painelAut_DIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel painelPas(num_pixels, painelPas_DIN, NEO_GRB + NEO_KHZ800);
LiquidCrystal_I2C lcd(0x27,16,2);
Button botao(17);

State Fechado = State(fechado);
State Amarelo = State(amarelo);
State Aberto = State(aberto);
State AbertoPCD = State(abertoPCD);

FSM semaforo = FSM(Fechado);

unsigned long millisInicial = 0;
unsigned long millisAtual = 0;
const long intervalo = 1000;
int timerFechado = 10;
int timerAtencao = 5;
int timerAberto = 10;
int timerAbertoPCD = 30;

bool contagem = true;

void setup() {
  pinMode (painelAut_DIN, OUTPUT);
  pinMode (painelPas_DIN, OUTPUT);
  pinMode(buzzer, OUTPUT);
  painelAut.begin();
  painelPas.begin();
  lcd.init();
  lcd.backlight();
  botao.begin();
}

void loop() {
  if (botao.pressed() && semaforo.isInState(Aberto)){
    timerAbertoPCD = timerAberto + timerAbertoPCD;
    tone(buzzer,1500,250);
    semaforo.immediateTransitionTo(AbertoPCD);
  }

  semaforo.update();
}

void fechado(){
  contagem = true;

  abertoAut();
  fechadoPas();

  millisAtual = millis();

  if(millisAtual - millisInicial > intervalo && contagem) {
    millisInicial = millis();

    lcd.clear();
    lcd.print("Fechado pedestre");
    lcd.setCursor(0,1);
    lcd.print("00:");

    if(timerFechado > 9){
      lcd.print(timerFechado);
    } else {
      lcd.print("0" + String(timerFechado));
    }

    tone(buzzer,500,100);

    timerFechado--;

    if(timerFechado < 0){
      contagem = false;
      timerFechado = 10;
      semaforo.immediateTransitionTo(Amarelo);
    }
  }
}

void amarelo(){
  contagem = true;

  atencao();

  millisAtual = millis();

  if(millisAtual - millisInicial > intervalo && contagem) {
    millisInicial = millis();
    
    lcd.clear();
    lcd.print("Atencao");
    lcd.setCursor(0,1);
    lcd.print("00:");

    if(timerAtencao > 9){
      lcd.print(timerAtencao);
    } else {
      lcd.print("0" + String(timerAtencao));
    }

    tone(buzzer,750,100);

    timerAtencao--;

    if(timerAtencao < 0){
      contagem = false;
      timerAtencao = 5;
      semaforo.immediateTransitionTo(Aberto);
    }
  }
}

void aberto(){  
  contagem = true;

  if(timerAberto < 5){
    if((timerAberto & 1) == 0){
      fechadoAut();
      fechadoPas();
    }
    else {
      painelPas.clear();
      painelPas.show();
    }
  } else {
    fechadoAut();
    abertoPas();
  }

  millisAtual = millis();

  if(millisAtual - millisInicial > intervalo && contagem) {
    millisInicial = millis();
    
    lcd.clear();
    lcd.print("Aberto pedestre");
    lcd.setCursor(0,1);
    lcd.print("00:");

    if(timerAberto > 9){
      lcd.print(timerAberto);
    } else {
      lcd.print("0" + String(timerAberto));
    }

    tone(buzzer,1000,100);

    timerAberto--;

    if(timerAberto < 0){
      contagem = false;
      timerAberto = 10;
      timerAbertoPCD = 30;
      semaforo.immediateTransitionTo(Fechado);
    }
  }
}

void abertoPCD(){
  contagem = true;

  if(timerAbertoPCD < 5){
    if((timerAbertoPCD & 1) == 0){
      fechadoAut();
      fechadoPas();
    }
    else {
      painelPas.clear();
      painelPas.show();
    }
  } else {
    fechadoAut();
    abertoPas();
  }

  millisAtual = millis();

  if(millisAtual - millisInicial > intervalo && contagem) {
    millisInicial = millis();
    
    lcd.clear();
    lcd.print("Aberto PcD");
    lcd.setCursor(0,1);
    lcd.print("00:");

    if(timerAbertoPCD > 9){
      lcd.print(timerAbertoPCD);
    } else {
      lcd.print("0" + String(timerAbertoPCD));
    }

    tone(buzzer,1000,100);
    
    timerAbertoPCD--;
    
    if(timerAbertoPCD < 0){
      contagem = false;
      timerAberto = 10;
      timerAbertoPCD = 30;
      semaforo.immediateTransitionTo(Fechado);
    }
  }
}

void fechadoAut(){
  painelAut.clear();
  
  for (int pixel = 0; pixel < 65; pixel++) {
    if (std::find(std::begin(vermelho_array), std::end(vermelho_array), pixel) != std::end(vermelho_array))
      painelAut.setPixelColor(pixel, painelAut.Color(127, 0, 0));
  }

  painelAut.show();
}

void atencao(){
  painelAut.clear();
  
  for (int pixel = 0; pixel < 65; pixel++) {
    if (std::find(std::begin(amarelo_array), std::end(amarelo_array), pixel) != std::end(amarelo_array))
      painelAut.setPixelColor(pixel, painelAut.Color(127, 82, 0));
  }

  painelAut.show();
}

void abertoAut(){
  painelAut.clear();
  
  for (int pixel = 0; pixel < 65; pixel++) {
    if (std::find(std::begin(verde_array), std::end(verde_array), pixel) != std::end(verde_array))
      painelAut.setPixelColor(pixel, painelAut.Color(0, 127, 0));
  }

  painelAut.show();
}

void fechadoPas(){
  painelPas.clear();
  
  for (int pixel = 0; pixel < 65; pixel++) {
    if (std::find(std::begin(vermelho_array), std::end(vermelho_array), pixel) != std::end(vermelho_array))
      painelPas.setPixelColor(pixel, painelPas.Color(127, 0, 0));
  }

  painelPas.show();
}

void abertoPas(){
  painelPas.clear();
  
  for (int pixel = 0; pixel < 65; pixel++) {
    if (std::find(std::begin(verde_array), std::end(verde_array), pixel) != std::end(verde_array))
      painelPas.setPixelColor(pixel, painelPas.Color(0, 127, 0));
  }

  painelPas.show();
}