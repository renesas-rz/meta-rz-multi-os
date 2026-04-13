FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

# List of kernel patches
SRC_URI:append = " \
	file://0001-Add-uio-support-for-inter-processor-communication.patch \
"

SRC_URI:append = " \
    ${@'file://0002-remoteproc-renesas-Add-bindings-for-RZ-remoteproc-driver.patch \
    file://0003-dts-renesas-Add-rproc-node-for-CR52.patch \
    file://0004-Add-RZ-remoteproc-driver.patch' if d.getVar('ENABLE_REMOTEPROC') == '1' else ''} \
"

SRC_URI:append = " \
    ${@'file://0005-dts-renesas-Disable-CPU1-and-update-uio-for-target-core.patch \
    file://0006-dts-renesas-Disable-CPU2-and-update-uio-for-target-core.patch \
    file://0007-dts-renesas-Disable-CPU3-and-update-uio-for-target-core.patch \
    file://0008-Add-support-CA55-fsp.patch' if d.getVar('RPMSG_REMOTE_CORE') == '1' else ''} \
"

SRC_URI:append = " \
	file://0009-dts-renesas-reserve-memory-for-CA55-resource-table.patch \
"

# Kernel confguration update
SRC_URI:append = " \
	file://uio.cfg \
"

SRC_URI:append = " \
	${@'file://remoteproc.cfg' if d.getVar('ENABLE_REMOTEPROC') == '1' else ''} \
"

# Add extra device tree A55 support
KERNEL_DEVICETREE += " \
	${@'renesas/r9a09g077m44-dev-fsp-a551-support.dtb \
	renesas/r9a09g077m44-dev-fsp-a552-support.dtb \
	renesas/r9a09g077m44-dev-fsp-a553-support.dtb' if d.getVar('RPMSG_REMOTE_CORE') == '1' else ''} \
"
