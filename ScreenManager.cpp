#include "ScreenManager.h"

void ScreenManager::createScreens() {
    screenArray[0] = createAddPasswordScreen();
    screenArray[1] = createLockScreen();
}

void ScreenManager::changeScreen(int index) {
    lv_scr_load(screenArray[index]);
}

lv_obj_t* ScreenManager::createAddPasswordScreen() {
    lv_obj_t *screen = lv_obj_create(NULL);
    
    // set background color
    lv_obj_set_style_bg_color(screen, lv_color_hex(0xf0f0f0), LV_PART_MAIN);

    // password input field
    lv_obj_t *passwordInput = lv_textarea_create(screen);
    
    focusedTextarea = passwordInput;  // focus text area / for physical keyboard

    lv_obj_add_event_cb(passwordInput, onTextAreaFocused, LV_EVENT_FOCUSED, this);
    
    lv_obj_set_width(passwordInput, 200);
    lv_obj_align(passwordInput, LV_ALIGN_CENTER, 0, -20);

    lv_textarea_set_placeholder_text(passwordInput, "Enter Password");
    lv_textarea_set_one_line(passwordInput, true);
    
    return screen;
}

lv_obj_t* ScreenManager::createLockScreen() {
    return lv_obj_create(NULL);
}

//  gpt code
void ScreenManager::onTextAreaFocused(lv_event_t *e) {
    ScreenManager *self = static_cast<ScreenManager*>(lv_event_get_user_data(e));
    self->focusedTextarea = lv_event_get_target(e);
}
