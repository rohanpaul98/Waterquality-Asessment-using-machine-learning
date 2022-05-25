
#include "GravityTDS.h"
//#include <OneWire.h>
//#include <DallasTemperature.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = "AGR6OUin_Hnk7ScS-XR2GTEiU_O_e5n_";       // You should get Auth Token in the Blynk App.
char ssid[] = "R";                       // Your WiFi credentials.
char pass[] = "12345678";


//#define ONE_WIRE_BUS 4
//OneWire oneWire(ONE_WIRE_BUS);
//DallasTemperature sensors(&oneWire);

// float Celcius=0;
// float Fahrenheit=0;
// float waterTemp = 400;

String str;


float ntu;
float aref = 3.3;
#define TdsSensorPin A0
float temperature = 25,tdsValue = 0;
GravityTDS gravityTds;

void setup()
{
  
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
//  sensors.begin();
  gravityTds.setPin(TdsSensorPin);
  gravityTds.setAref(5.0);  //reference voltage on ADC, default 5.0V on Arduino UNO
  gravityTds.setAdcRange(1024);  //1024 for 10bit ADC;4096 for 12bit ADC
  gravityTds.begin();  //initialization
  delay(2000);
  
}
 
void loop()
{
    Blynk.run();
//    sensors.requestTemperatures(); 
//    Celcius=sensors.getTempCByIndex(0);
//    Fahrenheit=sensors.toFahrenheit(Celcius);
//    waterTemp  = waterTemp*0.0625;
//    float rawEc = analogRead(TdsSensorPin) * aref / 1024.0; // read the analog value more stable by the median filtering algorithm, and convert to voltage value
//    float temperatureCoefficient = 1.0 + 0.02 * (waterTemp - 25.0);
//
//    ec = (rawEc / temperatureCoefficient) *ecCalibration*1000; // temperature and calibration compensation
  
    gravityTds.setTemperature(temperature);  // set the temperature and execute temperature compensation
    gravityTds.update();  //sample and calculate
    tdsValue = gravityTds.getTdsValue();  // then get the value
   
    
    Serial.print("TDS VALUE:");
    Serial.print(tdsValue);
    Serial.println("ppm");
//    Serial.print("Electrical Conductivity:");
//    Serial.print(ec);
//    Serial.println("μS/cm");
// 
//    
//   Serial.println("Water Temperature ");
//   Serial.print("In celcius : ");
//   Serial.print(Celcius);
//   Serial.println("°C");
//   Serial.print("In Farenheit : ");
//   Serial.print(Fahrenheit);
//   Serial.println("°F");
//   Serial.println();

   Blynk.virtualWrite(V1,tdsValue);
//   Blynk.virtualWrite(V2,ec);
//   Blynk.virtualWrite(V3,Celcius);
   
    delay(700);
}
 
