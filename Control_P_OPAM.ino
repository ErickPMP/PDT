#define PI 3.1415926535897932384626433832795

float Kp=2, Ki=0, Td=0;
int D10=10,D4=4,D3=3,D5=5,D9=9,D11=11;
float mil,sec=0,t;
float e,u,SPo,SensorC=0,IOut=0,IOut_1=0,eKp,DOut=0,DOut_1=0;
float frecuencia=1000;
float T=1/frecuencia;
double sen1=0;
void setup(){
  Serial.begin(9600);
  pinMode(D10, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D9, OUTPUT);
  pinMode(D11, OUTPUT);
  
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

SPo=100.0*sin(2.0*PI*0.2*t);

e=SPo-SensorC;

//*******Control Proporcional*********
eKp=Kp*e;
//*******Control Integral*************
IOut=e*Ki*T+IOut_1;
IOut_1=IOut;
//*******Control Derivativo**********
DOut=(e*Td-DOut_1)/T;
DOut_1=e*Td;


u=eKp+IOut+DOut;
u=constrain(u,-254,254);

if (u >= 0.0) {digitalWrite(D4,HIGH);digitalWrite(D5,LOW);OCR2B  =  50;} // D4 y D5 Compuerta AND seno POSITIVO
else {digitalWrite(D4,LOW);digitalWrite(D5,HIGH);OCR2B  =  abs(50);} // D4 y D5 Compuerta AND seno NEGATIVO

if (u >= 0.0) {digitalWrite(D10,HIGH);digitalWrite(D9,LOW);OCR2A=u;SensorC=sen1;} // D10 y D9 Compuerta AND seno POSITIVO
else {digitalWrite(D10,LOW);digitalWrite(D9,HIGH);OCR2A=abs(u);SensorC=-sen1;} // D10 y 9 Compuerta AND seno NEGATIVO

}

void loop() {
  //*********Lectura del A0**********
  //float sen0=analogRead(A0);
  sen1=analogRead(A1)*5.0/1024.0/42.0/0.1*100.0;
  //**********************Leyendas*********************
  Serial.println("u,e,SetPoint,Corriente");
  //*************************************************** IMPORTANTE NO MOVER O HAY CONFLICTO CON EL INA3221
Serial.print(u);
Serial.print(",");
Serial.print(e);
Serial.print(",");
Serial.print(SPo);
Serial.print(",");
Serial.println(SensorC);

}
