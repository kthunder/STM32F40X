# Contex-M4 驱动

## 编译

### 交叉编译器

arm-none-eabi-gcc

### 代码解析

 使用vscode自带插件C/C++解析代码,使用compile_commands.json文件辅助解析

#### 如何生成compile_commands.json文件

- win环境下生成compile_commands.json的工具：

```shell
#在win下依赖python的compiledb库
#1. 安装compiledb
pip install compiledb
#2. 使用compiledb运行make命令即可生成compile_commands.json
compiledb make
```

- win+wsl环境下生成compile_commands.json的工具：

```shell
#在wsl安装bear和gcc-arm-none-eabi
sudo apt-get install gcc-arm-none-eabi
sudo apt-get install bear
#生成compile_commands.json
bear -- make -j12
#在ps环境下使用wsl编译并生成compile_commands.json
wsl -e bear -- make -j12
```

## 下载

### 调试下载工具

openocd

```powershell
set old_dir=%cd%
cd /d %~dp0%

openocd.exe ^
-s "C:\\ENV\\EmbeddedToolChain\\xpack-openocd-0.11.0-5\\scripts" ^
-f "./stm32f4discovery.cfg" ^
-c "tcl_port disabled" ^
-c "gdb_port disabled" ^
-c "tcl_port disabled" ^
-c "program %1" ^
-c reset ^
-c shutdown

cd /d %old_dir%
```

## Debug工具

### 调试打印

https://wiki.segger.com/J-Link_SWO_Viewer

使用核内ITM即可 `JLinkSWOViewerCL -cpufreq 168000000 -device STM32F407VE`

### ContexM-Debug

配置文件 launch.json

```json
{
	// 使用 IntelliSense 了解相关属性。 
	// 悬停以查看现有属性的描述。
	// 欲了解更多信息，请访问: https://go.microsoft.com/fwlink/?linkid=830387
	"version": "0.2.0",
	"configurations": [
		{
			"name": "Cortex Debug",
			"cwd": "${workspaceFolder}",
			"executable": "./build/main.elf",
			"request": "launch",
			"type": "cortex-debug",
			"svdFile": "./env/STM32F407.svd",
			"runToEntryPoint": "main",
			"servertype": "openocd",
			"configFiles": [
				"interface/stlink.cfg",
				"target/stm32f4x.cfg"
			],
		}
	]
}
```


cmake -S . -B build -G "MinGW Makefiles" ; cmake --build build