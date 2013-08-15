#include <iostream>

#include "ff/log.h"

DEF_LOG_MODULE(main)
ENABLE_LOG_MODULE(main)

int main(int argc, char **argv) {
	ff::log<>::init(ff::LogLevel::ERROR, "log.txt");
	LOG_ERROR(main)<<"we get main!";
	
    std::cout << "Hello, world!" << std::endl;
    std::string s;
    return 0;
}
