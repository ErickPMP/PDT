#define PI 3.1415926535897932384626433832795
#include <Wire.h>
#include "SDL_Arduino_INA3221.h"
SDL_Arduino_INA3221 ina3221;

// the three channels of the INA3221 named for SunAirPlus Solar Power Controller channels (www.switchdoc.com)
#define LIPO_BATTERY_CHANNEL 1
#define SOLAR_CELL_CHANNEL 2
#define OUTPUT_CHANNEL 3

bool flag;
int D10=10;
int D4=4;
int D3=3;
int D5=5;
int D9=9;
int D11=11;
int duty1=25;
int duty2=25;
float mil,sec=0,t;
float Out1,Out2,Out1a,Out1b,Out2a,Out2b;
float e,u,un,en,SPo,Kp=11,SensorC=0;
float shuntvoltage1 = 0;
float busvoltage1 = 0;
float current_mA1 = 0;
float loadvoltage1 = 0;

void setup(){
  Serial.begin(19200);
  pinMode(D10, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D9, OUTPUT);
  pinMode(D11, OUTPUT);
  
  //**********************Leyendas*********************
  Serial.println("u,e,SPo,SensorC");
  //*************************************************** IMPORTANTE NO MOVER O HAY CONFLICTO CON EL INA3221
  
  noInterrupts();           // deshabilita las interrupciones

  // TIMER 0

   TCCR0A = 0;                        // importante colocar en cero
   TCCR0B = 0;                        // importante colocar en cero
   TCNT0  = 0;                        // importante colocar en cero

   TCCR0A |= (1 << WGM01);            // CTC mode
   TCCR0B = _BV(CS02)| _BV(CS00);     // Clock/1024   //prescaler1024
   OCR0A  = 15; //(16000000/1024/frec=1000Hz)-1;       // 
   TIMSK0 |= (1 << OCIE0A);           // enable 
  
  // TIMER 2

   TCCR2A = 0;                        // importante colocar en cero
   TCCR2B = 0;                        // importante colocar en cero
   TCNT2  = 0;                        // importante colocar en cero
                                                            

  // TIMER 2 Fast PWM 8-bit mode
  TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);  //Clear mode
  TCCR2B = (0 << CS22)| _BV(CS20);
  interrupts();

  //******************************* INA3221*****************************
  //Serial.println("SDA_Arduino_INA3221_Test");
  //Serial.println("Measuring voltage and current with ina3221 ...");
  ina3221.begin();
  //Serial.print("Manufactures ID=0x");
  int MID;
  MID = ina3221.getManufID();
  //Serial.println(MID,HEX);
  //********************************************************************
}

ISR(TIMER0_COMPA_vect){          // timer 0  (cada 1 mS)   
 mil =mil+1;
 if(mil == 1000.0){
     sec=sec+1.0;
    mil=0; 
  }
 t= sec+mil/1000.0;

 //Out1 = 255.0*sin(2.0*PI*0.2*t);     // SENO para EL PIN D3, D4 y D5
 Out2 = 255.0*sin(2.0*PI*0.2*t);    // SENO para EL PIN D11, D10 y D9

SPo=100.0*sin(2.0*PI*0.2*t);
if(SPo>SensorC){e=SPo-SensorC;}
else{e=SensorC-SPo;}

if(e<0){en=e;e=-e;}
else{en=e;e=e;}
u=Kp*e;
u=constrain(u,0,254);
//Out1=u;

if (SPo >= 0.0) {  un = u;digitalWrite(D4,HIGH);digitalWrite(D5,LOW);OCR2A  =  u;SensorC=current_mA1;} // D4 y D5 Compuerta AND seno POSITIVO
else { un = -u;digitalWrite(D4,LOW);digitalWrite(D5,HIGH);OCR2A  =  abs(u);SensorC=-current_mA1;} // D4 y D5 Compuerta AND seno NEGATIVO

if (SPo >= 0.0) {  Out2a = Out2;digitalWrite(D10,HIGH);digitalWrite(D9,LOW);OCR2B=Out2;} // D10 y D9 Compuerta AND seno POSITIVO
else { Out2b = abs(Out2);digitalWrite(D10,LOW);digitalWrite(D9,HIGH);OCR2B=abs(Out2);} // D10 y 9 Compuerta AND seno NEGATIVO

}

void loop() {
  //*********Lectura del A0**********
  float sen=analogRead(A0);
  double senmap=5.0*sen/1024.0; //Conversion 1024 a 5V
 //Serial.println(senmap);
 //Serial.print(",");
 //**********************************
 /*
 Serial.print(Out1a);
 Serial.print(",");
 Serial.print(Out1b);
 Serial.print(",");
*/
//*********************
//**********************************************************************

//*************************INA3221*************************************

  busvoltage1 = ina3221.getBusVoltage_V(LIPO_BATTERY_CHANNEL);
  shuntvoltage1 = ina3221.getShuntVoltage_mV(LIPO_BATTERY_CHANNEL);
  current_mA1 = ina3221.getCurrent_mA(LIPO_BATTERY_CHANNEL);  // minus is to get the "sense" right.   - means the battery is charging, + that it is discharging
  loadvoltage1 = busvoltage1 + (shuntvoltage1 / 1000);  
    //Serial.print("LIPO_Battery Bus Voltage:   "); Serial.print(busvoltage1); Serial.println(" V");
    //Serial.print("LIPO_Battery Shunt Voltage: "); Serial.print(shuntvoltage1); Serial.println(" mV");
    //Serial.print("LIPO_Battery Load Voltage:  "); Serial.print(loadvoltage1); Serial.println(" V");
//Serial.print("LIPO_Battery Current 1:       "); Serial.print(current_mA1); Serial.print(" mA");
//Serial.print(",");
 //*******************************************************************

Serial.print(un);
Serial.print(",");
Serial.print(en);
Serial.print(",");
Serial.print(SPo);
Serial.print(",");
Serial.println(SensorC);

}
