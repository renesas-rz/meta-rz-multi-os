FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

python () {
    features = d.getVar('MACHINE_FEATURES').split()

    if 'RZG3S_CM33_BOOT' in features:
        d.appendVar('EXTRA_OEMAKE', ' ENABLE_RZG3S_CM33_BOOT=1')

    if 'RZG3S_AWO_SUPPORT' in features:
        d.setVar('ENABLE_RZG3S_AWO_SUPPORT', '1')
    else:
        d.setVar('ENABLE_RZG3S_AWO_SUPPORT', '0')
}

SRC_URI:append = " \
    ${@'file://0001-cm33coldboot-support.patch' if d.getVar('ENABLE_RZG3S_AWO_SUPPORT') != '1' else ''} \
    ${@'file://0000-disable-TFA-AWO-M33boot-synchronous.patch' if d.getVar('ENABLE_RZG3S_AWO_SUPPORT') == '1' else ''} \
"

TFA_BOARD = "${@'smarc PLAT_SYSTEM_SUSPEND=awo PLAT_M33_BOOT_SUPPORT=1' if d.getVar('ENABLE_RZG3S_AWO_SUPPORT') == '1' else 'smarc PLAT_SYSTEM_SUSPEND=vbat'}"