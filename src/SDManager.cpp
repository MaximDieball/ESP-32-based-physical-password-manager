#include "SDManager.h"
#include <Arduino.h>
#include <ArduinoJson.h>

// removes all prev data from file and write new String to it
void SDManager::writeFile(String fileName, String string){
  SD.remove(fileName);
  File file = SD.open(fileName, FILE_WRITE);
  if(!file){
    return;
  }
  file.print(string);
  file.close();
}

// appends a file without deleting any old data
void SDManager::appendFile(String fileName, String string){
  File file = SD.open(fileName, FILE_APPEND);
  if(!file){
    return;
  }    
  file.print(string);
  file.close();
}

// reads string from file
String SDManager::readFile(String fileName){
  File file = SD.open(fileName);
  String content = "";
  if(!file){
    return "";
  }
  while (file.available()){
    content += (char)file.read();
  }
  file.close();
  return content;
}

StaticJsonDocument<512> SDManager::readJsonFile(String fileName){
  StaticJsonDocument<512> jsonData;
    File file = SD.open(fileName);
    if(!file){
      Serial.println("failed to read json file");
      return jsonData;
    }
    DeserializationError error = deserializeJson(jsonData, file);
    file.close();

    if(error){
      Serial.println("error deserializing json");  
    }
    return jsonData;
}

// removes all prev data from file and write new Json Data to it
void SDManager::writeJsonFile(String fileName, StaticJsonDocument<512> jsonData){
  SD.remove(fileName);
  File file = SD.open(fileName, FILE_WRITE);
  if(!file){
    Serial.println("failed to write json file");
    return;
  }
  if (serializeJson(jsonData, file) == 0) {
    Serial.println("failed to serialize json");
  }

  file.close();
}
