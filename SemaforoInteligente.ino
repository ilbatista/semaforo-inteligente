#include <Adafruit_LiquidCrystal.h>

Adafruit_LiquidCrystal lcd(0);
int tempoAbertura = 10000;
unsigned long timerInterno;
  
void setup(){
  timerInterno = millis();
 
  DDRD = 0xFC;
  DDRB = 0x3E;
  
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
  
  PORTD = 0x30;
  lcd.print("A: verde");
  lcd.setCursor(0,1);
  lcd.print("P: vermelho");
}

void loop(){
  int statusBotao = digitalRead(8);
  delay(50);
  
  if(statusBotao == HIGH && (millis() - timerInterno) > 5000){
  	lcd.setCursor(0,0);
    lcd.clear();
    alteraLuzes();
  }
}
     
void alteraLuzes(){
  tone(9,500,100);
  PORTD = 0x28;
  lcd.print("A: amarelo");
  lcd.setCursor(0,1);
  lcd.print("P: vermelho");
  delay(5000);
  lcd.clear();

  tone(9,750,100);
  PORTD = 0x44;
  lcd.print("A: vermelho");
  lcd.setCursor(0,1);
  lcd.print("P: verde");
  
  delay(tempoAbertura);
  
  for (int x=0; x<10; x++){
    PORTD = 0x4;
    delay(500);
    PORTD = 0x44;
    delay(500);
  }
  
  lcd.clear();
  
  tone(9,1000,100);
  PORTD = 0x30;  
  lcd.print("A: verde");
  lcd.setCursor(0,1);
  lcd.print("P: vermelho");
  
  timerInterno = millis();
}
