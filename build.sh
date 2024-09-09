export ARCH=arm64
export CROSS_COMPILE=aarch64-linux-gnu-
make lightbox_defconfig
make -j8 Image scripts_gdb
