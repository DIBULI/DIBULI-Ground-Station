#include <memory>
#include "dibuligs/views/main_view.hpp"
#include "dibuligs/contexts/ui_context.hpp"
#include "dibuligs/contexts/application_context.hpp"

int main(int argv, char** argc) {
  std::shared_ptr<ApplicationContext> appctx = std::make_shared<ApplicationContext>();
  std::shared_ptr<UIContext> uictx = std::make_shared<UIContext>();

  std::unique_ptr<MainView> main_view = std::make_unique<MainView>(appctx, uictx);

  appctx->pre_load();
  uictx->pre_load();
  
  main_view->pre_view();

  main_view->view();

  main_view->post_view();

  appctx->post_run();
  uictx->post_run();

  return 0;
}