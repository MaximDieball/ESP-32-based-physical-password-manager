#include "utilities.h"
#include <vector>
#include <string>
#include <ArduinoJson.h>

// split(String input, char breaker) splits a string at every accurence of the breaker char
// split(String input, char breaker) expects a breaker char at the end of the string - else the last chunk will not be read
std::vector<String> Util::split(String input, char breaker){
  std::vector<String> chunks; 
  size_t start = 0;
  for(size_t i = 0; i<input.length(); i++){
    if(input[i] == breaker){
      chunks.push_back(input.substring(start, i));
      start = i+1;
    }
  }
  return chunks;
}

// json(String input) takes a string containing json data and return it as a DynamicJsonDocument 
DynamicJsonDocument Util::json(String input) {
    DynamicJsonDocument jsonOutput(512);
    DeserializationError error = deserializeJson(jsonOutput, input);
    if (error) {
        Serial.print("deserialization failed: ");
        Serial.println(error.c_str());
    }
    return jsonOutput;
}
