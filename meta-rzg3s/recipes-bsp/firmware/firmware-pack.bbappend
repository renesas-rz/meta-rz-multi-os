do_compile:append () {
	
	for bl2boot in ${BL2_BOOT_TARGET}; do
	 if echo "${MACHINE_FEATURES}" | grep -q "RZG3S_CM33_BOOT"; then
		cat ${SYSROOT_TFA}/bl2.bin > ${S}/bl2_no_bp_$bl2boot.bin
		objcopy -I binary -O srec --adjust-vma=${BL2_ADJUST_VMA} --srec-forceS3 ${S}/bl2_no_bp_$bl2boot.bin ${S}/bl2_no_bp_$bl2boot.srec
	 fi
	done
}

do_deploy:append() {
		
	for bl2boot in ${BL2_BOOT_TARGET}; do
		if echo "${MACHINE_FEATURES}" | grep -q "RZG3S_CM33_BOOT"; then
			install -m 0644  ${S}/bl2_no_bp_$bl2boot.srec ${DEPLOYDIR}/bl2_no_bp_$bl2boot-${MACHINE}.srec
		fi
		
	done
}


