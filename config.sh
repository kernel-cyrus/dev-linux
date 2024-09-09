export ARCH=arm64
export CROSS_COMPILE=aarch64-linux-gnu-
make lightbox_defconfig
make menuconfig
make savedefconfig
mv defconfig arch/arm64/configs/lightbox_defconfig
