#ifndef MESSAGE_MANAGER_HPP
#define MESSAGE_MANAGER_HPP

#include <thread>
#include <atomic>

#include "dibuligs/managers/imanager.hpp"

#include "dprotocol/d_protocol_message.hpp"
#include "dprotocol/d_protocol.hpp"
#include "dprotocol/circular_byte_array.hpp"

#include "dibuligs/utils/serial_utils.hpp"

class MessageManager: public IManager{
public:
  MessageManager();
  ~MessageManager();

  void init() override;

  void destroy() override;

  void connect(uint8_t connectionType, int connectionIndex);

  void disconnect();

  void hanldeMessagTask();

  void process_message(DProtocolMessage msg);

  uint8_t *imuReadingRequestMessage;
  void requestIMUData();
  void readIMUData(float *data);

  CircularByteArray *cba;
  DProtocal dprotocol;
  std::thread *handleMessageThread;
  bool handlingMessage = false;
  
  // connection related
  int connectionType = 0;
  bool connectionConnected = false;
  
  // serial connection realted
  std::vector<itas109::SerialPortInfo> serials;
  int connectionIndex = -1;
  itas109::CSerialPort csp;
  SerialUtils *serialUtils;

  // sensor readings
  bool requestIMUReading = false;
  std::atomic<bool> getNewIMUData = false;
  float imuReadings[7];
};

#endif /* MESSAGE_MANAGER_HPP */
