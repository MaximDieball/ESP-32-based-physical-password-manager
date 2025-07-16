#pragma once
#include <lvgl.h>
#include <functional>

// Screen stuct to save a enterFunc for every Screen / only allows one enterFunc for each screen / it is recommended to only use one input field in a screen
struct Screen {
    lv_obj_t* lvScreen = nullptr;
    std::function<void()> enterFunc;
    lv_obj_t *mainTextarea = NULL;
};

class ScreenManager {
  public:
    lv_obj_t *focusedTextarea = NULL; // track current text area 
    Screen screenArray[3]; // array storing all screens/pages
    std::function<void()> enterFunc;// pointing to function that is called when pressing enter
  
    void createScreens();
    void screenChangeHandler(); // changes screen if any screen is queued
    void queueScreen(int screenIndex);
  
  private:
    int queuedScreen = -1; // index of screen 
    void changeScreen(int index);
    static void onTextAreaFocused(lv_event_t *e);
    Screen createAddMasterPasswordScreen();
    Screen createLockScreen();
    Screen createHomeScreen();
    static void enterBtnFunc(lv_event_t *e);
};
