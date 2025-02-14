#include "terminal.h"

#include <iostream>

#include <unistd.h>



int main(int argc, char* argv[])
{
    Cedar::Terminal::setMode(Cedar::Terminal::Mode::raw);
    Cedar::Terminal::enableAltScreenBuffer(true);

    Cedar::Terminal::writeLine("Hello world!");
    std::cout << "Making sure standard output works still" << std::endl;

    char c;
    while (read(STDIN_FILENO, &c, 1) == 1 && !(c == 'q' || c == 'Q'));
    
    Cedar::Terminal::setMode(Cedar::Terminal::Mode::cooked);
    
    return 0;
}