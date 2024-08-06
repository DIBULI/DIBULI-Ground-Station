#include "dibuligs/contexts/application_context.hpp"

ApplicationContext::ApplicationContext(std::shared_ptr<MessageManager> messageManager)
: messageManager(messageManager)
{}

ApplicationContext::~ApplicationContext() {}

void ApplicationContext::pre_load() {
  messageManager->init();
}

void ApplicationContext::post_run() {}

