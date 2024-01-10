#include <LiquidCrystal.h>
LiquidCrystal lcd(0, 1, 4, 5, 6, 7);

byte p[2][8] = {
  {B01100,B01100,B00000,B01110,B01100,B01100,B01100,B01110},
  {B01100,B01100,B00000,B01110,B11100,B01100,B11010,B10011}
};

unsigned long timerInterno;
int posicao = 0;
byte indice = 0;

void setup(){
  timerInterno = millis();

  DDRD = 0b11111011;
  DDRB = 0b111111;

  for(byte i=0;i<2;i++)
  {
    lcd.createChar(i,p[i]);
  }

  lcd.begin(16,2);
  
  lcd.print("Grupo 4");
  delay(1000);
  lcd.clear(); 
  lcd.print("Semaforo");
  lcd.setCursor(0,1);
  lcd.print("Inteligente");
  delay(2000);
  lcd.clear();

  tone(3,500,100);
  PORTB = 0b001100;
  lcd.print("Passagem fechada");
  lcd.setCursor(0,1);
  lcd.print("################");
}

void loop(){  
  int botao = digitalRead(2);
  delay(50);

  if(botao == HIGH &&(millis() - timerInterno) > 5000){
  	tone(3,750,100);
    acionaSemaforo();
  }
}

void acionaSemaforo(){
  PORTB = 0b001010;
  limpaLinha(0);
  lcd.home();
  lcd.print("Aguarde...      ");
  delay(5000);
  lcd.clear();

  tone(3,1000,100);
  PORTB = 0b010001;
  lcd.print("Passagem livre!");
  posicao = 0;
  
  for (int x=0; x<10; x++){
    animacao();
    delay(1000);
  }
  
  int t = 5;
  
  for (int x=0; x<5; x++){
    lcd.home();
    lcd.print("Fechando em 0:0");
    
    contadorPedestres(t);
    lcd.print(t);
    t--;
    PORTB = 0b010001;
    animacao();
    delay(500);
    PORTB = 0b000001;
    animacao();
    delay(500);
    tone(3,1000,100);
  }

  lcd.clear();
  tone(3,500,100);
  PORTB = 0b001100;
  lcd.print("Passagem fechada");
  lcd.setCursor(0,1);
  lcd.print("################");

  timerInterno = millis();
}

void limpaLinha(byte linha){
  lcd.setCursor(0,linha);
  lcd.print("________________");
  lcd.setCursor(0,linha);
}

void contadorPedestres(int cont){
  lcd.setCursor(15,0);
}

void animacao(){  
  limpaLinha(1);
  lcd.setCursor(posicao,1);
  lcd.write(indice);
  indice++;
  if(indice > 1)
    indice = 0;
  posicao++;
  if(posicao == 16)
    posicao = 0;
}

