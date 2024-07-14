import subprocess
import telnetlib
import time

debugger = subprocess.Popen([
      r"openocd.exe",
      "-f", r"./env/stm32f4discovery.cfg",
    #   "-c", "telnet_port 4444"
      ])
tel = telnetlib.Telnet('localhost', 4444)

# C:/ENV/EmbeddedToolChain/OpenOCD/bin/openocd.exe -f "./env/stm32f4discovery.cfg" -c "program ./build/%ProjectName%.elf" -c reset -c shutdown
def read_data():
    return tel.read_until(b"\r\n", 1.0)


def cmd(command):
    tel.write((f"{command}\r\n").encode())
    # return read_data()


if __name__ == "__main__":
    time.sleep(3)
    # while 1:
    #     pass
    print(f"begain")
    try:
        cmd("reset")
        # print(read_data().decode())
        # print(cmd('mdw 0x1FFF7590 3'))
        # print(read_data().decode())
    except Exception as e:
        print(f"Error: {e}")

    tel.close()
    debugger.terminate()