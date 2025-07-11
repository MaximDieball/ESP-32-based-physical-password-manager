#include "SDManager.h"


void SDManager::writeFile(String fileName, String string){
  File file = SD.open(fileName, FILE_WRITE);
  if(!file){
    return;
  }
  file.print(string);
  file.close();
}

void SDManager::appendFile(String fileName, String string){
  File file = SD.open(fileName, FILE_APPEND);
  if(!file){
    return  
  }    
  file.print(string);
  file.close();
}

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
