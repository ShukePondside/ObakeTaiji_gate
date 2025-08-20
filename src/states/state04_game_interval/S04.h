#ifndef S04_H
#define S04_H

#include "../../state_interface/state_interface.h"
#include "../../state_utility/state_utility.h"

// using Image = std::vector<uint8_t>;
// using Images = std::vector<Image>;

/**
 * @brief 起動
 *
 */
namespace Dx::State {
class S04 : public StateInterface {
  SemaphoreHandle_t display_mutex;

 public:
  S04(SemaphoreHandle_t display_mutex) {
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