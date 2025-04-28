#include <jni.h>
#include <sys/types.h>
#include <riru.h>
#include <malloc.h>

#include <fcntl.h>      // 定义 O_RDONLY 等文件操作标志
#include <sys/socket.h>
#include <sys/system_properties.h>
#include <arpa/inet.h>
#include <sys/un.h>

#include <string>
#include <unistd.h>

#include <logging.h>

#include <dlfcn.h>
#include <dirent.h>


#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>

#include <thread>

#include <nlohmann/json.hpp>

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

using json = nlohmann::json;

// 定义全局配置变量
json g_config;
//记录hook对象信息
json g_forkpre;
//gadget 配置
std::string gadget_conf;



// 读取配置文件
bool read_config(const std::string& config_path) {
    std::ifstream config_file(config_path);
    if (!config_file.is_open()) {
        // 使用日志输出替代 std::cerr
        LOG_THREAD("Unable to open configuration file: %s", config_path.c_str());
        return false;
    }

    try {
        config_file >> g_config;  // 将配置加载到全局变量
    } catch (const std::exception& e) {
        // 使用日志输出替代 std::cerr
        LOG_THREAD("Error parsing JSON: %s", e.what());
        return false;
    }

    return true;
}

bool write_config(const std::string& config_path) {
    std::ofstream config_file(config_path);
    if (!config_file.is_open()) {
        std::cerr << "Unable to open configuration file for writing: " << config_path << std::endl;
        return false;
    }

    try {
        config_file << g_config.dump(4);  // 格式化输出 JSON 内容
    } catch (const std::exception& e) {
        std::cerr << "Error writing JSON: " << e.what() << std::endl;
        return false;
    }
    return true;
}

// 判断文件是否存在
bool fileExists(const char* filePath) {
    // 使用access函数判断文件是否存在
    return access(filePath, F_OK) == 0;
}

const char* getEnvVariable(const char* varName) {
    return std::getenv(varName);
}

std::string JstringToString(JNIEnv* env, jstring* jstr) {
    std::string result;

    if (env == nullptr) {
        LOG_THREAD("Invalid JNIEnv");
        return result;
    }

    if (jstr == nullptr || *jstr == nullptr) {
        LOG_THREAD("jstring is null");
        return result;
    }

    // 解引用 jstring* 来获取实际的 jstring 对象
    const char* str = env->GetStringUTFChars(*jstr, nullptr);
    if (str == nullptr) {
        LOG_THREAD("GetStringUTFChars -> Failed.");
        return result;
    }

    // 处理字符串
    result.append(str);

    // 释放 UTF-8 字符串资源
    env->ReleaseStringUTFChars(*jstr, str);

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


std::string dlopen_gadget(){
    std::string restr;
    std::string libraryPath = "";//libfrida-gadget.so
#ifdef __LP64__
    libraryPath=formatString("/system/lib64/libfrida-gadget.so");
#else
    libraryPath=formatString("/system/lib/libfrida-gadget.so");
#endif
    LOG_THREAD("dlopen->%s fileExists=%s",libraryPath.c_str(), fileExists(libraryPath.c_str()) ? "true":"false");
    void* handle = dlopen(libraryPath.c_str(), RTLD_NOW);
    if (handle == nullptr) {
        // 处理加载错误
        const char* error = dlerror();
        restr.append(error);
        // ...
    }else{
        restr.append("dlopen success");
    }
    return restr;
}

// 判断是否应该 hook 该应用
bool should_hook(const std::string& package_name) {
    // 确保 app_list 存在且是一个数组
    if (g_config.contains("app_list") && g_config["app_list"].is_array()) {
        const auto& app_list = g_config["app_list"];
        
        // 遍历 app_list 判断是否包含该包名
        for (const auto& app : app_list) {
            if (app.is_string() && app.get<std::string>() == package_name) {
                return true;  // 找到匹配的包名，表示需要 hook
            }
        }
    }
    return false;  // 如果没有找到匹配的包名，表示不 hook
}

static bool isEnvironmentReady() {
    const char* prop_name = "riru.gadget.signal";
    char prop_value[PROP_VALUE_MAX];
    __system_property_get(prop_name, prop_value);
    if (strlen(prop_value) > 0 &&
    strcmp(prop_value, "null") != 0 &&
    strcmp(prop_value, "none") != 0 &&
    strcmp(prop_value, "0") != 0) {
        LOGD("Property %s detected: %s", prop_name, prop_value);
        // 触发模块逻辑
        return true;
    }else{
        return false;
    }
}

static bool isEnvironmentReady1() {
    const char* signal_file = "/sbin/.magisk/modules/riru_gadget/signal";
    if (access(signal_file, F_OK) == 0) {
        LOGD("Signal file detected, triggering module logic");
        // 触发模块逻辑
        return true;
    }else{
        LOGD("Failed to access signal file: %s", strerror(errno));
        return false;
    }
}
/**
    env	JNIEnv*	JNI 环境指针，用于 JNI 操作
    clazz	jclass	调用此 native 方法的 Java 类
    uid	jint*	新进程的 UID
    gid	jint*	新进程的 GID
    gids	jintArray*	新进程的 GID 列表（groups）
    runtimeFlags	jint*	运行时标志（例如是否为 debuggable）
    rlimits	jobjectArray*	资源限制数组（对应 C 的 rlimit）
    mountExternal	jint*	影响 /storage 挂载方式（如默认、read, write）
    seInfo	jstring*	SELinux 安全上下文信息
    niceName	jstring*	进程的 Nice Name（进程名，最常用于识别）⭐️
    fdsToClose	jintArray*	要在 fork 后关闭的文件描述符
    fdsToIgnore	jintArray*	要在 fork 后忽略的 FD
    is_child_zygote	jboolean*	是否是子 zygote
    instructionSet	jstring*	例如 "arm", "arm64" 等
    appDataDir	jstring*	应用的数据目录 /data/user/0/xxx
    isTopApp	jboolean*	是否为当前顶层 App
    pkgDataInfoList	jobjectArray*	App 数据信息结构（Android 11+）
    whitelistedDataInfoList	jobjectArray*	白名单数据目录（Android 11+）
    bindMountAppDataDirs	jboolean*	是否将 app 数据目录 bind mount
    bindMountAppStorageDirs	jboolean*	是否将 storage 数据目录 bind mount
 */
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
    // 将jstring类型的niceName转换为C++的string类型
    // 获取进程ID
    pid_t pid = getpid();
    std::string nice_name;
    std::string app_data_dir;
    std::string seg_Info;
    std::string arch_Type;
    // 将 jstring 类型的 niceName 转换为 C++ 的 string 类型
    if (niceName != nullptr) {
        nice_name = JstringToString(env, niceName);
    }

    if (appDataDir != nullptr) {
        app_data_dir = JstringToString(env, appDataDir);
    }

    if (appDataDir != nullptr) {
        seg_Info = JstringToString(env, seInfo);
    } 

    if (appDataDir != nullptr) {
        arch_Type = JstringToString(env, instructionSet);
    } 
    // 准备输出参数的字符串
    std::ostringstream log_stream;
    log_stream << "forkAndSpecializePre -> PID: " << pid
            << ", User Identifier: " << *uid
            << ", Group Identifier: " << *gid
            << ", niceName: " << nice_name
            << ", app_dir: " << app_data_dir
            << ", seg_Info: " << seg_Info
            << ", arch_Type: " << arch_Type
            << ", Runtime Flags: " << *runtimeFlags;
    
    // reload cfg
    if(isEnvironmentReady()){
        if (!read_config(gadget_conf)) {
            LOG_THREAD("load failed gadget_conf->%s",gadget_conf.c_str());
        }else{
            LOG_THREAD("load success gadget_conf->%s",gadget_conf.c_str());
        }
    }
    
    LOG_THREAD("%s", log_stream.str().c_str());

    g_forkpre = {
        {"nice_name", nice_name},
        {"app_dir", app_data_dir},
        {"seg_Info", seg_Info},
        {"arch_Type", arch_Type}
    };
    
}



static void forkAndSpecializePost(JNIEnv *env, jclass clazz, jint res) {
    // Called "after" com_android_internal_os_Zygote_nativeForkAndSpecialize in frameworks/base/core/jni/com_android_internal_os_Zygote.cpp
    // "res" is the return value of com_android_internal_os_Zygote_nativeForkAndSpecialize
    if (res == 0) {
        // 直接判断 should_hook 并执行相应操作
        std::string nice_name = g_forkpre.contains("nice_name") ? g_forkpre["nice_name"] : "";
        LOG_THREAD("In process, nice_name->%s", nice_name.c_str());
        if (should_hook(nice_name)) {
            LOG_THREAD("forkAndSpecializePost Hook -> %s", nice_name.c_str());
            LOG_THREAD("Calling dlopen_gadget ->");
            // 调用 dlopen_gadget
            std::string retstr = dlopen_gadget();
            // 输出 dlopen_gadget 返回值
            LOG_THREAD("dlopen_gadget <- ret= %s", retstr.c_str());
            LOG_THREAD("forkAndSpecializePost Hook success -> %s", nice_name.c_str());
        } else {
            // LOG_THREAD("Application %s is not hooked", nice_name.c_str());
        }
    } else {
        // 在 zygote 进程中
        // LOG_THREAD("In zygote process, res=%d", res);
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
    LOG_THREAD("specializeAppProcessPre->");
}

static void specializeAppProcessPost(
        JNIEnv *env, jclass clazz) {
    // Called "after" com_android_internal_os_Zygote_nativeSpecializeAppProcess in frameworks/base/core/jni/com_android_internal_os_Zygote.cpp
    LOG_THREAD("specializeAppProcessPost->");

}

static void forkSystemServerPre(
        JNIEnv *env, jclass clazz, uid_t *uid, gid_t *gid, jintArray *gids, jint *runtimeFlags,
        jobjectArray *rlimits, jlong *permittedCapabilities, jlong *effectiveCapabilities) {
    // Called "before" com_android_internal_os_Zygote_forkSystemServer in frameworks/base/core/jni/com_android_internal_os_Zygote.cpp
    // Parameters are pointers, you can change the value of them if you want
    // Some parameters are not exist is older Android versions, in this case, they are null or 0

}

static void forkSystemServerPost(JNIEnv *env, jclass clazz, jint res) {
    // Called "after" com_android_internal_os_Zygote_forkSystemServer in frameworks/base/core/jni/com_android_internal_os_Zygote.cpp
    LOG_THREAD("forkSystemServerPost-> res=%d",res);
    if (res == 0) {
        // In system server process
    } else {
        // In zygote process
    }
}

static int shouldSkipUid(int uid) {
    // By default (if the module does not provide this function in init), Riru will only call
    // module functions in "normal app processes" (10000 <= uid % 100000 <= 19999)

    // Provide this function so that the module can control if a specific uid should be skipped
    return false;
}

void list_hooks() {
    if (g_config.contains("app_list") && g_config["app_list"].is_array()) {
        const auto& app_list = g_config["app_list"];
        if (app_list.empty()) {
            LOG_THREAD("No applications to hook.\n");
        } else {
            LOG_THREAD("hooks to applications:\n");
            for (const auto& app : app_list) {
                if (app.is_string()) {
                    LOG_THREAD(" - %s\n", app.get<std::string>().c_str());
                } else {
                    LOG_THREAD(" - Invalid entry in app_list\n");
                }
            }
        }
    } else {
        LOG_THREAD("No 'app_list' found in configuration or it's not an array.\n");
    }
}


static void onModuleLoaded() {
    LOG_THREAD("UID: %d", getuid());
    LOG_THREAD("module Loaded..");
    LOG_THREAD("magisk_module_path-> %s", riru_magisk_module_path ? riru_magisk_module_path : "null");
    gadget_conf = riru_magisk_module_path ? std::string(riru_magisk_module_path) + "/gadget.conf" : "gadget.conf";
    LOG_THREAD("gadget_conf->%s",gadget_conf.c_str());
    LOG_THREAD("adb shell su -c setprop riru.gadget.signal 1");
    if (!read_config(gadget_conf)) {
        LOG_THREAD("load failed gadget_conf->%s",gadget_conf.c_str());
        g_config["app_list"] = json::array();
    }else{
        LOG_THREAD("load success gadget_conf->%s",gadget_conf.c_str());
    }
    list_hooks();
}

extern "C" {

int riru_api_version;
RiruApi *riru_api = nullptr;
const char *riru_magisk_module_path = nullptr;

/*
 * 
onModuleLoaded	模块加载时调用	模块初始化阶段，可以注册 hook 框架，打印日志等
shouldSkipUid(uid)	每次 fork 前调用	判断是否对某个进程（uid）注入（返回 true 会跳过）
forkAndSpecializePre(...)	App 进程 fork 之前	你可以修改 fork 参数，比如 UID、环境变量、进程名等
forkAndSpecializePost(...)	App 进程 fork 之后	fork 完后执行，比如注入 Java 层代码或 native 层 hook
forkSystemServerPre(...)	SystemServer fork 之前	修改 SystemServer 参数或决定是否干预
forkSystemServerPost(...)	SystemServer fork 之后	常用于注入对 system_server 的 hook
specializeAppProcessPre(...)	Android 11+ 的新接口，替代 fork	用于新系统中的 App fork 处理
specializeAppProcessPost(...)	和上面配对	fork 完成后执行的回调，等价于旧的 post
*/

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
    auto core_max_api_version = riru->riruApiVersion;
    riru_api_version = core_max_api_version <= RIRU_MODULE_API_VERSION ? core_max_api_version : RIRU_MODULE_API_VERSION;
    module.moduleApiVersion = riru_api_version;
    riru_api = riru->riruApi;
    riru_magisk_module_path = strdup(riru->magiskModulePath);
    LOG_THREAD("%s",riru_magisk_module_path);
    return &module;
}
#else
RiruVersionedModuleInfo *init(Riru *riru) {
    static int step = 0;
    step += 1;
    switch (step) {
        case 1: {
            auto core_max_api_version = riru->riruApiVersion;
            riru_api_version = core_max_api_version <= RIRU_MODULE_API_VERSION ? core_max_api_version : RIRU_MODULE_API_VERSION;
            if (riru_api_version >= 24) {
                module.moduleApiVersion = riru_api_version;
                riru_api = riru->riruApi;
                riru_magisk_module_path = strdup(riru->magiskModulePath);
                LOG_THREAD("init-> %s,riru_api_version->%d",riru_magisk_module_path,riru_api_version);
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