******************************************************************************
                                 Linux for Tegra
                              Building CBoot Binary
                                     README
******************************************************************************

Use these procedures to build the T186 CBoot binary using the standalone
source contained in the cboot_src.tbz2 archive.

==============================================================================
Requirements for Compiling on the Host
==============================================================================
Before you can compile on the host, the following requirements must be met:

- A recent 64-bit ARM toolchain is required. It must be in your PATH, unless
  you explicitly set it in your CROSS_COMPILE environment variable, as follows.

- The CBoot makefile uses the toolchain set in the CROSS_COMPILE environment
  variable. The variable must be set to point to the ARM 64-bit toolchain
  used by L4T, and must be in your path.

  For example:

  export CROSS_COMPILE=aarch64-unknown-linux-gnu-

  Consult the Linux for Tegra Development Guide for detailed toolchain information.

- A GNU Make tool must be available.

- Create a directory for the CBoot source to extract and build the source.
  In subsequent procedures, the source subdirectory is called "cboot".
  The resulting binaries are placed in the 'out' subdirectory, i.e. cboot/out.

=================================================================================
Building the CBoot Binary
=================================================================================
To build the CBoot binary:
1. Extract the CBoot-standalone source with the command:
   md cboot
   tar -xjf cboot_src.tbz2 -C cboot
   cd cboot

2. Export the cross compiler tools with the following enviroment variables:
   export CROSS_COMPILE=<your_64-bit_ARM_toolchain_triple>

   Where: <your_64-bit_ARM_toolchain_triple> can be: 'aarch64-linux-gnu-'

3. Build the T186 CBoot binary, lk.bin, with the command:
   make -C ./bootloader/partner/t18x/cboot PROJECT=t186 TOOLCHAIN_PREFIX="${CROSS_COMPILE}" DEBUG=2 BUILDROOT="${PWD}"/out NV_BUILD_SYSTEM_TYPE=l4t NOECHO=@

   The binary is located at: './out/build-t186/...':

   cboot/out/build-t186/lk.bin

4. Rename the binary lk.bin to cboot.bin to use with the Jetson TX2 (T186)
   Linux for Tegra Board Support Package.

   This CBoot binary replaces the one provided at: Linux_for_Tegra/bootloader/cboot.bin.
