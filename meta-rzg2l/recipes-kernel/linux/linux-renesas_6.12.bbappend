FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

ENABLE_REMOTEPROC = "0"

# List of kernel patches
SRC_URI += " \
    file://0001-Add-rzg2l-smarc-multi-os-dtsi-for-supporting-OpenAMP.patch \
    file://0002-arm64-dts-renesas-rzg2l-smarc-Disable-OSTM2.patch \
    file://0003-clk-renesas-r9a07g044-Set-SCIF1-SCIF2-OSTM2.patch \
"

SRC_URI += "${@'file://0004-clk-renesas-r9a07g044-Add-I2-clock.patch \
    file://0005-clk-renesas-r9a07g044-Add-CM33-clock-and-reset-signal.patch \
    file://0006-arm64-dts-renesas-rzg2l-smarc-Add-rproc-node.patch \
    file://0007-Add-bindings-for-RZ-remoteproc-driver.patch \
    file://0008-Add-RZ-remoteproc-driver.patch' if '${ENABLE_REMOTEPROC}' == '1' else ''}"

# Kernel configuration update
SRC_URI += "file://uio.cfg"
SRC_URI += "${@'file://remoteproc.cfg' if '${ENABLE_REMOTEPROC}' == '1' else ''}"
