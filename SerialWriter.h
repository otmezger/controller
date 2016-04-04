/*
  SerialWriter.h - Library for
*/
#ifndef SerialWriter_h
#define SerialWriter_h

unsigned int ledPinOK = LEDPIN_OK; // a LED that lights green if everything is OK

String cID = CONTROLLER_ID;

#include "Arduino.h"

class SerialWriter
{
  public:
    SerialWriter();
    void sendStatus(String myString);
    void sendError(String myString);
    void sendData(String sensorID, int value);

};

SerialWriter::SerialWriter(){}
void SerialWriter::sendStatus(String myString){


  /*
     How to use:
     sendSerialStatusMsg(F("string"));

     will produce this output (but in 1 line):
     {
       "type":"status",
       "data":"string"
     }

     You can also embedd an object into the "string", will produce
     input : {\"message\":\"Going to initialize sensor\", \"sensorID\": \"" + parseID + "\", \"dT_max\": \"" + dT_max + "\"}}

     output (in one line):
     {
       "type":"status",
       "data":{
         "message": "my Message",
         "sensorID": "parseID",
         "dT_max": 123
       }
     }

  */


  Serial.print("{\"type\":\"status\",\"cID\":\"" + cID +  "\",\"data\":");
  Serial.print(myString);
  Serial.println(F("}"));
}

void SerialWriter::sendError(String myString){
  Serial.print("{\"type\":\"error\",\"cID\":\"" + cID +  "\",\"data\":\"");
  Serial.print(myString);
  Serial.println(F("\"}"));
}

void SerialWriter::sendData(String sensorID, int value){
  digitalWrite(ledPinOK, HIGH);
  Serial.println("{\"type\":\"dataPoint\",\"cID\":\"" + cID +  "\", \"data\":{\"sensorID\": \"" + sensorID + "\", \"value\": " + value + "}}");
  digitalWrite(ledPinOK, LOW);
}



#endif
