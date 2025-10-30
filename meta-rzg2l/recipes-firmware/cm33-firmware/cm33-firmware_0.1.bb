SUMMARY = "CM33 RPMsg firmware"
LICENSE = "CLOSED" 

FIRMWARE_NAME = "*.elf" 

EXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI = " \
    file://rzg2l_cm33_rpmsg_linux-rtos_demo.elf \
    file://rzg2lc_cm33_rpmsg_linux-rtos_demo.elf \
    file://rzg2ul_cm33_rpmsg_linux-rtos_demo.elf \
" 

INSANE_SKIP:${PN} = "arch" 

do_install() {
    install -d ${D}/usr/lib/firmware
    install -m 0644 ${WORKDIR}/${FIRMWARE_NAME} ${D}/usr/lib/firmware/
}

FILES:${PN} += " \
    /usr/lib/firmware/${FIRMWARE_NAME} \
"
