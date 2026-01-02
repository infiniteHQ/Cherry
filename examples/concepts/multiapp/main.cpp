#include "../../../cherry.hpp"

void RenderA() { CherryKit::TextSimple("Hello A"); }

void RenderB() { CherryKit::TextSimple("Hello B"); }

CherryApplication *CreateAppA(int argc, char **argv) {
  Cherry::ApplicationSpecification config;
  config.SetMainRenderCallback(RenderA);
  return new CherryApplication(config);
}

CherryApplication *CreateAppB(int argc, char **argv) {
  Cherry::ApplicationSpecification config;
  config.SetMainRenderCallback(RenderB);
  return new CherryApplication(config);
}

int main(int argc, char *argv[]) {
  if (argc > 1) {
    if (std::string(argv[1]) == "-a") {
      Cherry::ThirdMain(argc, argv, CreateAppA);
    } else if (std::string(argv[1]) == "-b") {
      Cherry::ThirdMain(argc, argv, CreateAppB);
    }

  } else {
    printf("Please provide \'-a\' or \'-b\' after calling hello \n");
  }
  return 0;
}