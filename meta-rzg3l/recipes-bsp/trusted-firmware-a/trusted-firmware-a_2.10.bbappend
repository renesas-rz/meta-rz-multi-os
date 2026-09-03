FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

python () {
    machine_features = d.getVar('MACHINE_FEATURES').split()

    # CM33 firmware definitions are mutually exclusive
    exclusive = [f for f in ('RZ_CM33_COLDBOOT', 'RZ_CM33_FIRMWARE_LOAD', 'RZ_REMOTEPROC') if f in machine_features]
    if len(exclusive) > 1:
        bb.fatal("MACHINE_FEATURES has mutually exclusive features: %s. Only one of "
                 "RZ_CM33_COLDBOOT, RZ_CM33_FIRMWARE_LOAD, RZ_REMOTEPROC may be set." % ", ".join(exclusive))

    if 'RZ_CM33_FIRMWARE_LOAD' in machine_features:
        d.appendVar('EXTRA_OEMAKE', ' PLAT_M33_BOOT_SUPPORT=1')
    if 'RZ_CM33_COLDBOOT' in machine_features:
        d.appendVar('EXTRA_OEMAKE', ' PLAT_BOOT_FROM_M33_SUPPORT=1')
}

SRC_URI:append = " \
    file://0001-fix-disable-MMU-and-D-cache-before-CA55-WFI.patch \
    file://0002-add-support-for-AWO-suspend-on-RZG3L.patch \
    file://0003-add-software-spinlock-implementation-for-AWO-suspend.patch \
    file://0004-simplify-riic-for-pmic-usage.patch \
    file://0005-fix-g3l-init-RIIC-pins-before-PMIC-read-in-AWO-boot-.patch \
"
