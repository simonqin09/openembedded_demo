DESCRIPTION = "Qt demo applicaiton deployment"
SECTION = "examples"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"
PR = "r0"
 
SRC_URI = "git://github.com/simonqin09/QtGPIOProject.git;protocol=git;branch=master"
 
SRCREV = "efac8dfeb878f104dfcfcf54188b25d024f2f16b"
 
S = "${WORKDIR}/git"
 
inherit systemd
 
DEPENDS = "qtbase"
RDEPENDS_${PN} = "qtbase-plugins"
#DEPENDS = "qtdeclarative qtgraphicaleffects"
#RDEPENDS_${PN} = "qtdeclarative-qmlplugins qtgraphicaleffects-qmlplugins"
 
require recipes-qt/qt5/qt5.inc
 
do_install() {
     oe_runmake INSTALL_ROOT=${D} install
     install -m 0755 ${WORKDIR}/git/qtdemo.sh ${D}${bindir}
     install -d ${D}${systemd_unitdir}/system/
     install -m 0644 ${WORKDIR}/git/qtdemo_launch.service ${D}${systemd_unitdir}/system
}
 
NATIVE_SYSTEMD_SUPPORT = "1"
SYSTEMD_PACKAGES = "${PN}"
SYSTEMD_SERVICE_${PN} = "qtdemo_launch.service"
