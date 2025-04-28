import frida
from frida_tools.application import Reactor

class SimplePortalApp:
    def __init__(self):
        # 必须提供 run_until_return 参数
        self._reactor = Reactor(run_until_return=self._run_loop)
        self._service = frida.PortalService(
            frida.EndpointParameters(address="0.0.0.0", port=27042),
            None
        )

        self._service.on("controller-connected", lambda cid, addr: self._reactor.schedule(lambda: self._on_controller_connected(cid, addr)))
        self._service.on("controller-disconnected", lambda cid, addr: self._reactor.schedule(lambda: self._on_controller_disconnected(cid, addr)))

    def run(self):
        print(f"[*] Frida version: {frida.__version__}")
        self._reactor.schedule(self._start)
        self._reactor.run()

    def _start(self):
        print("[*] Starting PortalService...")
        self._service.start()

    def _on_controller_connected(self, connection_id, remote_address):
        print(f"[+] Gadget connected from {remote_address} (id={connection_id})")
        self._service.accept(connection_id)

    def _on_controller_disconnected(self, connection_id, remote_address):
        print(f"[-] Gadget disconnected from {remote_address} (id={connection_id})")

    def _run_loop(self, reactor):
        # 简单等待用户输入来阻止主线程退出
        try:
            input("Server running. Press Enter to stop...\n")
        except KeyboardInterrupt:
            pass


if __name__ == "__main__":
    app = SimplePortalApp()
    app.run()
