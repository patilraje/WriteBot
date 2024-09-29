
#include <ArduinoBLE.h>

BLEService ServiceID("19B10000-E8F2-537E-4F6C-D104768A1214"); // Bluetooth® Low Energy LED Service

// Bluetooth® Low Energy LED Switch Characteristic - custom 128-bit UUID, read and writable by central
BLEByteCharacteristic switchCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
#include <SparkFun_TB6612.h>

#define AIN1 2
#define BIN1 7
#define AIN2 4
#define BIN2 8
#define PWMA 5
#define PWMB 6
#define STBY 9

#define FORWARD_DELAY 2000
#define BACKWARD_DELAY 1000

#define TURN_DELAY 600
// these constants are used to allow you to make your motor configuration 
// line up with function names like forward.  Value can be 1 or -1
const int offsetA = 1;
const int offsetB = 1;

// Initializing motors.  The library will allow you to initialize as many
// motors as you have memory for.  If you are using functions like forward
// that take 2 motors as arguements you can either write new functions or
// call the function more than once.
Motor motor1 = Motor(AIN1, AIN2, PWMA, offsetA, STBY);
Motor motor2 = Motor(BIN1, BIN2, PWMB, offsetB, STBY);

void setup()
{
  Serial.begin(9600);
  while (!Serial);

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting Bluetooth® Low Energy module failed!");

    while (1);
  }

  // set advertised local name and service UUID:
  BLE.setLocalName("bot");
  BLE.setAdvertisedService(ServiceID);

  // add the characteristic to the service
  ServiceID.addCharacteristic(switchCharacteristic);

  // add service
  BLE.addService(ServiceID);

  // set the initial value for the characeristic:
  switchCharacteristic.writeValue(0);

  // start advertising
  BLE.advertise();

  Serial.println("BLE LED Peripheral");


}


void loop()
{
   // A
   motor2.drive(150,100); //right turn 45 deg
delay(100);
   brake(motor1, motor2);

motor2.drive(200,3000);
motor1.drive(150,3000);

  //  forward(motor1, motor2, 200);
 
delay(3000);
   brake(motor1, motor2);

   motor1.drive(150,600); //right turn 45 deg
delay(600);   
brake(motor1, motor2);
   back(motor1, motor2, -200);
 
delay(3000);
   brake(motor1, motor2);
delay(10000);

//S

   forward(motor1, motor2, 200); //_|
  delay(FORWARD_DELAY);
   brake(motor1, motor2);

   motor1.drive(175,600); //right turn 90 deg
  delay(500);

   forward(motor1, motor2, 200); //_|
   delay(FORWARD_DELAY/2);
   brake(motor1, motor2);
 
   motor1.drive(175,600); //turn left 90 deg
   delay(1000);

   forward(motor1, motor2, 200); //_|
   delay(FORWARD_DELAY);
   brake(motor1, motor2);

  //  motor2.drive(200,200); //turn left 90 deg
   motor1.drive(-200,650); //turn left 90 deg
   delay(1000);

   forward(motor1, motor2, 200); //_|
   delay(FORWARD_DELAY);
   brake(motor1, motor2);

  //  motor2.drive(200,200); //turn left 90 deg
   motor1.drive(-200,650); //turn left 90 deg
   delay(1000);

   forward(motor1, motor2, 200); //_|
   delay(FORWARD_DELAY);
   brake(motor1, motor2);

   delay(10000);

   // //U

   forward(motor1, motor2, 200); //_|
  delay(FORWARD_DELAY);
   brake(motor1, motor2);

   motor1.drive(175,600); //right turn 90 deg
  delay(500);

   forward(motor1, motor2, 200); //_|
   delay(FORWARD_DELAY/2);
   brake(motor1, motor2);
 
   motor1.drive(175,600); //turn left 90 deg
   delay(1000);

   forward(motor1, motor2, 200); //_|
   delay(FORWARD_DELAY);
   brake(motor1, motor2);


   delay(10000);
  // listen for Bluetooth® Low Energy peripherals to connect:
  BLEDevice central = BLE.central();

  // if a central is connected to peripheral:
  if (central) {
    Serial.print("Connected to central: ");
    // print the central's MAC address:
    Serial.println(central.address());

    // while the central is still connected to peripheral:
    while (central.connected()) {
      if (switchCharacteristic.written()) {
        if (switchCharacteristic.value() == 1) {  
          Serial.println("Forward");
             forward(motor1, motor2, 150);
             delay(1000); 
        } else if (switchCharacteristic.value() == 2){                 
          Serial.println("Backward");
             back(motor1, motor2, -150);
             delay(1000);
        }  else if (switchCharacteristic.value() == 3){              
          Serial.println("Left");
            //  left(motor1, motor2, 100);
   brake(motor1, motor2);
   motor2.drive(150,1000); //right turn 45 deg

             delay(1000);
        }  else if (switchCharacteristic.value() == 4){       
          Serial.println("Right");
            //  right(motor1, motor2, 100);
               brake(motor1, motor2);
   motor1.drive(150,1000); //right turn 45 deg
             delay(1000);
        } else {                     
          Serial.println("Brake");
             brake(motor1, motor2);
             delay(1000);
        }
      }
    }

    // when the central disconnects, print it out:
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
  
  }
  // right(motor1, motor2, 200);
  // forward(motor1, motor2, 150);

  // delay(1000);
  //  brake(motor1, motor2);
  //  delay(10000);
}

/*
// speed, duration
   motor1.drive(255,1000);
   motor1.drive(-255,1000);
   motor1.brake();
   motor2.drive(255,1000);
   motor2.drive(-255,1000);
   motor2.brake();
   forward(motor1, motor2, 150);
   back(motor1, motor2, -150);
   brake(motor1, motor2);
   left(motor1, motor2, 100);
   right(motor1, motor2, 100);
   brake(motor1, motor2);
  */


  /*
For A:
slight right turn - 500ms
forward - 1 unit
slight left turn - 1000ms - to negate prev right turn - double
backward - 1 unit

transition to S - 90 degree right turn & 0.5 units forward 
0.75 units forward
left turn - check duration
right turn - check duration

transition to U - 0.5 units forward, & 90degree right turn
0.75 units forward
left turn - check duration
0.75 units forward

stop

///-------------ASU start----------------------------------
// A:
  right(motor1, motor2, 100);
  delay(500);
  forward(motor1, motor2, 150);
  delay(1000);
  left(motor1, motor2, 100);
  delay(1000);
  back(motor1, motor2, 150);
  delay(1000);

// -- A done 
//
// -- A to S





*/
