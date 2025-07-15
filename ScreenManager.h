#pragma once
#include <lvgl.h>
#include <functional>

// Screen stuct to save a enterFunc for every Screen / only allows one enterFunc for each screen / it is recommended to only use one input field in a screen
struct Screen {
    lv_obj_t* lvScreen = nullptr;
    std::function<void()> enterFunc;
};

class ScreenManager {
  public:
    lv_obj_t *focusedTextarea = NULL; // track current text area 
    Screen screenArray[2]; // array storing all screens/pages
    std::function<void()> enterFunc;// pointing to function that is called when pressing enter
  
    void createScreens();
    void changeScreen(int index);
  
  private:
    static void onTextAreaFocused(lv_event_t *e);
    Screen createAddMasterPasswordScreen();
    Screen createLockScreen();
};
