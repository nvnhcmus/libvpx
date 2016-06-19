#ifndef _LOG_HEADER_H
#define _LOG_HEADER_H

#ifdef __cplusplus
extern "C" {
#endif

	void printLog(const char* logLevel, char * format, ...);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif