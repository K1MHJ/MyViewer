// pch.h: 미리 컴파일된 헤더 파일입니다.
// 아래 나열된 파일은 한 번만 컴파일되었으며, 향후 빌드에 대한 빌드 성능을 향상합니다.
// 코드 컴파일 및 여러 코드 검색 기능을 포함하여 IntelliSense 성능에도 영향을 미칩니다.
// 그러나 여기에 나열된 파일은 빌드 간 업데이트되는 경우 모두 다시 컴파일됩니다.
// 여기에 자주 업데이트할 파일을 추가하지 마세요. 그러면 성능이 저하됩니다.

#ifndef PCH_H
#define PCH_H

// 여기에 미리 컴파일하려는 헤더 추가
#include "framework.h"
#include<gdiplus.h>
#pragma comment(lib, "gdiplus")
using namespace Gdiplus;

#include <string>
#include <vector>
#include <memory>
#include <chrono>
#include <ctime>   
extern std::chrono::system_clock::time_point AppStartTime;


template <class ...Args>
std::string Format(const std::string& fmt, Args ...args)
{
    std::size_t len = snprintf(nullptr, 0, fmt.c_str(), args ...);
    std::vector<char> buf(len + 1);
    snprintf(buf.data(), len + 1, fmt.c_str(), args ...);
    return std::string(buf.data());
}
template <class ...Args>
void DebugPrint(const std::string& fmt, Args ...args)
{
    std::size_t len = snprintf(nullptr, 0, fmt.c_str(), args ...);
    std::vector<char> buf(len + 1);
    snprintf(buf.data(), len + 1, fmt.c_str(), args ...);
    OutputDebugStringA(buf.data());
}
double PassTime();

#define DEBUGM() DebugPrint("%s[%d][%f sec]\n", __FILE__, __LINE__,PassTime())
#define DEBUGN( n ) DebugPrint("%s[%d][%f sec]" #n " = %d\n" , __FILE__, __LINE__,PassTime(), n)
#define DEBUGF( f ) DebugPrint("%s[%d][%f sec]" #f " = %f\n" , __FILE__, __LINE__,PassTime(), f)
#define DEBUGS( s ) DebugPrint("%s[%d][%f sec]""%s\n" , __FILE__, __LINE__,PassTime(), s)

#endif //PCH_H
