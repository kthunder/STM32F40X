set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_SYSTEM_PROCESSOR arm)
cmake_minimum_required(VERSION 3.27)

set(ARCH armv7e-m)
set(CPU  cortex-m4)

# set jobs
set(CMAKE_BUILD_PARALLEL_LEVEL 12)

# specify cross-compilers and tools
set(COMPILER LLVM)

if(COMPILER STREQUAL GNU)
    set(COMPILER_PATH        C:/ENV/EmbeddedToolChain/arm-gnu-toolchain-13.3.rel1/bin)
    set(CMAKE_C_COMPILER     ${COMPILER_PATH}/arm-none-eabi-gcc.exe)
    set(CMAKE_CXX_COMPILER   ${COMPILER_PATH}/arm-none-eabi-g++.exe)
    set(CMAKE_ASM_COMPILER   ${COMPILER_PATH}/arm-none-eabi-gcc.exe)
    set(CMAKE_AR             ${COMPILER_PATH}/arm-none-eabi-ar.exe)
    set(CMAKE_OBJCOPY        ${COMPILER_PATH}/arm-none-eabi-objcopy.exe)
    set(CMAKE_OBJDUMP        ${COMPILER_PATH}/arm-none-eabi-objdump.exe)
    set(SIZE                 ${COMPILER_PATH}/arm-none-eabi-size.exe)
elseif(COMPILER STREQUAL LLVM)
    set(COMPILER_PATH        C:/ENV/EmbeddedToolChain/LLVM-ET-Arm/bin)
    set(CMAKE_C_COMPILER     ${COMPILER_PATH}/clang.exe)
    set(CMAKE_CXX_COMPILER   ${COMPILER_PATH}/clang++.exe)
    set(CMAKE_ASM_COMPILER   ${COMPILER_PATH}/clang.exe)
    set(CMAKE_AR             ${COMPILER_PATH}/llvm-ar.exe)
    set(CMAKE_OBJCOPY        ${COMPILER_PATH}/llvm-objcopy.exe)
    set(CMAKE_OBJDUMP        ${COMPILER_PATH}/llvm-objdump.exe)
    set(SIZE                 ${COMPILER_PATH}/llvm-size.exe)
elseif(COMPILER STREQUAL HighTEC)
    set(COMPILER_PATH        C:/IDE/ThinkTechStudio/toolchain/HighTec/toolchains/arm/v8.0.0/bin)
    set(CMAKE_C_COMPILER     ${COMPILER_PATH}/clang.exe)
    set(CMAKE_CXX_COMPILER   ${COMPILER_PATH}/clang++.exe)
    set(CMAKE_ASM_COMPILER   ${COMPILER_PATH}/clang.exe)
    set(CMAKE_AR             ${COMPILER_PATH}/aarch32-ar.exe)
    set(CMAKE_OBJCOPY        ${COMPILER_PATH}/aarch32-objcopy.exe)
    set(CMAKE_OBJDUMP        ${COMPILER_PATH}/aarch32-objdump.exe)
    set(SIZE                 ${COMPILER_PATH}/aarch32-size.exe)
    # 跳过编译器检查
    set(CMAKE_C_COMPILER_WORKS 1)
endif()
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# project settings
get_filename_component(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR} NAME) 
project(${PROJECT_DIR} C ASM)
set(CMAKE_C_STANDARD 99)
# add header files
include_directories(src/app/Inc)
include_directories(src/Drivers/CMSIS/Device/ST/STM32F1xx/Include)
include_directories(src/Drivers/CMSIS/Include)
include_directories(src/Drivers/HAL_Driver/Inc)
include_directories(src/Drivers/MinRTOS/Inc)
include_directories(src/threadx/common/inc)
include_directories(src/threadx/ports/inc)

# add source files
file(GLOB_RECURSE SOURCES "src/*.[c|s|S]" "src/plugins/*.*")
foreach(var ${SOURCES})
    message("SOURCES = ${var}")
endforeach()
# exclued source files
file(GLOB_RECURSE EXCLUDE_SOURCES)
list(REMOVE_ITEM SOURCES ${EXCLUDE_SOURCES})

# add definitions
add_definitions(-DSTM32F401xC)

#Uncomment for hardware floating point
# add_compile_definitions(ARM_MATH_CM4;ARM_MATH_MATRIX_CHECK;ARM_MATH_ROUNDING)
add_compile_options(-mfloat-abi=hard -mfpu=fpv5-d16)
add_link_options(-mfloat-abi=hard -mfpu=fpv5-d16)
 
#Uncomment for software floating point
# add_compile_options(-mfloat-abi=soft)

add_compile_options(-march=${ARCH})
add_compile_options(-mcpu=${CPU})
add_compile_options(-mthumb)
if(COMPILER STREQUAL "LLVM")
    add_compile_options(--target=arm-none-eabi)
    add_definitions(-D_LLVM_C_ALIOTHXX_)
elseif(COMPILER STREQUAL "HighTEC")
    add_compile_options(--target=arm-none-eabi)
    add_definitions(-D_HITECH_C_ALIOTHXX_)
elseif(COMPILER STREQUAL "GNU")
    add_compile_options(-mthumb-interwork)
    add_definitions(-D_GCC_C_ALIOTHXX_)
endif()

add_compile_options(-ffunction-sections -fdata-sections -fno-common)
add_compile_options(-fmessage-length=0 -fomit-frame-pointer -funsigned-char)
# add_compile_options(-pedantic)
add_compile_options(-Wunused)
add_compile_options(-Wall)
add_compile_options(-Wextra)

# uncomment to mitigate c++17 absolute addresses warnings
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-register")

# Enable assembler files preprocessing
add_compile_options($<$<COMPILE_LANGUAGE:ASM>:-x$<SEMICOLON>assembler-with-cpp>)
add_compile_options(-O0 -g)

# set linker file
set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/env/STM32F401RCTX_FLASH.ld)
add_link_options(-march=${ARCH})
add_link_options(-mcpu=${CPU})
add_link_options(-mthumb)
if(COMPILER STREQUAL "LLVM")
    add_link_options(--target=arm-none-eabi)
    add_link_options(-Wl,--print-memory-usage)
elseif(COMPILER STREQUAL "HighTEC")
    add_link_options(--target=arm-none-eabi)
elseif(COMPILER STREQUAL "GNU")
    # add_link_options(-specs=nosys.specs -specs=nano.specs)
    add_link_options(-mthumb-interwork)
    add_link_options(-Wl,--print-memory-usage)
    add_link_options(-nostartfiles)
endif()
add_link_options(-Wl,-gc-sections,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map)
add_link_options(-T ${LINKER_SCRIPT})

add_executable(${PROJECT_NAME}.elf ${SOURCES} ${LINKER_SCRIPT})

set(HEX_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)
set(DIS_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.dis)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
        COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
        COMMAND ${CMAKE_OBJDUMP} -d $<TARGET_FILE:${PROJECT_NAME}.elf> > ${DIS_FILE}
        COMMENT "Building ${HEX_FILE}
Building ${BIN_FILE}
Building ${DIS_FILE}")
