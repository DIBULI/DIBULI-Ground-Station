#ifndef IVIEW_HPP
#define IVIEW_HPP

#include <string>
#include <memory>

#include "dibuligs/contexts/ui_context.hpp"

class IView {
public:
  IView();
  ~IView();

  virtual void pre_view() = 0;
  virtual void view() = 0;
  virtual void after_first_view() = 0;
  virtual void post_view() = 0;
  virtual std::string get_name() = 0;
};

#endif /* IVIEW_HPP */
