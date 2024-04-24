#include <iostream>
#include <string>
#include <thread>
#include <fstream>

#include "../src/EntryPoint.h"
#include "example.h"

/**
 * @brief : Entry point of main Vortex runtime command.
*/
int main(int argc, char *argv[])
{
    std::thread mainthread;
    std::thread Thread([&]() { VortexMaker::VortexInterface(argc, argv); });
    mainthread.swap(Thread);

    while(true){}

    return 0;
}