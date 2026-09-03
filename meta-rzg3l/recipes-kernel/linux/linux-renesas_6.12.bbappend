FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

ENABLE_REMOTEPROC = "${@'1' if 'RZ_REMOTEPROC' in d.getVar('MACHINE_FEATURES').split() else '0'}"
ENABLE_SD2 = "${@'1' if 'RZ_SD2_SUPPORT' in d.getVar('MACHINE_FEATURES').split() else '0'}"
ENABLE_JTAG = "${@'1' if 'RZ_JTAG_SUPPORT' in d.getVar('MACHINE_FEATURES').split() else '0'}"

SRC_URI:append = " \
    file://0001-Add-rzg3l-smarc-multi-os-dtsi-for-supporting-OpenAMP.patch \
    file://0002-Set-OSTM2-as-critical-clock.patch \
"

SRC_URI:append = " ${@'file://0003-clk-renesas-r9a08g046-Add-CM33-clock-and-reset-signal.patch \
	file://0004-arm64-dts-renesas-r9a08g046-Add-rproc-node-for-CM33.patch \
	file://0005-Add-bindings-for-RZ-remoteproc-driver.patch \
	file://0006-remoteproc-Add-RZ-remoteproc-driver.patch' if '${ENABLE_REMOTEPROC}' == '1' else ''}"

SRC_URI:append = " ${@'file://0007-Enable-SD2-selection.patch' if '${ENABLE_SD2}' == '1' else ''}"

SRC_URI:append = " ${@'file://0008-Enable_JTAG_connection.patch' if '${ENABLE_JTAG}' == '1' else ''}"

# Kernel confguration update
SRC_URI:append = " file://uio.cfg"
SRC_URI:append = " ${@'file://remoteproc.cfg' if '${ENABLE_REMOTEPROC}' == '1' else ''}"
