#ifndef STATE_INTERFACE_H
#define STATE_INTERFACE_H

#include <M5Unified.h>

#include "modules/display/display.h"
// #include "modules/sound/sound.h"
#include "modules/espnow/espnow.h"

enum EventKind {
  kNone = 1 >> 1,                                   //  0 イベント無し
  kStateTransition = 1 << 0,                        //  1 状態遷移
  kReadSerial = 1 << 1,                             //  2 シリアル読み取り
  kApiRequest = 1 << 2,                             //  4 APIリクエスト
  kTimeOut = 1 << 3,                                //  8 タイムアウト
  kButtonA_Transition = kStateTransition | 1 << 1,  //  3 ボタンAによる状態遷移
  kButtonB_Transition = kStateTransition | 1 << 2,  //  5 ボタンBによる状態遷移
  kButtonC_Transition = kStateTransition | 1 << 3,  //  9 ボタンCによる状態遷移
  kTimeOut_Transition = kStateTransition | 1 << 4,  // 17 タイムアウトによる状態遷移
};

/**
 * @brief 状態に応じてソフトの振舞いを変化させるためのインターフェースクラス
 *
 */
class StateInterface {
 public:
  virtual bool PreChangeState() = 0;
  virtual bool PostChangeState() = 0;
  virtual int WasBtnAPressed() = 0;
  virtual int WasBtnBPressed() = 0;
  virtual int WasBtnCPressed() = 0;
  virtual int SpecialEvent(int event_kind) = 0;
};

#endif