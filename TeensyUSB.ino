/* Basic USB Joystick Example
   Teensy becomes a USB joystick

   You must select Joystick from the "Tools > USB Type" menu

   Pushbuttons should be connected to digital pins 0 and 1.
   Wire each button between the digital pin and ground.
   Potentiometers should be connected to analog inputs 0 to 1.

   This example code is in the public domain.
*/
        ////////////////////////////////////////////////////////////////////////////////
        // EEPROM include
        ////////////////////////////////////////////////////////////////////////////////          
        #include "EEPROM.h"

        ////////////////////////////////////////////////////////////////////////////////
        // Library for the OLED screen 
        ////////////////////////////////////////////////////////////////////////////////    
        #include <evilOLED.h>
        evilOLED disp(A4,A5);// initialise display (SDA,SCL)
        
        ////////////////////////////////////////////////////////////////////////////////
        // variable init
        ////////////////////////////////////////////////////////////////////////////////          
        int i;
        unsigned int carspeed;      // holds the speed data (0-65535 size)
        unsigned int gear;          // holds gear value data (0-65535 size)
        unsigned int rpm;           // holds the rpm data (0-65535 size)
        unsigned int oil;           // holds the oil temp data (0-65535 size)
        unsigned int water;         // holds the water temp data (0-65535 size)
        unsigned int fuel;          // holds the fuel data (0-65535 size)
        unsigned int lap;           // holds the lap data (0-65535 size)
        unsigned int posit;         // holds the position data (0-65535 size)
        
        unsigned int rpmleds;          // holds the 8 leds values
        unsigned int rpmmax = 5000;    // retrieves from rpm live with minimum setting on initialization and Shutdown - Output
        unsigned int beforeButton = 0; // holds the previous pressed button        
        unsigned int valButton = 0;    // retrieve the pressed buttons
        unsigned int offset = 0;       // holds the display position
        unsigned int leftButton;       // holds the pressed button (default left gear)
        unsigned int rightButton;      // holds the pressed button (default right speed)
        unsigned long timeButton;      // holds the first pressed button time
        
        byte speeddata = 0;         // marker that new data are available
        byte geardata = 0;          // marker that new data are available   
        byte rpmdata = 0;           // marker that new data are available
        byte oildata = 0;           // marker that new data are available
        byte waterdata = 0;           // marker that new data are available
        byte fueldata = 0;           // marker that new data are available
        byte lapdata = 0;           // marker that new data are available
        byte positiondata = 0;           // marker that new data are available
        
        // char* neutral = "n";        // sets the character for neutral
        char* neutral = "0";        // sets the character for neutral
        char* reverse = "r";        // sets the character for reverse

        const char test[1024] PROGMEM = { //Display aus (keine Ausgabe)
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
        B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000
        };

        void setup()
        {

        ////////////////////////////////////////////////////////////////////////////////
        // Set SDA + SCL pins as output
        ////////////////////////////////////////////////////////////////////////////////    
        pinMode(A4,OUTPUT);// set SDA + SCL pins as output (Pin 19 A5 and 18 A4
        pinMode(A5,OUTPUT);

        ////////////////////////////////////////////////////////////////////////////////
        // Setup of the pins used as analog inputs
        ////////////////////////////////////////////////////////////////////////////////
          pinMode(A10, INPUT_PULLUP);
          pinMode(A11, INPUT_PULLUP);
          pinMode(A21, INPUT_PULLUP);
          pinMode(A22, INPUT_PULLUP);
        }
        
        void loop() {
          readButtons();
          readSerialData();      
          setDisplayOutput();
        }

        ////////////////////////////////////////////////////////////////////////////////
        // Procedure: read Button inputs / encoders
        ////////////////////////////////////////////////////////////////////////////////
        void readButtons()
        ////////////////////////////////////////////////////////////////////////////////
        {
          // Pin 54-57 Encoder middle (Only three pins needed, if one (A), two (B) and three (C) isn't active it must be four (D))
          if (digitalRead(54)) { //A (two independent axis)
            Joystick.Z(analogRead(A21));
            Joystick.Zrotate(analogRead(A22));    
          } else if(digitalRead(55)) { //B (As single Clutch)
            if(analogRead(A21)>analogRead(A22)) {
              Joystick.Z(analogRead(A21));
            } else { 
              Joystick.Z(analogRead(A22));
            }
          } else if(digitalRead(55)) { //C (Single Clutch + right side Bite Point) Use Encoder (Up-, Down-) (Use Shift Light LEDs for configuration?)
            
          } else { //D (Single Clutch + left side Bite Point)
              
          }
          
          // read analog inputs and set X-Y position
          Joystick.X(analogRead(A10));
          Joystick.Y(analogRead(A11));
        
        /*
          // If Both Shift Paddles are activated at the same time engage clutch else ToDo: Update Numbers (1,2)
          if (digitalRead(1) && digitalRead(2)) {
            Joystick.Z(1023);  // max 1023
          } else if digitalRead(1) {
            Joystick.button(1, digitalRead(1));
          } else if digitalRead(2) {
            Joystick.button(2, digitalRead(2));
          }
        */

          // read the digital inputs and set the buttons 
          for(int i = 0; i < 18; i++) { // ToDo: Remove Buttons used for pedals / hat switch...
            Joystick.button(i+1, digitalRead(i));
          }
          
          // read the digital inputs and set the buttons 
          for(int i = 21; i <= 57; i++) { // ToDo: Remove Buttons used for pedals / hat switch...
            Joystick.button(i+1, digitalRead(i));
          }
        
          //Hat-switch on right side of the wheel
          //Joystick.hat(digitalRead(i));          // "angle" is 0,45,90,135,180,225,270,315,-1
        
          // a brief delay, so this runs 20 times per second
          delay(50);
        }

        ////////////////////////////////////////////////////////////////////////////////
        // Procedure: read serial data
        ////////////////////////////////////////////////////////////////////////////////
        void readSerialData()
        ////////////////////////////////////////////////////////////////////////////////
        {
          byte Data[5]={'0','0','0','0','0'};

          // parse the buffer
          if (Serial.available()>4){
            Data[0]=Serial.read();                   // read command
            if (Data[0]=='M'){                       // if rpm data
              Data[1]=Serial.read();                 // store high byte
              Data[2]=Serial.read();                 //
              Data[3]=Serial.read();                 //
              Data[4]=Serial.read();                 // store low byte
              rpm = NormalizeData(Data);
              rpmdata=1;                             // we got new data!       
              if (Serial.available()>4){
                Data[0]=Serial.read();               // re-read command
              }
            }
            if (Data[0] == 'S' ){                    // if speed data
              Data[1]=Serial.read();                 // store high byte
              Data[2]=Serial.read();                 //
              Data[3]=Serial.read();                 //
              Data[4]=Serial.read();                 // store low byte
              carspeed = NormalizeData(Data);
              speeddata=1;                           // we got new data!
              if (Serial.available()>4){
                Data[0]=Serial.read();               // re-read command
              }
            }
            if (Data[0] == 'G' ){                    // if gear data
              Data[1]=Serial.read();                 // store high byte
              Data[2]=Serial.read();                 //
              Data[3]=Serial.read();                 //
              Data[4]=Serial.read();                 // store low byte
              gear = NormalizeData(Data);
              geardata=1;                            // we got new data!
              if (Serial.available()>4){
                Data[0]=Serial.read();               // re-read command
              }
            }             
            if (Data[0] == 'O' ){                    // if oil data
              Data[1]=Serial.read();                 // store high byte
              Data[2]=Serial.read();                 //
              Data[3]=Serial.read();                 //
              Data[4]=Serial.read();                 // store low byte
              oil = NormalizeData(Data);
              oildata=1;                             // we got new data!
              if (Serial.available()>4){
                Data[0]=Serial.read();               // re-read command
              }
            }             
            if (Data[0] == 'W' ){                    // if water data
              Data[1]=Serial.read();                 // store high byte
              Data[2]=Serial.read();                 //
              Data[3]=Serial.read();                 //
              Data[4]=Serial.read();                 // store low byte
              water = NormalizeData(Data);
              waterdata=1;                           // we got new data!
              if (Serial.available()>4){
                Data[0]=Serial.read();               // re-read command
              }
            }             
            if (Data[0] == 'F' ){                    // if fuel data
              Data[1]=Serial.read();                 // store high byte
              Data[2]=Serial.read();                 //
              Data[3]=Serial.read();                 //
              Data[4]=Serial.read();                 // store low byte
              fuel = NormalizeData(Data);
              fueldata=1;                            // we got new data!
              if (Serial.available()>4){
                Data[0]=Serial.read();               // re-read command
              }
            }             
            if (Data[0] == 'L' ){                    // if lap data
              Data[1]=Serial.read();                 // store high byte
              Data[2]=Serial.read();                 //
              Data[3]=Serial.read();                 //
              Data[4]=Serial.read();                 // store low byte
              lap = NormalizeData(Data);
              lapdata=1;                             // we got new data!
              if (Serial.available()>4){
                Data[0]=Serial.read();               // re-read command
              }
            }             
            if (Data[0] == 'P' ){                    // if position data
              Data[1]=Serial.read();                 // store high byte
              Data[2]=Serial.read();                 //
              Data[3]=Serial.read();                 //
              Data[4]=Serial.read();                 // store low byte
              posit = NormalizeData(Data);
              positiondata=1;                        // we got new data!
              if (Serial.available()>4){
                Data[0]=Serial.read();               // re-read command
              }
            }             
            if (Data[0] == 'Z' ){                      // if Shutdown - Output
              Serial.flush();
              rpmmax=5000;                             // reset rpmmax
            }             
          }
          if (Serial.available()>60) Serial.flush();
        }

        ////////////////////////////////////////////////////////////////////////////////
        // Function: convert Hex to Dec
        ////////////////////////////////////////////////////////////////////////////////
        unsigned int NormalizeData(byte x[5])
        ////////////////////////////////////////////////////////////////////////////////
        {
        unsigned int result;
  
        for (int i=1; i<5; i++)
        {
          if (x[i]>47 && x[i]<58 ){   //for xA to xF
            x[i]=x[i]-48;
          }                       
          if (  x[i]>64 && x[i]<71 ){ //for x0 to x9
            x[i]=x[i]-55;               
          }
        }    
        result = (x[1]*16*16*16+x[2]*16*16+x[3]*16+x[4]);
        return result;
        }
        
        ////////////////////////////////////////////////////////////////////////////////
        // Outputs the gear of the Car to the oled display
        ////////////////////////////////////////////////////////////////////////////////
        void setDisplayOutput() {
          disp.cls(0x00); //calls clearscreen

          switch(geardata) {
            case 1: break;
            case 2: break;
            case 3: break;
            case 4: break;
            case 5: break;
            case 6: break;
            case 7: break;
            case 8: break;   
          }
          
          //disp.putString(1);  
          sendOne();
          delay(1500);
        }

        void sendOne() 
{
         int index = 0;
         disp.sendCmd(0x00);// -> page mode
         disp.sendCmd(0x00);// lower page
         disp.sendCmd(0x07);
         for (int p=0; p<8;p++){
          disp.sendCmd(0xb0 + p);
          disp.sendCmd(0x00);
          disp.sendCmd(0x10);
          disp.dataStart();
          disp.sendByte(0x78);
          disp.sendByte(0x40);
          
          for (int n=0;n<128;n++)
          {
            disp.sendByte(pgm_read_byte(&(test[index++])));
         
          }
          disp.dataStop();
          
         }
}
