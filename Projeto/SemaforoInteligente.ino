#include "LiquidCrystal_I2C.h" //Controla o display LCD da botoeira
#include "Button.h" //Controla o botão da botoeira
#include "FiniteStateMachine.h" //Controla a máquina de estados
#include "Adafruit_NeoPixel.h" //Controla os LEDs do painel
#include "math.h" //Requisito para o uso de funções matemáticas

#define painelAut_DIN 32 //Pino de dados do painel de LEDs do semáforo de automóveis
#define painelPed_DIN 33 //Pino de dados do painel de LEDs do semáforo de pedestres 
#define numPixels 64 //Número de LEDs do painel
#define buzzer 16 //Pino do buzzer

/*

Configuração dos LEDs do painel para o status de semáforo vermelho

    # # # #    
  # # # # # #  
# # #     # # #
# #         # #
# #         # #
# # #     # # #
  # # # # # #  
    # # # #     

*/
int vermelho_array[] = { 2,3,4,5,9,10,11,12,13,14,16,17,18,21,22,23,24,25,30,31,32,33,38,39,40,41,42,45,46,47,49,50,51,52,53,54,58,59,60,61 };

/*

Configuração dos LEDs do painel para o status de semáforo amarelo

  # # # # # #  
  #         #  
    #     #    
      # #      
      # #      
    #     #    
  #         #  
  # # # # # #  

*/
int amarelo_array[] = { 1,2,3,4,5,6,9,14,18,21,27,28,35,36,42,45,49,54,57,58,59,60,61,62 };

/*

Configuração dos LEDs do painel para o status de semáforo verde

      # #      
    # # # #    
  # #     # #  
# #   # #   # #
#   # # # #   #
  # #    # #
# #        # #
#            #

*/
int verde_array[] = { 0,1,6,7,9,10,13,14,18,19,20,21,24,27,28,31,32,33,38,39,41,42,45,46,50,51,52,53,59,60 };

Adafruit_NeoPixel painelAut(numPixels, painelAut_DIN, NEO_GRB + NEO_KHZ800); //Instância do painel de LEDs do semáforo de automóveis
Adafruit_NeoPixel painelPed(numPixels, painelPed_DIN, NEO_GRB + NEO_KHZ800); //Instância do painel de LEDs do semáforo de pedestres
LiquidCrystal_I2C lcd(0x27,16,2); //Instância do display LCD com endereço 0x27 e 16 colunas e 2 linhas
Button botao(17); //Instância do botão da botoeira no pino 17

State Fechado = State(fechado); //Definição do estado fechado para pedestres
State Atencao = State(atencao); //Definição do estado de atenção
State Aberto = State(aberto); //Definição do estado aberto para pedestres
State AbertoPCD = State(abertoPCD); //Definição do estado aberto para PCD

FSM semaforo = FSM(Fechado); //Instância da máquina de estados com o estado inicial fechado

unsigned long millisInicial = 0; //Variável para armazenar o tempo inicial
unsigned long millisAtual = 0; //Variável para armazenar o tempo atual
const long intervalo = 1000; //Intervalo de 1 segundo
int timerFechado = 10; //Tempo do semáforo fechado para pedestres
int timerAtencao = 5; //Tempo do semáforo em atenção
int timerAberto = 10; //Tempo do semáforo aberto para pedestres
int timerAbertoPCD = 30; //Tempo do semáforo aberto para PCD

bool contagem = true; //Variável para controlar a contagem do tempo

void setup() {
  pinMode (painelAut_DIN, OUTPUT); //Configura o pino do painel de LEDs do semáforo de automóveis como saída
  pinMode (painelPed_DIN, OUTPUT); //Configura o pino do painel de LEDs do semáforo de pedestres como saída
  pinMode(buzzer, OUTPUT); //Configura o pino do buzzer como saída
  painelAut.begin(); //Inicializa o painel de LEDs do semáforo de automóveis
  painelPed.begin(); //Inicializa o painel de LEDs do semáforo de pedestres
  lcd.init(); //Inicializa o display LCD
  lcd.backlight(); //Liga a luz de fundo do display LCD
  botao.begin(); //Inicializa o botão da botoeira
}

void loop() {
  if (botao.pressed() && semaforo.isInState(Aberto)){ //Verifica se o botão foi pressionado e se o semáforo está aberto
    contagem = false; //Desativa a contagem
    timerAbertoPCD = timerAberto + timerAbertoPCD; //Aumenta o tempo do semáforo aberto para PCD
    tone(buzzer,1500,250); //Emite um som no buzzer
    semaforo.immediateTransitionTo(AbertoPCD); //Transita para o estado aberto para PCD
  }

  semaforo.update(); //Atualiza a máquina de estados
}

void fechado(){
  contagem = true; //Ativa a contagem
  verdeAut(); //Atualiza o semáforo de automóveis para verde e o semáforo de pedestres para vermelho
  millisAtual = millis(); //Obtém o tempo atual

  if(millisAtual - millisInicial > intervalo && contagem) { //Executado a cada segundo
    millisInicial = millis(); //Atualiza o tempo inicial

    lcd.clear(); //Limpa o display LCD
    lcd.print("Fechado pedestre"); //Exibe a mensagem "Fechado pedestre"
    lcd.setCursor(0,1); //Define o cursor na segunda linha
    lcd.print("00:"); //Exibe "00:" no display LCD, para formatação

    if(timerFechado > 9){ //Executado se o tempo restante for maior que 9
      lcd.print(timerFechado); //Exibe o tempo restante
    } else { //Executado se o tempo restante for menor que 9
      lcd.print("0" + String(timerFechado)); //Exibe o tempo restante com um zero à esquerda
    }

    tone(buzzer,500,100); //Emite um tom de 500Hz no buzzer por 100ms
    timerFechado--; //Decrementa o tempo restante

    if(timerFechado < 0){ //Executado ao final do timer
      contagem = false; //Desativa a contagem
      timerFechado = 10; //Reinicia o tempo para o status atual
      semaforo.immediateTransitionTo(Atencao); //Transita para o estado de semáforo amarelo
    }
  }
}

void atencao(){
  contagem = true; //Ativa a contagem
  amarelo(); //Atualiza o semáforo de automóveis para amarelo e o semáforo de pedestres para vermelho
  millisAtual = millis(); //Obtém o tempo atual

  if(millisAtual - millisInicial > intervalo && contagem) { //Executado a cada segundo
    millisInicial = millis(); //Atualiza o tempo inicial
    
    lcd.clear(); //Limpa o display LCD
    lcd.print("Atencao"); //Exibe a mensagem "Atenção"
    lcd.setCursor(0,1); //Define o cursor na segunda linha
    lcd.print("00:"); //Exibe "00:" no display LCD, para formatação

    if(timerAtencao > 9){ //Executado se o tempo restante for maior que 9
      lcd.print(timerAtencao); //Exibe o tempo restante
    } else { //Executado se o tempo restante for menor que 9
      lcd.print("0" + String(timerAtencao)); //Exibe o tempo restante com um zero à esquerda
    }

    tone(buzzer,750,100); //Emite um tom de 750Hz no buzzer por 100ms
    timerAtencao--; //Decrementa o tempo restante

    if(timerAtencao < 0){ //Executado ao final do timer
      contagem = false; //Desativa a contagem
      timerAtencao = 5; //Reinicia o tempo para o status atual
      semaforo.immediateTransitionTo(Aberto); //Transita para o estado aberto para pedestres
    }
  }
}

void aberto(){  
  contagem = true; //Ativa a contagem

  if(timerAberto < 5){ //Executado se o tempo restante for menor que 5
    if((timerAberto & 1) == 0){ //Verifica se o tempo restante é par
      vermelhoAut(); //Atualiza o semáforo de automóveis para vermelho e o semáforo de pedestres para verde
    }
    else {
      painelPed.clear(); //Limpa o painel de LEDs do semáforo de pedestres
      painelPed.show(); //Atualiza o painel de LEDs do semáforo de pedestres
    }
  } else {
    vermelhoAut(); //Atualiza o semáforo de automóveis para vermelho e o semáforo de pedestres para verde
  }

  millisAtual = millis(); //Obtém o tempo atual

  if(millisAtual - millisInicial > intervalo && contagem) { //Executado a cada segundo
    millisInicial = millis(); //Atualiza o tempo inicial
    
    lcd.clear(); //Limpa o display LCD
    lcd.print("Aberto pedestre"); //Exibe a mensagem "Aberto pedestre"
    lcd.setCursor(0,1); //Define o cursor na segunda linha
    lcd.print("00:"); //Exibe "00:" no display LCD, para formatação

    if(timerAberto > 9){ //Executado se o tempo restante for maior que 9
      lcd.print(timerAberto); //Exibe o tempo restante
    } else { //Executado se o tempo restante for menor que 9
      lcd.print("0" + String(timerAberto)); //Exibe o tempo restante com um zero à esquerda
    }

    tone(buzzer,1000,100); //Emite um tom de 1kHz no buzzer por 100ms
    timerAberto--; //Decrementa o tempo restante

    if(timerAberto < 0){ //Executado ao final do timer
      contagem = false; //Desativa a contagem
      timerAberto = 10; //Reinicia o tempo para o status atual
      timerAbertoPCD = 30; //Reinicia o tempo para o status atual
      semaforo.immediateTransitionTo(Fechado); //Transita para o estado fechado para pedestres
    }
  }
}

void abertoPCD(){
  contagem = true; //Ativa a contagem

  if(timerAbertoPCD < 5){ //Executado se o tempo restante for menor que 5
    if((timerAbertoPCD & 1) == 0){ //Verifica se o tempo restante é par 
      vermelhoAut(); //Atualiza o semáforo de automóveis para vermelho e o semáforo de pedestres para verde
    }
    else { //Executado se o tempo restante for ímpar
      painelPed.clear(); //Limpa o painel de LEDs do semáforo de pedestres
      painelPed.show(); //Atualiza o painel de LEDs do semáforo de pedestres
    }
  } else { //Executado se o tempo restante for maior que 5
    vermelhoAut(); //Atualiza o semáforo de automóveis para vermelho e o semáforo de pedestres para verde
  }

  millisAtual = millis(); //Obtém o tempo atual

  if(millisAtual - millisInicial > intervalo && contagem) { //Executado a cada segundo
    millisInicial = millis(); //Atualiza o tempo inicial
    
    lcd.clear(); //Limpa o display LCD
    lcd.print("Aberto PcD"); //Exibe a mensagem "Aberto PCD"
    lcd.setCursor(0,1); //Define o cursor na segunda linha
    lcd.print("00:"); //Exibe "00:" no display LCD, para formatação

    if(timerAbertoPCD > 9){ //Executado se o tempo restante for maior que 9
      lcd.print(timerAbertoPCD); //Exibe o tempo restante
    } else { //Executado se o tempo restante for menor que 9
      lcd.print("0" + String(timerAbertoPCD)); //Exibe o tempo restante com um zero à esquerda
    }

    tone(buzzer,1000,100); //Emite um tom de 1kHz no buzzer por 100ms    
    timerAbertoPCD--; //Decrementa o tempo restante
    
    if(timerAbertoPCD < 0){ //Executado ao final do timer
      contagem = false; //Desativa a contagem
      timerAberto = 10; //Reinicia o tempo para o status atual
      timerAbertoPCD = 30; //Reinicia o tempo para o status atual
      semaforo.immediateTransitionTo(Fechado); //Transita para o estado fechado para pedestres
    }
  }
}

void vermelhoAut(){
  painelAut.clear(); //Limpa o painel de LEDs do semáforo de automóveis
  painelPed.clear(); //Limpa os painéis de LEDs do semáforo de automóveis e pedestres
  
  for (int pixel = 0; pixel < 65; pixel++) { //Percorre o array de LEDs (64 pixels)
    if (std::find(std::begin(vermelho_array), std::end(vermelho_array), pixel) != std::end(vermelho_array)) //Verifica se o pixel está no array de LEDs vermelhos
      painelAut.setPixelColor(pixel, painelAut.Color(127, 0, 0)); //Configura o LED para vermelho
  }

  for (int pixel = 0; pixel < 65; pixel++) { //Percorre o array de LEDs (64 pixels)
    if (std::find(std::begin(verde_array), std::end(verde_array), pixel) != std::end(verde_array)) //Verifica se o pixel está no array de LEDs verdes
      painelPed.setPixelColor(pixel, painelPed.Color(0, 127, 0)); //Configura o LED para verde
  }

  painelAut.show(); //Atualiza o painel de LEDs do semáforo de automóveis
  painelPed.show(); //Atualiza o painel de LEDs do semáforo de pedestres
}

void amarelo(){
  painelAut.clear(); //Limpa o painel de LEDs do semáforo de automóveis
  
  for (int pixel = 0; pixel < 65; pixel++) { //Percorre o array de LEDs (64 pixels)
    if (std::find(std::begin(amarelo_array), std::end(amarelo_array), pixel) != std::end(amarelo_array)) //Verifica se o pixel está no array de LEDs amarelos
      painelAut.setPixelColor(pixel, painelAut.Color(127, 82, 0)); //Configura o LED para amarelo
  }

  painelAut.show(); //Atualiza o painel de LEDs do semáforo de automóveis
}

void verdeAut(){
  painelAut.clear(); //Limpa o painel de LEDs do semáforo de automóveis
  painelPed.clear(); //Limpa o painel de LEDs do semáforo de pedestres
  
  for (int pixel = 0; pixel < 65; pixel++) { //Percorre o array de LEDs (64 pixels)
    if (std::find(std::begin(verde_array), std::end(verde_array), pixel) != std::end(verde_array)) //Verifica se o pixel está no array de LEDs verdes
      painelAut.setPixelColor(pixel, painelAut.Color(0, 127, 0)); //Configura o LED para verde
  }

  for (int pixel = 0; pixel < 65; pixel++) { //Percorre o array de LEDs (64 pixels)
    if (std::find(std::begin(vermelho_array), std::end(vermelho_array), pixel) != std::end(vermelho_array)) //Verifica se o pixel está no array de LEDs vermelhos
    painelPed.setPixelColor(pixel, painelPed.Color(127, 0, 0)); //Configura o LED para vermelho
  }

  painelAut.show(); //Atualiza o painel de LEDs do semáforo de automóveis
  painelPed.show(); //Atualiza o painel de LEDs do semáforo de pedestres
}