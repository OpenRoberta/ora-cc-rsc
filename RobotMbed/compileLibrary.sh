#!/bin/bash


compiler=''
library_name=''
library_location=''
compiler_resources=''

compiler="$1"
library_name="$2"
library_location="$3"
compiler_resources="$4"


include_paths="-I${compiler_resources}/microbit -I${compiler_resources}/microbit-dal -I${compiler_resources}/mbed-classic -I${compiler_resources}/ble -I${compiler_resources}/ble-nrf51822 -I${compiler_resources}/nrf51-sdk -I${compiler_resources}/microbit/inc -I${compiler_resources}/microbit-dal/inc/core -I${compiler_resources}/microbit-dal/inc/types -I${compiler_resources}/microbit-dal/inc/drivers -I${compiler_resources}/microbit-dal/inc/bluetooth -I${compiler_resources}/microbit-dal/inc/platform -I${compiler_resources}/mbed-classic/api -I${compiler_resources}/mbed-classic/hal -I${compiler_resources}/mbed-classic/targets/hal -I${compiler_resources}/mbed-classic/targets/cmsis -I${compiler_resources}/ble-nrf51822/source/btle -I${compiler_resources}/ble-nrf51822/source/btle/custom -I${compiler_resources}/ble-nrf51822/source/common -I${compiler_resources}/nrf51-sdk/source/nordic_sdk/components/ble/ble_radio_notification -I${compiler_resources}/nrf51-sdk/source/nordic_sdk/components/ble/ble_services/ble_dfu -I${compiler_resources}/nrf51-sdk/source/nordic_sdk/components/ble/common -I${compiler_resources}/nrf51-sdk/source/nordic_sdk/components/ble/device_manager -I${compiler_resources}/nrf51-sdk/source/nordic_sdk/components/ble/device_manager/config -I${compiler_resources}/nrf51-sdk/source/nordic_sdk/components/ble/peer_manager -I${compiler_resources}/nrf51-sdk/source/nordic_sdk/components/device -I${compiler_resources}/nrf51-sdk/source/nordic_sdk/components/drivers_nrf/ble_flash -I${compiler_resources}/nrf51-sdk/source/nordic_sdk/components/drivers_nrf/delay -I${compiler_resources}/nrf51-sdk/source/nordic_sdk/components/drivers_nrf/hal -I${compiler_resources}/nrf51-sdk/source/nordic_sdk/components/drivers_nrf/pstorage -I${compiler_resources}/nrf51-sdk/source/nordic_sdk/components/drivers_nrf/pstorage/config -I${compiler_resources}/nrf51-sdk/source/nordic_sdk/components/libraries/bootloader_dfu -I${compiler_resources}/nrf51-sdk/source/nordic_sdk/components/libraries/bootloader_dfu/hci_transport -I${compiler_resources}/nrf51-sdk/source/nordic_sdk/components/libraries/crc16 -I${compiler_resources}/nrf51-sdk/source/nordic_sdk/components/libraries/hci -I${compiler_resources}/nrf51-sdk/source/nordic_sdk/components/libraries/scheduler -I${compiler_resources}/nrf51-sdk/source/nordic_sdk/components/libraries/timer -I${compiler_resources}/nrf51-sdk/source/nordic_sdk/components/libraries/util -I${compiler_resources}/nrf51-sdk/source/nordic_sdk/components/libraries/fds -I${compiler_resources}/nrf51-sdk/source/nordic_sdk/components/libraries/fstorage -I${compiler_resources}/nrf51-sdk/source/nordic_sdk/components/libraries/experimental_section_vars -I${compiler_resources}/nrf51-sdk/source/nordic_sdk/components/softdevice/common/softdevice_handler -I${compiler_resources}/nrf51-sdk/source/nordic_sdk/components/softdevice/s130/headers -I${compiler_resources}/nrf51-sdk/source/nordic_sdk/components/toolchain -I${compiler_resources}/mbed-classic/targets -I${compiler_resources}/mbed-classic/targets/hal/TARGET_NORDIC -I${compiler_resources}/mbed-classic/targets/hal/TARGET_NORDIC/TARGET_MCU_NRF51822 -I${compiler_resources}/mbed-classic/targets/hal/TARGET_NORDIC/TARGET_MCU_NRF51822/TARGET_NRF51_CALLIOPE -I${compiler_resources}/mbed-classic/targets/hal/TARGET_NORDIC/TARGET_MCU_NRF51822/Lib -I${compiler_resources}/mbed-classic/targets/hal/TARGET_NORDIC/TARGET_MCU_NRF51822/Lib/nordic_sdk -I${compiler_resources}/mbed-classic/targets/hal/TARGET_NORDIC/TARGET_MCU_NRF51822/Lib/nordic_sdk/components -I${compiler_resources}/mbed-classic/targets/hal/TARGET_NORDIC/TARGET_MCU_NRF51822/Lib/nordic_sdk/components/libraries -I${compiler_resources}/mbed-classic/targets/hal/TARGET_NORDIC/TARGET_MCU_NRF51822/Lib/nordic_sdk/components/libraries/util -I${compiler_resources}/mbed-classic/targets/hal/TARGET_NORDIC/TARGET_MCU_NRF51822/Lib/nordic_sdk/components/libraries/scheduler -I${compiler_resources}/mbed-classic/targets/hal/TARGET_NORDIC/TARGET_MCU_NRF51822/Lib/nordic_sdk/components/libraries/crc16 -I${compiler_resources}/mbed-classic/targets/hal/TARGET_NORDIC/TARGET_MCU_NRF51822/Lib/s130_nrf51822_1_0_0 -I${compiler_resources}/mbed-classic/targets/hal/TARGET_NORDIC/TARGET_MCU_NRF51822/Lib/s110_nrf51822_8_0_0 -I${compiler_resources}/mbed-classic/targets/cmsis/TARGET_NORDIC -I${compiler_resources}/mbed-classic/targets/cmsis/TARGET_NORDIC/TARGET_MCU_NRF51822 -I${compiler_resources}/mbed-classic/targets/cmsis/TARGET_NORDIC/TARGET_MCU_NRF51822/TOOLCHAIN_GCC_ARM -I${compiler_resources}/mbed-classic/targets/cmsis/TARGET_NORDIC/TARGET_MCU_NRF51822/TOOLCHAIN_GCC_ARM/TARGET_MCU_NRF51_16K_S110 -I${compiler_resources}/FourDigitDisplay -I${compiler_resources}/Grove_LED_Bar -I${compiler_resources}/Sht31 -Isource"
objecs="${library_location}target/${library_name}.cpp.o  ${compiler_resources}/microbit.a ${compiler_resources}/ble.a ${compiler_resources}/microbit-dal.a ${compiler_resources}/ble-nrf51822.a ${compiler_resources}/nrf51-sdk.a ${compiler_resources}/mbed-classic.a"


cppc_flags='-fno-exceptions -fno-unwind-tables -ffunction-sections -fdata-sections -Wdeprecated -Wno-unused-variable -Wall -Wextra -fno-rtti -fno-threadsafe-statics -mcpu=cortex-m0 -mthumb -D__thumb2__ -std=c++11 -fwrapv -Os -g -gdwarf-3 -DNDEBUG   -DTOOLCHAIN_GCC -DTOOLCHAIN_GCC_ARM -DMBED_OPERATORS -DNRF51 -DTARGET_NORDIC -DTARGET_M0 -D__MBED__=1 -DMCU_NORDIC_16K -DTARGET_NRF51_CALLIOPE -DTARGET_MCU_NORDIC_16K -DTARGET_MCU_NRF51_16K_S110  -DTARGET_NRF_LFCLK_RC -DTARGET_MCU_NORDIC_16K -D__CORTEX_M0 -DARM_MATH_CM0 -MMD'
ld_flags='-fno-exceptions -fno-unwind-tables -Wl,--no-wchar-size-warning -Wl,--gc-sections -Wl,--sort-common -Wl,--sort-section=alignment -Wl,-wrap,main -mcpu=cortex-m0 -mthumb --specs=nano.specs'
ld_sys_libs='-lnosys  -lstdc++ -lsupc++ -lm -lc -lgcc -lstdc++ -lsupc++ -lm -lc -lgcc -Wl,'

echo "[1/4] Creating build directory"
run="mkdir -p ${library_location}/target"
if ! $run; then
    echo "Error in directory creation !!!"
    exit 1
fi

echo "[2/4] Building the ${library_name}."
run="${compiler}arm-none-eabi-g++ ${include_paths} ${cppc_flags} -MT ${library_location}/${library_name}.cpp.o -MF ${library_location}/target/${library_name}.cpp.o.d -o ${library_location}/target/${library_name}.cpp.o -c ${library_location}/${library_name}.cpp"
if ! $run; then
    echo "Error in building !!!"
    exit 1
fi

echo "[3/4] Generating the static library."
run="ar rvs ${compiler_resources}/${library_name}.a ${library_location}/target/${library_name}.cpp.o"
if ! $run; then
    echo "Error in static library generation !!!"
    exit 1
fi

echo "[4/4] Cleaning the build directory."
run="rm -R ${library_location}/target"
if ! $run; then
    echo "Error in cleaning !!!"
    exit 1
fi