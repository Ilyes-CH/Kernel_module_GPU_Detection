
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/pci.h>

#define NVIDIA_VENDOR_ID 0x10de // NVIDIA vendor ID
#define AMD_VENDOR_ID 0x1002    // Amd vendor ID
#define ARC_VENDOR_ID 0x8086    // Intel Arc vendor ID

// function prototypess
const char *get_amd_gpu_model_name(unsigned int device_id);
const char *get_intel_arc_gpu_model_name(unsigned int device_id);
const char *get_gpu_model_name(unsigned int device_id);

// function to match a device id with the model name for AMD
const char *get_amd_gpu_model_name(unsigned int device_id) {
  switch (device_id) {
  // RX 500 Series
  case 0x67DF:
    return "AMD RX 570 / RX 580 / RX 590";
  case 0x67FF:
    return "AMD RX 560";
  case 0x67E0:
    return "AMD RX 550";

  // RX 6000 Series (RDNA 2)
  case 0x73BF:
    return "AMD RX 6800";
  case 0x73E0:
    return "AMD RX 6800 XT";
  case 0x73F0:
    return "AMD RX 6900 XT";
  case 0x73C0:
    return "AMD RX 6700 XT";
  case 0x73A0:
    return "AMD RX 6600 XT";
  case 0x73B0:
    return "AMD RX 6600";
  case 0x73D0:
    return "AMD RX 6500 XT / Pro W5700";
  case 0x73E5:
    return "AMD RX 6400";

  // RX 5000 Series (RDNA)
  case 0x731F:
    return "AMD RX 5700 XT";
  case 0x731E:
    return "AMD RX 5700";
  case 0x7311:
    return "AMD RX 5600 XT";
  case 0x7310:
    return "AMD RX 5500 XT";

  // Vega Series
  case 0x6860:
    return "AMD Vega 56 / RX Vega 56";
  case 0x687F:
    return "AMD Vega 64 / RX Vega 64";
  case 0x66AF:
    return "AMD Vega Frontier Edition";

  // Radeon Pro Series
  case 0x67A0:
    return "Radeon Pro WX 7100";
  case 0x66B8:
    return "Radeon Pro WX 8200";

  default:
    return "Unknown AMD GPU";
  }
}

const char *get_intel_arc_gpu_model_name(unsigned int device_id) {
  switch (device_id) {
  // Intel Arc 3 Series
  case 0x56C0:
    return "Intel Arc A380";
  case 0x56C1:
    return "Intel Arc A350M";
  case 0x56C2:
    return "Intel Arc A370M";

  // Intel Arc 5 Series
  case 0x56C3:
    return "Intel Arc A580";
  case 0x56C4:
    return "Intel Arc A500M";

  // Intel Arc 7 Series
  case 0x56A0:
    return "Intel Arc A770";
  case 0x56A1:
    return "Intel Arc A750";

  // Intel Arc Pro Series
  case 0x56A2:
    return "Intel Arc Pro A40";
  case 0x56A3:
    return "Intel Arc Pro A50";
  case 0x56A4:
    return "Intel Arc Pro A60";

  // Intel Arc Mobile GPUs
  case 0x56C5:
    return "Intel Arc A550M";
  case 0x56C6:
    return "Intel Arc A730M";
  case 0x56C7:
    return "Intel Arc A770M";

  default:
    return "Unknown Intel Arc GPU";
  }
}

// Function to match a device ID with a model name for NVIDIA
const char *get_gpu_model_name(unsigned int device_id) {
  switch (device_id) {
  case 0x1d01:
    return "NVIDIA GT 1030";
  case 0x1d02:
    return "NVIDIA GTX 1050";
  case 0x1c81:
    return "NVIDIA GTX 1050 Ti";
  case 0x1c02:
    return "NVIDIA GTX 1060";
  case 0x1b80:
    return "NVIDIA GTX 1070";
  case 0x1b81:
    return "NVIDIA GTX 1070 Ti";
  case 0x1b82:
    return "NVIDIA GTX 1080";
  case 0x1b83:
    return "NVIDIA GTX 1080 Ti";
  case 0x1f07:
    return "NVIDIA RTX 2080";
  case 0x1f08:
    return "NVIDIA RTX 2080 Ti";
  case 0x1f82:
    return "NVIDIA GTX 1650";
  case 0x1f85:
    return "NVIDIA GTX 1650 Super";
  case 0x2191:
    return "NVIDIA GTX 1660";
  case 0x2192:
    return "NVIDIA GTX 1660 Ti";
  case 0x2182:
    return "NVIDIA RTX 3060";
  case 0x2183:
    return "NVIDIA RTX 3070";
  case 0x2184:
    return "NVIDIA RTX 3080";
  case 0x2185:
    return "NVIDIA RTX 3090";
  case 0x2206:
    return "NVIDIA RTX 4080";
  case 0x2207:
    return "NVIDIA RTX 4090";
  case 0x2201:
    return "NVIDIA RTX 4060";
  case 0x2202:
    return "NVIDIA RTX 4070";
  case 0x2204:
    return "NVIDIA RTX 4070 Ti";
  default:
    return "Unknown NVIDIA GPU";
  }
}

static int __init mod_init(void) {
  struct pci_dev *pdev = NULL;
  int found_gpu = 0;
  char *vendor_name = kmalloc(20 * sizeof(char), GFP_KERNEL);

  if (vendor_name == NULL) {
    printk(KERN_ERR "Error In Memory Allocation");
    return -ENOMEM;
  }

  printk(KERN_INFO "Entering Kernel with GPU Detection\n");

  // Iterate over all PCI devices
  for_each_pci_dev(pdev) {

    // Check if this is an NVIDIA device (vendor ID: 0x10de)
    if (pdev->vendor == NVIDIA_VENDOR_ID) {
      const char *gpu_model = get_gpu_model_name(pdev->device);
      strcpy(vendor_name, "NVIDIA");
      printk(KERN_INFO "Found %s GPU: %s (Device ID: 0x%04x)\n", vendor_name,
             gpu_model, pdev->device);
      found_gpu = 1;
    } else if (pdev->vendor == AMD_VENDOR_ID) {
      const char *gpu_model = get_amd_gpu_model_name(pdev->device);
      strcpy(vendor_name, "AMD");
      printk(KERN_INFO "Found %s GPU: %s (Device ID: 0x%04x)", vendor_name,
             gpu_model, pdev->device);
      found_gpu = 1;
    } else if (pdev->vendor == ARC_VENDOR_ID) {
      const char *gpu_model = get_intel_arc_gpu_model_name(pdev->device);
      strcpy(vendor_name, "INTEL ARC");
      printk(KERN_INFO "Found %s GPU: %s (Device ID: 0x%04x)", vendor_name,
             gpu_model, pdev->device);
      found_gpu = 1;
    }
  }
  kfree(vendor_name);
  if (found_gpu == 0) {
    printk(KERN_INFO "No GPU found or no GPU plugged in.\n");
  }

  return 0;
}

static void __exit mod_exit(void) { printk(KERN_INFO "Exiting Kernel...\n"); }

module_init(mod_init);
module_exit(mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ilyes Chaabane");
MODULE_DESCRIPTION(
    "A kernel module to check and identify the NVIDIA GPU model");
