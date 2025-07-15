#pragma once
#include <SPI.h>
#include <SD.h>

class SDManager{
  public:
    static void writeFile(String fileName, String string);
    static void appendFile(String fileName, String string);
    static String readFile(String fileName);
};
