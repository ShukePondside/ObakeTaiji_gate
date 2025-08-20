#ifndef STATE_UTILITY_H
#define STATE_UTILITY_H

#include <M5Unified.h>
#include <WiFi.h>

namespace Dx::StateUtility {

// enum Alignment {
//   NONE = 0,
//   LEFT = 1,
//   CENTER = 2,
//   RIGHT = 3
// };

// struct MessageConfig {
//   M5Canvas* canvas = nullptr;
//   String message;
//   uint16_t create_x = 0;
//   uint16_t create_y;
//   uint16_t back_color = TFT_BLACK;
//   uint16_t text_color = TFT_WHITE;
//   const lgfx::v1::IFont* font;
//   float text_size = 1;
//   Alignment text_alignment = NONE;
//   uint16_t cursor_x = 0;
//   uint16_t cursor_y = 0;
//   Alignment push_alignment = NONE;
//   bool transparent;
//   uint16_t push_x;
//   uint16_t push_y;
// };

// struct TableBConfig {
//   M5Canvas* canvas = nullptr;
//   String id_label;
//   String id_value;
//   String message;
//   String result_label;
//   float result_value;
//   String unit;
//   uint16_t push_x;
//   uint16_t push_y;
//   int scale;
// };

// enum ButtonAlignment {
//   A,
//   B,
//   C
// };

// enum ScaleState {
//   kBEGIN,
//   kST,
//   kUS,
//   kERR
// };

/**
 * @brief サンプルメソッド。
 * @param value 入力値。
 * @return 処理結果。
 */
// void DisplayAntenna(SemaphoreHandle_t* mutex_handle_display, wifi_mode_t* wifi_mode, int8_t rssia);

// bool DisplayTitle(SemaphoreHandle_t* mutex_handle_display, String title);
// bool DisplayMessage(SemaphoreHandle_t* mutex_handle_display, MessageConfig msg_cfg);
// bool DisplayButton(SemaphoreHandle_t* mutex_handle_display, ButtonAlignment alignment,
//                    uint16_t background_color = TFT_BLACK, uint16_t text_color = TFT_WHITE,
//                    uint8_t y_position = 0, String caption = "");
// bool DisplayWeight(SemaphoreHandle_t* mutex_handle_display, String weight, ScaleState scale_state);
// bool DisplayTableB(SemaphoreHandle_t* mutex_handle_display, TableBConfig table_cfg);

// bool DisplayTableA(M5Canvas* canvas, SemaphoreHandle_t* mutex_handle_display);

}  // namespace Dx::StateUtility
#endif