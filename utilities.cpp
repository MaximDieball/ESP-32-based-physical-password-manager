#include "utilities.h"
#include <iostream>
#include <vector>
#include <string>

// split splits a string at every accurence of the breaker char
// split expects a breaker char at the end of the string - else the last chunk will not be read
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
