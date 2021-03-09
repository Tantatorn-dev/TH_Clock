#include <Arduino.h>
#include <Wire.h>
#include <DS1302.h>

DS1302 rtc(8, 7, 6);

const int RS = 12;
const int E = 11;
const int D4 = 5;
const int D5 = 4;
const int D6 = 3;
const int D7 = 2;


bool bFourBitMode = false;
char ReadSendState = -1;

int h_1 = 0;
int h_0 = 1;
int m_1 = 2;
int m_0 = 3;
int s_1 = 4;
int s_0 = 5;

unsigned char Thai_0[8] =
{
	0b00000,
	0b00000,
	0b01110,
	0b10001,
	0b10001,
	0b10001,
	0b01110,
	0b00000
};

unsigned char Thai_1[8] =
{
	0b00000,
	0b00000,
	0b01110,
	0b10001,
	0b11001,
	0b00001,
	0b01110,
	0b00000
};

unsigned char Thai_2[8] =
{
	0b00000,
	0b10000,
	0b10110,
	0b10101,
	0b10001,
	0b10001,
	0b01110,
	0b00000
};

unsigned char Thai_3[8] =
{
	0b00000,
	0b00000,
	0b01110,
	0b10101,
	0b10101,
	0b10001,
	0b11001,
	0b00000
};

unsigned char Thai_4[8] =
{
	0b00000,
	0b00001,
	0b00110,
	0b01000,
	0b10010,
	0b10100,
	0b01111,
	0b00000
};


unsigned char Thai_5[8] =
{
	0b00000,
	0b01101,
	0b01110,
	0b01000,
	0b10010,
	0b10100,
	0b01111,
	0b00000
};

unsigned char Thai_6[8] =
{
	0b00000,
	0b10110,
	0b01001,
	0b00001,
	0b01101,
	0b01001,
	0b00110,
	0b00000
};


unsigned char Thai_7[8] =
{
	0b00000,
	0b00001,
	0b01111,
	0b10101,
	0b10101,
	0b10001,
	0b11001,
	0b00000
};

unsigned char Thai_8[8] =
{
	0b00000,
	0b00001,
	0b01110,
	0b10000,
	0b10001,
	0b10101,
	0b01110,
	0b00000
};

unsigned char Thai_9[8] =
{
	0b00000,
	0b00001,
	0b01110,
	0b11000,
	0b10100,
	0b10010,
	0b11001,
	0b00000
};

void LcdSend(unsigned char Data)
{
  if (bFourBitMode)
  {
    digitalWrite(D4, (Data >> 4) & 0x01);
    digitalWrite(D5, (Data >> 5) & 0x01);
    digitalWrite(D6, (Data >> 6) & 0x01);
    digitalWrite(D7, (Data >> 7) & 0x01);
   
    delayMicroseconds(10);
    digitalWrite(E, HIGH);
    delayMicroseconds(10);
    digitalWrite(E, LOW);
    delayMicroseconds(100);
  }
 
  digitalWrite(D4, (Data >> 0) & 0x01);
  digitalWrite(D5, (Data >> 1) & 0x01);
  digitalWrite(D6, (Data >> 2) & 0x01);
  digitalWrite(D7, (Data >> 3) & 0x01);
 
  delayMicroseconds(10);
  digitalWrite(E, HIGH);
  delayMicroseconds(10);
  digitalWrite(E, LOW);
  delayMicroseconds(100);
}

void LcdCommand(unsigned char Command)
{
  if (ReadSendState != LOW)
  {
    ReadSendState = LOW;
    digitalWrite(RS, LOW);
  }
 
  LcdSend(Command);
  if (Command == 0x01) delayMicroseconds(2000);// Clear command takes more time
}

void LcdWrite(int Letter)
{
  if (ReadSendState != HIGH)
  {
    ReadSendState = HIGH;
    digitalWrite(RS, HIGH);
  }
 
  LcdSend(Letter);
}

void LcdWrite(const char* Text)
{
  if (ReadSendState != HIGH)
  {
    ReadSendState = HIGH;
    digitalWrite(RS, HIGH);
  }
 
  for (; *Text != 0; Text++)
  {
    char Letter = *Text;
    LcdSend(Letter);
  }
}

void LcdSetCursor(unsigned char Column, unsigned char Row)
{
  LcdCommand(0x80 | (Column + (Row != 0 ? 0x40 : 0x00)));
}

void LcdCreateChar(unsigned char Location, unsigned char SpecialChar[8])
{
  LcdCommand(0x40 | (Location << 3));
  for (unsigned int i = 0; i < 8; i++)
    LcdWrite(SpecialChar[i]);
}

void LcdInit(bool bFirstInit)
{
  if (bFirstInit)
  {
    // Give it time to power up
    delayMicroseconds(15000);
   
    pinMode(RS, OUTPUT);
    pinMode(E, OUTPUT);
    pinMode(D4, OUTPUT);
    pinMode(D5, OUTPUT);
    pinMode(D6, OUTPUT);
    pinMode(D7, OUTPUT);
  }
 
  // Start
  bFourBitMode = false;
 
  LcdCommand(0x03);
 
  delayMicroseconds(4000);
 
  LcdCommand(0x03);
  LcdCommand(0x03);
  LcdCommand(0x02);
 
  bFourBitMode = true;
 
  LcdCommand(0x28);
  LcdCommand(0x0C);
  LcdCommand(0x01);// Clear
  LcdCommand(0x06);

}

void SetupFont(int position,int number){
  switch(number){
    case 0:{
        LcdCreateChar(position, Thai_0);
    	break;
    }
    case 1:{
        LcdCreateChar(position, Thai_1);
    	break;
    }
    case 2:{
        LcdCreateChar(position, Thai_2);
    	break;
    }
    case 3:{
        LcdCreateChar(position, Thai_3);
    	break;
    }
    case 4:{
        LcdCreateChar(position, Thai_4);
    	break;
    }
    case 5:{
        LcdCreateChar(position, Thai_5);
    	break;
    }
    case 6:{
        LcdCreateChar(position, Thai_6);
    	break;
    }
    case 7:{
        LcdCreateChar(position, Thai_7);
    	break;
    }
    case 8:{
        LcdCreateChar(position, Thai_8);
    	break;
    }
    case 9:{
        LcdCreateChar(position, Thai_9);
    	break;
    }
  }
}
int time[6] = {1,2,4,5,5,5};

void set_clock() {
  rtc.setDOW(WEDNESDAY);
  rtc.setTime(16,18,0);
  rtc.setDate(9, 3, 2021);
}

void set_start_digit() {
  char *time_str = rtc.getTimeStr();
  
  
  time[0] = time_str[0] - '0';
  time[1] = time_str[1] - '0';
  time[2] = time_str[3] - '0';
  time[3] = time_str[4] - '0';
  time[4] = time_str[6] - '0';
  time[5] = time_str[7] - '0';

}

void setup()
{
  rtc.halt(false);
  rtc.writeProtect(false);

  Serial.begin(9600);
  LcdInit(true);
  for(int idx = 0 ; idx < 6 ;idx++){
  	SetupFont(idx,time[5 - idx]);
  }

  // use this only when you want to set clock
  // set_clock();

  set_start_digit();
}

void loop()
{
  Serial.println(rtc.getTimeStr());
  delay(987);

  time[s_0] += 1;
  if( time[s_0] == 10 ){
  	time[s_0] = 0;
    time[s_1] += 1;
  }
  if( time[s_1] == 6 ){
  	time[s_1] = 0;
    time[m_0] += 1;
  }
  if( time[m_0] == 10 ){
  	time[m_0] = 0;
    time[m_1] += 1;
  }
  if( time[m_1] == 6){
  	time[m_1] = 0;
    time[h_0] += 1;
  }
  if( time[h_1] < 2 && time[h_0] == 10){
  	time[h_0] = 0;
    time[h_1] += 1; 
  }
  if( time[h_1] == 2 && time[h_0] == 4)
  {
    time[h_0] = 0;
    time[h_1] = 0;
  }
	
  for(int idx = 0 ; idx < 6 ;idx++){
  	SetupFont(idx,time[5-idx]);
  }
  
  int off_set = 4;
  int digit = 5;
  for(int idx = 0 ; idx < 8; idx ++){
  	LcdSetCursor(idx + off_set, 1);
    if(idx == 2 || idx == 5){
    	LcdWrite(":");
    }else{
    	LcdWrite(digit);
      	digit--;
    }
  }

}