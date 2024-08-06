#include "dibuligs/utils/serial_utils.hpp"
 
SerialUtils::SerialUtils(itas109::CSerialPort *sp, CircularByteArray *cba): p_sp(sp), p_cba(cba) {}

SerialUtils::~SerialUtils() {}

void SerialUtils::getAvailableSerialPorts(std::vector<itas109::SerialPortInfo> &serialPorts) {
  serialPorts = itas109::CSerialPortInfo::availablePortInfos();
  return;
}

void SerialUtils::onReadEvent(const char *portName, unsigned int readBufferLen) {
  if (readBufferLen > 0) {
      uint8_t *data = new uint8_t[readBufferLen]; // '\0'

      if (data) {
          // read
          int recLen = p_sp->readData(data, readBufferLen);

          if (recLen > 0) {
            if (p_cba->append(data, recLen) != 0) {
              std::cerr << "Failed to append data to cba" << std::endl;
            }
          }

          delete[] data;
          data = NULL;
      }
  }
  return;
}

void SerialUtils::sendMessage(uint8_t *data, uint16_t size) {
  p_sp->writeData(data, size);
}