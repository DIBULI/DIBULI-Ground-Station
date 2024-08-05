#include "dibuligs/contexts/application_context.hpp"

ApplicationContext::ApplicationContext() {}

ApplicationContext::~ApplicationContext() {}

void ApplicationContext::pre_load() {
  // list all the available serial ports
  SerialUtils::getAvailableSerialPorts(serials);
}

void ApplicationContext::post_run() {}
