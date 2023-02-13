#define PI 3.1415926535897932384626433832795
#include "SDL_Arduino_INA3221.h"
SDL_Arduino_INA3221 ina3221;

// the three channels of the INA3221 named for SunAirPlus Solar Power Controller channels (www.switchdoc.com)
#define LIPO_BATTERY_CHANNEL 1

float Kp=0.5, Ki=45;//, Td=0;
int D10=10,D4=4,D3=3,D5=5,D9=9,D11=11;
float mil,sec=0,t;
float Out1,Out2,Out1a,Out1b,Out2a,Out2b;
float e,u,un,en,SPo,SensorC=0,IOut=0,IOut_1=0,eKp,DOut=0,DOut_1=0;
float current_mA1 = 0;
float frecuencia=1000;
float T=1/frecuencia;

void setup(){
  Serial.begin(19200);
  pinMode(D10, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D9, OUTPUT);
  pinMode(D11, OUTPUT);
  
  
  //******************************* INA3221*****************************
  ina3221.begin();
  //Serial.print("Manufactures ID=0x");
  int MID;
  MID = ina3221.getManufID();
  //Serial.println(MID,HEX);
  //********************************************************************
  noInterrupts();           // deshabilita las interrupciones

  //**********************TIMER 0**********************

   TCCR0A = 0;                        // importante colocar en cero
   TCCR0B = 0;                        // importante colocar en cero
   TCNT0  = 0;                        // importante colocar en cero

   TCCR0A |= (1 << WGM01);            // CTC mode
   TCCR0B = _BV(CS02)| _BV(CS00);     // Clock/1024   //prescaler1024
   OCR0A  = 15; //(16000000/1024/frec=1000Hz)-1;       // 
   TIMSK0 |= (1 << OCIE0A);           // enable 
  
  //*********************TIMER 2***********************

   TCCR2A = 0;                        // importante colocar en cero
   TCCR2B = 0;                        // importante colocar en cero
   TCNT2  = 0;                        // importante colocar en cero
                                                            
  //*********TIMER 2 Fast PWM 8-bit mode***************
  TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);  //Clear mode
  TCCR2B = (0 << CS22)| _BV(CS20);
  interrupts();
}

ISR(TIMER0_COMPA_vect){          // timer 0  (cada 1 mS)   
 mil =mil+1;
 if(mil == 1000.0){
     sec=sec+1.0;
    mil=0; 
  }
 t= sec+mil/1000.0;

 //Out1 = 255.0*sin(2.0*PI*0.2*t);     // SENO para EL PIN D3, D4 y D5
 Out2 = 40.0*sin(2.0*PI*0.5*t)+50;    // SENO para EL PIN D11, D10 y D9

SPo=40.0*sin(2.0*PI*0.2*t)+50;

e=SPo-SensorC;

//*******Control Proporcional*********
eKp=Kp*e;
//*******Control Integral*************
IOut=e*Ki*T+IOut_1;
IOut_1=IOut;
//*******Control Derivativo**********
/*
DOut=(e*Td-DOut_1)/T;
DOut_1=e*Td;
*/

u=eKp+IOut;//+DOut;
u=constrain(u,-254,254);

if (Out2 >= 0.0) {  un = u;digitalWrite(D4,HIGH);digitalWrite(D5,LOW);OCR2B  =  Out2;} // D4 y D5 Compuerta AND seno POSITIVO
else { un = -u;digitalWrite(D4,LOW);digitalWrite(D5,HIGH);OCR2B  =  abs(Out2);} // D4 y D5 Compuerta AND seno NEGATIVO

if (SPo >= 0.0) {digitalWrite(D10,HIGH);digitalWrite(D9,LOW);OCR2A=u;SensorC=current_mA1;} // D10 y D9 Compuerta AND seno POSITIVO
else {digitalWrite(D10,LOW);digitalWrite(D9,HIGH);OCR2A=abs(u);SensorC=-current_mA1;} // D10 y 9 Compuerta AND seno NEGATIVO


}

void loop() {
  
//*************************INA3221*************************************
  current_mA1 = ina3221.getCurrent_mA(LIPO_BATTERY_CHANNEL);
 //*******************************************************************
//**********************Leyendas*********************
  Serial.println("u,e,SPo,SensorC");
  //*************************************************** IMPORTANTE NO MOVER O HAY CONFLICTO CON EL INA3221
Serial.print(u);
Serial.print(",");
Serial.print(e);
Serial.print(",");
Serial.print(SPo);
Serial.print(",");
Serial.println(SensorC);

}
