#include <Wire.h> 
#include <EEPROM.h>
#include "GravityTDS.h"
#include <OneWire.h>
#include <DallasTemperature.h>


#define ONE_WIRE_BUS 5
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

 float Celcius=0;
 float Fahrenheit=0;

String str;
int sensorPin = A1;
float volt;
float ntu;
float aref = 3.3;
#define TdsSensorPin A0
double waterTemp = 400.00;
float ec = 0;
float ecCalibration = 1;

float temperature = 25,tdsValue = 0;
GravityTDS gravityTds;

void setup()
{
  Serial.begin(115200);
  sensors.begin();
  gravityTds.setPin(TdsSensorPin);
  gravityTds.setAref(5.0);  //reference voltage on ADC, default 5.0V on Arduino UNO
  gravityTds.setAdcRange(1024);  //1024 for 10bit ADC;4096 for 12bit ADC
  gravityTds.begin();  //initialization
  delay(2000);
  
}
 
void loop()
{
    sensors.requestTemperatures(); 
    Celcius=sensors.getTempCByIndex(0);
    Fahrenheit=sensors.toFahrenheit(Celcius);
    waterTemp  = waterTemp*0.0625;
    float rawEc = analogRead(TdsSensorPin) * aref / 1024.0; // read the analog value more stable by the median filtering algorithm, and convert to voltage value
    float temperatureCoefficient = 1.0 + 0.02 * (waterTemp - 25.0);

    ec = (rawEc / temperatureCoefficient) *ecCalibration*1000; // temperature and calibration compensation
  
    gravityTds.setTemperature(temperature);  // set the temperature and execute temperature compensation
    gravityTds.update();  //sample and calculate
    tdsValue = gravityTds.getTdsValue();  // then get the value
   
    volt = 0;
    int s = analogRead(sensorPin);
    for(int i=0; i<800; i++)
    {
        volt += ((float)analogRead(sensorPin)/1023)*5;
    }
    volt = volt/800;
    volt = round_to_dp(volt,2);
    if(volt < 2.5){
      ntu = 3;
    }else{
      ntu = -1120.4*square(volt)+5742.3*volt-4353.8; 
      ntu = ntu/1000;
    }

    Serial.print("TDS VALUE:");
    Serial.print(tdsValue,0);
    Serial.println("ppm");
    Serial.print("Electrical Conductivity:");
    Serial.print(ec);
    Serial.println("μS/cm");
 
    Serial.print("Turbidity : ");
    Serial.print(ntu);
    Serial.println(" NTU");
    Serial.println();
   Serial.println("Water Temperature ");
   Serial.print("In celcius : ");
   Serial.print(Celcius);
   Serial.println("°C");
   Serial.print("In Farenheit : ");
   Serial.print(Fahrenheit);
   Serial.println("°F");
   Serial.println();
    delay(700);
}
 
float round_to_dp( float in_value, int decimal_place )
{
  float multiplier = powf( 10.0f, decimal_place );
  in_value = roundf( in_value * multiplier ) / multiplier;
  return in_value;
}
