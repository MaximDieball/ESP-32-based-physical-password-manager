#pragma once
#include "utilies.h"
#include <iostream>
#include <vector>
#include <string>

// split splits a string at every accurence of the breaker char
// split expects a breaker char at the end of the string - else the last chunk will not be read
std::vector<std::string> Util::split(String input, char breaker){
  std::vector<std::string> chunks; 
  size_t start = 0;
  for(size_t i = 0; i<string.size(); i++){
    if(str[i] == breaker){
      chunks.push_back(input.substr(start, i-start))
      start = i+1
    }
  }
  return chunks
}
