#ifndef IMANAGER_HPP
#define IMANAGER_HPP

class IManager {
  virtual void init() = 0;

  virtual void destroy() = 0;
};

#endif /* IMANAGER_HPP */
