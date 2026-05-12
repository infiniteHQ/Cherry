#include "../../../cherry.hpp"

void Render() {
  auto md_file_path = CherryPath("markdown.md");
  std::string md_content = "";

  std::ifstream t(md_file_path);
  if (t.is_open()) {
    std::stringstream buffer;
    buffer << t.rdbuf();
    md_content = buffer.str();
  }

  CherryGUI::MarkdownView(md_content);
}

CherryApplication CherryMain(int argc, char **argv) {
  Cherry::ApplicationSpecification config;
  config.SetMainRenderCallback(Render);
  return new CherryApplication(config);
}

int main(int argc, char *argv[]) {
  CherryRun(argc, argv);
  return 0;
}