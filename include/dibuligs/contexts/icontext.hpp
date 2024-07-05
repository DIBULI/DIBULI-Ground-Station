#ifndef ICONTEXT_HPP
#define ICONTEXT_HPP

class IContext {
public:
  IContext();
  ~IContext();

  virtual void pre_load() = 0;
  virtual void post_run() = 0;
};

#endif /* ICONTEXT_HPP */
