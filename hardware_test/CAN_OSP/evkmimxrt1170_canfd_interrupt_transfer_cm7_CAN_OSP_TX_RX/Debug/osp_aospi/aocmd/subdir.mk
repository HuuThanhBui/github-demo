################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../osp_aospi/aocmd/aocmd.c \
../osp_aospi/aocmd/aocmd_board.c \
../osp_aospi/aocmd/aocmd_cint.c \
../osp_aospi/aocmd/aocmd_echo.c \
../osp_aospi/aocmd/aocmd_help.c \
../osp_aospi/aocmd/aocmd_osp.c \
../osp_aospi/aocmd/aocmd_said.c \
../osp_aospi/aocmd/aocmd_version.c 

C_DEPS += \
./osp_aospi/aocmd/aocmd.d \
./osp_aospi/aocmd/aocmd_board.d \
./osp_aospi/aocmd/aocmd_cint.d \
./osp_aospi/aocmd/aocmd_echo.d \
./osp_aospi/aocmd/aocmd_help.d \
./osp_aospi/aocmd/aocmd_osp.d \
./osp_aospi/aocmd/aocmd_said.d \
./osp_aospi/aocmd/aocmd_version.d 

OBJS += \
./osp_aospi/aocmd/aocmd.o \
./osp_aospi/aocmd/aocmd_board.o \
./osp_aospi/aocmd/aocmd_cint.o \
./osp_aospi/aocmd/aocmd_echo.o \
./osp_aospi/aocmd/aocmd_help.o \
./osp_aospi/aocmd/aocmd_osp.o \
./osp_aospi/aocmd/aocmd_said.o \
./osp_aospi/aocmd/aocmd_version.o 


# Each subdirectory must supply rules for building sources it contributes
osp_aospi/aocmd/%.o: ../osp_aospi/aocmd/%.c osp_aospi/aocmd/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_MIMXRT1176DVMAA -DCPU_MIMXRT1176DVMAA_cm7 -DSDK_DEBUGCONSOLE=1 -DXIP_EXTERNAL_FLASH=1 -DXIP_BOOT_HEADER_ENABLE=1 -DFLEXCAN_WAIT_TIMEOUT=1000 -DMCUXPRESSO_SDK -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -DSDK_OS_FREE_RTOS -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_canfd_interrupt_transfer_cm7_CAN_OSP_TX_RX\utilities" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_canfd_interrupt_transfer_cm7_CAN_OSP_TX_RX\drivers" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_canfd_interrupt_transfer_cm7_CAN_OSP_TX_RX\device" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_canfd_interrupt_transfer_cm7_CAN_OSP_TX_RX\component\uart" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_canfd_interrupt_transfer_cm7_CAN_OSP_TX_RX\component\lists" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_canfd_interrupt_transfer_cm7_CAN_OSP_TX_RX\startup" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_canfd_interrupt_transfer_cm7_CAN_OSP_TX_RX\xip" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_canfd_interrupt_transfer_cm7_CAN_OSP_TX_RX\CMSIS" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_canfd_interrupt_transfer_cm7_CAN_OSP_TX_RX\drivers\freertos" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_canfd_interrupt_transfer_cm7_CAN_OSP_TX_RX\freertos\freertos-kernel\include" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_canfd_interrupt_transfer_cm7_CAN_OSP_TX_RX\freertos\freertos-kernel\portable\GCC\ARM_CM4F" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_canfd_interrupt_transfer_cm7_CAN_OSP_TX_RX\source" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_canfd_interrupt_transfer_cm7_CAN_OSP_TX_RX\osp_aospi\aocmd" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_canfd_interrupt_transfer_cm7_CAN_OSP_TX_RX\osp_aospi\aomw" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_canfd_interrupt_transfer_cm7_CAN_OSP_TX_RX\osp_aospi\aoosp" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_canfd_interrupt_transfer_cm7_CAN_OSP_TX_RX\osp_aospi\aoresult" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_canfd_interrupt_transfer_cm7_CAN_OSP_TX_RX\osp_aospi\aospi" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_canfd_interrupt_transfer_cm7_CAN_OSP_TX_RX\board" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_canfd_interrupt_transfer_cm7_CAN_OSP_TX_RX\evkmimxrt1170\driver_examples\canfd\interrupt_transfer\cm7" -O0 -fno-common -g3 -gdwarf-4 -c -ffunction-sections -fdata-sections -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m7 -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-osp_aospi-2f-aocmd

clean-osp_aospi-2f-aocmd:
	-$(RM) ./osp_aospi/aocmd/aocmd.d ./osp_aospi/aocmd/aocmd.o ./osp_aospi/aocmd/aocmd_board.d ./osp_aospi/aocmd/aocmd_board.o ./osp_aospi/aocmd/aocmd_cint.d ./osp_aospi/aocmd/aocmd_cint.o ./osp_aospi/aocmd/aocmd_echo.d ./osp_aospi/aocmd/aocmd_echo.o ./osp_aospi/aocmd/aocmd_help.d ./osp_aospi/aocmd/aocmd_help.o ./osp_aospi/aocmd/aocmd_osp.d ./osp_aospi/aocmd/aocmd_osp.o ./osp_aospi/aocmd/aocmd_said.d ./osp_aospi/aocmd/aocmd_said.o ./osp_aospi/aocmd/aocmd_version.d ./osp_aospi/aocmd/aocmd_version.o

.PHONY: clean-osp_aospi-2f-aocmd

