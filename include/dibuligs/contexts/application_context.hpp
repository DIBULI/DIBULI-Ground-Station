#ifndef APPLICATION_CONTEXT_HPP
#define APPLICATION_CONTEXT_HPP

#include <map>
#include <memory>
#include <thread>
#include <atomic>

#include "dibuligs/contexts/ui_context.hpp"

#include "dibuligs/utils/serial_utils.hpp"
#include "dprotocol/circular_byte_array.hpp"

#include "dibuligs/managers/message_manager.hpp"

class ApplicationContext: public IContext{
public:
  ApplicationContext(std::shared_ptr<MessageManager> messageManager);
  ~ApplicationContext();

  void pre_load() override;
  void post_run() override;
  
  std::shared_ptr<UIContext> ui_context;
  std::shared_ptr<MessageManager> messageManager;
};

#endif /* APPLICATION_CONTEXT_HPP */
