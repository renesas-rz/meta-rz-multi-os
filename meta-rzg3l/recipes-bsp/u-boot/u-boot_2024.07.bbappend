FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

ENABLE_SD2 = "${@'1' if 'RZ_SD2_SUPPORT' in d.getVar('MACHINE_FEATURES').split() else '0'}"

SRC_URI:append = " \
    file://0001-Disable-reserved-area-check.patch \
    file://0002-Enable_JTAG_connection.patch \
    ${@'file://0003-Enable_SD2_support.patch' if '${ENABLE_SD2}' == '1' else ''} \
    ${@'file://0004_make_default_env_support_sd2.patch' if '${ENABLE_SD2}' == '1' else ''} \
"
