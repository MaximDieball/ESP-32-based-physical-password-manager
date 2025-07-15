#include "SDManager.h"
#include <Arduino.h>

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
