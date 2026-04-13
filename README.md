# meta-rz-multi-os

This Multi-OS feature package provides Linux BSP Plus support for the RZ Group of 64bit Arm-based MPUs from Renesas Electronics. The following boards and MPUs are currently supported:

- Board: RZ/G2L SMARC Evaluation Board Kit / MPU: R9A07G044L (RZ/G2L)
- Board: RZ/G2LC SMARC Evaluation Board Kit / MPU: R9A07G044C (RZ/G2LC)
- Board: RZ/G2UL SMARC Evaluation Board Kit / MPU: R9A07G043U (RZ/G2UL)
- Board: RZ/G3S SMARC Evaluation Board Kit / MPU: R9A08G045 (RZ/G3S)
- Board: RZ/T2H Evaluation Board Kit / MPU: R9A09G077M44 (RZ/T2H)
- Board: RZ/N2H Evaluation Board Kit / MPU: R9A09G087M44 (RZ/N2H)

More details about Linux BSP Plus at: [Renesas RZ Linux BSP Plus](https://renesas-rz.github.io/rz_linux_bsp_plus/)

## Build Instructions

Assume that `$WORK` is the current working directory.

Please refer to [How To Build Linux BSP Plus](https://renesas-rz.github.io/rz_linux_bsp_plus/) for the details of building Linux BSP Plus source code.
To support RZ Linux BSP Plus for Kernel 6.12 v3.0, please check out BSP-PLUS-K6.12-V3.0 as following:
```bash
    $ cd $WORK # Ensure you are in your working directory
    $ git clone https://github.com/renesas-rz/meta-renesas
    $ cd meta-renesas
    $ git checkout -b tmp BSP-PLUS-K6.12-V3.0
    $ cd ../
```
(Optional) Regarding Graphics and Video Codecs packages, please download from [Renesas.com](https://www.renesas.com/) website.
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
   | RZ/T2H       | rzt2h    |
   | RZ/N2H       | rzn2h    |

   3. (Optional) Uncomment the following line in `meta-rz-features/meta-rz-multi-os/meta-rzg3s/conf/layer.conf` to enable the CM33 cold boot support for the RZ/G3S device:
      ```
      #MACHINE_FEATURES:append = " RZG3S_CM33_BOOT"
      ```
   4. (Optional) Uncomment the following line in `meta-rz-features/meta-rz-multi-os/meta-rzg3s/conf/layer.conf` to enable the AWO example support for the RZ/G3S device:
      ```
      #MACHINE_FEATURES:append = " RZG3S_AWO_SUPPORT"
      ```
   5. (Optional) RemoteProc support can be enabled by modifying the following lines in `meta-rz-features/meta-rz-multi-os/meta-<platform>/conf/layer.conf`:

      For the CM33 core on the RZ/G series, uncomment the following line:
      ```
      #MACHINE_FEATURES:append = " RZG3S_CM33_REMOTEPROC"
      ```
      For the CR52 core on the RZ/T-N series, set the value to "1":
      ```
      ENABLE_REMOTEPROC ?= "0"
      ```
   6. (Optional) Firmware loading support in BL2 of Trusted Firmware-A can be enabled by modifying the following lines in `meta-rz-features/meta-rz-multi-os/meta-<platform>/conf/layer.conf`:

      For CM33 firmware on the RZ/G series, uncomment the following line:
      ```
      #MACHINE_FEATURES:append = " RZG3S_CM33_FIRMWARE_LOAD"
      ```
      For CR52/CA55 firmware on the RZ/T-N series, uncomment the following line:
      ```
      #MACHINE_FEATURES_append = " <platform>_CA55_CR52_BL2_BOOT"
      ```   
      Then, select one of the following target cores by uncommenting the corresponding line:	  
      ```
      #MACHINE_FEATURES:append = " <platform>_CA551_BL2_BOOT"
      #MACHINE_FEATURES:append = " <platform>_CA552_BL2_BOOT"
      #MACHINE_FEATURES:append = " <platform>_CA553_BL2_BOOT"
      #MACHINE_FEATURES:append = " <platform>_CR520_BL2_BOOT"
      #MACHINE_FEATURES:append = " <platform>_CR521_BL2_BOOT"
      ```	  
   7. (Optional) Set "1" to the following line in `meta-rz-features/meta-rz-multi-os/meta-<platform>/conf/layer.conf` to enable CR52 firmware loading support in U-boot on RZ/T-N series:
      ```
      ENABLE_U_BOOT ?= "0"
      ```
   8. (Optional) Set “1” to the following line in `meta-rz-features/meta-rz-multi-os/meta-<platform>/conf/layer.conf` if the remote core (FSP side) uses the CA55 core on the RZ/T‑N series:
      ```
      RPMSG_REMOTE_CORE ?= "0"
      ```

## Cortex-M33, Cortex-R52, Cortex-A55 applications

To work with the **meta-rz-multi-os** layer, use the example projects provided by Renesas in the following GitHub repositories:
   - Please refer to [rz-rpmsg-examples](https://github.com/renesas-rz/rz-rpmsg-examples) for details of the RPMsg example projects.

   - Please refer to [rz-awo-examples](https://github.com/renesas-rz/rz-awo-examples) for details of the AWO example projects.
