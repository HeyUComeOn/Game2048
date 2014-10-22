#include "Dlog.h"

USING_NS_CC;

static bool isOpen = true;


void Dlog::_log(const char *format, va_list args)
{
	char buf[MAX_LOG_LENGTH];

	vsnprintf(buf, MAX_LOG_LENGTH-3, format, args);
	strcat(buf, "\n");

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	__android_log_print(ANDROID_LOG_DEBUG, "cocos2d-x debug info",  "%s", buf);

#elif CC_TARGET_PLATFORM ==  CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT || CC_TARGET_PLATFORM == CC_PLATFORM_WP8
	WCHAR wszBuf[MAX_LOG_LENGTH] = {0};
	MultiByteToWideChar(CP_UTF8, 0, buf, -1, wszBuf, sizeof(wszBuf));
	OutputDebugStringW(wszBuf);
	WideCharToMultiByte(CP_ACP, 0, wszBuf, -1, buf, sizeof(buf), nullptr, FALSE);
	printf("%s", buf);
	fflush(stdout);
#else
	// Linux, Mac, iOS, etc
	fprintf(stdout, "cocos2d: %s", buf);
	fflush(stdout);
#endif

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WINRT)
	Director::getInstance()->getConsole()->log(buf);
#endif

}

void Dlog::showLog(const char * format, ...)
{
	if(!isOpen)
		return;
	va_list args;
	va_start(args, format);
	_log(format, args);
	va_end(args);
}