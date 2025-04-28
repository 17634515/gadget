import socket
import signal
import sys

# 配置监听的 IP 和端口
HOST = '0.0.0.0'  # 监听所有 IP 地址（包括局域网连接）
PORT = 27042       # 要监听的端口

# 创建 socket 对象
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# 设置重用地址
server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

# 绑定 IP 地址和端口
server_socket.bind((HOST, PORT))

# 设置为非阻塞模式
server_socket.setblocking(False)

# 开始监听
server_socket.listen(5)
print(f"Listening on {HOST}:{PORT}...")

# 确保按 Ctrl+C 时可以正确退出
def signal_handler(sig, frame):
    print("Server stopped manually")
    server_socket.close()
    sys.exit(0)

signal.signal(signal.SIGINT, signal_handler)  # 捕获 Ctrl+C 信号

try:
    while True:
        try:
            # 非阻塞模式等待连接
            client_socket, client_address = server_socket.accept()
            print(f"Connection from {client_address}")

            # 发送欢迎信息
            client_socket.sendall(b"Hello from the server!\n")

            # 关闭客户端连接
            client_socket.close()

        except BlockingIOError:
            # 如果没有连接，继续循环，等待下次连接
            pass

except Exception as e:
    print(f"An error occurred: {e}")
finally:
    server_socket.close()
