#include "../ScreenManager.h"
#include "../PasswordManager.h"
#include "../SDManager.h"
#include <Arduino.h>
#include "../utilities.h"

Screen ScreenManager::createHomeScreen(){

    lv_obj_t *lvScreen = lv_obj_create(NULL);

    // set background color
    lv_obj_set_style_bg_color(lvScreen, lv_color_hex(0x000000), LV_PART_MAIN);
    
    int btnWidth = SCREEN_WIDTH/2-20;
    int btnHeight = SCREEN_HEIGHT/2-20;

    // lock device button
    lv_obj_t *lockDeviceBtn = lv_btn_create(lvScreen);
    lv_obj_set_size(lockDeviceBtn, btnWidth, btnHeight);
    lv_obj_align(lockDeviceBtn, LV_ALIGN_TOP_LEFT, 10, 10);
    lv_obj_t *lockDeviceLable = lv_label_create(lockDeviceBtn);
    lv_label_set_text(lockDeviceLable, "Lock Device");
    lv_obj_center(lockDeviceLable);
    // settings button
    lv_obj_t *settingBtn = lv_btn_create(lvScreen);
    lv_obj_set_size(settingBtn, btnWidth, btnHeight);
    lv_obj_align(settingBtn, LV_ALIGN_TOP_RIGHT, 10, 10);
    lv_obj_t *settingLable = lv_label_create(settingBtn);
    lv_label_set_text(settingLable, "Settings");
    lv_obj_center(settingLable);
    // 2fa button
    lv_obj_t *tfaBtn = lv_btn_create(lvScreen);
    lv_obj_set_size(tfaBtn, btnWidth, btnHeight);
    lv_obj_align(tfaBtn, LV_ALIGN_BOTTOM_LEFT, 10, 10);
    lv_obj_t *tfaLabel = lv_label_create(tfaBtn);
    lv_label_set_text(tfaLabel, "2FA");
    lv_obj_center(tfaLabel);
    // password manager button
    lv_obj_t *passwordManagerBtn = lv_btn_create(lvScreen);
    lv_obj_set_size(passwordManagerBtn, btnWidth, btnHeight);
    lv_obj_align(passwordManagerBtn, LV_ALIGN_BOTTOM_RIGHT, 10, 10);
    lv_obj_t *passwordManagerLabel = lv_label_create(passwordManagerBtn);
    lv_label_set_text(passwordManagerLabel, "Password Manager");
    lv_obj_center(passwordManagerLabel);

    // screen stuct
    Screen screen;
    screen.lvScreen = lvScreen;
    screen.mainTextarea = NULL;
    
    // enter function / called when pressing enter
    screen.enterFunc = []() {
      Serial.println("test");
    };

    return screen;
}