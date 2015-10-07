#!/bin/bash

export PATH=$PATH:/home/txino/Zauber/zynthian/rbpi_kernel/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin

# For RBPI2
cd linux
KERNEL=kernel7

# Default Config for RBPI2
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- bcm2709_defconfig

# Menu Config
#sudo apt-get install libncurses5-dev
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- menuconfig

# Compile:
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- zImage modules dtbs

# Mount SD
$ mkdir mnt/fat32
$ mkdir mnt/ext4
$ sudo mount /dev/sdj1 mnt/fat32
$ sudo mount /dev/sdj2 mnt/ext4

# Install Modules in SD
sudo make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- INSTALL_MOD_PATH=mnt/ext4 modules_install

# Copy Kernel and Files ...
sudo cp mnt/fat32/$KERNEL.img mnt/fat32/$KERNEL-backup.img
sudo scripts/mkknlimg arch/arm/boot/zImage mnt/fat32/$KERNEL.img
sudo cp arch/arm/boot/dts/*.dtb mnt/fat32/
sudo cp arch/arm/boot/dts/overlays/*.dtb* mnt/fat32/overlays/
sudo cp arch/arm/boot/dts/overlays/README mnt/fat32/overlays/
sudo umount mnt/fat32
sudo umount mnt/ext4

# Another option is to copy the kernel into the same place, but with a different filename - for instance, 
# kernel-myconfig.img - rather than overwriting the kernel.img file. You can then edit the config.txt 
# file to select the kernel that the Pi will boot into:
# kernel=kernel-myconfig.img