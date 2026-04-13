USE_OPENAMP ?= "1"

IMAGE_INSTALL:append = " \
	${@bb.utils.contains("USE_OPENAMP", "1", " libmetal ", "", d)} \
	${@bb.utils.contains("USE_OPENAMP", "1", " open-amp ", "", d)} \
	${@bb.utils.contains("USE_OPENAMP", "1", " rpmsg-sample ", "", d)} \
	${@bb.utils.contains("USE_OPENAMP", "1", " cr52-firmware ", "", d)} \
"
