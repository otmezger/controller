// Energy Monitor by Olmo Mezger
// this program pushes the sensor data to a php api.

#include <SPI.h> /// no idea what this is... I think it's mandatory.
//#include "/Users/olmo/Documents/Arduino/libraries/MAX31850_OneWire.h"
//#include "OneWire.h" // i'd rather use this to include it in the folder
#include <OneWire.h>  
#include <avr/pgmspace.h>
#include "config.h"
#include "SensorW1.h"

boolean initialRun = true;
short Nsensors =8;



short NmySensors = 8;
//int listOfSensors; // how to initialize as a vector without knowing iut's size?

OneWire ds(5);  // on pin 10 (a 4.7K resistor is necessary)
unsigned int ledPin = 13;
boolean ledPinStatus = true; // a LED that blinks.

//unsigned int ledPinOK = LEDPIN_OK; // a LED that lights green if everything is OK
unsigned int ledPinKO = 7; // a red LED that shows problems.



/*
 * -------------------------------------------------------------------------------------- START
 * DECLARE sensors
 *
 * Current problem:
 * this variables take to much memory.
 * I cant allocate all of them,otherwise the program will run crazy, possible a memory overflow or something.
 * 6 sensors works OK. 7 wont.
 */
//SensorW1 mySensor0("28-005ccae60","WRfMbVcDBL", 0.25);
//SensorW1 mySensor1("28-004bde0ce","wsuH8FMVwO", 0.25);
//SensorW1 mySensor2("28-003adb7af","jsKTNJ2h38", 0.25);

typedef struct {
  byte sensorAddress[8];
  String sensorLocation;
  float dv_max;
} SensorDictionaryElement;

SensorW1 SensorObjects[9]; // initialize with  the number of sensors in the array "MySensorDictionary"
/*
Sensorobject is the funtional array, it contains all the atributes and funtions for one-wire sensors and analog sensors
 */

/*
 * -------------------------------------------------------------------------------------- START
 * SETUP
 */
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); // Just for output so we can see if its working
  //Serial.println("Starting");
  pinMode(ledPin, OUTPUT);
  pinMode(ledPinKO, OUTPUT);
  pinMode(ledPinOK, OUTPUT);
  digitalWrite(ledPinOK, LOW);
  digitalWrite(ledPinKO, LOW);
  digitalWrite(ledPin, ledPinStatus);

        //digitalWrite(ledPinOK, HIGH);
      //digitalWrite(ledPinKO, HIGH);

  
  delay(500); // not actually needed, just for stability... or whatever. no need to rush.
  
}


/*
 * SETUP
 * -------------------------------------------------------------------------------------- END
 */


/*
* -------------------------------------------------------------------------------------- START
* MAIN LOOP
*/
void loop() { // put your main code here, to run repeatedly:
  // loop the LED
  ledPinStatus = !ledPinStatus;
  digitalWrite(ledPin, ledPinStatus);
  
  if (initialRun) {
    serialWriter.sendStatus("\"Initial run\"");
    // this code runs only on the very first run
    
    /*
   Fist we create each element, with id address , id parse and DV
   Here is where we are going to wrinte new elements (to setup the arduino)
    */
SensorDictionaryElement mySensor0={{0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01}, "xxxpoxxxxx",  25};
SensorDictionaryElement mySensor1={{0x28, 0x11, 0xC0, 0x29, 0x07, 0x00, 0x00, 0xC9}, "P9Vspkw3Gh", 25};
SensorDictionaryElement mySensor2={{0x28, 0xDE, 0xCB, 0x29, 0x07, 0x00, 0x00, 0x02}, "NA3Ci9LnsD", 25};
SensorDictionaryElement mySensor3={{0x28, 0xC1, 0xC2, 0x29, 0x07, 0x00, 0x00, 0x8E}, "afSzNSM0kQ", 25};
SensorDictionaryElement mySensor4={{0x28, 0xC9, 0x3D, 0xDD, 0x06, 0x00, 0x00, 0x32}, "AjwYuCDaLr", 25};
SensorDictionaryElement mySensor5={{0x28, 0x06, 0x1F, 0x29, 0x07, 0x00, 0x00, 0x26}, "HW23Srup8q", 25};
SensorDictionaryElement mySensor6={{0x28, 0xED, 0xC8, 0x29, 0x07, 0x00, 0x00, 0xF8}, "sAEOCSFNer", 25};
SensorDictionaryElement mySensor7={{0x28, 0x13, 0xA6, 0x29, 0x07, 0x00, 0x00, 0x2A}, "I7bqd64Wu5", 25};
SensorDictionaryElement mySensor8={{0x28, 0xD3, 0xEC, 0xDD, 0x06, 0x00, 0x00, 0x35}, "ZDMOFZUCNN", 25};


  /*
     Now create the array MySensorDictionary than includes each sensor previous
    */
SensorDictionaryElement MySensorDictionary[]={mySensor0,mySensor1,mySensor2,mySensor3,mySensor4,mySensor5,mySensor6,mySensor7,mySensor8};


 /*
    we will set the array SensorObjects(funtional array) with the sensor in MySensorDictionary(configurational array)
    */
    for (short iSensor = 0; iSensor <= NmySensors; iSensor++) {
     
    SensorObjects[iSensor].SensorW1ini(MySensorDictionary[iSensor].sensorAddress, MySensorDictionary[iSensor].sensorLocation,MySensorDictionary[iSensor].dv_max );
    
    SensorObjects[iSensor].checkAllValues();
    }

    

    Nsensors = findNumberOfSensorsInBus(ds);
     Nsensors =  Nsensors +1;
    // now we know how many sensors are in the one-wire.
    // the +1, is the number of analog sensors, it must be seted when add a new analog sensor ;
    
    serialWriter.sendStatus("{\"message\":\"Found Sensors\", \"amount\": " + String(Nsensors) + "}");
    /*Serial.print(F("-- in loop: We got "));
    Serial.print(Nsensors);
    Serial.println(F(" sensors"));
*/

    SensorDictionaryElement SensorDictionary[Nsensors]; // here we can initialize this variable knowing it's size.

    /*Serial.print(F("-- in loop: initialized SensorDictionary with "));
    Serial.print(sizeof(SensorDictionary));
    Serial.println(F(" bytes"));
*/
    //byte thisAddress[8] = {0x28, 0x71, 0xAB, 0xB5, 0x05, 0x00, 0x00, 0xD7};

    byte listOfSensors[Nsensors][8];
    list1WireSensors(listOfSensors, ds);
    //Serial.println(F("... returned ok"));

    /*Serial.print(F("---> Ok We got "));
    Serial.print(Nsensors);
    Serial.println(F(" sensors"));*/
    //serialWriter.sendStatus("{\"message\":\"Found Sensors\", \"amount\": " + Nsensors + "}}");
    
    for (short iSensor = 0; iSensor < Nsensors; iSensor++) {
     
      if (DO_DEBUG_MESSAGES){
        Serial.print(F("--- ---> Sensor Nr. "));
        Serial.print(iSensor);
        Serial.print(F(" with Address: "));
        for (short iAddr = 0; iAddr < 8; iAddr++) {
          Serial.print(listOfSensors[iSensor][iAddr], HEX);
          Serial.print(F(" "));
        }
        Serial.println();
      }
      // ok, now we need to update SensorDictionary with the elements that we have here.

      //SensorDictionaryElement thisSensorDictionaryElement;
      //SensorDictionary[iSensor] = thisSensorDictionaryElement;
      for (short iAddr = 0; iAddr < 8; iAddr++) {
        SensorDictionary[iSensor].sensorAddress[iAddr] = listOfSensors[iSensor][iAddr];
      }
      // now let's match against our library of sensors.
      boolean foundMatch= false;
      for (short iSensorDict = 0; iSensorDict < NmySensors; iSensorDict ++) {
        // loop over MySensorDictionary and see which matches.
        short NmatchAddres = 0;
        // now loop over addr and se if it maches.
        for (short iAddr = 0; iAddr < 8; iAddr++) {
          if (MySensorDictionary[iSensorDict].sensorAddress[iAddr] == SensorDictionary[iSensor].sensorAddress[iAddr]){
            NmatchAddres  ++;
          }
        }
        // now if the sensor iSensor had a match with the library iSensorDict, then NmatchAddres  should be 8!
        if (NmatchAddres  == 8){
          //serialWriter.sendStatus("{\"message\":\"Got a match with Dic Element\", \"sensorID\": \"" + MySensorDictionary[iSensorDict].sensorLocation + "\", \"sensor\"}}");
          //Serial.print(F("--- ---- ---> Got a match with Dictionary element Nr. "));
          //Serial.println(iSensorDict);
          SensorDictionary[iSensor].dv_max = MySensorDictionary[iSensorDict].dv_max;
          SensorDictionary[iSensor].sensorLocation = MySensorDictionary[iSensorDict].sensorLocation;
          foundMatch = true;
        }else{
          // nothing wrong, continue.
        }


      }// end looping over dictionary.
      if (!foundMatch){
        Serial.print(F("--- --- ---> Did not found anything for Sensor"));
        digitalWrite(ledPinKO, HIGH);
        break;// stop runtime
      }
      // at this point, if the arduino continues, is because we matched all sensors with the library. this is good :-)
      // now let's build the objects.
      //SensorObjects[iSensor].SensorW1ini(SensorDictionary[iSensor].sensorAddress, SensorDictionary[iSensor].sensorLocation,SensorDictionary[iSensor].dv_max );
      
      delay(500);
      //SensorObjects[iSensor].checkAllValues(); // 2DO: Why doesnt this here print anything interesting?
    } // end looping over the results.
    
    

    initialRun = false;
    
  } 
    else {
    
    //Serial.println(F("--> NOT Initial run"));
    // this code runs only from the 2 run.
    if (DO_DEBUG_START_STOP_LOOP) {
      Serial.println(F("------------- START"));
    }
    //digitalWrite(ledPinKO, HIGH);
    //ti = millis();
    //////////////////////////////////////////////////////////////////////
    ds.reset();
    ds.write(0xCC); //Skip ROM to broadcast to all sensors in bus.
    //delay(100);
    ds.write(0x44,1); //Start value conversion (for all sensors at the same time after Skip ROM 
    delay(750); //Delay for value conversion at 12-bit resolution
    //ds.depower(); //Release the one wire bus
    //////////////////////////////////////////////////////////////////////
    for (short iSensor = 0; iSensor < Nsensors; iSensor++) {
      
     // Serial.println(Nsensors);    // Chekear cantidad de sensores que se van a revisar
      SensorObjects[iSensor].updateValue(ds);
      SensorObjects[iSensor].checkAllValues();
    }
    //t = millis() - ti;
    //Serial.println("Tiempo: ");
    //Serial.println(t);
    
    if (!DO_DEBUG_START_STOP_LOOP) {
      Serial.println(F("------------- END"));
    }
    delay(10);
  }


}
/*
 * MAIN LOOP
 * -------------------------------------------------------------------------------------- END
 */


















/*
* -------------------------------------------------------------------------------------- START
* DECLARE read1WireBus function and it's return structure
*
* This function has the goal of reading through the 1 Wire bus and returning an array with
* all values for all sensors found in the array. The code has been taken from
* https://github.com/adafruit/MAX31850_OneWire/tree/master/examples/MAX31850_Vemperature
*/

byte findNumberOfSensorsInBus(OneWire ds) {
  byte Nsensors_ = 0;
  byte addr[8];
  boolean doLoop1WireBus = true;
  while (doLoop1WireBus ) {
    if ( !ds.search(addr)) {
      if (DO_DEBUG_MESSAGES ) {
        //Serial.println(F("No more addresses."));
        doLoop1WireBus = false;
        //Serial.println();
      }
      ds.reset_search();
      delay(250);
      break; // breaks the while?
    } else {
      // not the last elemnt.. this is a valid one.
      if (OneWire::crc8(addr, 7) != addr[7]) {
        break; // breaks the while?
      }
    } // end if last element.
    Nsensors_ ++;
  }
  //Serial.println("-- Inside findNumberOfSensorsInBus. Got " + String(Nsensors_) + " sensors");
  return Nsensors_;
}


void list1WireSensors(byte listOfSensors[][8], OneWire ds) {
  //Serial.println("In list1WireSensors:  --- START");
  // this function searches for sensors in the 1 Wire network and returns them as an array (of addressess)
  byte iWhile = 0;
  byte iAddr = 0;
  byte addr[8];

  boolean doLoop1WireBus = true;
  while (doLoop1WireBus ) {
    if (DO_DEBUG_MESSAGES){
      Serial.println("--- ---- looping for sensor " + String(iWhile));
    }
    if ( !ds.search(addr)) {
      doLoop1WireBus = false;
      if (DO_DEBUG_MESSAGES ) {
        Serial.println(F("No more addresses."));
        
        Serial.println();
      }
      ds.reset_search();
      delay(250);
      //return read1WireReturns;
      break; // breaks the while?
    } else {
      // not the last elemnt.. this is a valid one.
      if (DO_DEBUG_MESSAGES ) {
        Serial.print(F("Found a sensor with address ="));
        for ( iAddr = 0; iAddr < 8; iAddr++) {
          Serial.write(' ');
          Serial.print(addr[iAddr], HEX);
        }
        Serial.println();
      }


      if (OneWire::crc8(addr, 7) != addr[7]) {
        Serial.println("CRC is not valid!");
        //return read1WireReturns;
        break; // breaks the while?
      }


    } // end if last element.
    if (DO_DEBUG_MESSAGES){
      Serial.print(" ---> Address: ");
    }
      for ( iAddr = 0; iAddr < 8; iAddr ++) {
        listOfSensors[iWhile][iAddr] = addr[iAddr];
        if (DO_DEBUG_MESSAGES){
          Serial.print(listOfSensors[iWhile][iAddr], HEX);
          Serial.print(" ");
        }
      }
      if (DO_DEBUG_MESSAGES){
        
        Serial.println();
      }
    
    //listOfSensors[i] = addr;
    iWhile++;


  } // end looping over sensors
  //Serial.print(("returning list1WireSensors"));
  delay(1000);
  return;
}

/*
 * DECLARE read1WireBus function and it's return structure
 * -------------------------------------------------------------------------------------- END
 */
