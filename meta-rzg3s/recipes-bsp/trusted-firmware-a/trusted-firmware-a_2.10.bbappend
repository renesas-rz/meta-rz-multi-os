FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

python () {
    features = d.getVar('MACHINE_FEATURES').split()

    if 'RZ_CM33_FIRMWARE_LOAD' in features:
        d.appendVar('EXTRA_OEMAKE', ' PLAT_M33_BOOT_SUPPORT=1')
    if 'RZ_CM33_COLDBOOT' in features:
        d.appendVar('EXTRA_OEMAKE', ' ENABLE_RZG3S_CM33_BOOT=1')
    if 'RZ_AWO_SUPPORT' in features:
        d.appendVar('EXTRA_OEMAKE', ' PLAT_SYSTEM_SUSPEND=awo')
}

SRC_URI:append = " \
    file://0000-disable-TFA-AWO-M33boot-synchronous.patch \
    file://0001-cm33coldboot-support.patch \
    file://0002-update-TZC-permissions-and-set-ASRAM02-non-secure.patch \
"