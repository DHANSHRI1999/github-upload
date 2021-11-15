
#include <SoftwareSerial.h>
#include <SFE_BMP180.h>
#include <Wire.h>

#define USE_ARDUINO_INTERRUPTS true    
#include <PulseSensorPlayground.h>        
#include <LiquidCrystal.h>
#include "DHT.h"
 
#define DHTPIN A2            
#define DHTTYPE DHT11       
DHT dht(DHTPIN, DHTTYPE);   
//  Variables
//#define dht_dpin A2
SFE_BMP180 pressure;
const int PulseWire = 0;      
const int LED13 = 5;          
int Threshold = 550;          
                              
                              
  LiquidCrystal lcd(8,9,10,11,12,13);                             
PulseSensorPlayground pulseSensor;  
//dht DHT;
int ir=7;
int buzz=4;

int tled=47;
int hled=48;
int pled=49;
int sled=50;

int myBPM;
#define ALTITUDE 1655.0

char status;
  double T,P,p0,a;
void setup() {   

pinMode(ir, INPUT);
  
  pinMode(buzz , OUTPUT);
  pinMode(tled , OUTPUT);
  pinMode(hled , OUTPUT);
  pinMode(pled , OUTPUT);
  pinMode(sled , OUTPUT);
  Serial.begin(9600);          
 lcd.begin(16, 2);
  dht.begin();

if (pressure.begin())
   // Serial.println("BMP180 init success");
  
  lcd.print("Welcome");
  delay(800);
  // Configure the PulseSensor object, by assigning our variables to it. 
  pulseSensor.analogInput(PulseWire);   
  pulseSensor.blinkOnPulse(LED13);       
  pulseSensor.setThreshold(Threshold);   

  // Double-check the "pulseSensor" object was created and "began" seeing a signal. 
   if (pulseSensor.begin()) {
    //Serial.println("We created a pulseSensor Object !");  //This prints one time at Arduino power-up,  or on Arduino reset.  
  }
}



void loop() {

  myBPM = pulseSensor.getBeatsPerMinute();  
     int a= analogRead(A1);
     
int h = dht.readHumidity();
  
  int t = dht.readTemperature();
  int tempF=t*9/5+32;  


int snd= (a/1023)*5;

 // Serial.print("sound val");
  Serial.println(snd);
  
  lcd.clear();
     lcd.setCursor(0,0); 
     lcd.print("BPM :");
     lcd.setCursor(5,0); 
     lcd.print(myBPM);
     lcd.setCursor(0,1); 
     lcd.print("TEMP:");
      lcd.setCursor(6,1); 
     lcd.print(tempF);
      lcd.setCursor(9,1); 
     lcd.print("^F");
     delay(800);
     lcd.clear();
     lcd.setCursor(0,0); 
     lcd.print("BMP:");
     lcd.setCursor(5,0); 
    lcd.print(P);
    lcd.setCursor(11,0); 
     lcd.print("psl");
     lcd.setCursor(0,1); 
     lcd.print("Humidity:");
      lcd.setCursor(9,1); 
     lcd.print(h);
      lcd.setCursor(12,1); 
     lcd.print("%");
      delay(800);

   if (pulseSensor.sawStartOfBeat()) {             
 //Serial.println("♥  A HeartBeat Happened ! "); 
// Serial.print("BPM: ");                        
// Serial.println(myBPM);

}   


  delay(20);
bmp();
if(digitalRead(ir)==1)
{
 digitalWrite(buzz, HIGH);
}
else
{
digitalWrite(buzz, LOW);
} 
if(h>=90)
{
  digitalWrite(hled,HIGH);
}
else
{
digitalWrite(hled,LOW);
}

if(myBPM>=45)
{
  smsbpm();
  digitalWrite(pled,HIGH);
  
}
else
{
digitalWrite(pled,LOW);
}
if(t>=35)
{digitalWrite(tled,HIGH);
  smstemp();
}
else
{
digitalWrite(tled,LOW);
}


if(t<=30)
{
  //digitalWrite(tled,HIGH);
  smstemp1();
  
}

if(snd>=3)
{
  
  digitalWrite(sled,HIGH);
  
}
else
{
digitalWrite(sled,LOW);
}


}

void bmp()
{
  /*
 Serial.println();
  Serial.print("provided altitude: ");
  Serial.print(ALTITUDE,0);
  Serial.print(" meters, ");
  Serial.print(ALTITUDE*3.28084,0);
  Serial.println(" feet");
  */
  // If you want to measure altitude, and not pressure, you will instead need
  // to provide a known baseline pressure. This is shown at the end of the sketch.

  // You must first get a temperature measurement to perform a pressure reading.
  
  // Start a temperature measurement:
  // If request is successful, the number of ms to wait is returned.
  // If request is unsuccessful, 0 is returned.
//myBPM = pulseSensor.getBeatsPerMinute();  
  status = pressure.startTemperature();
  if (status != 0)
  {
    // Wait for the measurement to complete:
    delay(status);

    // Retrieve the completed temperature measurement:
    // Note that the measurement is stored in the variable T.
    // Function returns 1 if successful, 0 if failure.

    status = pressure.getTemperature(T);
    if (status != 0)
    {
     

      status = pressure.startPressure(3);
      if (status != 0)
      {
        
        delay(status);

        // Retrieve the completed pressure measurement:
        // Note that the measurement is stored in the variable P.
        // Note also that the function requires the previous temperature measurement (T).
        // (If temperature is stable, you can do one temperature measurement for a number of pressure measurements.)
        // Function returns 1 if successful, 0 if failure.

        status = pressure.getPressure(P,T);
        if (status != 0)
        {
          // Print out the measurement:
          //Serial.print("absolute pressure: ");
        //  Serial.print(P,2);
         // Serial.println(" mb, ");
         // Serial.print(P*0.0295333727,2);
         // Serial.println(" inHg");

          // The pressure sensor returns abolute pressure, which varies with altitude.
          // To remove the effects of altitude, use the sealevel function and your current altitude.
          // This number is commonly used in weather reports.
          // Parameters: P = absolute pressure in mb, ALTITUDE = current altitude in m.
          // Result: p0 = sea-level compensated pressure in mb

          p0 = pressure.sealevel(P,ALTITUDE); // we're at 1655 meters (Boulder, CO)
        //  Serial.print("relative (sea-level) pressure: ");
        //  Serial.print(p0,2);
        //  Serial.print(" mb, ");
          //Serial.print(p0*0.0295333727,2);
         // Serial.println(" inHg");

          // On the other hand, if you want to determine your altitude from the pressure reading,
          // use the altitude function along with a baseline pressure (sea-level or other).
          // Parameters: P = absolute pressure in mb, p0 = baseline pressure in mb.
          // Result: a = altitude in m.

          a = pressure.altitude(P,p0);
      /*    Serial.print("computed altitude: ");
         Serial.print(a,0);
        Serial.print(" meters, ");
          Serial.print(a*3.28084,0);
          Serial.println(" feet");
          */
        }
        else Serial.println("error retrieving pressure measurement\n");
      }
      else Serial.println("error starting pressure measurement\n");
    }
    else Serial.println("error retrieving temperature measurement\n");
  }
  else Serial.println("error starting temperature measurement\n");
// myBPM = pulseSensor.getBeatsPerMinute();  
  delay(1000);             
}

void smsbpm()
{
  
         
             lcd.clear();
            lcd.print("high puse rate");
         
              
                  Serial.println("AT+CMGF=1");    //To send SMS in Text Mode
            delay(1000);
            Serial.println("AT+CMGS=\"+919503973996\"\r"); // change to the phone number you using 
            delay(1000);
            Serial.println("heart beats are high");//the content of the message
            delay(200);
            Serial.println((char)26);//the stopping character
           // int myBPM = pulseSensor.getBeatsPerMinute();  
            delay(3000);
      }
      void smstemp()
      {
        
         
             lcd.clear();
            lcd.print("High temprature");
        
                  Serial.println("AT+CMGF=1");    //To send SMS in Text Mode
            delay(1000);
            Serial.println("AT+CMGS=\"+919503973996\"\r"); // change to the phone number you using 
            delay(1000);
            Serial.println("high temprature");//the content of the message
            delay(200);
            Serial.println((char)26);//the stopping character
            delay(3000);
             lcd.clear();
            lcd.print("High temprature");
        
                  Serial.println("AT+CMGF=1");    //To send SMS in Text Mode
            delay(1000);
            Serial.println("AT+CMGS=\"+918080401760\"\r"); // change to the phone number you using 
            delay(1000);
            Serial.println("high temprature");//the content of the message
            delay(200);
            Serial.println((char)26);//the stopping character
            // myBPM = pulseSensor.getBeatsPerMinute();  
            delay(3000);
}

void smstemp1()
      {
        
         
             lcd.clear();
            lcd.print("LOW temprature");
        
                  Serial.println("AT+CMGF=1");    //To send SMS in Text Mode
            delay(1000);
            Serial.println("AT+CMGS=\"+919503973996\"\r"); // change to the phone number you using 
            delay(1000);
            Serial.println("LOW temprature");//the content of the message
            delay(200);
            Serial.println((char)26);//the stopping character
            delay(3000);
             lcd.clear();
            lcd.print("High temprature");
        
                  Serial.println("AT+CMGF=1");    //To send SMS in Text Mode
            delay(1000);
            Serial.println("AT+CMGS=\"+918080401760\"\r"); // change to the phone number you using 
            delay(1000);
            Serial.println("LOW temprature");//the content of the message
            delay(200);
            Serial.println((char)26);//the stopping character
            // myBPM = pulseSensor.getBeatsPerMinute();  
            delay(3000);
}

void smsbp()
      {
        
         
             lcd.clear();
            lcd.print("LOW PULSES");
        
                  Serial.println("AT+CMGF=1");    //To send SMS in Text Mode
            delay(1000);
            Serial.println("AT+CMGS=\"+919503973996\"\r"); // change to the phone number you using 
            delay(1000);
            Serial.println("LOW temprature");//the content of the message
            delay(200);
            Serial.println((char)26);//the stopping character
            delay(3000);
             lcd.clear();
            lcd.print("LOW PULSES");
        
                  Serial.println("AT+CMGF=1");    //To send SMS in Text Mode
            delay(1000);
            Serial.println("AT+CMGS=\"+918080401760\"\r"); // change to the phone number you using 
            delay(1000);
            Serial.println("LOW PULSES");//the content of the message
            delay(200);
            Serial.println((char)26);//the stopping character
            // myBPM = pulseSensor.getBeatsPerMinute();  
            delay(3000);
}
  
