#ifndef SERIAL_UTILS_HPP
#define SERIAL_UTILS_HPP

#include <iostream>
#include <string>
#include <vector>

#include "CSerialPort/SerialPort.h"
#include "CSerialPort/SerialPortInfo.h"

class SerialUtils {
public:
  SerialUtils();
  ~SerialUtils();

  static void getAvailableSerialPorts(std::vector<itas109::SerialPortInfo> &serialPorts);
};

#endif /* SERIAL_UTILS_HPP */
