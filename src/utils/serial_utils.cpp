#include "dibuligs/utils/serial_utils.hpp"
 
SerialUtils::SerialUtils(itas109::CSerialPort *sp, CircularByteArray *cba): p_sp(sp), p_cba(cba) {}

SerialUtils::~SerialUtils() {}

void SerialUtils::getAvailableSerialPorts(std::vector<itas109::SerialPortInfo> &serialPorts) {
  serialPorts = itas109::CSerialPortInfo::availablePortInfos();
  return;
}

void SerialUtils::onReadEvent(const char *portName, unsigned int readBufferLen) {
  if (readBufferLen > 0) {
      char *data = new char[readBufferLen + 1]; // '\0'

      if (data) {
          // read
          int recLen = p_sp->readData(data, readBufferLen);

          if (recLen > 0) {
              data[recLen] = '\0';
              printf("%s - Length: %d, Str: %s\n", portName, recLen, data);
          }

          delete[] data;
          data = NULL;
      }
  }
  return;
}