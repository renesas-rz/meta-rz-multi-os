FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI:append = " \
	file://0001-Change-sys-ram-to-ns.patch \
"
# List of TF-A patches
SRC_URI:append = " \
	${@'file://0002-CA55-CR52-coldboot-support.patch \
	    file://0003-Add-bl2-boot-support-firmware-load.patch' if 'RZN2H_CA55_CR52_BL2_BOOT' in d.getVar('MACHINE_FEATURES').split() else ''} \
"

# Set EXTRA_FLAGS_rzn2h based on BL2 boot features in MACHINE_FEATURES
python () {
    if 'RZN2H_CA55_CR52_BL2_BOOT' in d.getVar('MACHINE_FEATURES').split():
        d.appendVar('EXTRA_FLAGS:rzn2h-dev', ' RZN2H_CA55_CR52_BL2_BOOT=1')
        
        if 'RZN2H_CA552_BL2_BOOT' in d.getVar('MACHINE_FEATURES').split():
            d.appendVar('EXTRA_FLAGS:rzn2h-dev', ' PLAT_A552_BL2_BOOT=1')
        elif 'RZN2H_CA553_BL2_BOOT' in d.getVar('MACHINE_FEATURES').split():
            d.appendVar('EXTRA_FLAGS:rzn2h-dev', ' PLAT_A553_BL2_BOOT=1')
        elif 'RZN2H_CR520_BL2_BOOT' in d.getVar('MACHINE_FEATURES').split():
            d.appendVar('EXTRA_FLAGS:rzn2h-dev', ' PLAT_R520_BL2_BOOT=1')
        elif 'RZN2H_CR521_BL2_BOOT' in d.getVar('MACHINE_FEATURES').split():
            d.appendVar('EXTRA_FLAGS:rzn2h-dev', ' PLAT_R521_BL2_BOOT=1')
        else:
            d.appendVar('EXTRA_FLAGS:rzn2h-dev', ' PLAT_A551_BL2_BOOT=1')
}

EXTRA_OEMAKE:prepend:rzn2h-dev := " ${EXTRA_FLAGS} "
