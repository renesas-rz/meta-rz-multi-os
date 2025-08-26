# meta-rz-multi-os

This is a Multi-OS feature that provides Linux BSP Plus support for the RZ/G2L of 64bit Arm-based MPU from Renesas Electronics. Currently the following boards and MPUs are supported:

- Board: RZ/G2L SMARC Evaluation Kit / MPU: R9A07G044L (RZ/G2L)

More details about Linux BSP Plus at: https://renesas-rz.github.io/rz_linux_bsp_plus/

## Build Instructions

Assume that $WORK is the current working directory.

First, refer following page for build procedure: https://renesas-rz.github.io/rz_linux_bsp_plus/how_to_build_linux_bsp_plus/

Copy the contents of this repository to $WORK/meta-rz-features/

- To apply Multi-OS feature, please run the command below:
```bash
   $ bitbake-layers add-layer ../meta-rz-features/meta-rz-multi-os/meta-<platform>
```
\<platform\> can be selected in below table:

|Renesas MPU| platform |
|:---------:|:--------:|
|RZ/G2L     |rzg2l     |

- To enable remoteproc support, apply the modification stated below to _$WORK/meta-rz-features/meta-rz-multi-os/meta-rzg2l/recipes-kernel/linux/linux-renesas_6.12.bbappend_
```
   ENABLE_REMOTEPROC = "1"
```

## Cortex-M33 applications

To work with meta-rz-multi-os layer, you need to use the example projects from the **rz-rpmsg-examples** repository on github. About Cortex-M33 example program setup and the procedure to carry out Inter-Processor Communication, you can refer in detail at: https://github.com/renesas-rz/rz-rpmsg-examples