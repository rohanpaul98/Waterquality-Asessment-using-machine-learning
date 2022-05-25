#include <Wire.h> 
#include <EEPROM.h>
#include "GravityTDS.h"
#define TdsSensorPin A0


String str;
#define sensorPin A0
float volt;
float ntu;


float temperature = 25,tdsValue = 0;
GravityTDS gravityTds;

void setup()
{
  Serial.begin(115200);
  gravityTds.setPin(TdsSensorPin);
  gravityTds.setAref(5.0);  //reference voltage on ADC, default 5.0V on Arduino UNO
  gravityTds.setAdcRange(1024);  //1024 for 10bit ADC;4096 for 12bit ADC
  gravityTds.begin();  //initialization
  delay(2000);
  
}

void TDS(){
    gravityTds.setTemperature(temperature);  // set the temperature and execute temperature compensation
    gravityTds.update();  //sample and calculate
    tdsValue = gravityTds.getTdsValue();  // then get the value
    Serial.print("TDS VALUE:");
    Serial.print(tdsValue,0);
    Serial.println("ppm");

}

void TURBIDITY(){
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
      ntu = -1120.4*(volt*volt)+5742.3*volt-4353.8; 
      ntu = ntu/1000;
    }

    Serial.print("Turbidity : ");
    Serial.print(ntu);
    Serial.println(" NTU");
    Serial.println();
}
 
void loop()
{
    TDS();
    TURBIDITY();
    
   
    delay(700);
}
 
float round_to_dp( float in_value, int decimal_place )
{
  float multiplier = powf( 10.0f, decimal_place );
  in_value = roundf( in_value * multiplier ) / multiplier;
  return in_value;
}
