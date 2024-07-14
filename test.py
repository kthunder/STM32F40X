import telnetlib
from time import sleep

tel = telnetlib.Telnet('localhost', 4444)

def cmd(command):
    tel.write((f"{command}\r\n").encode())
    tel.read_until((f"{command}\r\n").encode(), 1.0)
    return tel.read_until(b"> ", 1.0).decode("ascii").rstrip("> ")

def testTCL():
    # 写入用户名和密码
    # tel.write(b"username\n")  # 替换为实际的用户名
    tel.read_until(b"> ", 3)

    # tel.write(b"targets\r\n")  # 替换为实际的密码

    res = cmd("targets")
    print(res)

    # 读取登录后的输出直到看到提示符
    # tel.read_until(b"\r\n")  # 替换为实际的提示符

    # 执行一个命令
    # tel.write(b"ls\n")  # 执行 ls 命令

    # 读取命令的输出
    # output = tel.read_until(b"\r\n")  # 再次读取直到提示符

    # print(output.decode('ascii'))

    # output = tel.read_until(b"\r\n")  # 再次读取直到提示符

    # for i in range(5):
    #     output = tel.read_until(b"> ", 1)
    #     # print(i,"==>",":",output.decode("ascii"))
    #     print(i,"==>",":",output)

    # 关闭连接
    tel.close()

if __name__ == "__main__":
    # while 1:
    #     pass
    try:
        testTCL()
        # print(cmd('mdw 0x1FFF7590 3'))
        # print(read_data().decode())
        # while 1:
        #     sleep(1)
            # str = read_data().decode()
            # if str:
            #     print("",str.__len__(),str)
            # pass
    except Exception as e:
        print(f"Error: {e}")

    # tel.close()
    # debugger.terminate()