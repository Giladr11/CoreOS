# GENERAL_DIRS:
BUILD_DIR = build
SRC_DIR = src
BOOT_DIR = boot
MBR_DIR = mbr
STAGE2_DIR = stage2
KERNEL_DIR = kernel
INCLUDE_DIR = include
CRC-32_DIR = crc-32
INIT_DIR = init

# Compilers:
## Boot
NASM = nasm
NASM_FLAGS_BIN = -f bin
NASM_FLAGS_OBJ = -f obj

## Kernel
I686_GCC = i686-elf-gcc 
I686_LD = i686-elf-ld 
GCC_FLAGS =  -I/mnt/usb/CoreOS/src/kernel -m32 -ffreestanding -nostdlib -g -c

# SRC Files:
# Boot Source File
PRINT16_SRC = $(SRC_DIR)/$(BOOT_DIR)/print16.asm

##BOOTSEC
BOOTSEC_SRC = $(SRC_DIR)/$(BOOT_DIR)/$(MBR_DIR)/bootsec.asm

##STAGE2
LOADER_SRC = $(SRC_DIR)/$(BOOT_DIR)/$(STAGE2_DIR)/loader.asm

###INCLUDE
CRC32_SRC = $(SRC_DIR)/$(BOOT_DIR)/$(STAGE2_DIR)/$(INCLUDE_DIR)/crc32.asm
PREBOOT_CRC32_SRC = $(SRC_DIR)/$(BOOT_DIR)/$(STAGE2_DIR)/$(INCLUDE_DIR)/preboot_crc32_calc.asm
GDT_SRC = $(SRC_DIR)/$(BOOT_DIR)/$(STAGE2_DIR)/$(INCLUDE_DIR)/gdt.asm
A20_SRC = $(SRC_DIR)/$(BOOT_DIR)/$(STAGE2_DIR)/$(INCLUDE_DIR)/a20.asm
INITPM_SRC = $(SRC_DIR)/$(BOOT_DIR)/$(STAGE2_DIR)/$(INCLUDE_DIR)/initpm.asm
CHECKSUMS_VERIFIER_SRC = $(SRC_DIR)/$(BOOT_DIR)/$(STAGE2_DIR)/$(INCLUDE_DIR)/crc32_verifier.asm

## Kernel Source Files:
INIT_ASM_SRC = $(SRC_DIR)/$(KERNEL_DIR)/$(INIT_DIR)/init.asm
SCRIPT_LINKER = $(SRC_DIR)/$(KERNEL_DIR)/$(INIT_DIR)/linker.ld
INIT_C_SRC = $(SRC_DIR)/$(KERNEL_DIR)/$(INIT_DIR)/init.c
SOURCES_C = $(shell find $(SRC_DIR)/$(KERNEL_DIR) -type f -name "*.c")
SOURCES_CPP = $(shell find $(SRC_DIR)/$(KERNEL_DIR) -type f -name "*.cpp")
SOURCES_ASM = $(shell find $(SRC_DIR)/$(KERNEL_DIR) -type f -name "*.asm")
KERNEL_INCLUDES = $(shell find $(SRC_DIR)/$(KERNEL_DIR)/$(INCLUDE_DIR) -type f -name "*.h")

# Disk Image Commands:
SECTOR_SIZE = 512  	 		# bytes
STAGE2_SEEK = 1		 		# starts at sector 2
CONV = notrunc		 		# wont truncate the files
COUNT = 1            		# 1 block of 512 bytes

# Checksum Disk Image Commands:
CHECKSUM_SEEK = 506  		# right before the boot signature
CHECKSUM_SECTOR_SIZE = 1    # 1 byte
CHECKSUM_COUNT = 4          # 4 blocks of 1 byte

# BUILD Files:
# Boot Build Files 
##BOOTSEC
BOOTSEC_BIN = $(BUILD_DIR)/$(BOOT_DIR)/$(MBR_DIR)/bootsec.bin

##STAGE2
LOADER_BIN = $(BUILD_DIR)/$(BOOT_DIR)/$(STAGE2_DIR)/loader.bin
PREBOOT_CRC32_OBJ = $(BUILD_DIR)/$(BOOT_DIR)/$(STAGE2_DIR)/$(INCLUDE_DIR)/preboot_crc32_calc.o
PREBOOT_CRC32_ELF = $(BUILD_DIR)/$(BOOT_DIR)/$(STAGE2_DIR)/$(INCLUDE_DIR)/preboot_crc32_calc.elf
PREBOOT_CRC32_RESULT = $(BUILD_DIR)/$(BOOT_DIR)/$(STAGE2_DIR)/$(CRC-32_DIR)/preboot_crc32_result.bin

## Kernel OBJ Files
KERNEL_BIN = $(BUILD_DIR)/$(KERNEL_DIR)/kernel.bin
OBJECTS_C = $(patsubst $(SRC_DIR)/$(KERNEL_DIR)/%.c, $(BUILD_DIR)/$(KERNEL_DIR)/%.o, $(SOURCES_C))
OBJECTS_CPP = $(patsubst $(SRC_DIR)/$(KERNEL_DIR)/%.cpp, $(BUILD_DIR)/$(KERNEL_DIR)/%.o, $(SOURCES_CPP))
OBJECTS_ASM = $(patsubst $(SRC_DIR)/$(KERNEL_DIR)/%.asm, $(BUILD_DIR)/$(KERNEL_DIR)/%.o, $(SOURCES_ASM))

## Final Disk Image
DISK_IMG = $(BUILD_DIR)/disk.img

# Clean Build Directory:
RM_CMD = rm -f
CLEAN_BUILD1 = *.bin
CLEAN_BUILD2 = *.img
CLEAN_BUILD3 = *.o

# Build disk.img
all: $(DISK_IMG)

#DISK=COMPILATION===============================================================

$(DISK_IMG): $(BOOTSEC_BIN) $(LOADER_BIN) $(KERNEL_BIN) $(PREBOOT_CRC32_EXE) $(KERNEL_INCLUDES)
	@echo "Building $(DISK_IMG)..."
	dd if=$(PREBOOT_CRC32_RESULT) of=$(BOOTSEC_BIN) bs=$(CHECKSUM_SECTOR_SIZE) seek=$(CHECKSUM_SEEK) count=$(CHECKSUM_COUNT) conv=$(CONV)

	dd if=$(BOOTSEC_BIN) of=$(DISK_IMG) bs=$(SECTOR_SIZE) count=$(COUNT) conv=$(CONV)

	$(eval LOADER_SIZE := $(shell stat -c%s $(LOADER_BIN)))
	$(eval LOADER_COUNT := $(shell echo $$((($(LOADER_SIZE) + $(SECTOR_SIZE) - 1) / $(SECTOR_SIZE)))))
	dd if=$(LOADER_BIN) of=$(DISK_IMG) bs=$(SECTOR_SIZE) count=$(LOADER_COUNT) seek=$(STAGE2_SEEK) conv=$(CONV)

	$(eval KERNEL_SEEK := $(shell echo $$(((`stat -c%s $(LOADER_BIN)` + $(SECTOR_SIZE) - 1) / $(SECTOR_SIZE) + 1))))
	$(eval KERNEL_SIZE := $(shell stat -c%s $(KERNEL_BIN)))
	$(eval KERNEL_COUNT := $(shell echo $$((($(KERNEL_SIZE) + $(SECTOR_SIZE) - 1) / $(SECTOR_SIZE)))))
	dd if=$(KERNEL_BIN) of=$(DISK_IMG) bs=$(SECTOR_SIZE) count=$(KERNEL_COUNT) seek=$(KERNEL_SEEK) conv=$(CONV)

#BOOT=COMPILATION===============================================================

# Compile bootsec.bin
$(BOOTSEC_BIN): $(BOOTSEC_SRC) $(PRINT16_SRC)
	@echo "Compiling $(BOOTSEC_BIN)..."
	$(NASM) $(NASM_FLAGS_BIN) $(BOOTSEC_SRC) -o $(BOOTSEC_BIN)

# Compile loader.bin
$(LOADER_BIN): $(LOADER_SRC) $(GDT_SRC) $(A20_SRC) $(INITPM_SRC) $(CRC32_SRC) $(CHECKSUMS_VERIFIER_SRC) $(PRINT16_SRC)
	@echo "Compiling $(LOADER_BIN)..."
	$(NASM) $(NASM_FLAGS_BIN) $(LOADER_SRC) -o $(LOADER_BIN)

# Compiling kernel_crc32_calc.elf
$(PREBOOT_CRC32_ELF): $(PREBOOT_CRC32_OBJ)
	ld -m elf_i386 -o $(PREBOOT_CRC32_ELF) $(PREBOOT_CRC32_OBJ)

# Compiling kernel_crc32_calc.o
$(PREBOOT_CRC32_OBJ): $(PREBOOT_CRC32_SRC)
	$(NASM) -f elf32 $(PREBOOT_CRC32_SRC) -o $(PREBOOT_CRC32_OBJ)


#KERNEL=COMPILATION=============================================================

# Compiling Final kernel.bin
$(KERNEL_BIN): $(SCRIPT_LINKER) $(OBJECTS_C) $(OBJECTS_ASM) $(OBJECTS_CPP)
	@echo "Compiling $(KERNEL_BIN)..."
	$(I686_LD) -T $(SCRIPT_LINKER) -o $(KERNEL_BIN) $(OBJECTS_ASM) $(OBJECTS_C) $(OBJECTS_CPP)

$(BUILD_DIR)/$(KERNEL_DIR)/%.o: $(SRC_DIR)/$(KERNEL_DIR)/%.c
	@echo "Compiling kernel C files..."
	$(I686_GCC) $(GCC_FLAGS) -c $< -o $@

$(BUILD_DIR)/$(KERNEL_DIR)/%.o: $(SRC_DIR)/$(KERNEL_DIR)/%.asm
	@echo "Compiling kernel ASM files..."
	$(NASM) -f elf32 $< -o $@

$(BUILD_DIR)/$(KERNEL_DIR)/%.o: $(SRC_DIR)/$(KERNEL_DIR)/%.cpp
	@echo "Compiling kernel CPP files..."
	$(I686_GCC) $(GCC_FLAGS) -c $< -o $@	

#CLEAN==========================================================================

# Clean Build Directory
clean:
	@echo "Cleaning '$(BUILD_DIR)' Directory..."
	find $(BUILD_DIR) -type f ! -name '.gitkeep' -delete