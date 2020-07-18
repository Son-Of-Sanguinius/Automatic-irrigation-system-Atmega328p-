#include <Keypad.h>
#include <LiquidCrystal.h>
#include <Servo.h>
#include "DHT.h"
#define DHTPIN A0

Servo myservo;

DHT dht(DHTPIN, DHT11);

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const byte ROWS = 4; 
const byte COLS = 3;

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

byte rowPins[ROWS] = {8, A1, 6, 0}; 
byte colPins[COLS] = {13, 10, 9}; 

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

char button;
float temperature;
float humidity;
int val;
float h; 
float t;

void setup() {
  Serial.begin(9600);
  myservo.attach(7, 0, 180);
  lcd.begin(20, 4);
  dht.begin();
  h = dht.readHumidity();
  t = dht.readTemperature();
  lcd.print("1)Set temperature");
  lcd.setCursor(0,1);
  lcd.print("2)Set humidity");
  lcd.setCursor(0,2);
  lcd.print("0)Start");
  lcd.print("   *)Stop");
  Serial.print(h);
}

void loop() {
 while(1){
   h = dht.readHumidity();
   t = dht.readTemperature();
   button = customKeypad.getKey();
   lcd.setCursor(0,3);
   lcd.print("t:");
   lcd.print(t);
   lcd.print("  h:");
   lcd.print(h);
   if(button){
     break;
   }
  }
    
  while(1){
    h = dht.readHumidity();
    t = dht.readTemperature();
    if(button == '*'){
         h = dht.readHumidity();
         t = dht.readTemperature();
         button = customKeypad.getKey();
         lcd.clear();
         lcd.setCursor(0,0);
         lcd.print("1)Set temperature");
         lcd.setCursor(0,1);
         lcd.print("2)Set humidity");
         lcd.setCursor(0,2);
         lcd.print("0)Start");
         lcd.print("   *)Stop");
         
         while(1){
           h = dht.readHumidity();
           t = dht.readTemperature();
           button = customKeypad.getKey();
           lcd.setCursor(0,3);
           lcd.print("t:");
           lcd.print(t);
           lcd.print("  h:");
           lcd.print(h);
           if(button){
             break;
           }
         }
    }
    
    button = customKeypad.getKey();

    if(button == '1'){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Temperature:");
      lcd.print(temperature);
      lcd.setCursor(0,1);
      lcd.print(0.00);
      lcd.setCursor(0,2);
      lcd.print("*)Set");
      lcd.print("   #)Clear");
      button = " ";
      while(1){

        button = customKeypad.getKey();
        if(button >= '0' && button <= '9'){
            temperature = temperature*10 + (button - '0');
            lcd.setCursor(0,1);
            lcd.print(temperature);
        }

        if(button == '*'){
          if(temperature > 50){
            lcd.clear();
            while(1){
              button = customKeypad.getKey();
              temperature = 0.00;
              lcd.setCursor(0,0);
              lcd.print("Error!!!");
              lcd.setCursor(0,1);
              lcd.print("t > 50 degrees");
              lcd.setCursor(0,2);
              lcd.print("*)OK");
              if(button == '*'){
                break;
              }
            }
          }
          break;
        }

        if(button == '#'){
          lcd.clear();
          temperature = 0;
          lcd.setCursor(0,0);
          lcd.print("Temperature:");
          lcd.print(temperature);
          lcd.setCursor(0,2);
          lcd.print("*)Set");
          lcd.print("   #)Clear");
          lcd.setCursor(0,1);
          lcd.print(temperature);
        }
      }
    }
    if(button == '2'){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Humidity:");
      lcd.print(humidity);
      lcd.setCursor(0,1);
      lcd.print(0.00);
      lcd.setCursor(0,2);
      lcd.print("*)Set");
      lcd.print("   #)Clear");
      button = " ";
      
      while(1){
        button = customKeypad.getKey();
        if(button >= '0' && button <= '9'){
            humidity = humidity*10 + (button - '0');
            lcd.setCursor(0,1);
            lcd.print(humidity);
        }

        if(button == '*'){
          if(humidity < 20 || humidity > 80){
            lcd.clear();
            while(1){
              button = customKeypad.getKey();
              humidity = 0.00;
              lcd.setCursor(0,0);
              lcd.print("Error!!!");
              lcd.setCursor(0,1);
              lcd.print("h>80 or h<20 percent");
              lcd.setCursor(0,2);
              lcd.print("*)OK");
              if(button == '*'){
                break;
              }
            }
          }
          break;
        }

        if(button == '#'){
          lcd.clear();
          humidity = 0;
          lcd.setCursor(0,0);
          lcd.print("Humidity:");
          lcd.print(humidity);
          lcd.setCursor(0,2);
          lcd.print("*)Set");
          lcd.print("   #)Clear");
          lcd.setCursor(0,1);
          lcd.print(humidity);
        }
      }
    }
    if(button == '0'){
      while(1){
        button = customKeypad.getKey();
        h = dht.readHumidity();
        t = dht.readTemperature();
        if(h < humidity){
          myservo.write(180);
        }

        if(h >= humidity){
          myservo.write(70);
        }

        if(button == '*'){
          myservo.write(70);
          break;
        }
      }
    }
  }
  Serial.println(button);
}
