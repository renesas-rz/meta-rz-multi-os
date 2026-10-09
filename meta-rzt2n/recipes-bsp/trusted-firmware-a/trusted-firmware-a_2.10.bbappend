FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

# List of TF-A patches
SRC_URI:append = " \
	${@'file://0001-CA55-CR52-coldboot-support.patch \
	    file://0002-Add-bl2-boot-support-firmware-load.patch' if 'RZT2N_CA55_CR52_BL2_BOOT' in d.getVar('MACHINE_FEATURES').split() else ''} \
"

SRC_URI:append = " \
	file://0003-Overwrite-cr520-security-attribute-to-secure.patch \
"

# Set EXTRA_FLAGS_rzt2n based on BL2 boot features in MACHINE_FEATURES
python () {
    if 'RZT2N_CA55_CR52_BL2_BOOT' in d.getVar('MACHINE_FEATURES').split():
        d.appendVar('EXTRA_FLAGS:rzt2n-dev', ' RZT2N_CA55_CR52_BL2_BOOT=1')

        if 'RZT2N_CA551_BL2_BOOT' in d.getVar('MACHINE_FEATURES').split():
            d.appendVar('EXTRA_FLAGS:rzt2n-dev', ' PLAT_A551_BL2_BOOT=1')
        elif 'RZT2N_CR520_BL2_BOOT' in d.getVar('MACHINE_FEATURES').split():
            d.appendVar('EXTRA_FLAGS:rzt2n-dev', ' PLAT_R520_BL2_BOOT=1')
        elif 'RZT2N_CR521_BL2_BOOT' in d.getVar('MACHINE_FEATURES').split():
            d.appendVar('EXTRA_FLAGS:rzt2n-dev', ' PLAT_R521_BL2_BOOT=1')
}

EXTRA_OEMAKE:prepend:rzt2n-dev := " ${EXTRA_FLAGS} " 

EXTRA_OEMAKE:append:rzt2n-dev = " \
	${@ 'FSP_A551_SUPPORT=1' if d.getVar('RPMSG_REMOTE_CORE') == '1' else '' } \
"
