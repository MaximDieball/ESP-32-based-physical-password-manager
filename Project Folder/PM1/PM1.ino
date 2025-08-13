#include <TFT_eSPI.h>
#include <lvgl.h>
#include <Arduino.h>
#include <iostream>
#include <vector>
#include <string>

#include "TouchDrvGT911.hpp"

#include "src/utilities.h"
#include "src/ScreenManager.h"
#include "src/SDManager.h"
#include "src/PasswordManager.h"
#include "src/InputManager.h"

#include "Fonts/ShareTechMono_Regular.c"
#include "Fonts/ShareTechMono_Regular_28.c"

#include "USB.h"
#include "USBHIDKeyboard.h"
#include "src/HidManager.h"

#define USE_CUSTOM_STYLE 1

TFT_eSPI tft;
TouchDrvGT911 touch;

USBHIDKeyboard Keyboard;
HidManager hidManager(Keyboard);

PasswordManager passwordManager;
ScreenManager scrManager(passwordManager, hidManager);
InputManager inputManager(scrManager, touch);
Util util;

// styles
lv_style_t globalStyle;
lv_style_t titel;
//extern const lv_font_t SHARETECHMONO_REGULAR;
//extern const lv_font_t SHARETECHMONO_REGULAR_28;


static void disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
    uint32_t w = area->x2 - area->x1 + 1;
    uint32_t h = area->y2 - area->y1 + 1;
    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors((uint16_t *)&color_p->full, w * h, false);
    tft.endWrite();
    lv_disp_flush_ready(disp);
}

static void touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data) {
    data->state = LV_INDEV_STATE_REL;
    if (touch.isPressed()) {
        int16_t x, y;
        if (touch.getPoint(&x, &y, 1)) {
            data->state = LV_INDEV_STATE_PR;
            data->point.x = x;
            data->point.y = y;
        }
    }
}

void setBrightness(uint8_t value) {
    static uint8_t level = 0;
    if (value == 0) {
        digitalWrite(BOARD_BL_PIN, 0);
        delay(3);
        level = 0;
        return;
    }
    if (level == 0) {
        digitalWrite(BOARD_BL_PIN, 1);
        level = 16;
        delayMicroseconds(30);
    }
    for (int i = 0; i < (16 + 16 - value - level) % 16; i++) {
        digitalWrite(BOARD_BL_PIN, 0);
        digitalWrite(BOARD_BL_PIN, 1);
    }
    level = value;
}

void setupLvgl() {
    lv_init();
    static lv_color_t *buf = (lv_color_t *)ps_malloc(TFT_WIDTH * TFT_HEIGHT * sizeof(lv_color_t));
    static lv_disp_draw_buf_t draw_buf;
    lv_disp_draw_buf_init(&draw_buf, buf, NULL, TFT_WIDTH * TFT_HEIGHT);

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = TFT_HEIGHT;
    disp_drv.ver_res = TFT_WIDTH;
    disp_drv.flush_cb = disp_flush;
    disp_drv.draw_buf = &draw_buf;
    disp_drv.full_refresh = 1;
    lv_disp_drv_register(&disp_drv);

    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = touchpad_read;
    lv_indev_drv_register(&indev_drv);
}

void touchInputSetup(){
  pinMode(BOARD_TOUCH_INT, INPUT);
  delay(20);
  Wire.begin(BOARD_I2C_SDA, BOARD_I2C_SCL);
  touch.setPins(-1, BOARD_TOUCH_INT);
  touch.begin(Wire, GT911_SLAVE_ADDRESS_L);
  touch.setMaxCoordinates(320, 240);
  touch.setSwapXY(true);
  touch.setMirrorXY(false, true);
}

void hidSetup(){
  USB.begin();
  Keyboard.begin();
}

void setupSD(){
  digitalWrite(BOARD_SDCARD_CS, HIGH);
  digitalWrite(RADIO_CS_PIN, HIGH);
  digitalWrite(BOARD_TFT_CS, HIGH);

  pinMode(BOARD_SPI_MISO, INPUT_PULLUP);
  SPI.begin(BOARD_SPI_SCK, BOARD_SPI_MISO, BOARD_SPI_MOSI); //SD

  if (SD.begin(BOARD_SDCARD_CS, SPI, 800000U)) {
    uint8_t cardType = SD.cardType();
    if (cardType == CARD_NONE) {
      Serial.println("No SD_MMC card attached");
      return;
    }
    Serial.print("SD_MMC Card Type: ");
    if (cardType == CARD_MMC) {
      Serial.println("MMC");
    } else if (cardType == CARD_SD) {
      Serial.println("SDSC");
    } else if (cardType == CARD_SDHC) {
      Serial.println("SDHC");
    } else {
      Serial.println("UNKNOWN");
    }
    uint32_t cardSize = SD.cardSize() / (1024 * 1024);
    uint32_t cardTotal = SD.totalBytes() / (1024 * 1024);
    uint32_t cardUsed = SD.usedBytes() / (1024 * 1024);
    Serial.printf("SD Card Size: %lu MB\n", cardSize);
    Serial.printf("Total space: %lu MB\n",  cardTotal);
    Serial.printf("Used space: %lu MB\n",   cardUsed);
  } else {
    Serial.println("No SD_MMC card attached");
    return;
  }
  
  }

void setup() {

  Serial.begin(115200);
  Serial.println("SETUP START");
    
  pinMode(BOARD_POWERON, OUTPUT);
  digitalWrite(BOARD_POWERON, HIGH);

  pinMode(BOARD_SDCARD_CS, OUTPUT);
  pinMode(RADIO_CS_PIN, OUTPUT);
  pinMode(BOARD_TFT_CS, OUTPUT);


  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  touchInputSetup();
  setupLvgl();
  setupSD();
  hidSetup(); // comment out for debugging

  pinMode(BOARD_BL_PIN, OUTPUT);
  setBrightness(16);

  // global style
  lv_style_init(&globalStyle);
  #if USE_CUSTOM_STYLE
  lv_style_set_text_font(&globalStyle, &SHARETECHMONO_REGULAR);
  lv_style_set_radius(&globalStyle, 0);
  lv_style_set_bg_color(&globalStyle, lv_color_black());
  lv_style_set_border_color(&globalStyle, lv_color_white());
  lv_style_set_border_width(&globalStyle, 2);
  lv_style_set_border_opa(&globalStyle, LV_OPA_COVER);
  #else
  lv_style_set_text_font(&globalStyle, LV_FONT_DEFAULT);
  #endif

  lv_style_init(&titel);
  #if USE_CUSTOM_STYLE
  lv_style_set_text_font(&titel, &SHARETECHMONO_REGULAR_28);
  #else
  lv_style_set_text_font(&titel, &lv_font_montserrat_28);
  #endif

  Serial.println("beginning app startup logic");
  // create screens
  scrManager.createScreens();
  if(passwordManager.checkForMasterPassword()){
    Serial.println("master password found");
    scrManager.queueScreen(LOCK_SCR);
    //scrManager.queueScreen(HOME_SCR);

  } else{
    Serial.println("no master password");
    scrManager.queueScreen(ADD_MPW_SCR);
  }

  Serial.println("SETUP FINISHED");
}

void loop() {
  lv_timer_handler();
  // changing screen if new screen is queued
  scrManager.screenChangeHandler();

  inputManager.keyboardUpdate();
  inputManager.touchUpdate();
  
  delay(5);
}
