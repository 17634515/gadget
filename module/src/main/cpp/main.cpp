#include <jni.h>
#include <sys/types.h>
#include <riru.h>
#include <malloc.h>
// #include <string.h>
#include <string>
#include <unistd.h>

#include "logging.h"
#include <dlfcn.h>
#include <dirent.h>

#include <fstream>
#include <sstream>
#include <unordered_map>

// #include <android/log.h>

// #define LOG_TAG "frida_gadget" // 这个是自定义的LOG的标识   
// #define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
// #define LOGV(...)  __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
// #define LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
// #define LOGW(...)  __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
// #define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
// #define PLOGE(fmt, args...) LOGE(fmt " failed with %d: %s", ##args, errno, strerror(errno))

// 可变参数格式化函数
std::string formatString(const char* format, ...) {
    va_list args;
    va_start(args, format);

    int size = std::vsnprintf(nullptr, 0, format, args) + 1;
    std::string formattedString(size, '\0');
    std::vsnprintf(&formattedString[0], size, format, args);

    va_end(args);

    return formattedString;
}

std::string removeNewlinesAndTrim(const std::string& input) {
    // 去除换行符
    std::string result = input;
    result.erase(std::remove(result.begin(), result.end(), '\r'), result.end());
    result.erase(std::remove(result.begin(), result.end(), '\n'), result.end());
    // 去除前导空格
    result.erase(result.begin(), std::find_if(result.begin(), result.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));

    // 去除尾部空格
    result.erase(std::find_if(result.rbegin(), result.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), result.end());

    return result;
}
// 可变参数格式化函数
std::unordered_map<std::string, std::string> readConfigFile(const char* filePath) {
    std::unordered_map<std::string, std::string> configMap;
    std::ifstream file(filePath);

    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            // 使用字符串流进行分割
            std::istringstream iss(line);
            std::string key, value;
            if (std::getline(iss, key, '=') && std::getline(iss, value)) {
                // 移除空格
                key.erase(key.find_last_not_of(" \t\r\n") + 1);
                value.erase(0, value.find_first_not_of(" \t\r\n"));

                // 将键值对添加到配置映射中
                configMap[key] = removeNewlinesAndTrim(value);
            }
        }

        file.close();
    }
    return configMap;
}

bool fileExists(const char* filePath) {
    return access(filePath, F_OK) == 0;
}

const char* getEnvVariable(const char* varName) {
    return std::getenv(varName);
}

std::string JstringToString(JNIEnv* env, jstring *jstr) {
    std::string result;
    if(jstr){
        const char* str = env->GetStringUTFChars(*jstr, nullptr);
        result.append(str);
        env->ReleaseStringUTFChars(*jstr, str);
    }
    return result;
}

std::string getConfigValue(const std::unordered_map<std::string, std::string>& configMap, const std::string& key) {
    auto it = configMap.find(key);
    if (it != configMap.end()) {
        return it->second;
    } else {
        return ""; // 返回空字符串表示未找到属性
    }
}

std::string dlopen_gadget(std::string ver){
    std::string restr;
    std::string libraryPath = "";//libfrida-gadget.so
#ifdef __LP64__
    libraryPath=formatString("/system/lib64/libfrida-gadget.so");
#else
    libraryPath=formatString("/system/lib/libfrida-gadget.so");
#endif
    LOG_THREAD("dlopen ->%s",libraryPath.c_str());
    void* handle = dlopen(libraryPath.c_str(), RTLD_NOW);
    if (handle == nullptr) {
        // 处理加载错误
        const char* error = dlerror();
        restr.append(error);
        // ...
    }
    return restr;
}

std::string nice_name;
//gadget 配置
std::string gadget_conf;
std::string gadget_proc;
std::string gadget_ver;

static void forkAndSpecializePre(
        JNIEnv *env, jclass clazz, jint *uid, jint *gid, jintArray *gids, jint *runtimeFlags,
        jobjectArray *rlimits, jint *mountExternal, jstring *seInfo, jstring *niceName,
        jintArray *fdsToClose, jintArray *fdsToIgnore, jboolean *is_child_zygote,
        jstring *instructionSet, jstring *appDataDir, jboolean *isTopApp, jobjectArray *pkgDataInfoList,
        jobjectArray *whitelistedDataInfoList, jboolean *bindMountAppDataDirs, jboolean *bindMountAppStorageDirs) {
    // Called "before" com_android_internal_os_Zygote_nativeForkAndSpecialize in frameworks/base/core/jni/com_android_internal_os_Zygote.cpp
    // Parameters are pointers, you can change the value of them if you want
    // Some parameters are not exist is older Android versions, in this case, they are null or 0
    // LOGD("forkAndSpecializePre niceName=%s",jstringToString(env,*niceName).c_str());
    LOG_THREAD("forkAndSpecializePre");
    // 将jstring类型的niceName转换为C++的string类型
    nice_name=JstringToString(env,niceName);
    LOG_THREAD("nicename=%s",nice_name.c_str());
    LOG_THREAD("seInfo=%s",JstringToString(env,seInfo).c_str());
}


static void forkAndSpecializePost(JNIEnv *env, jclass clazz, jint res) {
    // Called "after" com_android_internal_os_Zygote_nativeForkAndSpecialize in frameworks/base/core/jni/com_android_internal_os_Zygote.cpp
    // "res" is the return value of com_android_internal_os_Zygote_nativeForkAndSpecialize
    if (res == 0) {
        // In app process
        std::unordered_map<std::string, std::string> configMap = readConfigFile(gadget_conf.c_str());
        if (configMap.size() > 0){
            gadget_proc = getConfigValue(configMap, "TARPROC");
            gadget_ver = getConfigValue(configMap, "TARVER");
        }
        LOG_THREAD("gadget_proc->%s",gadget_proc.c_str());
        LOG_THREAD("gadget_ver->%s",gadget_ver.c_str());
        LOG_THREAD("In process name=%s", nice_name.c_str());
        if(!nice_name.empty()) {
            if(strcasecmp(gadget_proc.c_str(),nice_name.c_str()) == 0){
                LOG_THREAD("dlopen_gadget ->");
                std::string retstr = dlopen_gadget(gadget_ver);
                LOG_THREAD("dlopen_gadget <- ret=%s",retstr.c_str());
            }
        }
    } else {
        // In zygote process
    }
}

static void specializeAppProcessPre(
        JNIEnv *env, jclass clazz, jint *uid, jint *gid, jintArray *gids, jint *runtimeFlags,
        jobjectArray *rlimits, jint *mountExternal, jstring *seInfo, jstring *niceName,
        jboolean *startChildZygote, jstring *instructionSet, jstring *appDataDir,
        jboolean *isTopApp, jobjectArray *pkgDataInfoList, jobjectArray *whitelistedDataInfoList,
        jboolean *bindMountAppDataDirs, jboolean *bindMountAppStorageDirs) {
    // Called "before" com_android_internal_os_Zygote_nativeSpecializeAppProcess in frameworks/base/core/jni/com_android_internal_os_Zygote.cpp
    // Parameters are pointers, you can change the value of them if you want
    // Some parameters are not exist is older Android versions, in this case, they are null or 0
    // ScopedUtfChars process(env, *niceName);
    // char processName[1024];
    // sprintf(processName, "%s", process.c_str());
    LOG_THREAD("specializeAppProcessPre");
}

static void specializeAppProcessPost(
        JNIEnv *env, jclass clazz) {
    // Called "after" com_android_internal_os_Zygote_nativeSpecializeAppProcess in frameworks/base/core/jni/com_android_internal_os_Zygote.cpp
    LOG_THREAD("specializeAppProcessPost");

}

static void forkSystemServerPre(
        JNIEnv *env, jclass clazz, uid_t *uid, gid_t *gid, jintArray *gids, jint *runtimeFlags,
        jobjectArray *rlimits, jlong *permittedCapabilities, jlong *effectiveCapabilities) {
    // Called "before" com_android_internal_os_Zygote_forkSystemServer in frameworks/base/core/jni/com_android_internal_os_Zygote.cpp
    // Parameters are pointers, you can change the value of them if you want
    // Some parameters are not exist is older Android versions, in this case, they are null or 0
    LOG_THREAD("forkSystemServerPre");
}

static void forkSystemServerPost(JNIEnv *env, jclass clazz, jint res) {
    // Called "after" com_android_internal_os_Zygote_forkSystemServer in frameworks/base/core/jni/com_android_internal_os_Zygote.cpp
    LOG_THREAD("forkSystemServerPost");
    if (res == 0) {
        // In system server process
    } else {
        // In zygote process
    }
}

static int shouldSkipUid(int uid) {
    LOG_THREAD("shouldSkipUid");
    // By default (if the module does not provide this function in init), Riru will only call
    // module functions in "normal app processes" (10000 <= uid % 100000 <= 19999)

    // Provide this function so that the module can control if a specific uid should be skipped
    return false;
}

static void onModuleLoaded() {
    LOG_THREAD("riru module Loaded..");
    LOG_THREAD("magisk_module_path-> %s", riru_magisk_module_path ? riru_magisk_module_path : "null");
    gadget_conf = riru_magisk_module_path ? std::string(riru_magisk_module_path) + "/gadget.conf" : "gadget.conf";
    LOG_THREAD("gadget_conf->%s",gadget_conf.c_str());
}

extern "C" {

int riru_api_version;
RiruApi *riru_api = nullptr;
const char *riru_magisk_module_path = nullptr;

static auto module = RiruVersionedModuleInfo{
        .moduleApiVersion = RIRU_MODULE_API_VERSION,
        .moduleInfo= RiruModuleInfo{
                .supportHide = true,
                .version = RIRU_MODULE_VERSION,
                .versionName = RIRU_MODULE_VERSION_NAME,
                .onModuleLoaded = onModuleLoaded,
                .shouldSkipUid = shouldSkipUid,
                .forkAndSpecializePre = forkAndSpecializePre,
                .forkAndSpecializePost = forkAndSpecializePost,
                .forkSystemServerPre = forkSystemServerPre,
                .forkSystemServerPost = forkSystemServerPost,
                .specializeAppProcessPre = specializeAppProcessPre,
                .specializeAppProcessPost = specializeAppProcessPost
        }
};

#ifndef RIRU_MODULE_LEGACY_INIT
RiruVersionedModuleInfo *init(Riru *riru) {
    LOG_THREAD("init");
    auto core_max_api_version = riru->riruApiVersion;
    riru_api_version = core_max_api_version <= RIRU_MODULE_API_VERSION ? core_max_api_version : RIRU_MODULE_API_VERSION;
    module.moduleApiVersion = riru_api_version;

    riru_api = riru->riruApi;
    riru_magisk_module_path = strdup(riru->magiskModulePath);
    LOG_THREAD("init-> %s",riru_magisk_module_path);
    return &module;
}
#else
RiruVersionedModuleInfo *init(Riru *riru) {
    static int step = 0;
    step += 1;
    LOG_THREAD("init");
    switch (step) {
        case 1: {
            auto core_max_api_version = riru->riruApiVersion;
            riru_api_version = core_max_api_version <= RIRU_MODULE_API_VERSION ? core_max_api_version : RIRU_MODULE_API_VERSION;
            if (riru_api_version >= 24) {
                module.moduleApiVersion = riru_api_version;
                riru_api = riru->riruApi;
                riru_magisk_module_path = strdup(riru->magiskModulePath);
                LOG_THREAD("magisk_module_path-> %s",riru_magisk_module_path);
                return &module;
            } else {
                return (RiruVersionedModuleInfo *) &riru_api_version;
            }
        }
        case 2: {
            riru_api = (RiruApi *) riru;
            return (RiruVersionedModuleInfo *) &module.moduleInfo;
        }
        case 3:
        default: {
            return nullptr;
        }
    }
}
#endif
}