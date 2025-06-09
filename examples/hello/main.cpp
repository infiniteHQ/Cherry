#include "../../cherry.hpp"

CherryApplication *Cherry::CreateApplication(int argc, char **argv) {
  Cherry::ApplicationSpecification config;
  config.SetMainRenderCallback([]() { CherryKit::TitleOne("Hello World"); });
  return new CherryApplication(config);
}

int main(int argc, char *argv[]) {
  std::thread([&]() { Cherry::Main(argc, argv); }).join();
  return 0;
}