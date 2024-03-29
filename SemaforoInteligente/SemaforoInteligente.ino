#include "LiquidCrystal_I2C.h"
#include "Button.h"
#include "FiniteStateMachine.h"
#include "Adafruit_NeoPixel.h"
#include "math.h"

#define PIN_WS2812B_1 32
#define PIN_WS2812B_2 33
#define NUM_PIXELS 64
#define buzzer 16

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
  botao.begin();
}

void loop() {
  //Transiciona o status e soma o tempo quando o botão for pressionado no status adequado
  if (botao.pressed() && semaforo.isInState(Aberto)){
    t_abertoPCD = t_aberto + t_abertoPCD; //Soma 30s ao timer atual
    tone(buzzer,1500,250); //Reproduz um sinal sonoro distinto com o buzzer, para acusar o pressionamento do botão
    semaforo.immediateTransitionTo(AbertoPCD); //Transição imediata para o estado 4
  }

  semaforo.update(); //Atualiza a máquina de estados
}

void fechado(){
  contando = true; //Ativa o contador regressivo

  //Seta os LED's
  verde_c();
  vermelho_p();

  millisAtual = millis(); //Registra o momento em que estado foi carregado

  // Executa a cada clique (1s) do timer
  if(millisAtual - millisInicial > intervalo && contando) {
    millisInicial = millis(); //Atualiza o timer inicial
    
    //Exibe uma mensagem no display
    lcd.clear();
    lcd.print("Fechado pedestre");
    lcd.setCursor(0,1);
    lcd.print("00:");

    //Exibe o timer na tela
    if(t_fechado > 9){
      lcd.print(t_fechado);
    } else {
      lcd.print("0" + String(t_fechado));
    }

    tone(buzzer,500,100); //Reproduz um sinal sonoro distinto através do buzzer

    t_fechado--; //Auto-decremento do timer

    //Para a contagem e avança para o próximo estado ao fim do timer
    if(t_fechado < 1){
      contando = false; //Para o contador
      t_fechado = 10; //Reseta o timer para este estado
      semaforo.transitionTo(Amarelo); //Transiciona para o próximo estado
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

    if(t_amarelo < 1){
      contando = false;
      t_amarelo = 5;
      semaforo.transitionTo(Aberto);
    }
  }
}

void aberto(){  
  contando = true;

  vermelho_c();
  verde_p();

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

    if(t_aberto < 1){
      contando = false;
      t_aberto = 10;
      t_abertoPCD = 30;
      semaforo.transitionTo(Fechado);
    }
  }
}

void abertoPCD(){
  contando = true;

  vermelho_c();
  verde_p();

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
    
    if(t_abertoPCD < 1){
      contando = false;
      t_aberto = 10;
      t_abertoPCD = 30;
      semaforo.transitionTo(Fechado);
    }
  }
}

void vermelho_c(){
  ws2812b_1.clear();

  for (int pixel = 0; pixel < 65; pixel++) {
    if(pixel != 2 && pixel != 3 && pixel != 4 && pixel != 5 && pixel != 11 && pixel != 12 && pixel != 16 && pixel != 23 && pixel != 24 && pixel != 25 && pixel != 30 && pixel != 31 && pixel != 32 && pixel != 33 && pixel != 38 && pixel != 39 && pixel != 40 && pixel != 47 && pixel != 51 && pixel != 52 && pixel != 58 && pixel != 59 && pixel != 60 && pixel != 61)
      ws2812b_1.setPixelColor(pixel, ws2812b_1.Color(255, 0, 0));
  }
  ws2812b_1.show();
}

void amarelo_c(){
  ws2812b_1.clear();

  for (int pixel = 0; pixel < 65; pixel++) {
    if(pixel != 3 && pixel != 4 && pixel != 11 && pixel != 12 && pixel != 19 && pixel != 20 && pixel != 27 && pixel != 28 && pixel != 35 && pixel != 36 && pixel != 51 && pixel != 52 && pixel != 59 && pixel != 60);
    else
      ws2812b_1.setPixelColor(pixel, ws2812b_1.Color(255, 255, 0));
  }
  ws2812b_1.show();
}

void verde_c(){
  ws2812b_1.clear();

  for (int pixel = 0; pixel < 65; pixel++) {
    if(pixel != 3 && pixel != 4 && pixel != 10 && pixel != 11 && pixel != 12 && pixel != 13 && pixel != 17 && pixel != 18 && pixel != 21 && pixel != 22 && pixel != 24 && pixel != 25 && pixel != 30 && pixel != 31 && pixel != 32 && pixel != 35 && pixel != 36 && pixel != 39 && pixel != 42 && pixel != 43 && pixel != 44 && pixel != 45 && pixel != 49 && pixel != 50 && pixel != 53 && pixel != 54 && pixel != 56 && pixel != 57 && pixel != 62 && pixel != 63);
    else
      ws2812b_1.setPixelColor(pixel, ws2812b_1.Color(0, 255, 0));
  }
  ws2812b_1.show();
}

void vermelho_p(){
  ws2812b_2.clear();

  for (int pixel = 0; pixel < 65; pixel++) {
    if(pixel != 2 && pixel != 3 && pixel != 4 && pixel != 5 && pixel != 11 && pixel != 12 && pixel != 16 && pixel != 23 && pixel != 24 && pixel != 25 && pixel != 30 && pixel != 31 && pixel != 32 && pixel != 33 && pixel != 38 && pixel != 39 && pixel != 40 && pixel != 47 && pixel != 51 && pixel != 52 && pixel != 58 && pixel != 59 && pixel != 60 && pixel != 61)
      ws2812b_2.setPixelColor(pixel, ws2812b_2.Color(255, 0, 0));
  }
    ws2812b_2.show();
}

void verde_p(){
  ws2812b_2.clear();

  for (int pixel = 0; pixel < 65; pixel++) {
    if(pixel != 3 && pixel != 4 && pixel != 10 && pixel != 11 && pixel != 12 && pixel != 13 && pixel != 17 && pixel != 18 && pixel != 21 && pixel != 22 && pixel != 24 && pixel != 25 && pixel != 30 && pixel != 31 && pixel != 32 && pixel != 35 && pixel != 36 && pixel != 39 && pixel != 42 && pixel != 43 && pixel != 44 && pixel != 45 && pixel != 49 && pixel != 50 && pixel != 53 && pixel != 54 && pixel != 56 && pixel != 57 && pixel != 62 && pixel != 63);
    else
      ws2812b_2.setPixelColor(pixel, ws2812b_2.Color(0, 255, 0));
  }
    ws2812b_2.show();
}
