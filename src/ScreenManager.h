#pragma once
#include <lvgl.h>
#include <functional>
#include "PasswordManager.h"
#include <map>

// Screen stuct to save a enterFunc for every Screen / only allows one enterFunc for each screen / it is recommended to only use one input field in a screen
struct Screen {
    lv_obj_t* lvScreen = nullptr;
    std::function<void()> enterFunc;
    std::function<void(String direction)> swipeFunc;
    lv_obj_t *mainTextarea = NULL;
    std::function<void()> onDisplayFunc;
    std::map<std::string, lv_obj_t*> updatableObjects;
};

class ScreenManager {
  public:
    ScreenManager(PasswordManager& passwordManager)
      : passwordManager(passwordManager) {}
      
    Password selectedPassword;
    lv_obj_t *focusedTextarea = NULL; // track current text area 
    Screen screenArray[5]; // array storing all screens/pages
    Screen currentScreen;
    std::function<void()> enterFunc;  // pointing to function that is called when pressing enter
    std::function<void(String direction)> swipeFunc;  // pointing to function that is called when user swipes
  
    void createScreens();
    void screenChangeHandler(); // changes screen if any screen is queued
    void queueScreen(int screenIndex);
  
  private:
    PasswordManager passwordManager;

    int queuedScreen = -1; // index of screen 
    void changeScreen(int index);
    static void onTextAreaFocused(lv_event_t *e);
    static void enterBtnFunc(lv_event_t *e);
    
    Screen createAddMasterPasswordScreen();
    Screen createLockScreen();
    Screen createHomeScreen();
    Screen createPasswordManagerScreen();
    Screen createPasswordScreen();

    // password manager screen
    void displayPasswords(int i);
    lv_obj_t* passwordButtons[5];
    int selectedPasswordIndex = 0;
};
