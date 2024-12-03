#pragma once


#include<iostream>
#include<string>

#ifdef HZ_DEBUG
	#define ASSERT(x) if (!(x)) __debugbreak();
	#define GLCall(x) GLClearError(); x; ASSERT(GLLogCall(#x, __FILE__, __LINE__));
#elif
	#define GLCall(x) x;
#endif


namespace Hazel {
	void GLClearError();
	std::string Enum2String(unsigned int ErrorEnum);
	bool GLLogCall(const char* function, const char* file, int line);
}