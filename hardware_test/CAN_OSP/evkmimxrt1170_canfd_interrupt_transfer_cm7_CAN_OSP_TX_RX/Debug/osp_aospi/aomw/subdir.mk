################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../osp_aospi/aomw/aomw.c \
../osp_aospi/aomw/aomw_as5600.c \
../osp_aospi/aomw/aomw_as6212.c \
../osp_aospi/aomw/aomw_color.c \
../osp_aospi/aomw/aomw_eeprom.c \
../osp_aospi/aomw/aomw_flag.c \
../osp_aospi/aomw/aomw_iox4b4l.c \
../osp_aospi/aomw/aomw_sfh5721.c \
../osp_aospi/aomw/aomw_sseg.c \
../osp_aospi/aomw/aomw_topo.c \
../osp_aospi/aomw/aomw_tscript.c 

C_DEPS += \
./osp_aospi/aomw/aomw.d \
./osp_aospi/aomw/aomw_as5600.d \
./osp_aospi/aomw/aomw_as6212.d \
./osp_aospi/aomw/aomw_color.d \
./osp_aospi/aomw/aomw_eeprom.d \
./osp_aospi/aomw/aomw_flag.d \
./osp_aospi/aomw/aomw_iox4b4l.d \
./osp_aospi/aomw/aomw_sfh5721.d \
./osp_aospi/aomw/aomw_sseg.d \
./osp_aospi/aomw/aomw_topo.d \
./osp_aospi/aomw/aomw_tscript.d 

OBJS += \
./osp_aospi/aomw/aomw.o \
./osp_aospi/aomw/aomw_as5600.o \
./osp_aospi/aomw/aomw_as6212.o \
./osp_aospi/aomw/aomw_color.o \
./osp_aospi/aomw/aomw_eeprom.o \
./osp_aospi/aomw/aomw_flag.o \
./osp_aospi/aomw/aomw_iox4b4l.o \
./osp_aospi/aomw/aomw_sfh5721.o \
./osp_aospi/aomw/aomw_sseg.o \
./osp_aospi/aomw/aomw_topo.o \
./osp_aospi/aomw/aomw_tscript.o 


# Each subdirectory must supply rules for building sources it contributes
osp_aospi/aomw/%.o: ../osp_aospi/aomw/%.c osp_aospi/aomw/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_MIMXRT1176DVMAA -DCPU_MIMXRT1176DVMAA_cm7 -DSDK_DEBUGCONSOLE=1 -DXIP_EXTERNAL_FLASH=1 -DXIP_BOOT_HEADER_ENABLE=1 -DFLEXCAN_WAIT_TIMEOUT=1000 -DMCUXPRESSO_SDK -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -DSDK_OS_FREE_RTOS -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_canfd_interrupt_transfer_cm7_CAN_OSP_TX_RX\utilities" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_canfd_interrupt_transfer_cm7_CAN_OSP_TX_RX\drivers" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_canfd_interrupt_transfer_cm7_CAN_OSP_TX_RX\device" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_canfd_interrupt_transfer_cm7_CAN_OSP_TX_RX\component\uart" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_canfd_interrupt_transfer_cm7_CAN_OSP_TX_RX\component\lists" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_canfd_interrupt_transfer_cm7_CAN_OSP_TX_RX\startup" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_canfd_interrupt_transfer_cm7_CAN_OSP_TX_RX\xip" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_canfd_interrupt_transfer_cm7_CAN_OSP_TX_RX\CMSIS" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_canfd_interrupt_transfer_cm7_CAN_OSP_TX_RX\drivers\freertos" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_canfd_interrupt_transfer_cm7_CAN_OSP_TX_RX\freertos\freertos-kernel\include" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_canfd_interrupt_transfer_cm7_CAN_OSP_TX_RX\freertos\freertos-kernel\portable\GCC\ARM_CM4F" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_canfd_interrupt_transfer_cm7_CAN_OSP_TX_RX\source" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_canfd_interrupt_transfer_cm7_CAN_OSP_TX_RX\osp_aospi\aocmd" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_canfd_interrupt_transfer_cm7_CAN_OSP_TX_RX\osp_aospi\aomw" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_canfd_interrupt_transfer_cm7_CAN_OSP_TX_RX\osp_aospi\aoosp" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_canfd_interrupt_transfer_cm7_CAN_OSP_TX_RX\osp_aospi\aoresult" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_canfd_interrupt_transfer_cm7_CAN_OSP_TX_RX\osp_aospi\aospi" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_canfd_interrupt_transfer_cm7_CAN_OSP_TX_RX\board" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_canfd_interrupt_transfer_cm7_CAN_OSP_TX_RX\evkmimxrt1170\driver_examples\canfd\interrupt_transfer\cm7" -O0 -fno-common -g3 -gdwarf-4 -c -ffunction-sections -fdata-sections -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m7 -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-osp_aospi-2f-aomw

clean-osp_aospi-2f-aomw:
	-$(RM) ./osp_aospi/aomw/aomw.d ./osp_aospi/aomw/aomw.o ./osp_aospi/aomw/aomw_as5600.d ./osp_aospi/aomw/aomw_as5600.o ./osp_aospi/aomw/aomw_as6212.d ./osp_aospi/aomw/aomw_as6212.o ./osp_aospi/aomw/aomw_color.d ./osp_aospi/aomw/aomw_color.o ./osp_aospi/aomw/aomw_eeprom.d ./osp_aospi/aomw/aomw_eeprom.o ./osp_aospi/aomw/aomw_flag.d ./osp_aospi/aomw/aomw_flag.o ./osp_aospi/aomw/aomw_iox4b4l.d ./osp_aospi/aomw/aomw_iox4b4l.o ./osp_aospi/aomw/aomw_sfh5721.d ./osp_aospi/aomw/aomw_sfh5721.o ./osp_aospi/aomw/aomw_sseg.d ./osp_aospi/aomw/aomw_sseg.o ./osp_aospi/aomw/aomw_topo.d ./osp_aospi/aomw/aomw_topo.o ./osp_aospi/aomw/aomw_tscript.d ./osp_aospi/aomw/aomw_tscript.o

.PHONY: clean-osp_aospi-2f-aomw

