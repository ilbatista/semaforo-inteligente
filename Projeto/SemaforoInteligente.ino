#include "LiquidCrystal_I2C.h"
#include "Button.h"
#include "FiniteStateMachine.h"
#include "Adafruit_NeoPixel.h"
#include "math.h"

#define PIN_WS2812B_1 32
#define PIN_WS2812B_2 33
#define NUM_PIXELS 64
#define buzzer 16

int vrm_array[] = { 2,3,4,5,9,10,11,12,13,14,16,17,18,21,22,23,24,25,30,31,32,33,38,39,40,41,42,45,46,47,49,50,51,52,53,54,58,59,60,61 };
int amr_array[] = { 1,2,3,4,5,6,9,14,18,21,27,28,35,36,42,45,49,54,57,58,59,60,61,62 };
int vrd_array[] = { 0,1,6,7,9,10,13,14,18,19,20,21,24,27,28,31,32,33,38,39,41,42,45,46,50,51,52,53,59,60 };

Adafruit_NeoPixel ws2812b_1(NUM_PIXELS, PIN_WS2812B_1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel ws2812b_2(NUM_PIXELS, PIN_WS2812B_2, NEO_GRB + NEO_KHZ800);
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
int t_fechado = 10;
int t_amarelo = 5;
int t_aberto = 10;
int t_abertoPCD = 30;

bool contando = true;

void setup() {
  pinMode (PIN_WS2812B_1, OUTPUT);
  pinMode (PIN_WS2812B_2, OUTPUT);
  pinMode(buzzer, OUTPUT);
  ws2812b_1.begin();
  ws2812b_2.begin();
  lcd.init();
  lcd.backlight();
  botao.begin();
}

void loop() {
  if (botao.pressed() && semaforo.isInState(Aberto)){
    t_abertoPCD = t_aberto + t_abertoPCD;
    tone(buzzer,1500,250);
    semaforo.immediateTransitionTo(AbertoPCD);
  }

  semaforo.update();
}

void fechado(){
  contando = true;

  verde_c();
  vermelho_p();

  millisAtual = millis();

  if(millisAtual - millisInicial > intervalo && contando) {
    millisInicial = millis();

    lcd.clear();
    lcd.print("Fechado pedestre");
    lcd.setCursor(0,1);
    lcd.print("00:");

    if(t_fechado > 9){
      lcd.print(t_fechado);
    } else {
      lcd.print("0" + String(t_fechado));
    }

    tone(buzzer,500,100);

    t_fechado--;

    if(t_fechado < 0){
      contando = false;
      t_fechado = 10;
      semaforo.immediateTransitionTo(Amarelo);
    }
  }
}

void amarelo(){
  contando = true;

  amarelo_c();

  millisAtual = millis();

  if(millisAtual - millisInicial > intervalo && contando) {
    millisInicial = millis();
    
    lcd.clear();
    lcd.print("Atencao");
    lcd.setCursor(0,1);
    lcd.print("00:");

    if(t_amarelo > 9){
      lcd.print(t_amarelo);
    } else {
      lcd.print("0" + String(t_amarelo));
    }

    tone(buzzer,750,100);

    t_amarelo--;

    if(t_amarelo < 0){
      contando = false;
      t_amarelo = 5;
      semaforo.immediateTransitionTo(Aberto);
    }
  }
}

void aberto(){  
  contando = true;

  if(t_aberto < 5){
    if((t_aberto & 1) == 0){
      vermelho_c();
      vermelho_p();
    }
    else {
      ws2812b_2.clear();
      ws2812b_2.show();
    }
  } else {
    vermelho_c();
    verde_p();
  }

  millisAtual = millis();

  if(millisAtual - millisInicial > intervalo && contando) {
    millisInicial = millis();
    
    lcd.clear();
    lcd.print("Aberto pedestre");
    lcd.setCursor(0,1);
    lcd.print("00:");

    if(t_aberto > 9){
      lcd.print(t_aberto);
    } else {
      lcd.print("0" + String(t_aberto));
    }

    tone(buzzer,1000,100);

    t_aberto--;

    if(t_aberto < 0){
      contando = false;
      t_aberto = 10;
      t_abertoPCD = 30;
      semaforo.immediateTransitionTo(Fechado);
    }
  }
}

void abertoPCD(){
  contando = true;

  if(t_abertoPCD < 5){
    if((t_abertoPCD & 1) == 0){
      vermelho_c();
      vermelho_p();
    }
    else {
      ws2812b_2.clear();
      ws2812b_2.show();
    }
  } else {
    vermelho_c();
    verde_p();
  }

  millisAtual = millis();

  if(millisAtual - millisInicial > intervalo && contando) {
    millisInicial = millis();
    
    lcd.clear();
    lcd.print("Aberto PcD");
    lcd.setCursor(0,1);
    lcd.print("00:");

    if(t_abertoPCD > 9){
      lcd.print(t_abertoPCD);
    } else {
      lcd.print("0" + String(t_abertoPCD));
    }

    tone(buzzer,1000,100);
    
    t_abertoPCD--;
    
    if(t_abertoPCD < 0){
      contando = false;
      t_aberto = 10;
      t_abertoPCD = 30;
      semaforo.immediateTransitionTo(Fechado);
    }
  }
}

void vermelho_c(){
  ws2812b_1.clear();
  
  for (int pixel = 0; pixel < 65; pixel++) {
    if (std::find(std::begin(vrm_array), std::end(vrm_array), pixel) != std::end(vrm_array))
      ws2812b_1.setPixelColor(pixel, ws2812b_1.Color(127, 0, 0));
  }

  ws2812b_1.show();
}

void amarelo_c(){
  ws2812b_1.clear();
  
  for (int pixel = 0; pixel < 65; pixel++) {
    if (std::find(std::begin(amr_array), std::end(amr_array), pixel) != std::end(amr_array))
      ws2812b_1.setPixelColor(pixel, ws2812b_1.Color(127, 82, 0));
  }

  ws2812b_1.show();
}

void verde_c(){
  ws2812b_1.clear();
  
  for (int pixel = 0; pixel < 65; pixel++) {
    if (std::find(std::begin(vrd_array), std::end(vrd_array), pixel) != std::end(vrd_array))
      ws2812b_1.setPixelColor(pixel, ws2812b_1.Color(0, 127, 0));
  }

  ws2812b_1.show();
}

void vermelho_p(){
  ws2812b_2.clear();
  
  for (int pixel = 0; pixel < 65; pixel++) {
    if (std::find(std::begin(vrm_array), std::end(vrm_array), pixel) != std::end(vrm_array))
      ws2812b_2.setPixelColor(pixel, ws2812b_2.Color(127, 0, 0));
  }

  ws2812b_2.show();
}

void verde_p(){
  ws2812b_2.clear();
  
  for (int pixel = 0; pixel < 65; pixel++) {
    if (std::find(std::begin(vrd_array), std::end(vrd_array), pixel) != std::end(vrd_array))
      ws2812b_2.setPixelColor(pixel, ws2812b_2.Color(0, 127, 0));
  }

  ws2812b_2.show();
}