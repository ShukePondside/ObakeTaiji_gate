#ifndef S06_H
#define S06_H

#include "../../state_interface/state_interface.h"
#include "../../state_utility/state_utility.h"

#include <KV_HostLink_Ethernet.h>

/**
 * @brief 起動
 *
 */
namespace Dx::State {
class S06 : public StateInterface {
  SemaphoreHandle_t display_mutex;
  KV_HostLink_Ethernet* kv;

 public:
  S06(SemaphoreHandle_t display_mutex,
      KV_HostLink_Ethernet* kv) {
    this->display_mutex = display_mutex;
    this->kv = kv;
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