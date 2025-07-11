#pragma once
#include <SPI.h>
#include <SD.h>

class SDManager{
  public:
    void writeFile(String fileName, String string);
    void appendFile(String fileName, String string);
    String readFile(String fileName);
};
