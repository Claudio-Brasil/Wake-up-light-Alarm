//Bibliotecas usadas
#include <Adafruit_NeoPixel.h> // Library for Neo Pixel Ring light
#include <LiquidCrystal.h> //Library for Display LCD
#include <Keypad.h> //Library used for Keypad

//LCD, keypad and LED Strip configurations

#define LED 6
#define LED_COUNT 12
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, LED, NEO_GRB + NEO_KHZ800);

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {{'1', '2', '3', 'A'},  {'4', '5', '6', 'B'},  {'7', '8', '9', 'C'},{'*', '0', '#', 'D'}}; //mudança de teclas para o teclado de 4 linhas em 3 colunas
byte rowPins[ROWS] = {10, 9, 8, 7};
byte colPins[COLS] = {A1, A2, A3, A4};
int LCDcol = 0;

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

//Variavies

void tela(String mostrai);
String setarhora(String hora2,String mostra);
void printaContador(int h, int m);


void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  strip.begin();
  strip.clear();
  strip.show();

}

void loop() { 
  
//limpa a tela e os leds para reinicio do loop
  lcd.clear();
  strip.clear();
  strip.show();
  
  String mostra="Hora Atual - 24H",mostra2="===Alarme hora==",hora, hora2;
 
  tela(mostra);
  hora=setarhora(hora,mostra);
  delay(200);
  tela(mostra2);
  hora2=setarhora(hora2,mostra2);

//Converção da hora informada (String) pra horas e minutos(inteiros)

  int hAtual =  hora.toInt() / 100;
  int hAlarme = hora2.toInt() / 100;
  int mAtual =  hora.toInt() % 100;
  int mAlarme = hora2.toInt() % 100;

  lcd.clear();
  Serial.print(hAtual);
  if(hAtual >= 30 || mAtual >= 70) {
    lcd.clear();
    lcd.print("Horario INVALIDO");
    delay(5000);
    return;
  }
  if (mAlarme < 10)
  {
    mAlarme += 50;
    hAlarme --;
  }
    
  
//Loop que incrementa a hora até ela chegar na hora desejada simulando um RCT
  
  for (;;)
  {
    lcd.setCursor(0, 0);
    lcd.print("=====Alarme=====");
    if (mAtual == 60)
    {
      hAtual ++;
      
      //Como o despertador funciona em um formato de 24 horas: Quando o horário chegar em 24, reinici para 00:00
      
      if (hAtual == 24)
      {
        hAtual = 0;
        mAtual = 0;
        lcd.clear();
      }
      else
        mAtual = 0;
    }
    Serial.print(mAlarme - mAtual);
    if ((mAlarme - 12) - mAtual <= 0 && hAlarme == hAtual) { //12 minutos antes do despertar, ligar o primeiro LED
      break;
    }
    printaContador(hAtual, mAtual);
    delay(60000); //espera 1 minuto real
    mAtual ++;
  }
  lcd.setCursor(0, 0); //Na primeira linha do LCD, mostrar:
  lcd.print("====Hora====");
  lcd.setCursor(0, 1);
  lcd.print("===De Acordar==="); //Na segunda linha do LCD, mostrar:
  int brilho, vetor[12] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
  
//Loop para acender o led gradualmente po POSIÇÂO
  //while (keypad.getKey() != '*') {
    for (brilho = 0; brilho <= 12/*12*/; brilho++)
    {
      //char tecla = keypad.getKey();
      //if (tecla == '*') {
      //  brilho = 65;
      //}
      //else {
        strip.setBrightness( brilho );
        strip.setPixelColor(vetor[1], 100, 100, 10);
        strip.setPixelColor(vetor[brilho], 100, 100, 10);
        strip.show();
      //}
      //Serial.print(brilho);
      
      for (int i = 1; i <= 2 /*100*/; i++)
      {
        if (keypad.getKey() == '*')
          return;
        
        delay(281);
      }
      
     //delay(500); // 28125
    }
  //}
}

//Função printar a horas até o despertar

void printaContador(int h, int m){
  lcd.setCursor(5, 1);
  lcd.print(h);
  lcd.setCursor(7, 1);
  lcd.print(":");
  lcd.setCursor(8, 1);
  lcd.print(m);
}

//Função que seta as horas

String setarhora(String hora2,String mostra){
  tela(mostra);
  int ii = 5;
  do {
    char tecla = keypad.getKey();
    if (tecla) {
      if (tecla == '#' || tecla == 'A' || tecla == 'B' || tecla == 'C') { //Se o usuário inserir "*", o LCD aparece "Tecla Inválida":
        ii = 5;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("=Tecla Ivalida=");
        lcd.setCursor(0, 0);
        delay(1000);
        lcd.clear();
        lcd.print(mostra);
        lcd.setCursor(7, 1);
        lcd.print(":");
        lcd.setCursor(ii, 1);
      }
      else if (tecla == 'D') { //limpando toda a tela
        ii = 5;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(mostra);
        lcd.setCursor(7, 1);
        lcd.print(":");
        lcd.setCursor(ii, 1);
        hora2 = "";

      }
      
      /* ----------- adicionar else if para, se o usuário inserir o número 3 ou maior na cursor de linha 2 e coluna 5 para que o usuário
      não insira algo do tipo: 30h ou <= 70 minutos ------------- */
      
      else if (ii == 7) {
        lcd.setCursor(8, 1);
        lcd.print(tecla);
        hora2 += tecla;
        ii += 2;
      }
      else {
        lcd.print(tecla);
        hora2 += tecla;
        ii++;
      }
      lcd.setCursor(ii, 1);
    }
  } while (ii <  10);
   return hora2;
  }

//Função para escolher inforção exibida na tela

void tela(String mostrai){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(mostrai);
  lcd.setCursor(7, 1);
  lcd.print(":");
  lcd.setCursor(5, 1);
}