#include <LiquidCrystal.h>
#include <LED.h>
#include "Button.h"
#include "FiniteStateMachine.h"

//Define os LED's
#define vmc 7
#define amr 6
#define vdc 5
#define vmp 4
#define vdp 3
#define buzzer A0

//Inicializa componentes
LiquidCrystal lcd(8,9,10,11,12,13);
Button botao(2);

//Inicializa os estados
State Fechado = State(fechado);
State Amarelo = State(amarelo);
State Aberto = State(aberto);
State AbertoPCD = State(abertoPCD);

FSM semaforo = FSM(Fechado); //Inicializa a máquina de estado no primeiro estado

//Inicializa os timers para controle do contador
unsigned long millisInicial = 0;
unsigned long millisAtual = 0;
const long intervalo = 1000;
int t_fechado = 10;
int t_amarelo = 5;
int t_aberto = 10;
int t_abertoPCD = 30;

bool contando = true; //Flag para controle do timer

void setup() {
  lcd.begin(16,2); //Instancia a tela
  botao.begin(); //Instancia o botão
  //Serial.begin(9600);

  //Pinos 3 a 5 como OUTPUT
  for(int i = 3; i < 8; i++)
    pinMode(i, OUTPUT);

  pinMode(buzzer, OUTPUT); //Buzzer como OUTPUT no pino A0
}

void loop() {
  //Transiciona o status e soma o tempo quando o botão for pressionado no status adequado
  if (botao.pressed() && semaforo.isInState(Aberto)){
    t_abertoPCD = t_aberto + t_abertoPCD;
    tone(buzzer,1500,250);
    semaforo.immediateTransitionTo(AbertoPCD);
  }

  semaforo.update(); //Atualiza a máquina de estados
}

void fechado(){
  contando = true; //Ativa o contador regressivo

  //Seta os LED's
  digitalWrite(vmc, LOW);
  digitalWrite(amr, LOW);
  digitalWrite(vdc, HIGH);
  digitalWrite(vmp, HIGH);
  digitalWrite(vdp, LOW);

  millisAtual = millis(); //Registra o momento em que estado foi carregado

  // Executa a cada clique (1s) do timer
  if(millisAtual - millisInicial > intervalo && contando) {
    millisInicial = millis(); //Atualiza o timer inicial
    
    //Prepara o display
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

    tone(buzzer,500,100);

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

  digitalWrite(vmc, LOW);
  digitalWrite(amr, HIGH);
  digitalWrite(vdc, LOW);
  digitalWrite(vmp, HIGH);
  digitalWrite(vdp, LOW);

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

  digitalWrite(vmc, HIGH);
  digitalWrite(amr, LOW);
  digitalWrite(vdc, LOW);
  digitalWrite(vmp, LOW);
  digitalWrite(vdp, HIGH);

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

  digitalWrite(vmc, HIGH);
  digitalWrite(amr, LOW);
  digitalWrite(vdc, LOW);
  digitalWrite(vmp, LOW);
  digitalWrite(vdp, HIGH);

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