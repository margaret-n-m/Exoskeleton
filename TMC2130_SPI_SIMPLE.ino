#include <SPI.h>

#define DIR 6
#define STEP 5
#define EN 7
#define CS 10
#define MISO 1 //icsp header
#define MOSI 4 //icsp header
#define CLK 3 //icsp header

//setting parameters

void setRegister(uint8_t adress, uint32_t newBytes) {
  uint8_t rx_buf[5];   
   
   SPI.beginTransaction(SPISettings(16000000/8, MSBFIRST, SPI_MODE3));
   digitalWrite(CS, LOW); //chip select      
     rx_buf[0] = SPI.transfer(adress); 
     rx_buf[1] = SPI.transfer((newBytes & 0xFF000000) >> 24); 
     rx_buf[2] = SPI.transfer((newBytes & 0xFF0000) >> 16); 
     rx_buf[3] = SPI.transfer((newBytes & 0xFF00) >> 8); 
     rx_buf[4] = SPI.transfer(newBytes & 0xFF); 
   digitalWrite(CS, HIGH);
   SPI.endTransaction();   
   
//checking out what was sent via SPI  
       Serial.print("Sent:  ");        
       Serial.print(adress, HEX);
       Serial.print(((newBytes & 0xFF000000) >> 24), HEX);
       Serial.print(((newBytes & 0xFF0000) >> 16), HEX);
       Serial.print(((newBytes & 0xFF00) >> 8), HEX);
       Serial.println((newBytes & 0xFF), HEX);  
       
//checking out what came back via SPI
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

  digitalWrite(EN, LOW); //switching the driver on
  digitalWrite(CS, HIGH);

//setting the parameters according to the datasheet
    Serial.begin(9600);
    SPI.begin();
      delay(100);
      setRegister(0xEC, 0x000080C8); //vsense 1, TOFF 8, TBL 1, HSTRT 4, HEND 1, CHM 0 - spreadCycle
      delay(100);
      setRegister(0x90, 0x0001130A); //I_RUN  19 I_HOLD 10 (calculated, normally 70% of I_RUN), IHOLDDELAY 1
      delay(100);
      setRegister(0x91, 0x0000000A); //TPOWERDOWN 10
      delay(100);
      setRegister(0x80, 0x00000004); // no analog scaling, en_pwm_mode 1
      delay(100);
      setRegister(0x93, 0x000001F4); //stealthChop (TPWMTHRS)
      delay(100);
      setRegister(0xF0, 0x000401C8); //autoPWM, 2/1024 Fclk
      delay(100);
      setRegister(0x6F, 0x00000000); //reading the DRV_STATUS
      delay(100);
      setRegister(0x6F, 0x00000000); //additional transfer to receive last inquiery
      delay(100);
    SPI.end();
  digitalWrite(DIR, HIGH); //direction setting
}

void loop() {
  
  digitalWrite(STEP, HIGH); //lets make some steps
  delay(1000);
  digitalWrite(STEP, LOW);
  delay(1000);
}
