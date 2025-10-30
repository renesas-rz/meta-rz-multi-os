# meta-rz-multi-os

This Multi-OS feature package provides Linux BSP Plus support for Renesas 64-bit Arm-based RZ/G series MPUs. The following boards and MPUs are currently supported:

- Board: RZ/G2L SMARC Evaluation Kit / MPU: R9A07G044L (RZ/G2L)
- Board: RZG2LC SMARC Evaluation Kit / MPU: R9A07G044C (RZ/G2LC)
- Board: RZG2UL SMARC Evaluation Kit / MPU: R9A07G043U (RZ/G2UL)
- Board: RZG3S SMARC Evaluation Kit / MPU: R9A08G045 (RZ/G3S)

More details about Linux BSP Plus at: [Renesas RZ Linux BSP Plus](https://renesas-rz.github.io/rz_linux_bsp_plus/)

## Build Instructions

Assume that `$WORK` is the current working directory.

Please refer to [How To Build Linux BSP Plus](https://renesas-rz.github.io/rz_linux_bsp_plus/how_to_build_linux_bsp_plus/) for the details of building Linux BSP Plus source code.

To apply Multi-OS feature package, follow the steps below.
   1. Copy the contents of this repository to `$WORK/meta-rz-features`
   2. Run the following commands to add the Multi-OS layer
      ```
      $ cd ${WORK}/build
      $ bitbake-layers add-layer ../meta-rz-features/meta-rz-multi-os/meta-<platform>
      ```
   **Note:** The `<platform>` can be selected from the table below:

   | Renesas MPU  | platform |
   |:------------:|:--------:|
   | RZ/G2L       | rzg2l    |
   | RZ/G2LC      | rzg2l    |
   | RZ/G2UL      | rzg2l    |
   | RZ/G3S       | rzg3s    |

   3. (Optional) Uncomment the following line in `meta-rz-features/meta-rz-multi-os/meta-rzg3s/conf/layer.conf` to enable the CM33 cold boot support for RZ/G3S device.
      ```
      #MACHINE_FEATURES:append = " RZG3S_CM33_BOOT"
      ```
   4. (Optional) Uncomment the following line in `meta-rz-features/meta-rz-multi-os/meta-rzg3s/conf/layer.conf` to enable the AWO example support for RZ/G3S device.
      ```
      #MACHINE_FEATURES:append = " RZG3S_AWO_SUPPORT"
      ```
   5. (Optional) Uncomment the following line in `meta-rz-features/meta-rz-multi-os/meta-<platform>/conf/layer.conf` to enable RemoteProc support for the CM33 core.
      ```
      #MACHINE_FEATURES:append = " CM33_REMOTEPROC"
      ```
   6. (Optional) Update `TFA_BOARD` definition to include the variable `PLAT_M33_BOOT_SUPPORT=1` in `meta-rz-features/meta-rz-multi-os/meta-rzg3s/recipes-bsp/trusted-firmware-a/trusted-firmware-a_2.10.bbappend`to enable CM33 boot support in BL2 of Trusted Firmware-A as shown below. This will allow BL2 to start the CM33 core during startup.
      ```
      TFA_BOARD = "${@'smarc PLAT_SYSTEM_SUSPEND=awo PLAT_M33_BOOT_SUPPORT=1' if d.getVar('ENABLE_RZG3S_AWO_SUPPORT') == '1' else 'smarc PLAT_SYSTEM_SUSPEND=vbat PLAT_M33_BOOT_SUPPORT=1'}"
      ```
## Cortex-M33 applications

To work with the **meta-rz-multi-os** layer, use the example projects provided by Renesas in the following GitHub repositories:
   - Please refer to [rz-rpmsg-examples](https://github.com/renesas-rz/rz-rpmsg-examples) for details of the RPMsg example projects.

   - Please refer to [rz-awo-examples](https://github.com/renesas-rz/rz-awo-examples) for details of the AWO example projects.