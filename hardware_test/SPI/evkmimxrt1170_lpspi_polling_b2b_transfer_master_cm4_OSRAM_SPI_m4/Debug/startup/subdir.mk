################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../startup/boot_multicore_slave.c \
../startup/startup_mimxrt1176_cm4.c 

C_DEPS += \
./startup/boot_multicore_slave.d \
./startup/startup_mimxrt1176_cm4.d 

OBJS += \
./startup/boot_multicore_slave.o \
./startup/startup_mimxrt1176_cm4.o 


# Each subdirectory must supply rules for building sources it contributes
startup/%.o: ../startup/%.c startup/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_MIMXRT1176DVMAA -DCPU_MIMXRT1176DVMAA_cm4 -DSDK_DEBUGCONSOLE=1 -DXIP_BOOT_HEADER_ENABLE=0 -DMCUXPRESSO_SDK -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_lpspi_polling_b2b_transfer_master_cm4_OSRAM_SPI_m4\source" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_lpspi_polling_b2b_transfer_master_cm4_OSRAM_SPI_m4\utilities" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_lpspi_polling_b2b_transfer_master_cm4_OSRAM_SPI_m4\drivers" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_lpspi_polling_b2b_transfer_master_cm4_OSRAM_SPI_m4\device" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_lpspi_polling_b2b_transfer_master_cm4_OSRAM_SPI_m4\component\uart" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_lpspi_polling_b2b_transfer_master_cm4_OSRAM_SPI_m4\component\lists" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_lpspi_polling_b2b_transfer_master_cm4_OSRAM_SPI_m4\startup" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_lpspi_polling_b2b_transfer_master_cm4_OSRAM_SPI_m4\xip" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_lpspi_polling_b2b_transfer_master_cm4_OSRAM_SPI_m4\CMSIS" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_lpspi_polling_b2b_transfer_master_cm4_OSRAM_SPI_m4\board" -I"C:\Users\admin\Documents\MCUXpressoIDE_25.6.136\workspace\evkmimxrt1170_lpspi_polling_b2b_transfer_master_cm4_OSRAM_SPI_m4\evkmimxrt1170\driver_examples\lpspi\polling_b2b_transfer\master\cm4" -O0 -fno-common -g3 -gdwarf-4 -c -ffunction-sections -fdata-sections -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-startup

clean-startup:
	-$(RM) ./startup/boot_multicore_slave.d ./startup/boot_multicore_slave.o ./startup/startup_mimxrt1176_cm4.d ./startup/startup_mimxrt1176_cm4.o

.PHONY: clean-startup

