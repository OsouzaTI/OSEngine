#ifndef OSMACROS_H
#define OSMACROS_H
#include <iostream>

#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

//std::string("[")+std::string(__TIME__)+std::string("]::")+

#define LOG(x) std::cout <<"["<< __FILENAME__ <<"] -> " << x << std::endl;
#define LOGERR(x, y) std::cout <<"["<< __FILENAME__ <<"] -> " << x << "::ERROR[ "<< y <<" ]" << std::endl;
#define INIT_LOG std::string("[")+std::string(__FILENAME__)+std::string("] -> ")
#define STRLOG(x)	INIT_LOG + std::string(x)
#define STR(x)	std::string(x)
#define NSTR(x)	std::to_string(x)

#endif // !OSMACROS_H
