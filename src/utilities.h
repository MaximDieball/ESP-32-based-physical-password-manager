/**
 * @file      utilities.h
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2023  Shenzhen Xin Yuan Electronic Technology Co., Ltd
 * @date      2023-04-11
 *
 */
#pragma once
#include <Arduino.h>
#include <vector>
#include <ArduinoJson.h>


//! The board peripheral power control pin needs to be set to HIGH when using the peripheral
#define BOARD_POWERON       10

#define BOARD_I2S_WS        5
#define BOARD_I2S_BCK       7
#define BOARD_I2S_DOUT      6

#define BOARD_I2C_SDA       18
#define BOARD_I2C_SCL       8

#define BOARD_BAT_ADC       4

#define BOARD_TOUCH_INT     16
#define BOARD_KEYBOARD_INT  46

#define BOARD_SDCARD_CS     39
#define BOARD_TFT_CS        12
#define RADIO_CS_PIN        9

#define BOARD_TFT_DC        11
#define BOARD_TFT_BACKLIGHT 42

#define BOARD_SPI_MOSI      41
#define BOARD_SPI_MISO      38
#define BOARD_SPI_SCK       40

#define BOARD_TBOX_G02      2
#define BOARD_TBOX_G01      3
#define BOARD_TBOX_G04      1
#define BOARD_TBOX_G03      15

#define BOARD_ES7210_MCLK   48
#define BOARD_ES7210_LRCK   21
#define BOARD_ES7210_SCK    47
#define BOARD_ES7210_DIN    14

#define RADIO_BUSY_PIN      13
#define RADIO_RST_PIN       17
#define RADIO_DIO1_PIN      45

#define BOARD_BOOT_PIN      0

#define BOARD_BL_PIN        42

// screen size
#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 240

// screens 
enum ScreenIndex {
  ADD_MPW_SCR = 0,  // add master password screen
  LOCK_SCR = 1,     
  HOME_SCR = 2,     
  PWM_SCR = 3,      // password manager screen
  USERNAME_SCREEN = 4      
};


class Util{
  public:
    static std::vector<String> split(String input, char breaker);
    static DynamicJsonDocument json(String input);
};
