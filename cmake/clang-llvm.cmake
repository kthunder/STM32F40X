set(CMAKE_SYSTEM_NAME               Generic)
set(CMAKE_SYSTEM_PROCESSOR          arm)

set(CMAKE_C_COMPILER_FORCED TRUE)
set(CMAKE_CXX_COMPILER_FORCED TRUE)
set(CMAKE_C_COMPILER_ID GNU)
set(CMAKE_CXX_COMPILER_ID GNU)

# Some default GCC settings
# arm-none-eabi- must be part of path environment
set(TOOLCHAIN_PREFIX                C:/ENV/EmbeddedToolChain/LLVM-ET-Arm/bin/)

set(CMAKE_C_COMPILER                ${TOOLCHAIN_PREFIX}clang.exe)
set(CMAKE_ASM_COMPILER              ${CMAKE_C_COMPILER})
set(CMAKE_CXX_COMPILER              ${TOOLCHAIN_PREFIX}clang++.exe)
set(CMAKE_LINKER                    ${TOOLCHAIN_PREFIX}clang)
set(CMAKE_OBJCOPY                   ${TOOLCHAIN_PREFIX}llvm-objcopy.exe)
set(CMAKE_SIZE                      ${TOOLCHAIN_PREFIX}llvm-size.exe)

set(CMAKE_EXECUTABLE_SUFFIX_ASM     ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_C       ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_CXX     ".elf")

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# MCU specific flags
# set(TARGET_FLAGS "-mfpu=fpv4-sp-d16 -mfloat-abi=hard ")
# set(TARGET_FLAGS "--target=armv7m-none-eabi -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16")
set(TARGET_FLAGS "--target=arm-none-eabi -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${TARGET_FLAGS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic -fdata-sections -ffunction-sections")
if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -g3")
endif()
if(CMAKE_BUILD_TYPE MATCHES Release)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Os -g0")
endif()

set(CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS} -x assembler-with-cpp -MMD -MP")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -fno-rtti -fno-exceptions -fno-threadsafe-statics")

set(CMAKE_C_LINK_FLAGS "${TARGET_FLAGS}")
set(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} -T \"${CMAKE_SOURCE_DIR}/STM32F401RCTx_FLASH.ld\"")
set(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} --specs=nano.specs")
set(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} -Wl,-Map=${CMAKE_PROJECT_NAME}.map -Wl,--gc-sections")
set(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} -Wl,--start-group -lc -lm -Wl,--end-group")
set(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} -Wl,--print-memory-usage")

set(CMAKE_CXX_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} -Wl,--start-group -lstdc++ -lsupc++ -Wl,--end-group")