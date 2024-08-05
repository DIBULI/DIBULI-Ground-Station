#ifndef SERIAL_UTILS_HPP
#define SERIAL_UTILS_HPP

#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>

#include "dprotocol/circular_byte_array.hpp"

#include "CSerialPort/SerialPort.h"
#include "CSerialPort/SerialPortInfo.h"

class SerialUtils : public itas109::CSerialPortListener {
public:
  SerialUtils(itas109::CSerialPort *sp, CircularByteArray *cba);
  ~SerialUtils();

  static void getAvailableSerialPorts(std::vector<itas109::SerialPortInfo> &serialPorts);

  void onReadEvent(const char *portName, unsigned int readBufferLen);

  itas109::CSerialPort *p_sp;
  CircularByteArray *p_cba;
};

#endif /* SERIAL_UTILS_HPP */
