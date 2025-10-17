################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/flexio_uart_interrupt_transfer.c \
../source/semihost_hardfault.c 

C_DEPS += \
./source/flexio_uart_interrupt_transfer.d \
./source/semihost_hardfault.d 

OBJS += \
./source/flexio_uart_interrupt_transfer.o \
./source/semihost_hardfault.o 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c source/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_MIMXRT1176DVMAA -DCPU_MIMXRT1176DVMAA_cm4 -DSDK_DEBUGCONSOLE=1 -DXIP_BOOT_HEADER_ENABLE=0 -DMCUXPRESSO_SDK -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_flexio_uart_interrupt_transfer_cm4_LIN_RX_Flex\source" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_flexio_uart_interrupt_transfer_cm4_LIN_RX_Flex\utilities" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_flexio_uart_interrupt_transfer_cm4_LIN_RX_Flex\drivers" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_flexio_uart_interrupt_transfer_cm4_LIN_RX_Flex\device" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_flexio_uart_interrupt_transfer_cm4_LIN_RX_Flex\component\uart" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_flexio_uart_interrupt_transfer_cm4_LIN_RX_Flex\component\lists" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_flexio_uart_interrupt_transfer_cm4_LIN_RX_Flex\startup" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_flexio_uart_interrupt_transfer_cm4_LIN_RX_Flex\xip" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_flexio_uart_interrupt_transfer_cm4_LIN_RX_Flex\CMSIS" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_flexio_uart_interrupt_transfer_cm4_LIN_RX_Flex\board" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_flexio_uart_interrupt_transfer_cm4_LIN_RX_Flex\evkmimxrt1170\driver_examples\flexio\uart\interrupt_transfer\cm4" -O0 -fno-common -g3 -gdwarf-4 -c -ffunction-sections -fdata-sections -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-source

clean-source:
	-$(RM) ./source/flexio_uart_interrupt_transfer.d ./source/flexio_uart_interrupt_transfer.o ./source/semihost_hardfault.d ./source/semihost_hardfault.o

.PHONY: clean-source

