#include <Wire.h> //работа с шиной I2C (SDA-данные и SCI-тактовый сигнал)
#include <Adafruit_GFX.h> //для графики
#include <Adafruit_SSD1306.h> //драйвер для отправки картинки на экран
#include <EEPROM.h>  //энергозависимая память

#define PIN_BTN_UP 2
#define PIN_BTN_DOWN 3
#define PIN_BTN_LEFT 4
#define PIN_BTN_RIGHT 5
#define PIN_BUZZER  8
int wolfX=50;
int wolfFloor=0; //этаж от 0 - 2 (всего 3 шт)
int floorY[3]={50,30,10}; //система координат перевернута, 0.0 в левом верхнем углу
int eggX=0;
int eggY=0;
bool eggActive=false;
unsigned long TimerforEgg=0;
unsigned long TimerforUp=0;
unsigned long TimerforDown=0;
unsigned long TimerforLeft=0;
unsigned long TimerforRight=0;

Adafruit_SSD1306 display(128,64,&Wire,-1); //&Wire общение через шину I2C (провода к A4 и A5)
void setup(){
  Wire.begin();
  pinMode(PIN_BTN_UP,INPUT_PULLUP);
  pinMode(PIN_BTN_DOWN,INPUT_PULLUP);
  pinMode(PIN_BTN_LEFT,INPUT_PULLUP);
  pinMode(PIN_BTN_RIGHT,INPUT_PULLUP);
  pinMode(PIN_BUZZER,OUTPUT);
  
  display.begin(SSD1306_SWITCHCAPVCC,0x3C); 
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(15,25);
  display.print("START!");
  display.display();
  delay(500);
  while(digitalRead(PIN_BTN_UP)==HIGH && //пока не нажмется любая кнопка -> висит стартовый экран
        digitalRead(PIN_BTN_DOWN)==HIGH &&
        digitalRead(PIN_BTN_LEFT)==HIGH &&
        digitalRead(PIN_BTN_RIGHT)==HIGH) {
  }
}
void loop(){
  display.clearDisplay();
  
  if(digitalRead(PIN_BTN_LEFT)==LOW && millis()-TimerforLeft>200){
    wolfX-=10; 
    if (wolfX<0) wolfX=0;
    TimerforLeft=millis();
  }
  if(digitalRead(PIN_BTN_RIGHT)==LOW && millis()-TimerforRight>200){
    wolfX+=10; 
    if (wolfX>118) wolfX=118; //правая граница 128, а ширина волка 10
    TimerforRight=millis();
  }
  if(digitalRead(PIN_BTN_UP)==LOW && millis()-TimerforUp>200){
    if (wolfFloor<2) wolfFloor++;
    TimerforUp=millis();
  }
  if(digitalRead(PIN_BTN_DOWN)==LOW && millis()-TimerforDown>200){
    if (wolfFloor>0) wolfFloor--;
    TimerforDown=millis();
  }

  if(eggActive==false){
    eggActive=true;
    eggX=random(10,118);
    eggY=0;
  }
  if(eggActive==true && millis()-TimerforEgg >50){ //яйцо опускается на 2 пикселя каждые 50 миллисек
    eggY+=2;
    TimerforEgg=millis();
  }
  if(eggY>64){
    eggActive=false;
  }


  if(eggActive==true){
    if(eggY>=floorY[wolfFloor]-2 && eggY<=floorY[wolfFloor]+8 &&
        eggX>=wolfX-3 && eggX<=wolfX+10){ //
          eggActive=false;
          tone(PIN_BUZZER,1000,100);
        }
  }
  
  
  display.drawLine(0,28,127,28,SSD1306_WHITE); //средняя полка (х нач, у нач, х кон, у кон)
  display.drawLine(0,48,127,48,SSD1306_WHITE); //верхняя полка
  if(eggActive==true){
    display.fillCircle(eggX,eggY,3,SSD1306_WHITE); //круг радиус 3
  }
  display.fillRect(wolfX, floorY[wolfFloor], 10, 8, SSD1306_WHITE); //волк ширина 10, высота 8
  display.display();
 }
