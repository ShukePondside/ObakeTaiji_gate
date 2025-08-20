#ifndef S05_H
#define S05_H

#include "../../state_interface/state_interface.h"
#include "../../state_utility/state_utility.h"

// #include <M5_DLight.h>
#include "../../tasks/obake.h"

/**
 * @brief 起動
 *
 */
namespace Dx::State {
class S05 : public StateInterface {
  SemaphoreHandle_t display_mutex;

 public:
  S05(SemaphoreHandle_t display_mutex) {
    this->display_mutex = display_mutex;
  }
  bool PreChangeState() override;
  bool PostChangeState() override;
  int WasBtnAPressed() override;
  int WasBtnBPressed() override;
  int WasBtnCPressed() override;
  int SpecialEvent(int event_kind) override;
};
}  // namespace Dx::State

#endif