
#include <Wire.h>            // access to the I2C library to communicate with the Gyro

#define GYRO_PWR_MGMT     0x6B                  // REG ADDR
#define GYRO_G_CONFIG     0x1B                  // REG ADDR
#define GYRO_CONFIG       0x1A
#define TILT_GYRO       0x69                  // I2C address of the MPU6050 gyro, pin SA0 tied HIGH
#define ROLL_GYRO       0x68                  // I2C address, pin SA0 tied LOW

int tiltvalue=0, rollvalue=0 ;

void setup() {
  Wire.begin();                             // set up I2C for gyro on pin 20
  Serial.begin(57600);                       // set up serial communication on USB
   
// http://www.i2cdevlib.com/devices/mpu6050#registers
 //  or Documents/Balancer/MPU6050 Datasheet.pdf
  writeI2C(TILT_GYRO, GYRO_PWR_MGMT, B00000000);       // bit 6=0 to wake
  writeI2C(TILT_GYRO, GYRO_G_CONFIG,   B00000000);       // bits 3 & 4 = 0 for full scale 250 deg/sec
  writeI2C(ROLL_GYRO, GYRO_PWR_MGMT, B00000000);       // bit 6=0 to wake
  writeI2C(ROLL_GYRO, GYRO_G_CONFIG,   B00000000);       // bits 3 & 4 = 0 for full scale 250 deg/sec
//  writeI2C(ROLL_GYRO, GYRO_CONFIG,    B00000110);        // 5 Hz bandwidth, 18.6 ms delay, 1 KHz sampling
//  writeI2C(ROLL_GYRO, GYRO_CONFIG,    B00000001);        // 188 Hz bandwidth, 1.9 ms delay, 1 KHz sampling
   

}

void loop() {
  rollvalue = getGyroX(ROLL_GYRO);
  Serial.println(rollvalue);
  delay(100);

}


int getGyroX (int GYRO_ADDR) {
  byte MSB, LSB;
  MSB = readI2C(GYRO_ADDR, 0x43);  // reads x axis angular velocity
  LSB = readI2C(GYRO_ADDR, 0x44);
  return ((MSB << 8) | LSB);
}

int getGyroY (int GYRO_ADDR) {
  byte MSB, LSB;
  MSB = readI2C(GYRO_ADDR, 0x45);  // reads x axis angular velocity
  LSB = readI2C(GYRO_ADDR, 0x46);
  return ((MSB << 8) | LSB);
}

int getGyroZ (int GYRO_ADDR) {
  byte MSB, LSB;
  MSB = readI2C(GYRO_ADDR, 0x47);  // reads x axis angular velocity
  LSB = readI2C(GYRO_ADDR, 0x48);
  return ((MSB << 8) | LSB);
}

int readI2C (int GYRO_ADDR,byte regAddr) {                  // read Gyro communications
    Wire.beginTransmission(GYRO_ADDR);                  // Register address to read
    Wire.write(regAddr);                     // Register address to read
    Wire.endTransmission();                 // Terminate request   
    Wire.requestFrom( GYRO_ADDR, 1);              // Read a byte
    while(!Wire.available()) { };           // Wait for receipt
    return(Wire.read());                 // Get result
}

void writeI2C (int GYRO_ADDR, byte regAddr, byte val) {
    Wire.beginTransmission(GYRO_ADDR);           // Begin transmission
    Wire.write(regAddr);                     // Open communications
    Wire.write(val);                         // Send value
    Wire.endTransmission();                 // End transmission
}
