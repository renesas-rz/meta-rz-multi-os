FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI:append = " \
	file://0001-Change-sys-ram-to-ns.patch \
"
# List of TF-A patches
SRC_URI:append = " \
	${@'file://0002-CA55-CR52-coldboot-support.patch \
	    file://0003-Add-bl2-boot-support-firmware-load.patch' if 'RZT2H_CA55_CR52_BL2_BOOT' in d.getVar('MACHINE_FEATURES').split() else ''} \
"

# Set EXTRA_FLAGS_rzt2h based on BL2 boot features in MACHINE_FEATURES
python () {
    if 'RZT2H_CA55_CR52_BL2_BOOT' in d.getVar('MACHINE_FEATURES').split():
        d.appendVar('EXTRA_FLAGS:rzt2h-family', ' RZT2H_CA55_CR52_BL2_BOOT=1')
        
        if 'RZT2H_CA552_BL2_BOOT' in d.getVar('MACHINE_FEATURES').split():
            d.appendVar('EXTRA_FLAGS:rzt2h-family', ' PLAT_A552_BL2_BOOT=1')
        elif 'RZT2H_CA553_BL2_BOOT' in d.getVar('MACHINE_FEATURES').split():
            d.appendVar('EXTRA_FLAGS:rzt2h-family', ' PLAT_A553_BL2_BOOT=1')
        elif 'RZT2H_CR520_BL2_BOOT' in d.getVar('MACHINE_FEATURES').split():
            d.appendVar('EXTRA_FLAGS:rzt2h-family', ' PLAT_R520_BL2_BOOT=1')
        elif 'RZT2H_CR521_BL2_BOOT' in d.getVar('MACHINE_FEATURES').split():
            d.appendVar('EXTRA_FLAGS:rzt2h-family', ' PLAT_R521_BL2_BOOT=1')
        else:
            d.appendVar('EXTRA_FLAGS:rzt2h-family', ' PLAT_A551_BL2_BOOT=1')
}

EXTRA_OEMAKE:prepend:rzt2h-family := " ${EXTRA_FLAGS} " 
