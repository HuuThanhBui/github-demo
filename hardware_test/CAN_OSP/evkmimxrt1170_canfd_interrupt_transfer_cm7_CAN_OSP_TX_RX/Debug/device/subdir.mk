################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../device/system_MIMXRT1176_cm7.c 

C_DEPS += \
./device/system_MIMXRT1176_cm7.d 

OBJS += \
./device/system_MIMXRT1176_cm7.o 


# Each subdirectory must supply rules for building sources it contributes
device/%.o: ../device/%.c device/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_MIMXRT1176DVMAA -DCPU_MIMXRT1176DVMAA_cm7 -DSDK_DEBUGCONSOLE=1 -DXIP_EXTERNAL_FLASH=1 -DXIP_BOOT_HEADER_ENABLE=1 -DFLEXCAN_WAIT_TIMEOUT=1000 -DMCUXPRESSO_SDK -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -DSDK_OS_FREE_RTOS -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_canfd_interrupt_transfer_cm7_CAN_OSP_TX_RX\utilities" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_canfd_interrupt_transfer_cm7_CAN_OSP_TX_RX\drivers" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_canfd_interrupt_transfer_cm7_CAN_OSP_TX_RX\device" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_canfd_interrupt_transfer_cm7_CAN_OSP_TX_RX\component\uart" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_canfd_interrupt_transfer_cm7_CAN_OSP_TX_RX\component\lists" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_canfd_interrupt_transfer_cm7_CAN_OSP_TX_RX\startup" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_canfd_interrupt_transfer_cm7_CAN_OSP_TX_RX\xip" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_canfd_interrupt_transfer_cm7_CAN_OSP_TX_RX\CMSIS" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_canfd_interrupt_transfer_cm7_CAN_OSP_TX_RX\drivers\freertos" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_canfd_interrupt_transfer_cm7_CAN_OSP_TX_RX\freertos\freertos-kernel\include" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_canfd_interrupt_transfer_cm7_CAN_OSP_TX_RX\freertos\freertos-kernel\portable\GCC\ARM_CM4F" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_canfd_interrupt_transfer_cm7_CAN_OSP_TX_RX\source" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_canfd_interrupt_transfer_cm7_CAN_OSP_TX_RX\osp_aospi\aocmd" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_canfd_interrupt_transfer_cm7_CAN_OSP_TX_RX\osp_aospi\aomw" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_canfd_interrupt_transfer_cm7_CAN_OSP_TX_RX\osp_aospi\aoosp" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_canfd_interrupt_transfer_cm7_CAN_OSP_TX_RX\osp_aospi\aoresult" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_canfd_interrupt_transfer_cm7_CAN_OSP_TX_RX\osp_aospi\aospi" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_canfd_interrupt_transfer_cm7_CAN_OSP_TX_RX\board" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_canfd_interrupt_transfer_cm7_CAN_OSP_TX_RX\evkmimxrt1170\driver_examples\canfd\interrupt_transfer\cm7" -O0 -fno-common -g3 -gdwarf-4 -c -ffunction-sections -fdata-sections -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m7 -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-device

clean-device:
	-$(RM) ./device/system_MIMXRT1176_cm7.d ./device/system_MIMXRT1176_cm7.o

.PHONY: clean-device

