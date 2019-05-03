#include <LiquidCrystal.h>

#define DDR_KEYPAD  DDRF
#define PORT_KEYPAD PORTF
#define PIN_KEYPAD  PINF
#include "keypad4x4.h"

const int buzzerPin = 21;

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 5, rw = 6, en = 7, d4 = 8, d5 = 9, d6 = 10, d7 = 11;
LiquidCrystal lcd(rs, rw, en, d4, d5, d6, d7);

const PROGMEM  char sixty[60][3] = {
  {"00"}, {"01"}, {"02"}, {"03"}, {"04"}, {"05"}, {"06"}, {"07"}, {"08"}, {"09"},
  {"10"}, {"11"}, {"12"}, {"13"}, {"14"}, {"15"}, {"16"}, {"17"}, {"18"}, {"19"},
  {"20"}, {"21"}, {"22"}, {"23"}, {"24"}, {"25"}, {"26"}, {"27"}, {"28"}, {"29"},
  {"30"}, {"31"}, {"32"}, {"33"}, {"34"}, {"35"}, {"36"}, {"37"}, {"38"}, {"39"},
  {"40"}, {"41"}, {"42"}, {"43"}, {"44"}, {"45"}, {"46"}, {"47"}, {"48"}, {"49"},
  {"50"}, {"51"}, {"52"}, {"53"}, {"54"}, {"55"}, {"56"}, {"57"}, {"58"}, {"59"}
};

struct Time
{
  unsigned char second, minute, hour;
};
Time T2 = {0, 0, 0};

void LCD_WriteStrPROGMEM(char *str, int n)  //вивід масиву символів,
{ //записаних у флеші
  for (int i = 0; i < n; i++)
    lcd.print( (char)pgm_read_byte( &(str[i]) ) );
}

ISR(TIMER1_COMPA_vect)  // Таймер Т1 по співпадінню А, кожної 1 сек.
{
  if (++T2.second == 60)
  {
    T2.second = 0;
    if (++T2.minute == 60)
    {
      T2.minute = 0;
      if (++T2.hour == 24)
        T2.hour = 0;
    }
  }
  lcd.setCursor(3, 0);
  LCD_WriteStrPROGMEM(sixty[T2.hour], 2);
  lcd.write(':');
  LCD_WriteStrPROGMEM(sixty[T2.minute], 2);
  lcd.write(':');
  LCD_WriteStrPROGMEM(sixty[T2.second], 2);
}

void setup() {
  noInterrupts();           // disable all interrupts

  // Таймер#1: Скид при співпадінні OCR1A (1sec) + дільник=256
  TCCR1A = 0x00;
  TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10); //CTC mode & Prescaler @ 1024
  TIMSK1 = (1 << OCIE1A); // дозвіл на переривання по співпадінню
  OCR1A = 0x3D08;// compare value = 1 sec (16MHz AVR)
  
  //KeyPad 4x4
  initKeyPad();

  //LCD 16x2
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  interrupts();  // Enable global interrupts

  //Buzzer
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);
}

void loop() {
    if ( isButtonPressed() ) {
      digitalWrite(buzzerPin, HIGH);
      lcd.setCursor(0, 1);
      lcd.write(readKeyFromPad4x4());
      delay(100);
      digitalWrite(buzzerPin, LOW);
    }

}
