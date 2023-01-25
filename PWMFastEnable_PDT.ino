// C++ code
#define PI 3.1415926535897932384626433832795
#include <Wire.h>

int D10=10;
int D4=4;
int D3=3;
int D5=5;
int D9=9;
int D11=11;
int duty1=75;
int duty2=75;
float   mil,sec=0,t;
float Out1,Out2,Out1a,Out1b,Out2a,Out2b;
void setup(){
  Serial.begin(9600);
  pinMode(D10, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D9, OUTPUT);
  pinMode(D11, OUTPUT);
  
  noInterrupts();           // deshabilita las interrupciones
  
  // TIMER 2

   TCCR2A = 0;                        // importante colocar en cero
   TCCR2B = 0;                        // importante colocar en cero
   TCNT2  = 0;                        // importante colocar en cero
                                                            
   TCCR2A |= (1 << WGM21);            // CTC mode
   TCCR2B = _BV(CS22)| _BV(CS20);     // Clock/1024   //prescaler128
   OCR2A  = 124; //(16000000/128/frec=1000Hz)-1;       // 
   TIMSK2 |= (1 << OCIE2A);           // enable 


  // TIMER 2 Fast PWM 8-bit mode
  TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);  //Clear mode
  TCCR2B = (0 << CS22)| _BV(CS20);
  
  OCR2B  =  ((16000000/1/62500)-1)*duty1/100;
  OCR2A  =  ((16000000/1/62500)-1)*duty2/100;
  interrupts();

}

ISR(TIMER2_COMPA_vect){          // timer 0  (cada 1 mS)   
 mil =mil+1;
 if(mil == 1000.0){
   	sec=sec+1.0;
   	mil=0; 
  }
 t= sec+mil/1000.0;

 Out1 = 255.0*sin(2.0*PI*0.2*t);     // relacionado ao pino 6 
 Out2 = 255.0*sin(2.0*PI*0.2*t);    // relacionado ao pino 7
 if (Out1 >= 0.0) {  Out1a = Out1;} // pina 5 || pinb 6
else { Out1b = abs(Out1);} // pina 5 || pinb 6
analogWrite(D4,Out1a);
if (Out1a>=127){digitalWrite(D5,LOW);}
else{digitalWrite(D5,HIGH);}

if (Out2 >= 0.0) {  Out2a = Out2;} // pina 5 || pinb 6
else { Out2b = abs(Out2);} // pina 5 || pinb 6
analogWrite(D10,Out2a);
if (Out2a>=127){digitalWrite(D9,LOW);}
else{digitalWrite(D9,HIGH);}
//analogWrite(D5,-Out1a);
}

void loop() {
 //digitalWrite(D4,HIGH);      // HIGH
 //digitalWrite(D10,HIGH);     // HIGH
 //digitalWrite(D5, LOW);      // LOW
 //digitalWrite(D9, LOW);      // LOW
 //Serial.println(Out1);
 //analogWrite(D3,Out1);
 //analogWrite(D11,Out2);
}