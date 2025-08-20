#include "DxEthernet.h"

bool DxEthernet::Initialize() {
  SPI.begin(SCK, MISO, MOSI, -1);
  Ethernet.init(CS);
  Ethernet.begin(MAC_address_, IP_Address_, dns_, gateway_, subnet_mask_);
  return true;
}