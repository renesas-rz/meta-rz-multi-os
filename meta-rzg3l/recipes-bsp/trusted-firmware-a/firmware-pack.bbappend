python () {
    machine_features = d.getVar('MACHINE_FEATURES').split()

    if 'RZ_CM33_FIRMWARE_LOAD' in machine_features or 'RZ_CM33_COLDBOOT' in machine_features:
        d.setVar('BL2_BASE_ADDR', '0x5C000')
}
