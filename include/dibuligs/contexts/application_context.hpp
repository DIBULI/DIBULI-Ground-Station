#ifndef APPLICATION_CONTEXT_HPP
#define APPLICATION_CONTEXT_HPP

#include <map>
#include <memory>
#include <thread>
#include <atomic>

#include "dibuligs/contexts/ui_context.hpp"

class ApplicationContext: public IContext{
public:
  ApplicationContext();
  ~ApplicationContext();

  void pre_load() override;
  void post_run() override;
  
  std::shared_ptr<UIContext> ui_context;
};

#endif /* APPLICATION_CONTEXT_HPP */
