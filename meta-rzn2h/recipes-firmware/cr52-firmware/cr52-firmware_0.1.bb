SUMMARY = "CR52 RPMsg firmware"
LICENSE = "CLOSED"

EXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI = "file://."

INSANE_SKIP:${PN} += "arch"

do_install() {
    install -d ${D}${nonarch_base_libdir}/firmware
    install -m 0644 ${WORKDIR}/*.elf ${D}${nonarch_base_libdir}/firmware/ || true
    install -m 0644 ${WORKDIR}/*.out ${D}${nonarch_base_libdir}/firmware/ || true
}

FILES:${PN} += "${nonarch_base_libdir}/firmware/*"
