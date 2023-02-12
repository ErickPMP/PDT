#define PI 3.1415926535897932384626433832795
float mil,sec=0,t;
float frecuencia=0.5;
float T=1/frecuencia;
float Out1,Out2,Out1a,Out1b,Out2a,Out2b;
void setup() {
  Serial.begin(115200);
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

  //******************************* INA3221*****************************
  //Serial.println("SDA_Arduino_INA3221_Test");
  //Serial.println("Measuring voltage and current with ina3221 ...");
  //ina3221.begin();
  //Serial.print("Manufactures ID=0x");
  //int MID;
  //MID = ina3221.getManufID();
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
 //**************************Señal seno alterna **************************************************
 Out1 = 100.0*sin(2.0*PI*0.5*t); 
 if (Out1 <5 && Out1 >-5)
 {
  for(int i = 0; i<20000 ; i++)
  {
    Out2 = 0*t;
  }
  
 }
 else {Out2 = Out1;}
}


void loop() {
  Serial.println(Out2);
  delay(100);

}
