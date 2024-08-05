#include "dibuligs/contexts/application_context.hpp"

ApplicationContext::ApplicationContext() {}

ApplicationContext::~ApplicationContext() {}

void ApplicationContext::pre_load() {
  cba = new CircularByteArray(4096*2);
  serialUtils = new SerialUtils(&csp, cba);
  // list all the available serial ports
  SerialUtils::getAvailableSerialPorts(serials);
}

void ApplicationContext::post_run() {}


void ApplicationContext::connect(uint8_t connectionType, int connectionIndex) {
  if (connectionType == 0) { // serial port
    // according to the connection index get the serial port info
    itas109::SerialPortInfo spi = serials[connectionIndex];
    
    

    csp.init(
      serials[connectionIndex].portName,
      itas109::BaudRate115200,
      itas109::ParityNone,   // parity
      itas109::DataBits8,    // data bit
      itas109::StopOne,      // stop bit
      itas109::FlowNone,     // flow
      4096                   // read buffer size
    );
    csp.setReadIntervalTimeout(0);               // read interval timeout 0ms
    if (csp.open()) {
      connectionConnected = true;
      this->connectionIndex = connectionIndex;
      std::cout << "Connect to port: " << serials[connectionIndex].portName << std::endl;
    } else {
      connectionConnected = false;
      std::cout << "Fail to connect to port: " << serials[connectionIndex].portName << std::endl;
    }

    csp.connectReadEvent(serialUtils);
  }
  
  return;
}

void ApplicationContext::disconnect() {
  connectionConnected = false;
  std::cout << "Disconnect to port: " << serials[connectionIndex].portName << std::endl;
  return;
}