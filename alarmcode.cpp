//Bibliotecas usadas
// #include <Adafruit_NeoPixel.h> // Library for Neo Pixel Ring light
// #include <LiquidCrystal.h> //Library for Display LCD
// #include <Keypad.h> //Library used for Keypad

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

//Variables

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
  
//Erase what is on the screen and the LEDs restarting the loop
  lcd.clear();
  strip.clear();
  strip.show();
  
  String mostra="Hora Atual - 24H",mostra2="===Alarme hora==",hora, hora2;
 
  tela(mostra);
  hora=setarhora(hora,mostra);
  delay(200);
  tela(mostra2);
  hora2=setarhora(hora2,mostra2);

//Converting the informed hour (String) to whole hours and minutes

  int hAtual =  hora.toInt() / 100;
  int hAlarme = hora2.toInt() / 100;
  int mAtual =  hora.toInt() % 100;
  int mAlarme = hora2.toInt() % 100;

  /* add else if to, if the user inserts the number 3 or higher of the cursor in the line 2 and column 5, so that
  the user does not insert something like: 30h ou <= 70 minutes */

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
    
//Loop that increments the hour until it gets to the desired hour just like a RCT
  
  for (;;)
  {
    lcd.setCursor(0, 0);
    lcd.print("=====Alarme=====");
    if (mAtual == 60)
    {
      hAtual ++;
      
      //Since the alarm works in a 24-hours format: When the time gets to 24, it restarts becoming 00:00

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
    if ((mAlarme - 12) - mAtual <= 0 && hAlarme == hAtual) { // 12 minutes before the alarm's time, the first LED turns on and then the others
      break; 
    }
    printaContador(hAtual, mAtual);
    delay(60000); //waits for 1 real minute
    mAtual ++;
  }
  lcd.setCursor(0, 0); //In the first line of the LCD, shows:
  lcd.print("====Hora====");
  lcd.setCursor(0, 1);
  lcd.print("===De Acordar==="); //In the second line of the LCD, shows:
  int brilho, vetor[12] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

//Loop that turns the LEDs on gradually per POSITION
  //while (keypad.getKey() != '*') {
    for (brilho = 0; brilho <= 12; brilho++)
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

// Function that prints the hour until it gets to the alarms time

void printaContador(int h, int m)
{
  lcd.setCursor(5, 1);
  lcd.print(h);
  lcd.setCursor(7, 1);
  lcd.print(":");
  lcd.setCursor(8, 1);
  lcd.print(m);
}

//Funtion that sets the hour

String setarhora(String hora2,String mostra){
  tela(mostra);
  int ii = 5;
  do {
    char tecla = keypad.getKey();
    if (tecla) {
      //If the user inserts "*", the LCD prints "Tecla Inválida":
      if (tecla == '#' || tecla == 'A' || tecla == 'B' || tecla == 'C') { 
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
      else if (tecla == 'D') { //Erase the entire screen
        ii = 5;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(mostra);
        lcd.setCursor(7, 1);
        lcd.print(":");
        lcd.setCursor(ii, 1);
        hora2 = "";
      }     
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

//Function to choose the information that will be displayed on the screen

void tela(String mostrai){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(mostrai);
  lcd.setCursor(7, 1);
  lcd.print(":");
  lcd.setCursor(5, 1);
}