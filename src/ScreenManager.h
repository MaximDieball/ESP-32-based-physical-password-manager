#pragma once
#include <lvgl.h>
#include <functional>
#include "PasswordManager.h"
#include <map>
#include <stack>

// Screen stuct to save a enterFunc for every Screen / only allows one enterFunc for each screen / it is recommended to only use one input field in a screen
struct Screen {
    lv_obj_t* lvScreen = nullptr;
    std::function<void()> enterFunc;
    std::function<void(String direction)> swipeFunc;
    lv_obj_t *mainTextarea = NULL;
    std::function<void()> onDisplayFunc;
    std::map<std::string, lv_obj_t*> updatableObjects;
};

// screens 
enum ScreenIndex {  // TODO RENAME 
  ADD_MPW_SCR = 0,  // add master password screen
  LOCK_SCR = 1,     
  HOME_SCR = 2,     
  PWM_SCR = 3,      // password manager screen
  PASSWORD_SCREEN = 4,
  ADD_PASSWORD_SCREEN_WEBSITE = 5,
  ADD_PASSWORD_SCREEN_USERNAME = 6,
  ADD_PASSWORD_SCREEN_PASSWORD = 7,
  ADD_PASSWORD_SCREEN_CONFIRM = 8,
  EDIT_PASSWORD_SCREEN = 9,
};

class ScreenManager {
  public:
    ScreenManager(PasswordManager& passwordManager)
      : passwordManager(passwordManager) {}

    PasswordManager passwordManager;
    // TODO maybe move to password manager
    Password selectedPassword;
    Password newPassword;
    
    lv_obj_t *focusedTextarea = NULL; // track current text area 
    Screen screenArray[10]; // array storing all screens/pages
    Screen currentScreen;
    std::function<void()> enterFunc;  // pointing to function that is called when pressing enter
    std::function<void(String direction)> swipeFunc;  // pointing to function that is called when user swipes
  
    void createScreens();
    void screenChangeHandler(); // changes screen if any screen is queued
    void queueScreen(int screenIndex);
    void reset();
    void queuePrevScreen();
  
  private:
    int queuedScreen = -1; // index of screen 
    std::stack<int> screenStack;
    void changeScreen(int index);
    static void onTextAreaFocused(lv_event_t *e);
    static void enterBtnFunc(lv_event_t *e);
    
    Screen createAddMasterPasswordScreen();
    Screen createLockScreen();
    Screen createHomeScreen();
    Screen createPasswordManagerScreen();
    Screen createPasswordScreen();
    Screen createAddPasswordScreen_Website();
    Screen createAddPasswordScreen_Username();
    Screen createAddPasswordScreen_Password();
    Screen createAddPasswordScreen_Confirm();
    Screen createEditPasswordScreen();

    // password manager screen
    void displayPasswords(int i);
    lv_obj_t* passwordButtons[5];
    int selectedPasswordIndex = 0;
};
