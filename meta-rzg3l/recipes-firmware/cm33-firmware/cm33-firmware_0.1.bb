SUMMARY = "CM33 RPMsg firmware"
LICENSE = "CLOSED" 

EXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI = " \
    file://. \
"

INSANE_SKIP:${PN} = "arch" 

do_install() {
    install -d ${D}/usr/lib/firmware
    install -d ${D}/boot

    for f in ${WORKDIR}/*.elf; do
	[ -e "$f" ] && install -m 0644 "$f" ${D}/usr/lib/firmware
    done

    for f in ${WORKDIR}/*.bin; do
	[ -e "$f" ] && install -m 0644 "$f" ${D}/boot/
    done
} 

FILES:${PN} += " \
    /usr/lib/firmware \
    /boot \
"
