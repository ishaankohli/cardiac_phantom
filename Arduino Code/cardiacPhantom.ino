/*----------------------------------------------------------------------------------
Cardiac Phantom Quasar Insert

Hardware:
Arduino Nano BLE 33
L298N Motor Drive Controller

Ishaan Kohli
Supervisor: Don Ta & Steven Thomas

*/
//--------------------------------- Bluetooth Constructor --------------------------
bool start = false;
short Speed =1;
short Amplitude = 1;
#include <ArduinoBLE.h>
BLEService phantomService("5b1512ba-16f1-4a80-9200-5b5bc3327b85");
BLEShortCharacteristic speedShort("6901123c-e494-4c7c-ad5f-bf1541c6fd2b", BLERead | BLENotify | BLEWrite);
BLEShortCharacteristic ampShort("24b3d8a8-cff7-4832-b391-65d8af8c65e1", BLERead | BLENotify | BLEWrite);
BLECharCharacteristic startChar("0fdab7e1-cab0-4d43-a969-61bf7ca6fe2a", BLERead | BLENotify | BLEWrite);

void blePeripheralConnectHandler(BLEDevice central) {
  // central connected event handler
  Serial.print("Connected event, central: ");
  Serial.println(central.address());
}
void blePeripheralDisconnectHandler(BLEDevice central) {
  // central disconnected event handler
  Serial.print("Disconnected event, central: ");
  Serial.println(central.address());
}
void startCharacteristicWritten(BLEDevice central, BLECharacteristic characteristic) {
  if (startChar.value() == 0){
    start = false;
    Serial.println("stop");
    startPosition();
  } else if (startChar.value() == 1){
    start = true;
    Serial.println("start");
  }
}
void speedWritten(BLEDevice central, BLECharacteristic characteristic){
  int add = speedShort.value();
  Serial.println(add);
}
void ampWritten(BLEDevice central, BLECharacteristic characteristic){
  Amplitude = ampShort.value();
  Serial.println(Amplitude);
}

void bleBegin(){
  if (!BLE.begin()){Serial.println("starting BLE failed!"); while (1);}
  BLE.setLocalName("CardiacPhantom");
  BLE.setAdvertisedService(phantomService);
  phantomService.addCharacteristic(speedShort);
  phantomService.addCharacteristic(ampShort);
  phantomService.addCharacteristic(startChar);
  speedShort.setEventHandler(BLEWritten, speedWritten);
  ampShort.setEventHandler(BLEWritten, ampWritten);
  startChar.setEventHandler(BLEWritten, startCharacteristicWritten);
  BLE.addService(phantomService);
  BLE.advertise();
  Serial.println("Bluetooth device active, waiting for connections...");
}
//--------------------------------- Setup & Loop ---------------------------------

void setup() {
  initializePins(); // see file: cp_Motion_Basic
  Serial.begin(9600);
  bleBegin();
  calibrate(); // see file: cp_Motion_Calibration
  startPosition(); // see file: cp_Motion_Calibration
}
void loop() {
  BLEDevice central = BLE.central();
  central.poll();
  if (start){
    osccillate(); // see file: cp_Motion_Cardiac
  }
}
