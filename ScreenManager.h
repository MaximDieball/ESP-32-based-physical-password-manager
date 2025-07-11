#pragma once
#include <lvgl.h>

class ScreenManager {
  public:
    lv_obj_t *focusedTextarea = NULL; // track current text area 
    lv_obj_t* screenArray[2]; // array storing all screens/pages
  
    void createScreens();
    void changeScreen(int index);
  
  private:
    static void onTextAreaFocused(lv_event_t *e);
    lv_obj_t* createAddPasswordScreen();
    lv_obj_t* createLockScreen();
};
