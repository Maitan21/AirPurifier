/* ***************************************************************************** */
/* ************************** Header - LCD  ************************************ */
/* ***************************************************************************** */

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUMFLAKES     10 // Number of snowflakes in the animation example

#define LOGO_WIDTH    128
#define LOGO_HEIGHT   64

#include "bit_cat.h"

/* ***************************************************************************** */
/* ************************** Header - Dust Sensor  **************************** */
/* ***************************************************************************** */
int Vo = A0;
int V_LED = 2;

float Vo_value = 0;
float Voltage = 0;
float dustDensity = 0;

int red_light_pin = 11;
int green_light_pin = 10;
int blue_light_pin = 9;



/* ***************************************************************************** */
/* ************************** Header - Dust Sensor  **************************** */
/* ***************************************************************************** */
int FAN = A1;

void setup() {

/* ***************************************************************************** */
/* ************************** Setup - Dust Sensor  ***************************** */
/* ***************************************************************************** */
  Serial.begin(9600);

  pinMode(V_LED, OUTPUT);
  pinMode(Vo, INPUT);

  delay(1000);
  Serial.println();
  Serial.println("**************************************");
  Serial.println("***** Hello **************************");
  Serial.println("***** Welcome to ROBOFRIEN world *****");
  Serial.println("***** Thank you **********************");
  Serial.println("**************************************");
/* ***************************************************************************** */
/* ************************** Setup - RGB LED  ************************************* */
/* ***************************************************************************** */
 
  pinMode(red_light_pin,OUTPUT);
  pinMode(green_light_pin,OUTPUT);
  pinMode(blue_light_pin, OUTPUT);

/* ***************************************************************************** */
/* ************************** Setup - LCD  ************************************* */
/* ***************************************************************************** */

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  
  display.setTextSize(1); // Draw 2X-scale text
  display.setTextColor(WHITE);

/* ********************** Image Slide ************************ */  
  display.clearDisplay();
  display.drawBitmap( 0, 0, cat1, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display();
  delay(500);

  display.clearDisplay();
  display.drawBitmap( 0, 0, cat2, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display();
  delay(300);

  display.clearDisplay();
  display.drawBitmap( 0, 0, cat3, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display();
  delay(300);

  display.clearDisplay();
  display.drawBitmap( 0, 0, cat4, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display();
  delay(1000);

  for(int i=0;i<5;i++){
    display.clearDisplay();
    display.drawBitmap( 0, 0, cat5, LOGO_WIDTH, LOGO_HEIGHT, 1);
    display.display();
    delay(200);
  
    display.clearDisplay();
    display.drawBitmap( 0, 0, cat6, LOGO_WIDTH, LOGO_HEIGHT, 1);
    display.display();
    delay(200);
  
    display.clearDisplay();
    display.drawBitmap( 0, 0, cat7, LOGO_WIDTH, LOGO_HEIGHT, 1);
    display.display();
    delay(200);
  
    display.clearDisplay();
    display.drawBitmap( 0, 0, cat6, LOGO_WIDTH, LOGO_HEIGHT, 1);
    display.display();
    delay(200);
    display.clearDisplay();
    display.drawBitmap( 0, 0, cat5, LOGO_WIDTH, LOGO_HEIGHT, 1);
    display.display();
    delay(200);
  }
  
  display.clearDisplay();
  display.display();

/* ***************************************************************************** */
/* ************************** Setup - FAN  ************************************* */
/* ***************************************************************************** */
  pinMode(FAN, OUTPUT);
  digitalWrite(FAN, LOW);
/* ***************************************************************************** */
/* ************************** Setup - MotionDetect ***************************** */
/* ***************************************************************************** */
  pinMode(7,INPUT);
}


void loop() {

  RGB_color(138, 43, 226);
  display.clearDisplay();     //lcd 화면을 지웁니다.
  display.setCursor(10,10);     //lcd 커서 위치를 10,10로 위치시킵니다.
  display.println(F("KNU Air Cleaner"));
  display.setCursor(10,30);
  display.println("Detecting...");


  //감지됨
  if(digitalRead(7) == HIGH){
    
  /* ***************************************************************************** */
  /* ************************** Loop - Dust Sensor   ***************************** */
  /* ***************************************************************************** */
    digitalWrite(V_LED, LOW);
    delayMicroseconds(280);
    Vo_value = analogRead(Vo);
    delayMicroseconds(40);
    digitalWrite(V_LED, HIGH);
    delayMicroseconds(9680);

    Voltage = Vo_value / 1024 * 5.0;
    dustDensity = (Voltage - 0.3) / 0.005;
    Serial.println(dustDensity);

    delay(1000);

  /* ***************************************************************************** */
  /* ************************** Loop - LCD   ************************************* */
  /* ***************************************************************************** */
      display.clearDisplay();     //lcd 화면을 지웁니다.
      display.setCursor(10,10);     //lcd 커서 위치를 10,10로 위치시킵니다.
      display.println(F("KNU Air Cleaner"));
      display.setCursor(10,30);     //lcd 커서 위치를 10,30로 위치시킵니다.
      if( dustDensity > 150 ){    // 매우 나쁨 //
        display.println("AIR : VERY BAD!!");
        RGB_color(255,0,0); // RED
      }else if( dustDensity > 80){   // 나쁨 //
        display.println("AIR : BAD!      ");   
        RGB_color(255,165,0); // Orange
      }else if( dustDensity > 30){  // 보통 //
        display.println("AIR : NORMAL     ");
        RGB_color(0,0,255);  //Blue   
      }else{                    // 좋음
        display.println("AIR : GOOD      ");
        RGB_color(0,255,0); //Green 초록 표시   
      }
      display.setCursor(10,40);     //lcd 커서 위치를 10,40로 위치시킵니다.
      display.print("ug/m3:");
      display.println(dustDensity);
      display.drawBitmap( 104, 46, cat_foot, 24, 18, 1);

      display.display();


  /* ***************************************************************************** */
  /* ************************** Loop - FAN   ************************************* */
  /* ***************************************************************************** */
    if(dustDensity > 80){
      digitalWrite(FAN, HIGH);
      
    }else{
      digitalWrite(FAN, LOW);  
    }

    delay(1000);

  /* ***************************************************************************** */
  /* ************************** Loop - RGB ************************************* */
  /* ***************************************************************************** */
    
  }
}


/* RGB Fuction */
void RGB_color(int red_light_value, int green_light_value, int blue_light_value)
{
  analogWrite(red_light_pin,red_light_value);
  analogWrite(green_light_pin, green_light_value);
  analogWrite(blue_light_pin,blue_light_value);
}
