set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_SYSTEM_PROCESSOR arm)
cmake_minimum_required(VERSION 3.27)

set(ARCH armv7e-m)
set(CPU  cortex-m4)

# set jobs
set(CMAKE_BUILD_PARALLEL_LEVEL 12)

# specify cross-compilers and tools
set(COMPILER HighTEC)

if(COMPILER STREQUAL GNU)
    set(CMAKE_C_COMPILER     arm-none-eabi-gcc)
    set(CMAKE_CXX_COMPILER   arm-none-eabi-g++)
    set(CMAKE_ASM_COMPILER   arm-none-eabi-gcc)
    set(CMAKE_AR             arm-none-eabi-ar)
    set(CMAKE_OBJCOPY        arm-none-eabi-objcopy)
    set(CMAKE_OBJDUMP        arm-none-eabi-objdump)
    set(SIZE                 arm-none-eabi-size)
elseif(COMPILER STREQUAL LLVM)
    set(CMAKE_C_COMPILER     clang)
    set(CMAKE_CXX_COMPILER   clang++)
    set(CMAKE_ASM_COMPILER   clang)
    set(CMAKE_AR             aarch32-ar)
    set(CMAKE_OBJCOPY        aarch32-objcopy)
    set(CMAKE_OBJDUMP        aarch32-objdump)
    set(SIZE                 aarch32-size)
elseif(COMPILER STREQUAL HighTEC)
    set(CMAKE_C_COMPILER     clang)
    set(CMAKE_CXX_COMPILER   clang++)
    set(CMAKE_ASM_COMPILER   clang)
    set(CMAKE_AR             aarch32-ar)
    set(CMAKE_OBJCOPY        aarch32-objcopy)
    set(CMAKE_OBJDUMP        aarch32-objdump)
    set(SIZE                 aarch32-size)
    # 跳过编译器检查
    set(CMAKE_C_COMPILER_WORKS 1)
endif()
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# project settings
get_filename_component(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR} NAME) 
project(${PROJECT_DIR} C ASM)
set(CMAKE_C_STANDARD 99)
# add header files
include_directories(src/Core/Inc)
include_directories(src/Core/SpiFlash)
include_directories(src/Drivers/HAL_Driver/Inc)
include_directories(src/MinRTOS/Inc)
include_directories(src/Drivers/CMSIS/Include)
include_directories(src/Drivers/CMSIS/Device/ST/STM32F1xx/Include)
# add source files
file(GLOB_RECURSE SOURCES "src/*.*")
# add definitions
add_definitions(-DSTM32F401xC)

#Uncomment for hardware floating point
#add_compile_definitions(ARM_MATH_CM4;ARM_MATH_MATRIX_CHECK;ARM_MATH_ROUNDING)
#add_compile_options(-mfloat-abi=hard -mfpu=vfpv4-sp-d16)
#add_link_options(-mfloat-abi=hard -mfpu=vfpv4-sp-d16)

#Uncomment for software floating point
# add_compile_options(-mfloat-abi=soft)

# add_compile_options(-mcpu=cortex-m4 -mthumb -mthumb-interwork)
add_compile_options(-march=${ARCH})
add_compile_options(-mcpu=${CPU})
add_compile_options(-mthumb)
if(COMPILER STREQUAL "LLVM")
    add_compile_options(--target=arm-none-eabi)
elseif(COMPILER STREQUAL "HighTEC")
    add_compile_options(--target=arm-none-eabi)
elseif(COMPILER STREQUAL "GNU")
    add_compile_options(-mthumb-interwork)
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
# set(COMMON_FLAGS "-specs=nosys.specs -specs=nano.specs -u _printf_float -u _scanf_float")
add_link_options(-march=${ARCH})
add_link_options(-mcpu=${CPU})
add_link_options(-mthumb)
if(COMPILER STREQUAL "LLVM")
    add_link_options(--target=arm-none-eabi)
    add_link_options(-Wl,--print-memory-usage)
elseif(COMPILER STREQUAL "HighTEC")
    add_link_options(--target=arm-none-eabi)
elseif(COMPILER STREQUAL "GNU")
    add_link_options(-specs=nosys.specs -specs=nano.specs)
    add_link_options(-mthumb-interwork)
    add_link_options(-Wl,--print-memory-usage)
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

