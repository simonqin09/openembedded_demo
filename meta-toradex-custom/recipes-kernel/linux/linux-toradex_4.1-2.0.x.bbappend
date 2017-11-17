#include conf/tdx_version.conf
FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

#PACKAGE_ARCH = "${MACHINE_ARCH}"

SRC_URI += " \
           file://defconfig \
           file://0001-custom-devicetree.patch \
"

#LOCALVERSION = "-${PR}"
#PR = "${TDX_VER_INT}"

SRCBRANCH = "toradex_4.1-2.0.x-imx"
SRCREV = "82f0f4f012a646a735d6b44de77b7c9d0712c714"
COMPATIBLE_MACHINE = "(mx7|mx6)"
