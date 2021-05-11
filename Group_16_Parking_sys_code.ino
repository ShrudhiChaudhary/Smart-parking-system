//Pseudo code for interfacing IR sensors and LEDs with Arduino MEGA.
//Rough skeleton code to show the Digital Communication with NodeMCU.
//This is the code which will be uploaded on the Arduino MEGA.

//////////////////////////////////////////////////////////////////////

//We are showing the code structure for a single parking slot.
//This same code can be extended for all the 3 parking slots in our prototype.
//Each parking slot consists of 4 IR sensors and 3 LEDs.

/////////////////////////////////////////////////////////////////////

//Initializing variables for Parking Slot 1.
//PCB can be referred for the same.
int IR_1 = 12; 
int IR_2 = 10;
int IR_3 = 8;
int IR_4 = 6;
// The trigger pins of IR sensors are connected to digital pins of Arduino Mega 

int R_pin = A1;
int Y_pin = A0;
int G_pin = A2;
//The LEDs are connected to Analog Pin. Analog Pins can be made to work as digital pins by sending either 0 (LOW) or 255 (HIGH) as per requirement.

void setup() 
{
 pinMode(IR_1, INPUT);
 pinMode(IR_2, INPUT);
 pinMode(IR_3, INPUT);
 pinMode(IR_4, INPUT);
 //IR trigger pins will serve as input to Arduino

 pinMode(R_pin, OUTPUT);
 pinMode(Y_pin, OUTPUT);
 pinMode(G_pin, OUTPUT);
//LEDs are the output in our system

Serial.begin(9600); //baud rate set
}

void loop() 
{
  //The algorithm runs in this part
  //IR is an active low device, so it gives a LOW when it detects an object.

  if (IR_1==LOW && IR_2==LOW && IR_3==LOW && IR_4==LOW) // if all IR sensors are detecting a vehicle
  {
    analogWrite(R_pin, 255);
    analogWrite(Y_pin, 0);
    analogWrite(G_pin, 0);
    //RED light shows up in the parking slot.
    
    digitalWrite(30, HIGH);
    //This pin establishes and initiates digital communication with Node MCU. Node MCU reads this pin as HIGH and updates the website saying the slot is OCCUPIED.
    digitalWrite(31, LOW);
    digitalWrite(32, LOW);
    //LOW signal ensures that conditions dont conflict with the previous status.    
  }

  if (IR_1==LOW || IR_2==LOW || IR_3==LOW || IR_4==LOW) // if few IR sensors are detecting a vehicle means vehicle is moving and not stationary
  {
    analogWrite(R_pin, 0);
    analogWrite(Y_pin, 255);
    analogWrite(G_pin, 0);
    //YELLOW light shows up in the parking slot.
    
    digitalWrite(31, HIGH);
    //This pin establishes and initiates digital communication with Node MCU. Node MCU reads this pin as HIGH and updates the website saying the slot is BUSY.
    digitalWrite(30, LOW);
    digitalWrite(32, LOW);
    //LOW signal ensures that conditions dont conflict with the previous status.
  }

  if (IR_1==HIGH && IR_2==HIGH && IR_3==HIGH && IR_4==HIGH) // if all IR sensors are detecting nothing, means the slot is empty.
  {
    analogWrite(R_pin, 0);
    analogWrite(Y_pin, 0);
    analogWrite(G_pin, 255);
    //GREEN light shows up in the parking slot.
    
    digitalWrite(32, HIGH);
    //This pin establishes and initiates digital communication with Node MCU. Node MCU reads this pin as HIGH and updates the website saying the slot is OCCUPIED.
    digitalWrite(31, LOW);
    digitalWrite(30, LOW);
    //LOW signal ensures that conditions dont conflict with the previous status.    
  }
}

//The digitalWrite() under each IF statement is very important for the digital communication that we are using. The logic level on these pins are responsible for updating the web page.
//All the details and status on our web page are dependent on the digital pins connected between Arduino MEGA and Node MCU.
