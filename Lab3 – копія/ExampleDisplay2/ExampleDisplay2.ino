#include <LiquidCrystal.h>

#define DDR_KEYPAD  DDRK
#define PORT_KEYPAD PORTK
#define PIN_KEYPAD  PINK

#include "keypad4x4.h"
#include <string.h>

const int buzzerPin = 21;
int state = 1;
int numberElementArray = 0;
int numberList = 0;
char saveDate[60][10];
int roomSelection = 0;
int selectNumber[3];
int counterSelectNumber = 0;
int nub1 = 0;
int nub2 = 0;

const int rs = 22, rw = 24, en = 26 , d4 = 57, d5 = 56, d6 = 55, d7 = 54;
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

ISR(TIMER4_COMPA_vect)  // Таймер Т4 по співпадінню А, кожної 1 сек.
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
}

void LCD_WriteStrPROGMEM(char *str, int n)  //вивід масиву символів,
{ //записаних у флеші
  for (int i = 0; i < n; i++)
    lcd.print( (char)pgm_read_byte( &(str[i]) ) );
}

void updateDate () {
    lcd.setCursor(3, 0);
    LCD_WriteStrPROGMEM(sixty[T2.hour], 2);
    lcd.write(':');
    LCD_WriteStrPROGMEM(sixty[T2.minute], 2);
    lcd.write(':');
    LCD_WriteStrPROGMEM(sixty[T2.second], 2);
}

void eventKey() {
    if ( 'A' == readKeyFromPad4x4() ) {
      char bufA[4];
      sprintf(bufA,"N %i",numberElementArray);
      lcd.setCursor(3, 1);
      lcd.write(bufA);

      roomSelection = 0;
    }

    if ( 'B' == readKeyFromPad4x4() ) {
      roomSelection = 1;
      char bufB[15];
      sprintf(bufB,"%s -%i",saveDate[numberList],numberList+1);
      lcd.setCursor(3, 1);
      if (numberList < numberElementArray ) {
        lcd.write(bufB);
      }
      numberList++;
    }else{
      numberList = 0;
    }
    
    if ( 'C' == readKeyFromPad4x4() ) {
      state = 0;
      noInterrupts();
      T2.second = 0;
      T2.minute = 0;
      T2.hour = 0;

      roomSelection = 0;
    }

    if ( 'D' == readKeyFromPad4x4() ) {
      state = !state;
      if ( 1 == state ) {
        interrupts();
      }
      if ( 0 == state ) {
        noInterrupts();
      }
      roomSelection = 0;
    }
    
    if ( 'E' == readKeyFromPad4x4() ) { 
      if ( 1 == roomSelection && 0 < selectNumber[0] ) {
        if ( 1 == counterSelectNumber ) {
          char bufEN1[15];
          nub1 = selectNumber[0]-1;
          sprintf(bufEN1,"%s -%i",saveDate[nub1],nub1+1);
          selectNumber[0] = 0;
          lcd.setCursor(3, 1);
          lcd.write(bufEN1);
          counterSelectNumber = 0;
        }
        if ( 2 == counterSelectNumber ) {
          char bufEN2[15];
          nub2 = (selectNumber[0] * 10 + selectNumber[1])-1;
          if ( 60 < nub2 ) {
            digitalWrite(buzzerPin, HIGH);
            delay(1000);
            digitalWrite(buzzerPin, LOW);
          }else {
            sprintf(bufEN2,"%s",saveDate[nub2]);
            selectNumber[0]=0;
            selectNumber[1]=0;
            lcd.setCursor(3, 1);
            lcd.write(bufEN2);
            counterSelectNumber = 0;
          }
        }
        
      }else {    
        if( 60 == numberElementArray ) {
          memset(saveDate,"",60);
          numberElementArray = 0;
        }else {
          char bufE1[15];
          sprintf(bufE1,"%i:%i:%i",T2.hour,T2.minute,T2.second);
          strcpy(saveDate[numberElementArray],bufE1);
          char bufE2[15];
          sprintf(bufE2,"%s",saveDate[numberElementArray]);
          lcd.setCursor(3, 1);
          lcd.write(bufE2);
        }
        numberElementArray++;
        roomSelection = 0;
      }
    }
    
    if ( 'F' == readKeyFromPad4x4() ) {
      memset(saveDate,"",60);
      numberElementArray = 0;
      char bufF[15];
      sprintf(bufF,"N %i",numberElementArray);
      lcd.setCursor(3, 1);
      lcd.write(bufF);
      roomSelection = 0;
    }
    eventNumber ();
}

void eventNumber () {
    if ( '1' == readKeyFromPad4x4() && 1 == roomSelection ) {
      selectNumber[counterSelectNumber] = 1;
      numberEventsUpdate ();
    }
    if ( '2' == readKeyFromPad4x4() && 1 == roomSelection ) {
      selectNumber[counterSelectNumber] = 2;
      numberEventsUpdate ();
    }
    if ( '3' == readKeyFromPad4x4() && 1 == roomSelection ) {
      selectNumber[counterSelectNumber] = 3;
      numberEventsUpdate ();
    }
    if ( '4' == readKeyFromPad4x4() && 1 == roomSelection ) {
      selectNumber[counterSelectNumber] = 4;
      numberEventsUpdate ();
    }
    if ( '5' == readKeyFromPad4x4() && 1 == roomSelection ) {
      selectNumber[counterSelectNumber] = 5;
      numberEventsUpdate ();
    }
    if ( '6' == readKeyFromPad4x4() && 1 == roomSelection ) {
      selectNumber[counterSelectNumber] = 6;
      numberEventsUpdate ();
    }
    if ( '7' == readKeyFromPad4x4() && 1 == roomSelection ) {
      selectNumber[counterSelectNumber] = 7;
      numberEventsUpdate ();
    }
    if ( '8' == readKeyFromPad4x4() && 1 == roomSelection ) {
      selectNumber[counterSelectNumber] = 8;
      numberEventsUpdate ();
    }
    if ( '9' == readKeyFromPad4x4() && 1 == roomSelection ) {
      selectNumber[counterSelectNumber] = 9;
      numberEventsUpdate ();
    }
    if ( '0' == readKeyFromPad4x4() && 1 == roomSelection ) {
      selectNumber[counterSelectNumber] = 0;
      numberEventsUpdate ();
    }
}

void writeKey () {
  lcd.setCursor(0, 1);
  lcd.write(readKeyFromPad4x4());
}

void numberEventsUpdate () {
  counterSelectNumber++;
  writeKey ();
  if ( 3 == counterSelectNumber ){
    counterSelectNumber = 0;
    digitalWrite(buzzerPin, HIGH);
    delay(100);
    digitalWrite(buzzerPin, LOW);
  }
}

void setup() {
  noInterrupts();           // disable all interrupts

  // Таймер#4: Скид при співпадінні OCR1A (1sec) + дільник=256
  TCCR4A = 0x00;
  TCCR4B = (1 << WGM12) | (1 << CS12) | (1 << CS10); //CTC mode & Prescaler @ 1024
  TIMSK4 = (1 << OCIE1A); // дозвіл на переривання по співпадінню
  OCR4A = 0x3D08;// compare value = 1 sec (16MHz AVR)

//  TCCR1A = 0x00;
//  TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10); //CTC mode & Prescaler @ 1024
//  TIMSK1 = (1 << OCIE1A); // дозвіл на переривання по співпадінню
//  OCR1A = 0x3D08;// compare value = 1 sec (16MHz AVR)
  
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
  updateDate ();
  if ( isButtonPressed() ) {
    lcd.clear();
    digitalWrite(buzzerPin, HIGH);
    eventKey();
    delay(100);
    digitalWrite(buzzerPin, LOW);
  }
}
