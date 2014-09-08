#ifndef Dlog_h__
#define Dlog_h__

#include "cocos2d.h"



class Dlog
{
public:
	
	static void _log(const char *format, va_list args);
	static void showLog(const char * format, ...);

private:
	static const int MAX_LOG_LENGTH = 16*1024;
	
};
#endif // Dlog_h__
