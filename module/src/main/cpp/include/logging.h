#ifndef LOGGING_H
#define LOGGING_H

#include <errno.h>
#include <string.h>
#include "android/log.h"

#ifndef LOG_TAG
#ifdef __LP64__
#define LOG_TAG    "frida_gadget64"
#else
#define LOG_TAG    "frida_gadget"
#endif
#endif

#ifdef DEBUG
#define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#else
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#endif
#define LOGV(...)  __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGW(...)  __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define PLOGE(fmt, args...) LOGE(fmt " failed with %d: %s", ##args, errno, strerror(errno))

#define LOG_THREAD(fmt, ...) \
    do { \
        __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, \
                            "[%s:%d] " fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
    } while (0)

#endif // LOGGING_H
