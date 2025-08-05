#include "HidManager.h"
#include <Arduino.h>
#include "USBHIDKeyboard.h"

void HidManager::typeString(String string){
    if(language == "GER"){
        typeGER(string);
    } else if(language == "ENG"){
        typeENG(string);
    }
}
void HidManager::typeGER(String string){
    for(size_t i = 0; i < string.length(); i++){
        char c = string.charAt(i);
        switch(c){
            case 'y': Keyboard.print('z'); break;
            case 'Y': Keyboard.print('Z'); break;
            case 'z': Keyboard.print('y'); break;
            case 'Z': Keyboard.print('Y'); break;
            case '#': Keyboard.print('\\'); break;
            case '(': Keyboard.print('*'); break;
            case ')': Keyboard.print('('); break;
            case '_': Keyboard.print('?'); break;
            case '-': Keyboard.print('/'); break;
            case '+': Keyboard.print(']'); break;
            case '*': Keyboard.print('}'); break;
            case '/': Keyboard.print('&'); break;
            case ':': Keyboard.print('>'); break;
            case ';': Keyboard.print('<'); break;
            case '\'': Keyboard.print('|'); break;
            case '"': Keyboard.print('@'); break;
            case '?': Keyboard.print('_'); break;
            case '!': Keyboard.print('!'); break;
            case '@': 
                Keyboard.press(KEY_RIGHT_ALT);
                delay(5);
                Keyboard.press('q');
                delay(5);
                Keyboard.releaseAll();
                break;
            case '\\': 
                Keyboard.press(KEY_RIGHT_ALT);
                delay(5);
                Keyboard.press('-');
                delay(5);
                Keyboard.releaseAll();
            default:
                Keyboard.print(c);
                break;
        }
        delay(20);
    }
}

void HidManager::typeENG(String string){
    Keyboard.print(string);
}

void HidManager::typeTab(){
    Keyboard.press(KEY_TAB);
    delay(20);
    Keyboard.release(KEY_TAB);
    delay(20);
}

void HidManager::typeEnter(){
    Keyboard.press(KEY_RETURN);
    delay(20);
    Keyboard.release(KEY_RETURN);
    delay(20);
}