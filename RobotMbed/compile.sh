#!/bin/bash

COMPILER_BIN_DIR="$1"
PROGRAM_NAME="$2"
PATH_TO_PROG_DIR="$3"
CC_RSC="$4"
BLUETOOTH="$5"

INCLUDE="-I${CC_RSC}/microbit -I${CC_RSC}/microbit-dal -I${CC_RSC}/mbed-classic \
-I${CC_RSC}/ble -I${CC_RSC}/ble-nrf51822 -I${CC_RSC}/nrf51-sdk -I${CC_RSC}/microbit/inc \
-I${CC_RSC}/microbit-dal/inc/core -I${CC_RSC}/microbit-dal/inc/types -I${CC_RSC}/microbit-dal/inc/drivers \
-I${CC_RSC}/microbit-dal/inc/BLUETOOTH -I${CC_RSC}/microbit-dal/inc/platform \
-I${CC_RSC}/mbed-classic/api -I${CC_RSC}/mbed-classic/hal -I${CC_RSC}/mbed-classic/targets/hal \
-I${CC_RSC}/mbed-classic/targets/cmsis -I${CC_RSC}/ble-nrf51822/source/btle \
-I${CC_RSC}/ble-nrf51822/source/btle/custom -I${CC_RSC}/ble-nrf51822/source/common \
-I${CC_RSC}/nrf51-sdk/source/nordic_sdk/components/ble/ble_radio_notification \
-I${CC_RSC}/nrf51-sdk/source/nordic_sdk/components/ble/ble_services/ble_dfu \
-I${CC_RSC}/nrf51-sdk/source/nordic_sdk/components/ble/common -I${CC_RSC}/nrf51-sdk/source/nordic_sdk/components/ble/device_manager \
-I${CC_RSC}/nrf51-sdk/source/nordic_sdk/components/ble/device_manager/config \
-I${CC_RSC}/nrf51-sdk/source/nordic_sdk/components/ble/peer_manager \
-I${CC_RSC}/nrf51-sdk/source/nordic_sdk/components/device -I${CC_RSC}/nrf51-sdk/source/nordic_sdk/components/drivers_nrf/ble_flash \
-I${CC_RSC}/nrf51-sdk/source/nordic_sdk/components/drivers_nrf/delay -I${CC_RSC}/nrf51-sdk/source/nordic_sdk/components/drivers_nrf/hal \
-I${CC_RSC}/nrf51-sdk/source/nordic_sdk/components/drivers_nrf/pstorage \
-I${CC_RSC}/nrf51-sdk/source/nordic_sdk/components/drivers_nrf/pstorage/config \
-I${CC_RSC}/nrf51-sdk/source/nordic_sdk/components/libraries/bootloader_dfu \
-I${CC_RSC}/nrf51-sdk/source/nordic_sdk/components/libraries/bootloader_dfu/hci_transport \
-I${CC_RSC}/nrf51-sdk/source/nordic_sdk/components/libraries/crc16 -I${CC_RSC}/nrf51-sdk/source/nordic_sdk/components/libraries/hci \
-I${CC_RSC}/nrf51-sdk/source/nordic_sdk/components/libraries/scheduler -I${CC_RSC}/nrf51-sdk/source/nordic_sdk/components/libraries/timer \
-I${CC_RSC}/nrf51-sdk/source/nordic_sdk/components/libraries/util \
-I${CC_RSC}/nrf51-sdk/source/nordic_sdk/components/libraries/fds -I${CC_RSC}/nrf51-sdk/source/nordic_sdk/components/libraries/fstorage \
-I${CC_RSC}/nrf51-sdk/source/nordic_sdk/components/libraries/experimental_section_vars \
-I${CC_RSC}/nrf51-sdk/source/nordic_sdk/components/softdevice/common/softdevice_handler \
-I${CC_RSC}/nrf51-sdk/source/nordic_sdk/components/softdevice/s130/headers \
-I${CC_RSC}/nrf51-sdk/source/nordic_sdk/components/toolchain -I${CC_RSC}/mbed-classic/targets \
-I${CC_RSC}/mbed-classic/targets/hal/TARGET_NORDIC -I${CC_RSC}/mbed-classic/targets/hal/TARGET_NORDIC/TARGET_MCU_NRF51822 \
-I${CC_RSC}/mbed-classic/targets/hal/TARGET_NORDIC/TARGET_MCU_NRF51822/TARGET_NRF51_CALLIOPE \
-I${CC_RSC}/mbed-classic/targets/hal/TARGET_NORDIC/TARGET_MCU_NRF51822/Lib \
-I${CC_RSC}/mbed-classic/targets/hal/TARGET_NORDIC/TARGET_MCU_NRF51822/Lib/nordic_sdk \
-I${CC_RSC}/mbed-classic/targets/hal/TARGET_NORDIC/TARGET_MCU_NRF51822/Lib/nordic_sdk/components \
-I${CC_RSC}/mbed-classic/targets/hal/TARGET_NORDIC/TARGET_MCU_NRF51822/Lib/nordic_sdk/components/libraries \
-I${CC_RSC}/mbed-classic/targets/hal/TARGET_NORDIC/TARGET_MCU_NRF51822/Lib/nordic_sdk/components/libraries/util \
-I${CC_RSC}/mbed-classic/targets/hal/TARGET_NORDIC/TARGET_MCU_NRF51822/Lib/nordic_sdk/components/libraries/scheduler \
-I${CC_RSC}/mbed-classic/targets/hal/TARGET_NORDIC/TARGET_MCU_NRF51822/Lib/nordic_sdk/components/libraries/crc16 \
-I${CC_RSC}/mbed-classic/targets/hal/TARGET_NORDIC/TARGET_MCU_NRF51822/Lib/s130_nrf51822_1_0_0 \
-I${CC_RSC}/mbed-classic/targets/hal/TARGET_NORDIC/TARGET_MCU_NRF51822/Lib/s110_nrf51822_8_0_0 \
-I${CC_RSC}/mbed-classic/targets/cmsis/TARGET_NORDIC -I${CC_RSC}/mbed-classic/targets/cmsis/TARGET_NORDIC/TARGET_MCU_NRF51822 \
-I${CC_RSC}/mbed-classic/targets/cmsis/TARGET_NORDIC/TARGET_MCU_NRF51822/TOOLCHAIN_GCC_ARM \
-I${CC_RSC}/mbed-classic/targets/cmsis/TARGET_NORDIC/TARGET_MCU_NRF51822/TOOLCHAIN_GCC_ARM/TARGET_MCU_NRF51_16K_S110 \
-I${CC_RSC}/FourDigitDisplay -I${CC_RSC}/Grove_LED_Bar -I${CC_RSC}/Sht31 -Isource"
OBJECTS="${PATH_TO_PROG_DIR}target/${PROGRAM_NAME}.cpp.o \
${CC_RSC}/microbit${BLUETOOTH}.a \
${CC_RSC}/ble.a \
${CC_RSC}/microbit-dal${BLUETOOTH}.a \
${CC_RSC}/ble-nrf51822.a \
${CC_RSC}/nrf51-sdk.a \
${CC_RSC}/mbed-classic${BLUETOOTH}.a \
${CC_RSC}/FourDigitDisplay.a \
${CC_RSC}/Grove_LED_Bar.a \
${CC_RSC}/Sht31.a"

cppc_flags='-fno-exceptions -fno-unwind-tables -ffunction-sections -fdata-sections -Wdeprecated -Wno-unused-variable -Wall -Wextra \
-fno-rtti -fno-threadsafe-statics -mcpu=cortex-m0 -mthumb -D__thumb2__ -std=c++11 -fwrapv -Os -g -gdwarf-3 -DNDEBUG \
-DTOOLCHAIN_GCC -DTOOLCHAIN_GCC_ARM -DMBED_OPERATORS -DNRF51 -DTARGET_NORDIC -DTARGET_M0 -D__MBED__=1 \
-DMCU_NORDIC_16K -DTARGET_NRF51_CALLIOPE -DTARGET_MCU_NORDIC_16K -DTARGET_MCU_NRF51_16K_S110 \
-DTARGET_NRF_LFCLK_RC -DTARGET_MCU_NORDIC_16K -D__CORTEX_M0 -DARM_MATH_CM0 -MMD'
ld_flags='-fno-exceptions -fno-unwind-tables -Wl,--no-wchar-size-warning -Wl,--gc-sections -Wl,--sort-common \
-Wl,--sort-section=alignment -Wl,-wrap,main -mcpu=cortex-m0 -mthumb --specs=nano.specs'
ld_sys_libs='-lnosys  -lstdc++ -lsupc++ -lm -lc -lgcc -lstdc++ -lsupc++ -lm -lc -lgcc -Wl,'

echo "[1/3] Building the ${PROGRAM_NAME}."
run="${COMPILER_BIN_DIR}arm-none-eabi-g++ ${INCLUDE} ${cppc_flags} -MT ${PATH_TO_PROG_DIR}source/${PROGRAM_NAME}.cpp.o -MF ${PATH_TO_PROG_DIR}target/${PROGRAM_NAME}.cpp.o.d -o ${program_location}target/${PROGRAM_NAME}.cpp.o -c ${PATH_TO_PROG_DIR}source/${PROGRAM_NAME}.cpp"
if ! $run; then
    echo "Error in building !!!"
    exit 1
fi

echo "[2/3] Linking CXX executable"
run="${COMPILER_BIN_DIR}arm-none-eabi-g++ ${ld_flags} -T ${CC_RSC}/NRF51822.ld -Wl,-Map,${PATH_TO_PROG_DIR}target/${PROGRAM_NAME}.map -Wl,--start-group ${OBJECTS} ${ld_sys_libs}--end-group -o ${PATH_TO_PROG_DIR}target/${PROGRAM_NAME}"
if ! $run; then
    echo "Error in linking !!!"
    exit 1
fi
echo "[3/3] Creating HEX file"
run="${COMPILER_BIN_DIR}arm-none-eabi-objcopy -O ihex ${PATH_TO_PROG_DIR}target/${PROGRAM_NAME} ${PATH_TO_PROG_DIR}target/firmware.hex"
if ! $run; then
    echo "Error creating firmware.hex !!!"
    exit 1
fi
run="srec_cat ${CC_RSC}/BLE_BOOTLOADER_RESERVED.hex -intel ${CC_RSC}/s110_nrf51822_8.0.0_softdevice.hex -intel ${PATH_TO_PROG_DIR}target/firmware.hex -intel -o ${PATH_TO_PROG_DIR}target/${PROGRAM_NAME}.hex -intel --line-length=44"
if ! $run; then
    echo "Error creating the HEX !!!"
    exit 1
fi
