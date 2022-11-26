#include <SPI.h>
#include <Wire.h> 
#include <TinyGPS++.h>
#include <TimeLib.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h> 

LiquidCrystal_I2C lcd(0x27,20,4); // Initialize LCD Display at address 0x27 / 0X3F 


TinyGPSPlus gps;

//#define S_RX   0
//#define S_TX   1
//SoftwareSerial SoftSerial(S_RX, S_TX);
SoftwareSerial SoftSerial(4, 3);  // GPS Tx Pin - D4; GPS Rx Pin D3 

//LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

#define time_offset  19800  //for IST time zone is second
char Time[] = "00:00:00";
char Date[] = "00-00-2000";
byte last_second, Second, Minute, Hour, Day, Month;
int Year;
int gpssignal;
int a;

void setup(void)
{
   SoftSerial.begin(9600);
   Serial.begin(9600);
   Wire.begin();
    lcd.init();
    lcd.backlight();
  SPI.begin(); // Init SPI bus
   lcd.begin(20,4);
   
   lcd.setCursor(0,0);
   lcd.print("Welcome John");
   lcd.setCursor(0,1);
   lcd.print("GPS clock");
   delay(3000);
   lcd.clear();
   
   while(!SoftSerial.available())
   {
     lcd.print("GPS Failed!");
   delay(200);
   lcd.clear();
   }
   
   if(SoftSerial.available())
     {
       lcd.print("GPS Start");
     delay(3000);
     lcd.clear();
   }
   }

void gpstimedatewday(int i, int j, int k, int l)
{

   start:
     while(SoftSerial.available() > 0)
     {

       if (gps.encode(SoftSerial.read()))
       {

          gpssignal = (gps.time.hour()+gps.time.minute()+gps.time.second()+gps.date.day()+gps.date.month()+gps.date.year());

            while(gpssignal == 2000)
            {
              lcd.setCursor(0,0);
               lcd.print("Connecting");
              for(a=10; a<13; a++)
              {
                lcd.setCursor(a,0);
                lcd.print(":");
                delay(200);
              }
              lcd.setCursor(10,0);
              lcd.print("  ");
              delay(200);
        lcd.clear();
        goto start;
            }
      
    if (gps.time.isValid())
        {
          Minute = gps.time.minute();
          Second = gps.time.second();
          Hour   = gps.time.hour();
        }


        if (gps.date.isValid())
        {
          Day   = gps.date.day();
      Month = gps.date.month();
      Year  = gps.date.year();
        }     
    
    if(last_second != gps.time.second())
    {
      last_second = gps.time.second();
      
      setTime(Hour, Minute, Second, Day, Month, Year);
      
      adjustTime(time_offset);
      
      Time[0] = hour()   / 10 + '0';
      Time[1] = hour()   % 10 + '0';
      Time[3] = minute() / 10 + '0';
      Time[4] = minute() % 10 + '0';
      Time[6] = second() / 10 + '0';
      Time[7] = second() % 10 + '0';
      Date[0] =  day()    / 10 + '0';
      Date[1] =  day()    % 10 + '0';
      Date[3] =  month()  / 10 + '0';
      Date[4] =  month()  % 10 + '0';
          Date[8] =  (year()  / 10) % 10 + '0';
          Date[9] =   year()  % 10 + '0';

          print_wday(weekday());
          lcd.setCursor(i, j);
          lcd.print(Time);
          lcd.setCursor(k, l);
          lcd.print(Date);      
    }
    if (gps.location.isValid())
    {
      lcd.setCursor(0,2);
      lcd.print("Lat:");
      lcd.setCursor(12,2);
      lcd.print("h=");
      lcd.setCursor(0,3);
      lcd.print("Lng:");
      lcd.setCursor(12,3);
      lcd.print("V=");
      lcd.setCursor(4,2);
      lcd.print(gps.location.lat(), 4);
      lcd.setCursor(14,2);
      lcd.print(gps.altitude.meters(), 1);
      lcd.setCursor(4,3);
      lcd.print(gps.location.lng(), 4);
      lcd.setCursor(14,3);
      lcd.print(gps.speed.kmph(),1);
    }
        if(second() == 0)
        {
          lcd.clear();
        }   
    
    }
   }    
}

void serialprint()
{
   Serial.print("Satelites: ");
   Serial.print(gps.satellites.value());
   Serial.print("  IST time: ");
   Serial.print(Time);
   Serial.print("  Date:  ");
   Serial.print(Date);
   Serial.print("  Latitude: ");
   Serial.print(gps.location.lat(), 4);
   Serial.print("  Longitude: ");
   Serial.print(gps.location.lng(), 4);
   Serial.print(" Altitude(meters): ");
   Serial.print(gps.altitude.meters(), 2);
   Serial.print(" Speed(kmph): ");
   Serial.print(" Speed(kmph): ");
   Serial.print(gps.speed.kmph(), 2);
   Serial.println("  ...programmed by John");   
}

void print_wday(byte wday)
{

   lcd.setCursor(12, 1);
   switch(wday)
   {
     case 1: lcd.print("Sun");   break;
   case 2: lcd.print("Mon");   break;
   case 3: lcd.print("Tue");   break;
   case 4: lcd.print("Wed");   break;
   case 5: lcd.print("Thu");   break;
   case 6: lcd.print("Fri");   break;
   case 7: lcd.print("Sat");
   }     
}
   
void loop()
{
    gpstimedatewday(0,0,0,1);
  serialprint();
}   
