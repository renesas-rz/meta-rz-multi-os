SUMMARY = "CM33 RPMsg firmware"
LICENSE = "CLOSED"

FIRMWARE_NAME = "*.elf"

EXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI = "file://."

INSANE_SKIP:${PN} = "arch"

do_install() {
    install -d ${D}/usr/lib/firmware
    install -m 0644 ${WORKDIR}/${FIRMWARE_NAME} ${D}/usr/lib/firmware/
}

FILES:${PN} += " \
    /usr/lib/firmware/${FIRMWARE_NAME} \
"
