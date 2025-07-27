#pragma once
#include <SPI.h>
#include <SD.h>
#include <ArduinoJson.h>


class SDManager{
  public:
    static void writeFile(String fileName, String string);
    static void appendFile(String fileName, String string);
    static String readFile(String fileName);
    static StaticJsonDocument<512> readJsonFile(String fileName);
    static void writeJsonFile(String fileName, StaticJsonDocument<512> jsonData);
};
