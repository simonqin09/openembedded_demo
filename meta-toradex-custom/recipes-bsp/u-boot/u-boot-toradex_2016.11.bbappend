#include conf/tdx_version.conf

#PR = "${TDX_VER_INT}-gitr${@d.getVar("SRCREV", False)[0:7]}"
#LOCALVERSION ?= "-${TDX_VER_INT}"

FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

SRC_URI += " \
           file://colibri_imx6_uboot_custom.patch \
"

SRCREV = "f0e414972b5b225e33ebe75574562266116746f9"
SRCBRANCH = "2016.11-toradex"
