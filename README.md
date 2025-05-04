# GPU Detection Kernel Module

This project implements a GPU detection module for Linux kernel. It detects and logs the available NVIDIA, AMD, and Intel Arc GPUs in the system.

### Features

- Detects GPUs from major vendors like NVIDIA, AMD, and Intel Arc.

- Displays the vendor and model information for each detected GPU.

- Logs the GPU information to the kernel logs (dmesg).

- Supports both desktop and mobile GPUs.

### How It Works

This kernel module iterates over all PCI devices in the system and checks if any device matches the vendor IDs for NVIDIA, AMD, or Intel Arc. Once a GPU is found, its model name is determined based on its device ID and logged with the printk function.

For Intel Arc GPUs, device IDs for various models are checked, such as Intel Arc A380, A580, A770, and others.

### Requirements

- Linux kernel development environment.

- GCC and Make utilities installed.

### Compilation

This project uses a Makefile for compilation. Follow these steps to compile and build the kernel module:

1. Clone the repository (if applicable) or navigate to the project directory.

2. Compile the module using make:

```bash

make
```

This will create a .ko (kernel object) file in the directory.

3. The compiled module will be named gpu_detect.ko.

### Kernel Module Commands

#### Insert the Module (insmod)

To load the kernel module into the system, use the following command:

```bash
sudo insmod gpu_detect.ko

```

This will insert the module into the kernel and begin the detection process. You can check the kernel log to see the detected GPUs.

#### Remove the Module (rmmod)

To remove the kernel module, use the following command:

```bash
sudo rmmod gpu_detect

```

This will unload the kernel module from the system.

#### Check Kernel Logs (dmesg)

Once the module is loaded, the detected GPU(s) will be logged. To view the log messages, use dmesg:

```bash
dmesg | grep GPU

```

### Example Output

After running the module, the kernel log might show:

```bash
[28055.325729] Found INTEL ARC GPU: Intel Arc A380 (Device ID: 0x56C0)
[28055.325737] Found NVIDIA GPU: NVIDIA GTX 1650 (Device ID: 0x1f82)
[28055.325766] Found AMD GPU: AMD Radeon RX 6800 (Device ID: 0x731f)
```

### Troubleshooting

- If you see the message No GPU found or no GPU plugged in., ensure that your GPU is correctly installed and recognized by the system.

- If your Intel Arc GPU is not being recognized, update the switch statement with the correct device IDs as per your GPU model.

- You can view detailed logs using the dmesg command to troubleshoot any issues related to the module.

### License

This project is licensed under the GPL License.
