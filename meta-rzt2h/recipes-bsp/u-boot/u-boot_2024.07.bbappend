FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

# List of U-boot patches
SRC_URI:append = " \
    ${@'file://0001-configs-rzt2h-dev_defconfig-Enable-CR52-command.patch \
    file://0002-cmd-Add-CR52-command.patch' if d.getVar('ENABLE_U_BOOT') == '1' else ''} \
"
