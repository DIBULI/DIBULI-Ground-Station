#include "dibuligs/managers/message_manager.hpp"

MessageManager::MessageManager() {
  imuReadingRequestMessage = new uint8_t[8];
  *((uint16_t *) imuReadingRequestMessage) = MESSAGE_START_MAGIC_NUMBER;
  *((uint8_t *) imuReadingRequestMessage + 2) = 1;
  *((uint8_t *) imuReadingRequestMessage + 3) = 1;
  *((uint16_t *) (imuReadingRequestMessage + 4)) = 0;
  *((uint16_t *) (imuReadingRequestMessage + 6)) = 2;
}

MessageManager::~MessageManager() {}

void MessageManager::init() {
  cba = new CircularByteArray(4096*2);
  serialUtils = new SerialUtils(&csp, cba);
  // list all the available serial ports
  SerialUtils::getAvailableSerialPorts(serials);

  // start handling message
  handlingMessage = true;
  handleMessageThread = new std::thread(&MessageManager::hanldeMessagTask, this);
}

void MessageManager::destroy() {
  requestIMUReading = false;
  handlingMessage = false;
}

void MessageManager::connect(uint8_t connectionType, int connectionIndex) {
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

void MessageManager::disconnect() {
  connectionConnected = false;
  std::cout << "Disconnect to port: " << serials[connectionIndex].portName << std::endl;
  return;
}

void MessageManager::hanldeMessagTask() {
  DProtocolMessage msg;
  while(handlingMessage) {
    uint8_t ret = this->dprotocol.retrieveMessage(cba, &msg);

    if (ret == 0) {
      this->process_message(msg);
      msg.clean();
    } else if (ret == 10) {
      cba->remove(1);
      dprotocol.cleanCBA(cba);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
  
  std::cout << "Exiting handling message taks." << std::endl;
}

void MessageManager::process_message(DProtocolMessage msg) {
  if (msg.type == 2) { // imu message
    getNewIMUData = true;
    memcpy(imuReadings, msg.body, sizeof(float) * 7);
  }
}

void MessageManager::requestIMUData() {
  serialUtils->sendMessage(imuReadingRequestMessage, 8);
}

void MessageManager::readIMUData(float *data) {
  if (getNewIMUData) {
    memcpy(data, imuReadings, sizeof(float) * 7);
    getNewIMUData = false;
  }
}
