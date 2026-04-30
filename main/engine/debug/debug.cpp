#include "debug.hpp"
#include "./ui/devtool.hpp"

#include <iostream>
#ifdef CHERRY_DEBUG
namespace Cherry {

static Tools::Devtool devtool;

void Debugger::StartDevTools() { devtool.Start(); }
void Debugger::StopDevTools() { devtool.Stop(); }

} // namespace Cherry
#endif