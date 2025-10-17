################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../utilities/fsl_assert.c \
../utilities/fsl_debug_console.c \
../utilities/fsl_str.c 

S_UPPER_SRCS += \
../utilities/fsl_memcpy.S 

C_DEPS += \
./utilities/fsl_assert.d \
./utilities/fsl_debug_console.d \
./utilities/fsl_str.d 

OBJS += \
./utilities/fsl_assert.o \
./utilities/fsl_debug_console.o \
./utilities/fsl_memcpy.o \
./utilities/fsl_str.o 


# Each subdirectory must supply rules for building sources it contributes
utilities/%.o: ../utilities/%.c utilities/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_MIMXRT1176DVMAA -DCPU_MIMXRT1176DVMAA_cm4 -DSDK_DEBUGCONSOLE=1 -DXIP_BOOT_HEADER_ENABLE=0 -DFLEXCAN_WAIT_TIMEOUT=1000 -DMCUXPRESSO_SDK -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_canfd_interrupt_transfer_cm4_can_3\source" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_canfd_interrupt_transfer_cm4_can_3\utilities" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_canfd_interrupt_transfer_cm4_can_3\drivers" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_canfd_interrupt_transfer_cm4_can_3\device" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_canfd_interrupt_transfer_cm4_can_3\component\uart" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_canfd_interrupt_transfer_cm4_can_3\component\lists" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_canfd_interrupt_transfer_cm4_can_3\startup" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_canfd_interrupt_transfer_cm4_can_3\xip" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_canfd_interrupt_transfer_cm4_can_3\CMSIS" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_canfd_interrupt_transfer_cm4_can_3\board" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_canfd_interrupt_transfer_cm4_can_3\evkmimxrt1170\driver_examples\canfd\interrupt_transfer\cm4" -O0 -fno-common -g3 -gdwarf-4 -c -ffunction-sections -fdata-sections -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

utilities/%.o: ../utilities/%.S utilities/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU Assembler'
	arm-none-eabi-gcc -c -x assembler-with-cpp -D__REDLIB__ -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_canfd_interrupt_transfer_cm4_can_3\source" -g3 -gdwarf-4 -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -specs=redlib.specs -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-utilities

clean-utilities:
	-$(RM) ./utilities/fsl_assert.d ./utilities/fsl_assert.o ./utilities/fsl_debug_console.d ./utilities/fsl_debug_console.o ./utilities/fsl_memcpy.o ./utilities/fsl_str.d ./utilities/fsl_str.o

.PHONY: clean-utilities

