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
short Nsensors = 0;



short NmySensors = NMYSENSORS;
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
  unsigned int dt_max;
} SensorDictionaryElement;


SensorW1 SensorObjects[NMYSENSORS]; // initialize with as much as possible.





/*
SensorW1 mySensor0("28-005ccae60","N3 f", 25); // dt_max unsigned int!!!
SensorW1 mySensor1("28-005cd784c","nevera 4 frio", 25); //  APAGADO
SensorW1 mySensor2("28-0062af8f9","C1 f", 25);// Congelador 1 -20 - frio
SensorW1 mySensor3("28-0062c46d3","congelador 2 frio", 25); //
SensorW1 mySensor4("28-0065d469b","congelador 2 caliente", 25); //  APAGADO
SensorW1 mySensor5("28-0065c5b4f","C1 c", 25);// Congelador -20 - caliente
SensorW1 mySensor6("28-005ccce1f","ambiente", 25);// Congelador -20 - caliente
SensorW1 mySensor7("3b-000187513","horno 1", 50); // horno
SensorW1 mySensor8("3b-000187513","horno 1", 50); // horno
*/
/*
 * DECLARE sensors
 * -------------------------------------------------------------------------------------- END
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

  //ds.reset();
  //Serial.println(" This Sensor ID= " + mySensor1.getSensorID());
  delay(500); // not actually needed, just for stability... or whatever. no need to rush.
  // discoverOneWireDevices(); for finding 




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
  //Serial.println();
  //Serial.print(F("In loop with initial run: "));
  //Serial.println(initialRun);
  if (initialRun) {
    serialWriter.sendStatus("\"Initial run\"");
    // this code runs only on the very first run
    /*
     * here we initialize the variables SensorDictionary. I don't do it in setup because I don't know
     * how to declare it outside without knowing the number of elements...
     * so for simplicity, right now, I have everything here.
     */

    SensorDictionaryElement MySensorDictionary[NmySensors]; // All our sensors, doenst matter if they are connected or not.
    /*
     *  this array contains all our sensors and it's locations.
     *
     *  we need another similar dictionary, build dynamically which contains ONLY the elements f
     *  rom this one that are present in the bus.
     *  Their index order does not matter.
     *
     *  Adding a new sensor will be easy. we put it in MySensorsDictionary,
     *  and then it will appear automatically in SensorDictionary if it was found on the bus.
     */
    /*typedef struct {
      //byte sensorAddress[8];
      String sensorLocation;
      unsigned int dt_max;
    } SensorDictionaryElement;*/
    /*
     * NEW SENSOR.
     *    PARSE ID: G3JTRDta9I
     *    NAME: Nevera
     *    MACHINE: Nevera Principal @ olmo
    */
    short iSensor = 0;
    //{byte thisAddress[8] = {0x28, 0x71, 0xAB, 0xB5, 0x05, 0x00, 0x00, 0xD7};
    {//byte thisAddress[8] = {0x28, 0x60, 0xAE, 0xCC, 0x05, 0x00, 0x00, 0x52};
    byte thisAddress[8] = {0x28, 0x11, 0xC0, 0x29, 0x07, 0x00, 0x00, 0xC9};
    for (short ii = 0; ii < 8; ii++) {MySensorDictionary[iSensor].sensorAddress[ii] = thisAddress[ii];}
    MySensorDictionary[iSensor].sensorLocation = (String) "P9Vspkw3Gh";
    MySensorDictionary[iSensor].dt_max = 25;
    }
    /*
     * NEW SENSOR.
     *    PARSE ID: VBbBztL3IR
     *    NAME: Congelador
     *    MACHINE: Nevera Principal @ olmo
    */
    iSensor ++;
    { // putting thisAddress in a "lower scope" so it can be reassigned http://www.cplusplus.com/forum/beginner/82290/#msg441681
    //byte thisAddress[8] = {0x28, 0x1F, 0xCE, 0xCC, 0x05, 0x00, 0x00, 0xFF};
    //byte thisAddress[8] = {0x28, 0x4C, 0x78, 0xCD, 0x05, 0x00, 0x00, 0xD4};
    byte thisAddress[8] = {0x28, 0xDE, 0xCB, 0x29, 0x07, 0x00, 0x00, 0x02};
    for (short ii = 0; ii < 8; ii++) {MySensorDictionary[iSensor].sensorAddress[ii] = thisAddress[ii];}
    //MySensorDictionary[iSensor].sensorLocation = (String) "fMft50HAD0";
    MySensorDictionary[iSensor].sensorLocation = (String) "NA3Ci9LnsD";
    MySensorDictionary[iSensor].dt_max = 25;
    }
    /*
     * NEW SENSOR.
     *    PARSE ID: 7q0kBqY2aT
     *    NAME: Caliente
     *    MACHINE: Nevera Principal @ olmo
    */
    iSensor ++; // 28 D3 46 2C 6 0 0 45
    { // putting thisAddress in a "lower scope" so it can be reassigned http://www.cplusplus.com/forum/beginner/82290/#msg441681
    //byte thisAddress[8] = {0x28, 0xD3, 0x46, 0x2C, 0x06, 0x00, 0x00, 0x45};
    byte thisAddress[8] = {0x28, 0xC1, 0xC2, 0x29, 0x07, 0x00, 0x00, 0x8E};
    for (short ii = 0; ii < 8; ii++) {MySensorDictionary[iSensor].sensorAddress[ii] = thisAddress[ii];}
    MySensorDictionary[iSensor].sensorLocation = (String) "afSzNSM0kQ";
    MySensorDictionary[iSensor].dt_max = 25;
    }
    /*
     * NEW SENSOR.
     *    PARSE ID: zwiDCzlKUA
     *    NAME: Ambiente
     *    MACHINE: Congelador 7 
    */
    iSensor ++; // 28 4F 5B 5C 6 0 0 B1
    { // putting thisAddress in a "lower scope" so it can be reassigned http://www.cplusplus.com/forum/beginner/82290/#msg441681
    //byte thisAddress[8] = {0x28, 0xD3, 0x46, 0x2C, 0x06, 0x00, 0x00, 0x45};
    byte thisAddress[8] = {0x28, 0xC9, 0x3D, 0xDD, 0x06, 0x00, 0x00, 0x32};
    for (short ii = 0; ii < 8; ii++) {MySensorDictionary[iSensor].sensorAddress[ii] = thisAddress[ii];}
    MySensorDictionary[iSensor].sensorLocation = (String) "AjwYuCDaLr";
    MySensorDictionary[iSensor].dt_max = 25;
    }
        /*
     * NEW SENSOR.
     *    PARSE ID: NMnCopzz5I
     *    NAME: Caliente
     *    MACHINE: Congelador 8 
    */
    iSensor ++; //28 AA 47 DD 6 0 0 78

    { // putting thisAddress in a "lower scope" so it can be reassigned http://www.cplusplus.com/forum/beginner/82290/#msg441681
    //byte thisAddress[8] = {0x28, 0xD3, 0x46, 0x2C, 0x06, 0x00, 0x00, 0x45};
    byte thisAddress[8] = {0x28, 0x06, 0x1F, 0x29, 0x07, 0x00, 0x00, 0x26};
    for (short ii = 0; ii < 8; ii++) {MySensorDictionary[iSensor].sensorAddress[ii] = thisAddress[ii];}
    MySensorDictionary[iSensor].sensorLocation = (String) "HW23Srup8q";
    MySensorDictionary[iSensor].dt_max = 25;
    }

    iSensor ++; //28 AA 47 DD 6 0 0 78

    { // putting thisAddress in a "lower scope" so it can be reassigned http://www.cplusplus.com/forum/beginner/82290/#msg441681
    //byte thisAddress[8] = {0x28, 0xD3, 0x46, 0x2C, 0x06, 0x00, 0x00, 0x45};
    byte thisAddress[8] = {0x28, 0xED, 0xC8, 0x29, 0x07, 0x00, 0x00, 0xF8};
    for (short ii = 0; ii < 8; ii++) {MySensorDictionary[iSensor].sensorAddress[ii] = thisAddress[ii];}
    MySensorDictionary[iSensor].sensorLocation = (String) "sAEOCSFNer";
    MySensorDictionary[iSensor].dt_max = 25;
    }

    iSensor ++; //28 AA 47 DD 6 0 0 78

    { // putting thisAddress in a "lower scope" so it can be reassigned http://www.cplusplus.com/forum/beginner/82290/#msg441681
    //byte thisAddress[8] = {0x28, 0xD3, 0x46, 0x2C, 0x06, 0x00, 0x00, 0x45};
    byte thisAddress[8] = {0x28, 0x13, 0xA6, 0x29, 0x07, 0x00, 0x00, 0x2A};
    for (short ii = 0; ii < 8; ii++) {MySensorDictionary[iSensor].sensorAddress[ii] = thisAddress[ii];}
    MySensorDictionary[iSensor].sensorLocation = (String) "I7bqd64Wu5";
    MySensorDictionary[iSensor].dt_max = 25;
    }

    iSensor ++; //28 AA 47 DD 6 0 0 78

    { // putting thisAddress in a "lower scope" so it can be reassigned http://www.cplusplus.com/forum/beginner/82290/#msg441681
    //byte thisAddress[8] = {0x28, 0xD3, 0x46, 0x2C, 0x06, 0x00, 0x00, 0x45};
    byte thisAddress[8] = {0x28, 0xD3, 0xEC, 0xDD, 0x06, 0x00, 0x00, 0x35};
    for (short ii = 0; ii < 8; ii++) {MySensorDictionary[iSensor].sensorAddress[ii] = thisAddress[ii];}
    MySensorDictionary[iSensor].sensorLocation = (String) "ZDMOFZUCNN";
    MySensorDictionary[iSensor].dt_max = 25;
    }
    
    

    Nsensors = findNumberOfSensorsInBus(ds);
    // now we know how many sensors are there.
    // now we need to populate the array listOfSensors;
    
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
          SensorDictionary[iSensor].dt_max = MySensorDictionary[iSensorDict].dt_max;
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
      SensorObjects[iSensor].SensorW1ini(SensorDictionary[iSensor].sensorAddress, SensorDictionary[iSensor].sensorLocation,SensorDictionary[iSensor].dt_max );
      delay(500);
      SensorObjects[iSensor].checkAllValues(); // 2DO: Why doesnt this here print anything interesting?
    } // end looping over the results.


    initialRun = false;
  } else {
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
    ds.write(0x44,1); //Start Temperature conversion (for all sensors at the same time after Skip ROM 
    delay(750); //Delay for Temperature conversion at 12-bit resolution
    //ds.depower(); //Release the one wire bus
    //////////////////////////////////////////////////////////////////////
    for (short iSensor = 0; iSensor < Nsensors; iSensor++) {
      //Serial.println(iSensor);
      SensorObjects[iSensor].updateTemperature(ds);
      SensorObjects[iSensor].checkAllValues();
    }
    //t = millis() - ti;
    //Serial.println("Tiempo: ");
    //Serial.println(t);
    
    if (DO_DEBUG_START_STOP_LOOP) {
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
* https://github.com/adafruit/MAX31850_OneWire/tree/master/examples/MAX31850_Temperature
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
