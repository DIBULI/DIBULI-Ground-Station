#include "dibuligs/utils/serial_utils.hpp"
 
void SerialUtils::getAvailableSerialPorts(std::vector<itas109::SerialPortInfo> &serialPorts) {
  serialPorts = itas109::CSerialPortInfo::availablePortInfos();
  return;
}


