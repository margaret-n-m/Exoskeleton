#include <SPI.h>

#define DIR 6
#define STEP 5
#define EN 7
#define CS 10
#define MISO 1           //icsp header
#define MOSI 4           //icsp header
#define CLK 3            //icsp header


//setting parameters

void setRegister(uint8_t adress, uint32_t newBytes) {
  uint8_t rx_buf[5];   
   
   SPI.beginTransaction(SPISettings(16000000/8, MSBFIRST, SPI_MODE3));
   digitalWrite(CS, LOW);     //chip select enabled  
     rx_buf[0] = SPI.transfer(adress); 
     rx_buf[1] = SPI.transfer((newBytes & 0xFF000000) >> 24); 
     rx_buf[2] = SPI.transfer((newBytes & 0xFF0000) >> 16); 
     rx_buf[3] = SPI.transfer((newBytes & 0xFF00) >> 8); 
     rx_buf[4] = SPI.transfer(newBytes & 0xFF); 
   digitalWrite(CS, HIGH);
   SPI.endTransaction();   
   
//CHECKING OUT WHAT WAS SENT VIA SPI
       Serial.print("Sent:  ");        
       Serial.print(adress, HEX);
       Serial.print(((newBytes & 0xFF000000) >> 24), HEX);
       Serial.print(((newBytes & 0xFF0000) >> 16), HEX);
       Serial.print(((newBytes & 0xFF00) >> 8), HEX);
       Serial.println((newBytes & 0xFF), HEX);  
       
//CHECKING OUT WHAT WAS RECEIVED VIA SPI
       Serial.print("Received:  ");  
      for(int i = 0; i<5; i++){
       Serial.print(rx_buf[i], HEX);
       }
       Serial.println("");
}

void setup() {
  
  pinMode(DIR, OUTPUT);
  pinMode(STEP, OUTPUT);
  pinMode(EN, OUTPUT);
  pinMode(CS, OUTPUT);

  digitalWrite(EN, LOW);        //switching the driver on
  digitalWrite(CS, HIGH);       //chip select disabled

//SETTING THE PARAMETERS
    Serial.begin(9600);
    SPI.begin();

      delay(100);
      setRegister(0x80, 0x00000000); // GCONF: no analog scaling, stealthChop disabled
      delay(100);
      setRegister(0x90, 0x00000000); // IHOLD IRUN: all current 0 (I_RUN 0, I_HOLD 0, IHOLDDELAY 0)
      delay(100);
      setRegister(0x91, 0x00000000); // TPOWERDOWN: 0, immediate power down
      delay(100);
      setRegister(0x93, 0x00000000); // TPWMTHRS: upper velocity for stealthChop disabled
      delay(100);
      setRegister(0x94, 0x00000000); // TCOOLTHRS: lower velocity for CoolStep disabled
      delay(100);
      setRegister(0x95, 0x00000000); // THIGH: velocity dependent swithing to another mode disabled
      delay(100);
      setRegister(0xAD, 0x00000000); // XDIRECT: normal SPI operation
      delay(100);
      setRegister(0xB3, 0x00000000); // VDCMIN: automatic commutation dcStep disabled
      delay(100);
      setRegister(0xEC, 0x00000000); // CHOPCONF: native 256 microstepping, vsense 0, TBL 0, chm 0, HEND 0, HSTRT 0, TOFF 0
      delay(100);
/*    setRegister(0xE0, 0xAAAAB554); // MSLUT0
      delay(100);
      setRegister(0xE1, 0x4A9554AA); // MSLUT1
      delay(100);
      setRegister(0xE2, 0x24492929); // MSLUT2
      delay(100);
      setRegister(0xE3, 0x10104222); // MSLUT3
      delay(100);
      setRegister(0xE4, 0xFBFFFFFF); // MSLUT4
      delay(100);
      setRegister(0xE5, 0xB5BB777D); // MSLUT5
      delay(100);
      setRegister(0xE6, 0x49295556); // MSLUT6
      delay(100);   
      setRegister(0xE7, 0x00404222); // MSLUT7
      delay(100); 
      setRegister(0xE8, 0xFFFF8056); // MSLUTSEL
      delay(100);
      setRegister(0xE0, 0xAAAAB554); // MSLUT0
      delay(100);
      setRegister(0xE0, 0xAAAAB554); // MSLUT0
      delay(100);
      setRegister(0xE9, 0x00F70000); // MSLUTSTART
      delay(100); */
      setRegister(0xED, 0x00000000); // COOLCONF: standard stallGuard mode, threshold value 0, seimin 1/2 I_RUN, semax 0, semin 0 (coolStep off)
      delay(100); 
      setRegister(0xF0, 0x00050480); // PWMCONF: normal standstill operation, standard PWM mode, automatic current control, fPWM = 2/683fCLK, PWM_GRAD 4, PWM_AMPL 128
      delay(100); 
      setRegister(0xF2, 0x00000000); // ENCM_CTRL: encoder mode configuration disabled
      delay(100);           
/*    setRegister(0xEE, 0x00000000); // DCCTRL
      delay(100);    
      setRegister(0x01, 0x00000000); // GSTAT: read only
      delay(100);  */
      setRegister(0xEC, 0x00024008); // CHOPCONF: native 256 microstepping, vsense 1, TBL 1, chm 0, HEND 0, HSTRT 0, TOFF 8
      delay(100);     
      setRegister(0x90, 0x00001309); // IHOLD_IRUN: I_RUN  19 I_HOLD 9 (calculated, normally 70% of I_RUN), IHOLDDELAY 0
      delay(100);                      
      setRegister(0x6F, 0x00000000); //reading the DRV_STATUS
      delay(100);
      setRegister(0x6F, 0x00000000); //additional transfer to receive last inquiery
      delay(100);
      
    SPI.end();
   
  digitalWrite(DIR, LOW);       //direction setting
}

void loop() {
  digitalWrite(STEP, HIGH);
  delayMicroseconds(10);
  digitalWrite(STEP, LOW);
  delayMicroseconds(10);

}
