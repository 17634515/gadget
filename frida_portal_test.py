import frida
import sys
import argparse

def on_message(message, data):
    """处理脚本发送的消息"""
    print(f"[*] Message from script: {message}")
    if data:
        print(f"[*] Data: {data}")

def test_portal(host, port):
    """测试 Frida Portal 连接并列出进程"""
    try:
        # 构造 Portal 的控制接口地址
        print(f"[*] Frida version:{frida.__version__}")
        portal_address = f"{host}:{port}"
        print(f"[*] Connecting to Frida Portal at {portal_address}...")

        # 获取远程设备
        device = frida.get_device_manager().add_remote_device(portal_address)
        print(f"[*] Successfully connected to Portal!")

        # 列出所有注册的进程
        print(f"[*] Enumerating processes...")
        processes = device.enumerate_processes()
        
        if not processes:
            print(f"[!] No processes found. Ensure Gadgets are connected to the Portal.")
        else:
            print(f"[*] Found {len(processes)} processes:")
            for process in processes:
                print(f"    - PID: {process.pid}, Name: {process.name}")

        # 示例：附加到一个进程并加载简单脚本（可选）
        # 取消注释以下代码以测试附加功能
        """
        if processes:
            target_process = processes[0]  # 选择第一个进程
            print(f"[*] Attaching to process: {target_process.name} (PID: {target_process.pid})")
            session = device.attach(target_process.pid)
            script_content = '''
            console.log("[*] Script loaded in process!");
            '''
            script = session.create_script(script_content)
            script.on('message', on_message)
            script.load()
            print(f"[*] Script injected. Press Ctrl+C to detach.")
            sys.stdin.read()  # 保持脚本运行
            session.detach()
        """

    except frida.ServerNotRunningError:
        print(f"[!] Error: Frida Portal is not running at {portal_address}.")
    except frida.TransportError:
        print(f"[!] Error: Could not connect to {portal_address}. Check network or address.")
    except Exception as e:
        print(f"[!] Error: {str(e)}")

def main():
    # 解析命令行参数
    parser = argparse.ArgumentParser(description="Frida Portal Test Client")
    parser.add_argument("--host", default="127.0.0.1", help="Frida Portal host (default: 127.0.0.1)")
    parser.add_argument("--port", default=27042, type=int, help="Frida Portal port (default: 27042)")
    args = parser.parse_args()

    # 测试 Portal
    test_portal(args.host, args.port)

if __name__ == "__main__":
    main()