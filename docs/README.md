# 🌀 **CoreOS**

![CoreOS Screen:](/docs/CoreOS_shell.png)

---

## 📖 **Overview**

CoreOS is a custom **32-bit operating system** designed from scratch to showcase fundamental operating system concepts. It is built with **Assembly**, **C** and demonstrates low-level hardware interaction, memory management, and interrupt handling. CoreOS features a multi-stage bootloader, a robust kernel, and an efficient heap manager and hardware drivers.

---

## ⚙️ **Key Features**

### **1. Bootloader**
- **`Multi-stage Design`**: Handles the transition from real mode to protected mode.

- **`A20 Line Activation`**: Enables extended memory beyond 1MB.

- **`Global Descriptor Table (GDT)`**: Configures memory segments for protected mode.

- **`Checksum Validation`**: Ensures the kernel's integrity using CRC-32.

- **`Protected Mode Initialization`**: Prepares the system to run 32-bit code.

### **2. Kernel**
The kernel manages hardware and system resources, providing essential OS services.

- **`Heap Manager`**:
  - Handles memory allocation and deallocation dynamically.
  - Resolves memory fragmentation to ensure efficient utilization.

![Allocation Screen:](/docs/alloc.png)
![Heap Dump and Free Screen:](/docs/heap_dump&free.png)

- **`Interrupt Handling`**:
  - Set up the **IDT** that defines the address of the **ISR**s.
  - Configures the **PIC** and supports **ISRs** for managing hardware interrupts.

- **`Command-line Interface (CLI)`**:
  - An interface for interacting with the operating system.

- **`Hardware Abstraction Layer (HAL)`**:
  - Provides a clean interface for hardware-specific functionalities.

---

## 🛠️ **How to Run the Project**

### Prerequisites
- Install [QEMU](https://www.qemu.org/) (or any x86 emulator) for running the OS.
- Install **NASM** (for assembling bootloader code) and **GCC** (for compiling C/C++ kernel code).

### Instructions
1. **Clone the Repository**:
   ```bash
   git clone https://github.com/Giladr11/CoreOS.git

   cd CoreOS

2. **Set Execution Permissions (only required once)**:
    ```bash
    chmod +x build_scripts/build.sh

3.  **Build and Run**:
    ```bash
    ./build_scripts/build.sh

    qemu-system-x86_64 -drive format=raw,file=build/disk.img

---

## 🗂️ **Project Structure**
The project follows a structured folder architecture for better organization:

    CoreOS/
    ├── build/
    │   └── Contains all compiled files and the final `disk.img`.
    |
    ├── src/
    |   |
    │   ├── boot/
    │   │   ├── mbr/
    │   │   └── stage2/
    │   │       └── include/
    |   |
    │   └── kernel/
    │       ├── arch/
    │       │   └── x86/
    │       │       ├── cpu/
    │       │       ├── hal/
    │       │       ├── idt/
    │       │       ├── isr/
    │       │       └── pic/
    |       |
    │       ├── cli/
    │       ├── drivers/
    │       ├── heap/
    │       ├── include/
    │       ├── init/
    │       └── syscalls/
    |
    ├── build_scripts/
    ├── docs/
    ├── Makefile
    └── .gitignore

## 📝 Folder Breakdown

### `build/`
Contains the final OS binary (`disk.img`) and compiled bootloader/kernel files.

### `src/`

#### `boot/`:
- **`mbr/`**: Code for the first stage of the bootloader.
- **`stage2/`**:
  - **`include/`**: Header files for the second stage of the bootloader.

#### `kernel/`:
- **`arch/x86/`**: Architecture-specific code for x86 systems.
  - **`cpu/`**: CPU initialization and control.
  - **`hal/`**: Hardware abstraction layer for x86 hardware.
  - **`idt/`**: Interrupt Descriptor Table (IDT) setup.
  - **`isr/`**: Interrupt Service Routines (ISRs).
  - **`pic/`**: Programmable Interrupt Controller setup.
- **`cli/`**: Command-line interface code.
- **`drivers/`**: Drivers for hardware components, including:
  - **Screen**: Implements VGA text mode for screen output.
  - **Keyboard**: Handles keyboard input and key mappings.
- **`heap/`**: Heap memory manager for dynamic allocation and deallocation.
- **`include/`**: Kernel header files.
- **`init/`**: System initialization code.
- **`syscalls/`**: System call implementations.

---

## 📝 License

Licensed under the MIT License.

**CoreOS** is an ambitious project that demonstrates advanced low-level programming concepts and provides a foundation for learning about operating systems.
