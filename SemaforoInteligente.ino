#include <Adafruit_LiquidCrystal.h>

Adafruit_LiquidCrystal lcd(0);

int aut_vermelho = 2;
int aut_amarelo = 3;
int aut_verde = 4;

int ped_vermelho = 5;
int ped_verde = 6;

int buzzer = 7;
int botao = 8;

int tempoAbertura = 10000;
unsigned long timerInterno;
  
void setup()
{
  timerInterno = millis();
  
  for (int i = 2; i < 8; i++) 
  {   
    pinMode(i, OUTPUT);
  }
  
  pinMode(8, INPUT);
  
  lcd.begin(16, 2);
  lcd.print("Grupo 4");
  lcd.setCursor(0,1);
  lcd.print("Semaforo");
  delay(1000);
  lcd.clear();
  lcd.print("A: automoveis");
  lcd.setCursor(0,1);
  lcd.print("P: pedestres");
  delay(1000);
  lcd.clear();
  
  digitalWrite(aut_vermelho,LOW);
  digitalWrite(aut_amarelo,LOW);
  digitalWrite(aut_verde,HIGH);
  digitalWrite(ped_vermelho,HIGH);
  digitalWrite(ped_verde,LOW);
  
  // ...e o status é refletido na tela
  lcd.print("A: verde");
  lcd.setCursor(0,1);
  lcd.print("P: vermelho");
}

void loop()
{
  int statusBotao = digitalRead(botao);
  delay(50);
  
  if(statusBotao == HIGH && (millis() - timerInterno) > 5000)
  {
  	lcd.setCursor(0,0);
    lcd.clear();
    alteraLuzes();
  }
}
     
void alteraLuzes()
{
  tone(buzzer,500,100);
  
  digitalWrite(aut_verde,LOW);
  digitalWrite(aut_amarelo,HIGH);
  lcd.print("A: amarelo");
  lcd.setCursor(0,1);
  lcd.print("P: vermelho");
  delay(5000);
  lcd.clear();

  tone(buzzer,750,100);
  
  digitalWrite(aut_vermelho,HIGH);
  digitalWrite(aut_amarelo,LOW);  
  digitalWrite(ped_vermelho,LOW);
  digitalWrite(ped_verde,HIGH);  
  lcd.print("A: vermelho");
  lcd.setCursor(0,1);
  lcd.print("P: verde");
  
  delay(tempoAbertura);
  
  for (int x=0; x<10; x++) {
    digitalWrite(ped_verde, LOW);
    delay(500);
    digitalWrite(ped_verde, HIGH);
    delay(500);
  }
  
  lcd.clear();
  
  tone(buzzer,1000,100);
  
  digitalWrite(ped_verde,LOW);
  digitalWrite(aut_vermelho,LOW);
  digitalWrite(ped_vermelho,HIGH);
  digitalWrite(aut_verde,HIGH);
  
  lcd.print("A: verde");
  lcd.setCursor(0,1);
  lcd.print("P: vermelho");
  
  timerInterno = millis();
}
