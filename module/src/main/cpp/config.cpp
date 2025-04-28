#include <iostream>
#include <fstream>
#include <string>

#include <sys/system_properties.h>

#include <nlohmann/json.hpp>
#include <httplib.h>
#include <mustache.hpp>

#include <logging.h>


using json = nlohmann::json;
// 定义全局配置变量
json g_config = nullptr;

bool is_port_available(int port) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) return false;

    sockaddr_in addr {};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY); // 0.0.0.0
    addr.sin_port = htons(port);

    // 尝试绑定
    bool available = bind(sock, (struct sockaddr*)&addr, sizeof(addr)) == 0;
    close(sock);
    return available;
}

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

int main(int argc, char* argv[]) {
    httplib::Server server;

    std::string modulePath;
    std::string configPath;
    int port = 27041;        // 默认端口

    // 简单解析 --module-path 参数
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "--module-path" && i + 1 < argc) {
            modulePath = argv[i + 1];
            break;
        }
    }

    if (!modulePath.empty()) {
        std::cout << "模块路径是: " << modulePath << std::endl;
        configPath = modulePath + "/gadget.conf";
    } else {
        std::cerr << "没有找到模块路径参数！" << std::endl;

        char path[PATH_MAX]; 
        ssize_t len = readlink("/proc/self/exe", path, sizeof(path) - 1);
        if (len == -1) {
            perror("readlink 失败");
            return -1;
        }
        path[len] = '\0'; // 很重要！给字符串手动补个 '\0'
    
        std::string exePath(path);
        // 找最后一个 '/'，把文件名截掉，只保留目录
        size_t lastSlash = exePath.rfind('/');
        if (lastSlash == std::string::npos) {
            std::cerr << "解析路径失败: 没有找到 '/'" << std::endl;
            return -1;
        }
        std::string dirPath = exePath.substr(0, lastSlash);  // 只取目录部分
        configPath = dirPath + "/gadget.conf";
    }

    std::cout << "conf file path: " << configPath << std::endl;
    
    if (!read_config(configPath)) {
        std::cout<<"load failed gadget.conf"<< std::endl;
    }else{
        std::cout<<"load success gadget.conf"<< std::endl;
        if(!g_config.is_null()){
            port = g_config.value("port", 27049);
        }
    }

    server.Get("/",  [configPath](const httplib::Request&, httplib::Response& res) {
        kainjow::mustache::mustache html(R"HTML(
            <!DOCTYPE html>
            <html lang="zh-CN">
            <head>
                <meta charset="UTF-8">
                <title>配置修改页面</title>
                <style>
                    body {
                        font-family: Arial, sans-serif;
                        margin: 0;
                        padding: 20px;
                        background-color: #f4f4f4;
                    }
                    h1 {
                        text-align: center;
                    }
                    .container {
                        max-width: 800px;
                        margin: 0 auto;
                        background-color: white;
                        padding: 20px;
                        border-radius: 8px;
                        box-shadow: 0 2px 10px rgba(0, 0, 0, 0.1);
                    }
                    label {
                    display: block;
                    font-weight: bold;
                    margin-bottom: 8px;
                    color: #333;
                    }
                    textarea {
                        width: 100%;
                        height: 300px;
                        margin-bottom: 20px;
                        font-family: monospace;
                        font-size: 14px;
                        padding: 10px;
                        border: 1px solid #ccc;
                        border-radius: 4px;
                        background-color: #fafafa;
                        box-sizing: border-box;
                    }
                    input[type="text"], input[type="submit"] {
                        padding: 10px;
                        margin-bottom: 20px;
                        width: 100%;
                        border: 1px solid #ccc;
                        border-radius: 4px;
                        font-size: 16px;
                    }
                    input[type="submit"] {
                        background-color: #4CAF50;
                        color: white;
                        cursor: pointer;
                    }
                    input[type="submit"]:hover {
                        background-color: #45a049;
                    }
                    input[type="text"], input[type="submit"] {
                        padding: 10px;
                        margin-bottom: 20px;
                        width: 100%;
                        border: 1px solid #ccc;
                        border-radius: 4px;
                        font-size: 16px;
                        box-sizing: border-box;
                    }
                    button {
                        padding: 10px 20px;
                        background-color: #007bff;
                        color: white;
                        border: none;
                        border-radius: 4px;
                        font-size: 16px;
                        cursor: pointer;
                        transition: background-color 0.3s ease;
                    }
                    .button-container {
                        text-align: center;
                    }
                    button:hover {
                        background-color: #0056b3;
                    }
                    .info-message {
                        font-size: 1.2em;
                        margin-top: 20px;
                        color: #888;
                        text-align: center;
                    }
                </style>
            </head>
            <body>
                <div class="container">
                    <h1>配置文件修改</h1>
                    <p>当前配置文件路径: {{config_path}}</p>
                     <form id="configForm" onsubmit="saveConfig(event)">
                        <label for="json_config">修改配置 (JSON 格式):</label>
                        <textarea id="json_config" name="json_config">{{json_config}}</textarea>
                        <input type="submit" value="save config">
                    </form>
                    <h1>signal</h1>
                    <label for="prop">riru.gadget.signal.prop:</label>
                    <input type="text" id="prop" name="prop">
                    <button type="button" onclick="getProp()">Get Prop</button>
                    <button type="button" onclick="setProp()">Set Prop</button>
                </div>
                <script>
                    // 
                    function saveConfig(event) {
                        event.preventDefault(); //  

                        const jsonConfig = document.getElementById('json_config').value;

                        fetch('/save', {
                            method: 'POST',
                            headers: {
                                'Content-Type': 'application/x-www-form-urlencoded',
                            },
                            body: 'json_config=' + encodeURIComponent(jsonConfig),
                        })
                        .then(response => response.text())
                        .then(result => {
                            alert(result);
                        })
                        .catch(error => {
                            alert('failed to save config!');
                        });
                    }
                        //get prop 
                    function getProp() {
                        fetch('/getprop')
                        .then(response => response.text())
                        .then(result => {
                            document.getElementById('prop').value = result;
                        })
                        .catch(error => {
                            alert('get prop failed!');
                        });
                    }
                         // set prop
                    function setProp() {
                        const propValue = document.getElementById('prop').value;

                        fetch('/setprop', {
                            method: 'POST',
                            headers: {
                                'Content-Type': 'application/x-www-form-urlencoded',
                            },
                            body: 'prop=' + encodeURIComponent(propValue),
                        })
                        .then(response => response.text())
                        .then(result => {
                            alert(result);
                        })
                        .catch(error => {
                            alert('failed to set prop!');
                        });
                    }
                </script>
            </body>
            </html>
            )HTML");
        kainjow::mustache::data data;
        data.set("config_path", configPath);
        if (!read_config(configPath)) {
            data.set("json_config", "read failed gadget.conf");
        }else{
            data.set("json_config", g_config.dump(4));
        }
        res.set_content(html.render(data), "text/html");
    });

        // 处理 POST 请求，保存修改后的配置
    server.Post("/save", [configPath](const httplib::Request& req, httplib::Response& res) {
        // 获取用户提交的 JSON 配置
        std::string jsonConfig = req.get_param_value("json_config");

        res.set_header("Content-Type", "text/plain; charset=utf-8");

        try {
            // 尝试将用户提交的 JSON 解析并保存
            g_config = nlohmann::json::parse(jsonConfig);
            
            // 将解析后的数据保存到文件
            if(write_config(configPath)){
                res.set_content("Success!", "text/html");
            }else{
                res.set_content("Failed!", "text/html");
            }
        } catch (const nlohmann::json::parse_error& e) {
            res.set_content("json parse error!", "text/html");
        }
    });

    server.Get("/getprop", [configPath](const httplib::Request& req, httplib::Response& res) {
        const char* prop_name = "riru.gadget.signal";
        char prop_value[PROP_VALUE_MAX];
        __system_property_get(prop_name, prop_value);
        res.set_content(prop_value, "text/plain");
    });

    server.Post("/setprop", [configPath](const httplib::Request& req, httplib::Response& res) {
        std::string prop = req.get_param_value("prop");
        const char *prop_name = "riru.gadget.signal";
        int result = __system_property_set(prop_name, prop.c_str());
        res.set_content(result == 0 ? "Success!" : "Failed!", "text/plain");
    });

    std::cout<<"Listening on 127.0.0.1 port "<< port << std::endl;
    server.listen("127.0.0.1", port);

    return 0;
}