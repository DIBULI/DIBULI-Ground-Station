#ifndef APPLICATION_CONTEXT_HPP
#define APPLICATION_CONTEXT_HPP

#include <map>
#include <memory>
#include <thread>
#include <atomic>

#include "dibuligs/contexts/ui_context.hpp"

#include "dibuligs/utils/serial_utils.hpp"
#include "dprotocol/circular_byte_array.hpp"

class ApplicationContext: public IContext{
public:
  ApplicationContext();
  ~ApplicationContext();

  void pre_load() override;
  void post_run() override;
  
  std::shared_ptr<UIContext> ui_context;

  // connection related
  CircularByteArray* cba;
  int connectionType = 0;
  bool connectionConnected = false;
  
  
  // serial connection realted
  std::vector<itas109::SerialPortInfo> serials;
  int connectionIndex = -1;
  itas109::CSerialPort csp;
  SerialUtils *serialUtils;

  void connect(uint8_t connectionType, int connectionIndex);

  void disconnect();
};

#endif /* APPLICATION_CONTEXT_HPP */
