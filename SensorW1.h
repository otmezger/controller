/*
  Morse.h - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/
#ifndef SensorW1_h
#define SensorW1_h

#include "Arduino.h"
#include "SerialWriter.h"
//#include "OneWire.h" // the one wire library for the sensors.
#include <OneWire.h>  

SerialWriter serialWriter;

/*
 * -------------------------------------------------------------------------------------- START
 * DEFINE CLASS SensorW1 - each sensor will be an object form this calss
 */

class SensorW1
{
  private:
    signed int  _T; // temperature in C/100
    signed int _T_old;
    //int _timeSinceOld;
    //long description
    //int _sensorCheck;// check every 3 runs
    signed int _dT;
    signed int _dT_max; // max T in sensorCheck
    byte _sensorID[8];
    String _parseID;
    //bool _networkMode; // this could be done better... but so far is ok.
    //bool _sdMode;
    bool _OneWireReadOK; //
    byte _temptype; // the temperature sensor type.
    boolean _active;
  public:
    SensorW1();
    void SensorW1ini(byte sensorID[8], String parseID, signed int dT_max);
    String getSensorIDString();
    void updateTemperature(OneWire ds);
    signed int getTFromBus(OneWire ds);
    String getSensorType();
    void deactivate();
    void activate();
    String getSensorPrettyName();
    void checkAllValues();
};

SensorW1::SensorW1(){
  _active = false;
}
void SensorW1::SensorW1ini(byte sensorID[8], String parseID, signed int dT_max)
{
  //for (short iAddr = 0; iAddr < 8; iAddr ++){Serial.print(sensorID[iAddr],HEX); Serial.print(" ");}
  //String prettyAdress = "";
  for (short iAddr = 0; iAddr<8; iAddr++){
    _sensorID[iAddr] = sensorID[iAddr];
    //prettyAdress = prettyAdress + _sensorID[iAddr];
  }
  _active = true;
  _parseID = parseID;
  _dT_max = dT_max;
  _OneWireReadOK = true; // initialized with some value.
  serialWriter.sendStatus("{\"message\":\"Going to initialize sensor\", \"sensorID\": \"" + parseID + "\", \"dT_max\": " + dT_max + ",\"addr\":\"" + getSensorIDString() + "\" }");
  getSensorType();
  return;
}
void SensorW1::checkAllValues(){
  if (DO_DEBUG_MESSAGES ) {
    Serial.print(F("Sensor with ID "));
    for (short iAddr = 0; iAddr < 8; iAddr ++){Serial.print(_sensorID[iAddr],HEX); Serial.print(" ");}
    Serial.print(F(" ("));
    Serial.print(getSensorIDString());
    Serial.print(F(") ,parseID= "));
    Serial.print(_parseID);
    Serial.print(F(" and current temperature: "));
    Serial.println((float)_T_old/100);
  }

}
void SensorW1::deactivate(){
  // dont know what for we have this
  _active = false;
}
void SensorW1::activate(){
  // dont know what for we have this
  _active = true;
}
String SensorW1::getSensorType() {
  String SensorType;
  switch (_sensorID[0]) { // we could put this in the constructor of the object, but let's sacrify performace in favor of memory.
    case 0x10:
      SensorType = "DS18S20";
      _temptype = TYPE_DS18S20;
      break;
    case 0x28:
      SensorType = "DS18B20";
      _temptype = TYPE_DS18B20;
      break;
    case 0x22:
      SensorType = "DS1822";
      _temptype = TYPE_DS18S22;
      break;
    // ADDED SUPPORT FOR MAX31850!
    case 0x3B:
      SensorType = "MAX31850";
      _temptype = TYPE_MAX31850;
      break;
    default:
      //Serial.println(F(""));
      serialWriter.sendError(F("Device is not a DS18x20 family device."));
      break;
  }
  return SensorType;
}

String SensorW1::getSensorIDString() {
  String sensorIDString = String(_sensorID[0], HEX) + "-" + String(_sensorID[6], HEX) + String(_sensorID[5], HEX) + String(_sensorID[4], HEX) + String(_sensorID[3], HEX) + String(_sensorID[2], HEX) + String(_sensorID[1], HEX) ;
  return sensorIDString;
}
String SensorW1::getSensorPrettyName(){
  String sensorPrettyName = _parseID + " (" + getSensorIDString() + ")";
  return sensorPrettyName;
}
signed int SensorW1::getTFromBus(OneWire ds) {
  _OneWireReadOK = false; // initialized with false.

  // this function asks the 1Wire bus for the value.
  float celsius;
  byte present = 0;
  int i;
  byte data[12];

  ds.reset();
  ds.select(_sensorID);
  ds.write(0x44, 1);        // start conversion, with parasite power on at the end

  delay(1000);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.

  present = ds.reset();
  ds.select(_sensorID);
  ds.write(0xBE);         // Read Scratchpad

  if (DO_DEBUG_MESSAGES ) {
    Serial.print(F("  Data = "));
    Serial.print(present, HEX);
    Serial.print(F(" "));
  }

  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
    if (DO_DEBUG_MESSAGES ) {
      Serial.print(data[i], HEX);
      Serial.print(F(" "));
    }

  }
  if (DO_DEBUG_MESSAGES ) {
    Serial.print(F(" CRC="));
    Serial.print(OneWire::crc8(data, 8), HEX);
    Serial.println();
  }


  // Convert the data to actual temperature
  // because the result is a 16 bit signed integer, it should
  // be stored to an "int16_t" type, which is always 16 bits
  // even when compiled on a 32 bit processor.
  int16_t raw = (data[1] << 8) | data[0];
  if (_temptype == TYPE_DS18S20) {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10) {
      // "count remain" gives full 12 bit resolution
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else if (_temptype == TYPE_MAX31850) {
    //Serial.println(raw, HEX);
    if (raw & 0x01) { // specially for MAX31850;
      String stringOne = "{\"message\":\"Fault. cant be true.\", \"raw\": " + raw;
      serialWriter.sendError(stringOne + "\"_temptype\": " + _temptype + "}");
      return -999; //return now without setting _OneWireReadOK true.
    }
  } else {
    byte cfg = (data[4] & 0x60);
    // at lower res, the low bits are undefined, so let's zero them
    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
    //// default is 12 bit resolution, 750 ms conversion time
  }
  celsius = (float)raw / 16.0;
  _OneWireReadOK = true;
  return (signed int) (celsius * 100);
}

void SensorW1::updateTemperature(OneWire ds) {
  // this function reads the temperature from the 1Wire bus and updates _T if the change was enough.
  if (_active){
    signed int _T = getTFromBus(ds); // in C/100;

    signed int original_T_old = _T_old;
    //Serial.println("--- In update Temperature after process. Old Temp = " + String(_T_old) + ", Actual temp = " + String(_T));
    bool updateSensor = false;
    _dT = abs(_T - _T_old);
    if (_dT >= _dT_max) {
      // we need to update the sensor because it changed enough since last time.
      // 1. update _T_old!
      _T_old = _T;
      // 2. change updateSensor flag
      updateSensor = true;
    }
    if (updateSensor) {
      if (DO_DEBUG_MESSAGES ) {
        Serial.print(F("Sensor with ID: "));
        //Serial.print(getSensorIDString());
        Serial.print(getSensorPrettyName());
        Serial.print(F(" changed enough from "));
        Serial.println(String((float)original_T_old/100) + " to " + String((float)_T/100));
      }

      serialWriter.sendData( _parseID,_T);




      /*if (_sdMode){      // start of SD mode

      } // end of SD mode
      */
    } //end if update sensor
  } // end if active
}

/*
 * DEFINE CLASS SensorW1 - each sensor will be an object form this calss
 * -------------------------------------------------------------------------------------- END
 */

#endif
