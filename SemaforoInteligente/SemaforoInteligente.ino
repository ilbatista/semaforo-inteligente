#include <LiquidCrystal.h>
#include <Button.h>
#include <FiniteStateMachine.h>

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

//Declara o número de estados
const byte NUMBER_OF_STATES = 4;

//Inicializa os estados
State Fechado = State(fechado);
State Amarelo = State(amarelo);
State Aberto = State(aberto);
State AbertoPCD = State(abertoPCD);

//Inicializa a máquina de estado no primeiro estado
FSM semaforo = FSM(Fechado);

//Inicializa os timers para controle do contador
unsigned long millisInicial = 0;
unsigned long millisAtual = 0;
const long intervalo = 1000;
int t_fechado = 10;
int t_amarelo = 5;
int t_aberto = 10;
int t_abertoPCD = 30;

bool contando = true;

void setup() {
  lcd.begin(16,2); //Instancia a tela
  botao.begin(); //Instancia o botão
  //Serial.begin(9600);

  //Pinos 3 a 5 como OUTPUT
  for(int i = 3; i < 8; i++)
    pinMode(i, OUTPUT);

  //Buzzer como OUTPUT no pino A0
  pinMode(buzzer, OUTPUT);
}

void loop() {
  //Transiciona o status e soma o tempo quando o botão for pressionado no status adequado
  if (botao.pressed() && semaforo.isInState(Aberto)){
    t_abertoPCD = t_aberto + t_abertoPCD;
    semaforo.immediateTransitionTo(AbertoPCD);
  }

  //Atualiza a máquina de estados
  semaforo.update();
}

void fechado(){
  //Ativar o contador regressivo
  contando = true;

  //Seta os LED's
  digitalWrite(vmc, LOW);
  digitalWrite(amr, LOW);
  digitalWrite(vdc, HIGH);
  digitalWrite(vmp, HIGH);
  digitalWrite(vdp, LOW);

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
    t_fechado--;
    if(t_fechado < 1){
      contando = false;
      t_fechado = 10;
      semaforo.transitionTo(Amarelo);
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
    t_abertoPCD--;
    if(t_abertoPCD < 1){
      contando = false;
      t_aberto = 10;
      t_abertoPCD = 30;
      semaforo.transitionTo(Fechado);
    }
  }
}