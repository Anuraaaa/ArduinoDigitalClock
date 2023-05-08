#include <SPI.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width,  in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); // // create SSD1306 display object connected to I2C
SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

#include <virtuabotixRTC.h> 
virtuabotixRTC myRTC(6, 7, 8);

const int BUTTON_PIN = 2;       // the number of the pushbutton pin
int lastState = LOW;  // the previous state from the input pin
int currentState;                // the current reading from the input pin

const float pi = 3.14159267 ;
const int clock_center_x=64;
const int clock_center_y=32;
int o=1;

int x;
int y;
int x1;
int y1;

unsigned long previousTimeCheck = 0;
byte secondsCheck ;
byte minutesCheck ;
byte hoursCheck ;

int seconds=0;
int minutes;
int hours;

void setup() {
  Serial.begin(9600);
  
//  myRTC.setDS1302Time(30, 13 , 12 , 03, 03, 05, 2023);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  
  display.clearDisplay();
  line_center_clock();   
  display.display();


  mySoftwareSerial.begin(9600);
  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(1);
  }
  else
  {
    Serial.println(F("STATUS: DFPlayer Mini connected."));
  }
  myDFPlayer.setTimeOut(500); //Set serial communictaion time out 500ms
  
  //----Set volume----
  myDFPlayer.volume(30);  //Set volume value (0~30).
  myDFPlayer.volumeUp(); //Volume Up
  myDFPlayer.volumeDown(); //Volume Down
  
  //----Set different EQ----
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);
//  myDFPlayer.EQ(DFPLAYER_EQ_POP);
//  myDFPlayer.EQ(DFPLAYER_EQ_ROCK);
//  myDFPlayer.EQ(DFPLAYER_EQ_JAZZ);
//  myDFPlayer.EQ(DFPLAYER_EQ_CLASSIC);
//  myDFPlayer.EQ(DFPLAYER_EQ_BASS);
  
  //----Set device we use SD as default----
//  myDFPlayer.outputDevice(DFPLAYER_DEVICE_U_DISK);
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);

}

void line_second(int second, int mode){
   y= (20*cos(pi-(2*pi)/60*second))+clock_center_y;
   x =(20*sin(pi-(2*pi)/60*second))+clock_center_x;
   if (mode==1) display.drawCircle(x, y, 2, WHITE); 
   else display.drawCircle(x, y, 2, BLACK);
}

void line_hour(int hour, int minute, int mode){
   y= (18*cos(pi-(2*pi)/12*hour-(2*PI)/720*minute))+clock_center_y;
   x =(18*sin(pi-(2*pi)/12*hour-(2*PI)/720*minute))+clock_center_x;
   y1=(18*cos(pi-(2*pi)/12*hour-(2*PI)/720*minute))+clock_center_y+1;
   x1=(18*sin(pi-(2*pi)/12*hour-(2*PI)/720*minute))+clock_center_x+1;
   if (mode==1){
    display.drawLine(clock_center_x,clock_center_y,x,y,WHITE);
    display.drawLine(clock_center_x+1,clock_center_y+1,x1,y1,WHITE);
   }
   else{
    display.drawLine(clock_center_x,clock_center_y,x,y,BLACK);
    display.drawLine(clock_center_x+1,clock_center_y+1,x1,y1,BLACK);
   }  
}

void line_minute(int minute, int mode){
   y= (24*cos(pi-(2*pi)/60*minute))+clock_center_y;
   x =(24*sin(pi-(2*pi)/60*minute))+clock_center_x;
   if (mode==1)display.drawLine(clock_center_x,clock_center_y,x,y,WHITE); else display.drawLine(clock_center_x,clock_center_y,x,y,BLACK);
}

void line_center_clock(void){
  display.drawCircle(clock_center_x, clock_center_y,3, WHITE);
  display.fillCircle(clock_center_x, clock_center_y,3, WHITE);
  display.drawCircle(clock_center_x, clock_center_y,36, WHITE);

for (int i=0;i<12;i++){
   y= (32*cos(pi-(2*pi)/12*((i*2)-1)))+clock_center_y;
   x =(32*sin(pi-(2*pi)/12*((i*2)-1)))+clock_center_x;
   y1= (28*cos(pi-(2*pi)/12*((i*2)-1)))+clock_center_y;
   x1 =(28*sin(pi-(2*pi)/12*((i*2)-1)))+clock_center_x;
   display.drawLine(x1,y1,x,y,WHITE);
}

  display.drawCircle(26*sin(pi)+clock_center_x, (26*cos(pi))+clock_center_y, 6, BLACK);
  display.fillCircle(26*sin(pi)+clock_center_x, (26*cos(pi))+clock_center_y, 5, BLACK);
  display.setTextSize(1);             
  display.setTextColor(WHITE);        
  display.setCursor(clock_center_x-3,0);             
  display.println(F("12"));
  display.setCursor(36,16);             
  display.println(F("10"));
  display.setCursor(88,16);             
  display.println(F("2"));
  display.setCursor(clock_center_x-3,56);             
  display.println(F("6"));
  display.setCursor(86,45);             
  display.println(F("4"));
  display.setCursor(36,45);             
  display.println(F("8"));
}

void line_center_clock_elements(void){
    display.drawCircle(clock_center_x, clock_center_y,3, WHITE);
    display.fillCircle(clock_center_x, clock_center_y,3, WHITE);
    display.setCursor(clock_center_x-3,0);             
    display.println(F("12"));
    display.setCursor(36,16);
    display.println(F("10"));
    display.setCursor(88,16);             
    display.println(F("2"));
    display.setCursor(clock_center_x-3,56);             
    display.println(F("6"));
    display.setCursor(86,45);             
    display.println(F("4"));
    display.setCursor(36,45);             
    display.println(F("8"));
}

void SetTime()
{
  myRTC.updateTime();
  if (myRTC.seconds!=seconds)
  {
    line_second(seconds,0);
    line_minute(minutes,0);
    line_hour(hours,minutes,0);
    line_second(myRTC.seconds,1);
    line_minute(myRTC.minutes,1);
    line_hour(myRTC.hours,myRTC.minutes,1);
    display.display(); 
    seconds=myRTC.seconds;
    minutes=myRTC.minutes;
    hours=myRTC.hours;
    line_center_clock_elements();
  }
}

void TriggerButton()
{
  currentState = digitalRead(BUTTON_PIN);
  if(lastState == HIGH && currentState == LOW)
  {
    Serial.print("Sekarang pukul: ");
    Serial.print(hours);
    Serial.print(":");
    Serial.print(minutes);
    Serial.print(":");
    Serial.println(seconds); 
    suarawaktu(hours, minutes); 
  }  
  lastState = currentState;  
}

void loop() 
{
//  if (millis() >= previousTimeCheck) 
//  {
//     previousTimeCheck = ((1 * 60) + (5000)); 
//     secondsCheck = secondsCheck +1;
//     if (secondsCheck == 60)
//     {
//        secondsCheck = 0;
//        minutesCheck = minutesCheck +1;
//     }
//     if (minutesCheck == 60)
//     {
//        minutesCheck = 0;
//        hoursCheck = hoursCheck +1;
//     }
//     if (hoursCheck == 13)
//     {
//        hoursCheck = 1;
//     }
//      SetTime();
//  }
//  TriggerButton();
  SetTime();
  TriggerButton();  
}

void suarawaktu(int hours, int minutes)
{
  if (hours > 0 && hours < 10)
  {
    if (minutes == 0)
    {
      myDFPlayer.playMp3Folder(17); //pukul
      delay(1500);
      myDFPlayer.playMp3Folder(hours);              
      delay(1000);
      myDFPlayer.playMp3Folder(13);              
      delay(1000);                          
    }
    else if (minutes > 0 && minutes < 10)
    {
      myDFPlayer.playMp3Folder(17); //pukul
      delay(1500);
      myDFPlayer.playMp3Folder(hours);              
      delay(1000);
      myDFPlayer.playMp3Folder(0);              
      delay(1500);
      myDFPlayer.playMp3Folder(minutes);              
      delay(1500);                    
      myDFPlayer.playMp3Folder(13);              
      delay(1000);                    
                                                   
    }
    else if (minutes > 10 && minutes < 20)
    {
      if (minutes == 11)
      {
        myDFPlayer.playMp3Folder(17); //pukul
        delay(1500);
        myDFPlayer.playMp3Folder(hours);              
        delay(1000);
        myDFPlayer.playMp3Folder(10);              
        delay(1500);
        myDFPlayer.playMp3Folder(12);              
        delay(1500);                    
        myDFPlayer.playMp3Folder(13);              
        delay(1000);   
                                                     
      }
      else
      {
        char str[2];
        sprintf(str, "%d", minutes);
        int satuan = str[1] - '0';   
               
        myDFPlayer.playMp3Folder(17); //pukul
        delay(1500);
        myDFPlayer.playMp3Folder(hours);              
        delay(1000);
        myDFPlayer.playMp3Folder(satuan);
        delay(1500);
        myDFPlayer.playMp3Folder(12);              
        delay(1500);                    
        myDFPlayer.playMp3Folder(13);              
        delay(1000);   
                                                     
      }
    }
    else if (minutes == 20 || minutes == 30 || minutes == 40 || minutes == 50)
    {
        char str[2];
        sprintf(str, "%d", minutes);
        int satuan = str[0] - '0';   
               
        myDFPlayer.playMp3Folder(17); //pukul
        delay(1500);
        myDFPlayer.playMp3Folder(hours);              
        delay(1000);
        myDFPlayer.playMp3Folder(satuan);
        delay(1500);
        myDFPlayer.playMp3Folder(11);              
        delay(1500);                    
        myDFPlayer.playMp3Folder(13);              
        delay(1000);   
                                                     
    }
    else if (minutes > 20 && minutes < 30 || minutes > 30 && minutes < 40 || minutes > 40 && minutes < 50 || minutes > 50 && minutes < 60)
    {
        char str[2];
        sprintf(str, "%d", minutes);
        int satuanAwal = str[0] - '0';   
        int satuanAkhir = str[1] - '0';   
               
        myDFPlayer.playMp3Folder(17); //pukul
        delay(1500);
        myDFPlayer.playMp3Folder(hours);              
        delay(1000);
        myDFPlayer.playMp3Folder(satuanAwal);
        delay(1500);
        myDFPlayer.playMp3Folder(satuanAkhir);              
        delay(1500);                    
        myDFPlayer.playMp3Folder(13);              
        delay(1000);   
                                                                     
    }
  }  
  else if (hours == 10)
  {
    if (minutes == 0)
    {
      myDFPlayer.playMp3Folder(17); //pukul
      delay(1500);
      myDFPlayer.playMp3Folder(10);              
      delay(1000);
      myDFPlayer.playMp3Folder(11);              
      delay(1500);
      myDFPlayer.playMp3Folder(13);              
      delay(1000);                          
    }
    else if (minutes > 0 && minutes < 10)
    {
      myDFPlayer.playMp3Folder(17); //pukul
      delay(1500);
      myDFPlayer.playMp3Folder(10);              
      delay(1000);
      myDFPlayer.playMp3Folder(11);              
      delay(1500);
      myDFPlayer.playMp3Folder(0);              
      delay(1500);
      myDFPlayer.playMp3Folder(minutes);              
      delay(1500);                    
      myDFPlayer.playMp3Folder(13);              
      delay(1000);                    
                                                   
    }
    else if (minutes > 10 && minutes < 20)
    {
      if (minutes == 11)
      {
        myDFPlayer.playMp3Folder(17); //pukul
        delay(1500);
        myDFPlayer.playMp3Folder(10);              
        delay(1000);
        myDFPlayer.playMp3Folder(11);              
        delay(1500);
        myDFPlayer.playMp3Folder(10);              
        delay(1500);
        myDFPlayer.playMp3Folder(12);              
        delay(1500);                    
        myDFPlayer.playMp3Folder(13);              
        delay(1000);   
                                                     
      }
      else
      {
        char str[2];
        sprintf(str, "%d", minutes);
        int satuan = str[1] - '0';   
               
        myDFPlayer.playMp3Folder(17); //pukul
        delay(1500);
        myDFPlayer.playMp3Folder(10);              
        delay(1000);
        myDFPlayer.playMp3Folder(11);              
        delay(1500);
        myDFPlayer.playMp3Folder(satuan);
        delay(1500);
        myDFPlayer.playMp3Folder(12);              
        delay(1500);                    
        myDFPlayer.playMp3Folder(13);              
        delay(1000);   
                                                     
      }
    }
    else if (minutes == 20 || minutes == 30 || minutes == 40 || minutes == 50)
    {
        char str[2];
        sprintf(str, "%d", minutes);
        int satuan = str[0] - '0';   
               
        myDFPlayer.playMp3Folder(17); //pukul
        delay(1500);
        myDFPlayer.playMp3Folder(10);              
        delay(1000);
        myDFPlayer.playMp3Folder(11);              
        delay(1500);
        myDFPlayer.playMp3Folder(satuan);
        delay(1500);
        myDFPlayer.playMp3Folder(11);              
        delay(1500);                    
        myDFPlayer.playMp3Folder(13);              
        delay(1000);   
                                                     
    }
    else if (minutes > 20 && minutes < 30 || minutes > 30 && minutes < 40 || minutes > 40 && minutes < 50 || minutes > 50 && minutes < 60)
    {
        char str[2];
        sprintf(str, "%d", minutes);
        int satuanAwal = str[0] - '0';   
        int satuanAkhir = str[1] - '0';   
               
        myDFPlayer.playMp3Folder(17); //pukul
        delay(1500);
        myDFPlayer.playMp3Folder(10);              
        delay(1000);
        myDFPlayer.playMp3Folder(11);              
        delay(1500);
        myDFPlayer.playMp3Folder(satuanAwal);
        delay(1500);
        myDFPlayer.playMp3Folder(satuanAkhir);              
        delay(1500);                    
        myDFPlayer.playMp3Folder(13);              
        delay(1000);   
                                                                     
    }                
  }
  else if (hours == 11)
  {
    if (minutes == 0)
    {
      myDFPlayer.playMp3Folder(17); //pukul
      delay(1500);
      myDFPlayer.playMp3Folder(10);              
      delay(1000);
      myDFPlayer.playMp3Folder(12);              
      delay(1500);
      myDFPlayer.playMp3Folder(14);              
      delay(1000);                          
    }
    else if (minutes > 0 && minutes < 10)
    {
      myDFPlayer.playMp3Folder(17); //pukul
      delay(1500);
      myDFPlayer.playMp3Folder(10);              
      delay(1000);
      myDFPlayer.playMp3Folder(12);              
      delay(1500);
      myDFPlayer.playMp3Folder(0);              
      delay(1500);
      myDFPlayer.playMp3Folder(minutes);              
      delay(1500);                    
      myDFPlayer.playMp3Folder(14);              
      delay(1000);                    
                                                   
    }
    else if (minutes > 10 && minutes < 20)
    {
      if (minutes == 11)
      {
        myDFPlayer.playMp3Folder(17); //pukul
        delay(1500);
        myDFPlayer.playMp3Folder(10);              
        delay(1000);
        myDFPlayer.playMp3Folder(12);              
        delay(1500);
        myDFPlayer.playMp3Folder(10);              
        delay(1500);
        myDFPlayer.playMp3Folder(12);              
        delay(1500);                    
        myDFPlayer.playMp3Folder(14);              
        delay(1000);   
                                                     
      }
      else
      {
        char str[2];
        sprintf(str, "%d", minutes);
        int satuan = str[1] - '0';   
               
        myDFPlayer.playMp3Folder(17); //pukul
        delay(1500);
        myDFPlayer.playMp3Folder(10);              
        delay(1000);
        myDFPlayer.playMp3Folder(12);              
        delay(1500);
        myDFPlayer.playMp3Folder(satuan);
        delay(1500);
        myDFPlayer.playMp3Folder(12);              
        delay(1500);                    
        myDFPlayer.playMp3Folder(14);              
        delay(1000);   
                                                     
      }
    }
    else if (minutes == 20 || minutes == 30 || minutes == 40 || minutes == 50)
    {
        char str[2];
        sprintf(str, "%d", minutes);
        int satuan = str[0] - '0';   
               
        myDFPlayer.playMp3Folder(17); //pukul
        delay(1500);
        myDFPlayer.playMp3Folder(10);              
        delay(1000);
        myDFPlayer.playMp3Folder(12);              
        delay(1500);
        myDFPlayer.playMp3Folder(satuan);
        delay(1500);
        myDFPlayer.playMp3Folder(11);              
        delay(1500);                    
        myDFPlayer.playMp3Folder(14);              
        delay(1000);   
                                                     
    }
    else if (minutes > 20 && minutes < 30 || minutes > 30 && minutes < 40 || minutes > 40 && minutes < 50 || minutes > 50 && minutes < 60)
    {
        char str[2];
        sprintf(str, "%d", minutes);
        int satuanAwal = str[0] - '0';   
        int satuanAkhir = str[1] - '0';   
               
        myDFPlayer.playMp3Folder(17); //pukul
        delay(1500);
        myDFPlayer.playMp3Folder(10);              
        delay(1000);
        myDFPlayer.playMp3Folder(12);              
        delay(1500);
        myDFPlayer.playMp3Folder(satuanAwal);
        delay(1500);
        myDFPlayer.playMp3Folder(satuanAkhir);              
        delay(1500);                    
        myDFPlayer.playMp3Folder(14);              
        delay(1000);   
                                                                     
    }                
  }
  else if (hours == 12)
  {
    if (minutes == 0)
    {
      myDFPlayer.playMp3Folder(17); //pukul
      delay(1500);
      myDFPlayer.playMp3Folder(2);              
      delay(1000);
      myDFPlayer.playMp3Folder(12);              
      delay(1500);
      myDFPlayer.playMp3Folder(14);              
      delay(1000);                          
    }
    else if (minutes > 0 && minutes < 10)
    {
      myDFPlayer.playMp3Folder(17); //pukul
      delay(1500);
      myDFPlayer.playMp3Folder(2);              
      delay(1000);
      myDFPlayer.playMp3Folder(12);              
      delay(1000);
      myDFPlayer.playMp3Folder(0);              
      delay(1500);
      myDFPlayer.playMp3Folder(minutes);              
      delay(1500);                    
      myDFPlayer.playMp3Folder(14);              
      delay(1000);                    
                                                   
    }
    else if (minutes > 10 && minutes < 20)
    {
      if (minutes == 11)
      {
        myDFPlayer.playMp3Folder(17); //pukul
        delay(1500);
        myDFPlayer.playMp3Folder(2);              
        delay(1000);
        myDFPlayer.playMp3Folder(12);              
        delay(1000);
        myDFPlayer.playMp3Folder(10);              
        delay(1500);
        myDFPlayer.playMp3Folder(12);              
        delay(1500);                    
        myDFPlayer.playMp3Folder(14);              
        delay(1000);   
                                                     
      }
      else
      {
        char str[2];
        sprintf(str, "%d", minutes);
        int satuan = str[1] - '0';   
               
        myDFPlayer.playMp3Folder(17); //pukul
        delay(1500);
        myDFPlayer.playMp3Folder(2);              
        delay(1000);
        myDFPlayer.playMp3Folder(12);              
        delay(1000);
        myDFPlayer.playMp3Folder(satuan);
        delay(1500);
        myDFPlayer.playMp3Folder(12);              
        delay(1500);                    
        myDFPlayer.playMp3Folder(14);              
        delay(1000);   
                                                     
      }
    }
    else if (minutes == 20 || minutes == 30 || minutes == 40 || minutes == 50)
    {
        char str[2];
        sprintf(str, "%d", minutes);
        int satuan = str[0] - '0';   
               
        myDFPlayer.playMp3Folder(17); //pukul
        delay(1500);
        myDFPlayer.playMp3Folder(2);              
        delay(1000);
        myDFPlayer.playMp3Folder(12);              
        delay(1000);
        myDFPlayer.playMp3Folder(satuan);
        delay(1500);
        myDFPlayer.playMp3Folder(11);              
        delay(1500);                    
        myDFPlayer.playMp3Folder(14);              
        delay(1000);   
                                                     
    }
    else if (minutes > 20 && minutes < 30 || minutes > 30 && minutes < 40 || minutes > 40 && minutes < 50 || minutes > 50 && minutes < 60)
    {
        char str[2];
        sprintf(str, "%d", minutes);
        int satuanAwal = str[0] - '0';   
        int satuanAkhir = str[1] - '0';   
               
        myDFPlayer.playMp3Folder(17); //pukul
        delay(1500);
        myDFPlayer.playMp3Folder(2);              
        delay(1000);
        myDFPlayer.playMp3Folder(12);              
        delay(1000);
        myDFPlayer.playMp3Folder(satuanAwal);
        delay(1500);
        myDFPlayer.playMp3Folder(satuanAkhir);              
        delay(1500);                    
        myDFPlayer.playMp3Folder(14);              
        delay(1000);   
                                                                     
    }                        
  }
  else if (hours > 12)
  {
    int pmHour = hours - 12;
    if (pmHour > 0 && pmHour < 10)
    {
      if (minutes == 0)
      {
        myDFPlayer.playMp3Folder(17); //pukul
        delay(1500);
        myDFPlayer.playMp3Folder(pmHour);              
        delay(1500);
        if (pmHour > 0 && pmHour < 3)
        {
          myDFPlayer.playMp3Folder(14);                                            
        }
        else if (pmHour > 2 && pmHour < 7)
        {
          myDFPlayer.playMp3Folder(15);                                            
        }
        else if (pmHour > 6 && pmHour < 13)
        {
          myDFPlayer.playMp3Folder(16);                                            
        }
        delay(1000);                          
      }
      else if (minutes > 0 && minutes < 10)
      {
        myDFPlayer.playMp3Folder(17); //pukul
        delay(1500);
        myDFPlayer.playMp3Folder(pmHour);              
        delay(1000);
        myDFPlayer.playMp3Folder(0);              
        delay(1500);
        myDFPlayer.playMp3Folder(minutes);              
        delay(1500);
        if (pmHour > 0 && pmHour < 3)
        {
          myDFPlayer.playMp3Folder(14);                                            
        }
        else if (pmHour > 2 && pmHour < 7)
        {
          myDFPlayer.playMp3Folder(15);                                            
        }
        else if (pmHour > 6 && pmHour < 13)
        {
          myDFPlayer.playMp3Folder(16);                                            
        }
        delay(1000);                    
                                                     
      }
      else if (minutes > 10 && minutes < 20)
      {
        if (minutes == 11)
        {
          myDFPlayer.playMp3Folder(17); //pukul
          delay(1500);
          myDFPlayer.playMp3Folder(pmHour);              
          delay(1000);
          myDFPlayer.playMp3Folder(10);              
          delay(1500);
          myDFPlayer.playMp3Folder(12);              
          delay(1500);                    
          if (pmHour > 0 && pmHour < 3)
          {
            myDFPlayer.playMp3Folder(14);                                            
          }
          else if (pmHour > 2 && pmHour < 7)
          {
            myDFPlayer.playMp3Folder(15);                                            
          }
          else if (pmHour > 6 && pmHour < 13)
          {
            myDFPlayer.playMp3Folder(16);                                            
          }
          delay(1000);   
                                                       
        }
        else
        {
          char str[2];
          sprintf(str, "%d", minutes);
          int satuan = str[1] - '0';   
                 
          myDFPlayer.playMp3Folder(17); //pukul
          delay(1500);
          myDFPlayer.playMp3Folder(pmHour);              
          delay(1000);
          myDFPlayer.playMp3Folder(satuan);
          delay(1500);
          myDFPlayer.playMp3Folder(12);              
          delay(1500);                    
          if (pmHour > 0 && pmHour < 3)
          {
            myDFPlayer.playMp3Folder(14);                                            
          }
          else if (pmHour > 2 && pmHour < 7)
          {
            myDFPlayer.playMp3Folder(15);                                            
          }
          else if (pmHour > 6 && pmHour < 13)
          {
            myDFPlayer.playMp3Folder(16);                                            
          }
          delay(1000);   
                                                       
        }
      }
      else if (minutes == 20 || minutes == 30 || minutes == 40 || minutes == 50)
      {
          char str[2];
          sprintf(str, "%d", minutes);
          int satuan = str[0] - '0';   
                 
          myDFPlayer.playMp3Folder(17); //pukul
          delay(1500);
          myDFPlayer.playMp3Folder(pmHour);              
          delay(1000);
          myDFPlayer.playMp3Folder(satuan);
          delay(1500);
          myDFPlayer.playMp3Folder(11);              
          delay(1500);                    
          if (pmHour > 0 && pmHour < 3)
          {
            myDFPlayer.playMp3Folder(14);                                            
          }
          else if (pmHour > 2 && pmHour < 7)
          {
            myDFPlayer.playMp3Folder(15);                                            
          }
          else if (pmHour > 6 && pmHour < 13)
          {
            myDFPlayer.playMp3Folder(16);                                            
          }
          delay(1000);   
                                                       
      }
      else if (minutes > 20 && minutes < 30 || minutes > 30 && minutes < 40 || minutes > 40 && minutes < 50 || minutes > 50 && minutes < 60)
      {
          char str[2];
          sprintf(str, "%d", minutes);
          int satuanAwal = str[0] - '0';   
          int satuanAkhir = str[1] - '0';   
                 
          myDFPlayer.playMp3Folder(17); //pukul
          delay(1500);
          myDFPlayer.playMp3Folder(pmHour);              
          delay(1000);
          myDFPlayer.playMp3Folder(satuanAwal);
          delay(1500);
          myDFPlayer.playMp3Folder(satuanAkhir);              
          delay(1500);                    
          if (pmHour > 0 && pmHour < 3)
          {
            myDFPlayer.playMp3Folder(14);                                            
          }
          else if (pmHour > 2 && pmHour < 7)
          {
            myDFPlayer.playMp3Folder(15);                                            
          }
          else if (pmHour > 6 && pmHour < 13)
          {
            myDFPlayer.playMp3Folder(16);                                            
          }
          delay(1000);   
                                                                       
      }
    }  
    else if (pmHour == 10)
    {
      if (minutes == 0)
      {
        myDFPlayer.playMp3Folder(17); //pukul
        delay(1500);
        myDFPlayer.playMp3Folder(10);              
        delay(1000);
        myDFPlayer.playMp3Folder(11);              
        delay(1500);
        if (pmHour > 0 && pmHour < 3)
        {
          myDFPlayer.playMp3Folder(14);                                            
        }
        else if (pmHour > 2 && pmHour < 7)
        {
          myDFPlayer.playMp3Folder(15);                                            
        }
        else if (pmHour > 6 && pmHour < 13)
        {
          myDFPlayer.playMp3Folder(16);                                            
        }
        delay(1000);                          
      }
      else if (minutes > 0 && minutes < 10)
      {
        myDFPlayer.playMp3Folder(17); //pukul
        delay(1500);
        myDFPlayer.playMp3Folder(10);              
        delay(1000);
        myDFPlayer.playMp3Folder(11);              
        delay(1500);
        myDFPlayer.playMp3Folder(0);              
        delay(1500);
        myDFPlayer.playMp3Folder(minutes);              
        delay(1500);                    
        if (pmHour > 0 && pmHour < 3)
        {
          myDFPlayer.playMp3Folder(14);                                            
        }
        else if (pmHour > 2 && pmHour < 7)
        {
          myDFPlayer.playMp3Folder(15);                                            
        }
        else if (pmHour > 6 && pmHour < 13)
        {
          myDFPlayer.playMp3Folder(16);                                            
        }
        delay(1000);                    
                                                     
      }
      else if (minutes > 10 && minutes < 20)
      {
        if (minutes == 11)
        {
          myDFPlayer.playMp3Folder(17); //pukul
          delay(1500);
          myDFPlayer.playMp3Folder(10);              
          delay(1000);
          myDFPlayer.playMp3Folder(11);              
          delay(1500);
          myDFPlayer.playMp3Folder(10);              
          delay(1500);
          myDFPlayer.playMp3Folder(12);              
          delay(1500);                    
          if (pmHour > 0 && pmHour < 3)
          {
            myDFPlayer.playMp3Folder(14);                                            
          }
          else if (pmHour > 2 && pmHour < 7)
          {
            myDFPlayer.playMp3Folder(15);                                            
          }
          else if (pmHour > 6 && pmHour < 13)
          {
            myDFPlayer.playMp3Folder(16);                                            
          }
          delay(1000);   
                                                       
        }
        else
        {
          char str[2];
          sprintf(str, "%d", minutes);
          int satuan = str[1] - '0';   
                 
          myDFPlayer.playMp3Folder(17); //pukul
          delay(1500);
          myDFPlayer.playMp3Folder(10);              
          delay(1000);
          myDFPlayer.playMp3Folder(11);              
          delay(1500);
          myDFPlayer.playMp3Folder(satuan);
          delay(1500);
          myDFPlayer.playMp3Folder(12);              
          delay(1500);                    
          if (pmHour > 0 && pmHour < 3)
          {
            myDFPlayer.playMp3Folder(14);                                            
          }
          else if (pmHour > 2 && pmHour < 7)
          {
            myDFPlayer.playMp3Folder(15);                                            
          }
          else if (pmHour > 6 && pmHour < 13)
          {
            myDFPlayer.playMp3Folder(16);                                            
          }
          delay(1000);   
                                                       
        }
      }
      else if (minutes == 20 || minutes == 30 || minutes == 40 || minutes == 50)
      {
          char str[2];
          sprintf(str, "%d", minutes);
          int satuan = str[0] - '0';   
                 
          myDFPlayer.playMp3Folder(17); //pukul
          delay(1500);
          myDFPlayer.playMp3Folder(10);              
          delay(1000);
          myDFPlayer.playMp3Folder(11);              
          delay(1500);
          myDFPlayer.playMp3Folder(satuan);
          delay(1500);
          myDFPlayer.playMp3Folder(11);              
          delay(1500);                    
          if (pmHour > 0 && pmHour < 3)
          {
            myDFPlayer.playMp3Folder(14);                                            
          }
          else if (pmHour > 2 && pmHour < 7)
          {
            myDFPlayer.playMp3Folder(15);                                            
          }
          else if (pmHour > 6 && pmHour < 13)
          {
            myDFPlayer.playMp3Folder(16);                                            
          }
          delay(1000);   
                                                       
      }
      else if (minutes > 20 && minutes < 30 || minutes > 30 && minutes < 40 || minutes > 40 && minutes < 50 || minutes > 50 && minutes < 60)
      {
          char str[2];
          sprintf(str, "%d", minutes);
          int satuanAwal = str[0] - '0';   
          int satuanAkhir = str[1] - '0';   
                 
          myDFPlayer.playMp3Folder(17); //pukul
          delay(1500);
          myDFPlayer.playMp3Folder(10);              
          delay(1000);
          myDFPlayer.playMp3Folder(11);              
          delay(1500);
          myDFPlayer.playMp3Folder(satuanAwal);
          delay(1500);
          myDFPlayer.playMp3Folder(satuanAkhir);              
          delay(1500);                    
          if (pmHour > 0 && pmHour < 3)
          {
            myDFPlayer.playMp3Folder(14);                                            
          }
          else if (pmHour > 2 && pmHour < 7)
          {
            myDFPlayer.playMp3Folder(15);                                            
          }
          else if (pmHour > 6 && pmHour < 13)
          {
            myDFPlayer.playMp3Folder(16);                                            
          }
          delay(1000);   
                                                                       
      }                
    }
    else if (pmHour == 11)
    {
      if (minutes == 0)
      {
        myDFPlayer.playMp3Folder(17); //pukul
        delay(1500);
        myDFPlayer.playMp3Folder(10);              
        delay(1000);
        myDFPlayer.playMp3Folder(12);              
        delay(1500);
        if (pmHour > 0 && pmHour < 3)
        {
          myDFPlayer.playMp3Folder(14);                                            
        }
        else if (pmHour > 2 && pmHour < 7)
        {
          myDFPlayer.playMp3Folder(15);                                            
        }
        else if (pmHour > 6 && pmHour < 13)
        {
          myDFPlayer.playMp3Folder(16);                                            
        }
        delay(1000);                          
      }
      else if (minutes > 0 && minutes < 10)
      {
        myDFPlayer.playMp3Folder(17); //pukul
        delay(1500);
        myDFPlayer.playMp3Folder(10);              
        delay(1000);
        myDFPlayer.playMp3Folder(12);              
        delay(1500);
        myDFPlayer.playMp3Folder(0);              
        delay(1500);
        myDFPlayer.playMp3Folder(minutes);              
        delay(1500);                    
        if (pmHour > 0 && pmHour < 3)
        {
          myDFPlayer.playMp3Folder(14);                                            
        }
        else if (pmHour > 2 && pmHour < 7)
        {
          myDFPlayer.playMp3Folder(15);                                            
        }
        else if (pmHour > 6 && pmHour < 13)
        {
          myDFPlayer.playMp3Folder(16);                                            
        }
        delay(1000);                    
                                                     
      }
      else if (minutes > 10 && minutes < 20)
      {
        if (minutes == 11)
        {
          myDFPlayer.playMp3Folder(17); //pukul
          delay(1500);
          myDFPlayer.playMp3Folder(10);              
          delay(1000);
          myDFPlayer.playMp3Folder(12);              
          delay(1500);
          myDFPlayer.playMp3Folder(10);              
          delay(1500);
          myDFPlayer.playMp3Folder(12);              
          delay(1500);                    
          if (pmHour > 0 && pmHour < 3)
          {
            myDFPlayer.playMp3Folder(14);                                            
          }
          else if (pmHour > 2 && pmHour < 7)
          {
            myDFPlayer.playMp3Folder(15);                                            
          }
          else if (pmHour > 6 && pmHour < 13)
          {
            myDFPlayer.playMp3Folder(16);                                            
          }
          delay(1000);   
                                                       
        }
        else
        {
          char str[2];
          sprintf(str, "%d", minutes);
          int satuan = str[1] - '0';   
                 
          myDFPlayer.playMp3Folder(17); //pukul
          delay(1500);
          myDFPlayer.playMp3Folder(10);              
          delay(1000);
          myDFPlayer.playMp3Folder(12);              
          delay(1500);
          myDFPlayer.playMp3Folder(satuan);
          delay(1500);
          myDFPlayer.playMp3Folder(12);              
          delay(1500);                    
          if (pmHour > 0 && pmHour < 3)
          {
            myDFPlayer.playMp3Folder(14);                                            
          }
          else if (pmHour > 2 && pmHour < 7)
          {
            myDFPlayer.playMp3Folder(15);                                            
          }
          else if (pmHour > 6 && pmHour < 13)
          {
            myDFPlayer.playMp3Folder(16);                                            
          }
          delay(1000);   
                                                       
        }
      }
      else if (minutes == 20 || minutes == 30 || minutes == 40 || minutes == 50)
      {
          char str[2];
          sprintf(str, "%d", minutes);
          int satuan = str[0] - '0';   
                 
          myDFPlayer.playMp3Folder(17); //pukul
          delay(1500);
          myDFPlayer.playMp3Folder(10);              
          delay(1000);
          myDFPlayer.playMp3Folder(12);              
          delay(1500);
          myDFPlayer.playMp3Folder(satuan);
          delay(1500);
          myDFPlayer.playMp3Folder(11);              
          delay(1500);                    
          if (pmHour > 0 && pmHour < 3)
          {
            myDFPlayer.playMp3Folder(14);                                            
          }
          else if (pmHour > 2 && pmHour < 7)
          {
            myDFPlayer.playMp3Folder(15);                                            
          }
          else if (pmHour > 6 && pmHour < 13)
          {
            myDFPlayer.playMp3Folder(16);                                            
          }
          delay(1000);   
                                                       
      }
      else if (minutes > 20 && minutes < 30 || minutes > 30 && minutes < 40 || minutes > 40 && minutes < 50 || minutes > 50 && minutes < 60)
      {
          char str[2];
          sprintf(str, "%d", minutes);
          int satuanAwal = str[0] - '0';   
          int satuanAkhir = str[1] - '0';   
                 
          myDFPlayer.playMp3Folder(17); //pukul
          delay(1500);
          myDFPlayer.playMp3Folder(10);              
          delay(1000);
          myDFPlayer.playMp3Folder(12);              
          delay(1500);
          myDFPlayer.playMp3Folder(satuanAwal);
          delay(1500);
          myDFPlayer.playMp3Folder(satuanAkhir);              
          delay(1500);                    
          if (pmHour > 0 && pmHour < 3)
          {
            myDFPlayer.playMp3Folder(14);                                            
          }
          else if (pmHour > 2 && pmHour < 7)
          {
            myDFPlayer.playMp3Folder(15);                                            
          }
          else if (pmHour > 6 && pmHour < 13)
          {
            myDFPlayer.playMp3Folder(16);                                            
          }
          delay(1000);   
                                                                       
      }                
    }
    else if (pmHour == 12)
    {
      if (minutes == 0)
      {
        myDFPlayer.playMp3Folder(17); //pukul
        delay(1500);
        myDFPlayer.playMp3Folder(2);              
        delay(1000);
        myDFPlayer.playMp3Folder(12);              
        delay(1500);
        if (pmHour > 0 && pmHour < 3)
        {
          myDFPlayer.playMp3Folder(14);                                            
        }
        else if (pmHour > 2 && pmHour < 7)
        {
          myDFPlayer.playMp3Folder(15);                                            
        }
        else if (pmHour > 6 && pmHour < 13)
        {
          myDFPlayer.playMp3Folder(16);                                            
        }
        delay(1000);                          
      }
      else if (minutes > 0 && minutes < 10)
      {
        myDFPlayer.playMp3Folder(17); //pukul
        delay(1500);
        myDFPlayer.playMp3Folder(2);              
        delay(1000);
        myDFPlayer.playMp3Folder(12);              
        delay(1500);
        myDFPlayer.playMp3Folder(0);              
        delay(1500);
        myDFPlayer.playMp3Folder(minutes);              
        delay(1500);                    
        if (pmHour > 0 && pmHour < 3)
        {
          myDFPlayer.playMp3Folder(14);                                            
        }
        else if (pmHour > 2 && pmHour < 7)
        {
          myDFPlayer.playMp3Folder(15);                                            
        }
        else if (pmHour > 6 && pmHour < 13)
        {
          myDFPlayer.playMp3Folder(16);                                            
        }
        delay(1000);                    
                                                     
      }
      else if (minutes > 10 && minutes < 20)
      {
        if (minutes == 11)
        {
          myDFPlayer.playMp3Folder(17); //pukul
          delay(1500);
          myDFPlayer.playMp3Folder(2);              
          delay(1000);
          myDFPlayer.playMp3Folder(12);              
          delay(1500);
          myDFPlayer.playMp3Folder(10);              
          delay(1500);
          myDFPlayer.playMp3Folder(12);              
          delay(1500);                    
          if (pmHour > 0 && pmHour < 3)
          {
            myDFPlayer.playMp3Folder(14);                                            
          }
          else if (pmHour > 2 && pmHour < 7)
          {
            myDFPlayer.playMp3Folder(15);                                            
          }
          else if (pmHour > 6 && pmHour < 13)
          {
            myDFPlayer.playMp3Folder(16);                                            
          }
          delay(1000);   
                                                       
        }
        else
        {
          char str[2];
          sprintf(str, "%d", minutes);
          int satuan = str[1] - '0';   
                 
          myDFPlayer.playMp3Folder(17); //pukul
          delay(1500);
          myDFPlayer.playMp3Folder(2);              
          delay(1000);
          myDFPlayer.playMp3Folder(12);              
          delay(1500);
          myDFPlayer.playMp3Folder(satuan);
          delay(1500);
          myDFPlayer.playMp3Folder(12);              
          delay(1500);                    
          if (pmHour > 0 && pmHour < 3)
          {
            myDFPlayer.playMp3Folder(14);                                            
          }
          else if (pmHour > 2 && pmHour < 7)
          {
            myDFPlayer.playMp3Folder(15);                                            
          }
          else if (pmHour > 6 && pmHour < 13)
          {
            myDFPlayer.playMp3Folder(16);                                            
          }
          delay(1000);   
                                                       
        }
      }
      else if (minutes == 20 || minutes == 30 || minutes == 40 || minutes == 50)
      {
          char str[2];
          sprintf(str, "%d", minutes);
          int satuan = str[0] - '0';   
                 
          myDFPlayer.playMp3Folder(17); //pukul
          delay(1500);
          myDFPlayer.playMp3Folder(2);              
          delay(1000);
          myDFPlayer.playMp3Folder(12);              
          delay(1500);
          myDFPlayer.playMp3Folder(satuan);
          delay(1500);
          myDFPlayer.playMp3Folder(11);              
          delay(1500);                    
          if (pmHour > 0 && pmHour < 3)
          {
            myDFPlayer.playMp3Folder(14);                                            
          }
          else if (pmHour > 2 && pmHour < 7)
          {
            myDFPlayer.playMp3Folder(15);                                            
          }
          else if (pmHour > 6 && pmHour < 13)
          {
            myDFPlayer.playMp3Folder(16);                                            
          }
          delay(1000);   
                                                       
      }
      else if (minutes > 20 && minutes < 30 || minutes > 30 && minutes < 40 || minutes > 40 && minutes < 50 || minutes > 50 && minutes < 60)
      {
          char str[2];
          sprintf(str, "%d", minutes);
          int satuanAwal = str[0] - '0';   
          int satuanAkhir = str[1] - '0';   
                 
          myDFPlayer.playMp3Folder(17); //pukul
          delay(1500);
          myDFPlayer.playMp3Folder(2);              
          delay(1000);
          myDFPlayer.playMp3Folder(12);              
          delay(1500);
          myDFPlayer.playMp3Folder(satuanAwal);
          delay(1500);
          myDFPlayer.playMp3Folder(satuanAkhir);              
          delay(1500);                    
          if (pmHour > 0 && pmHour < 3)
          {
            myDFPlayer.playMp3Folder(14);                                            
          }
          else if (pmHour > 2 && pmHour < 7)
          {
            myDFPlayer.playMp3Folder(15);                                            
          }
          else if (pmHour > 6 && pmHour < 13)
          {
            myDFPlayer.playMp3Folder(16);                                            
          }
          delay(1000);   
                                                                       
      }                        
    }    
  }
//  loop();
}
