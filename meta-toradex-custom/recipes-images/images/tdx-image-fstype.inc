#create the deployment directory-tree

include conf/tdx_version.conf

BURNFLASH := "${THISDIR}/files/${MACHINE}"
IMAGE_DIR = "${IMAGE_NAME}_${PV}"
IMAGE_ROOTFS = "${WORKDIR}/${IMAGE_DIR}/rootfs"

UBOOT_EXT = "bin"
UBOOT_EXT_mx6 = "imx"
UBOOT_EXT_mx7 = "imx"
UBOOT_EXT_vf = "imx"
UBOOT_SYMLINK = "u-boot-${MACHINE}.${UBOOT_EXT}"
UBOOT_SYMLINK_tegra = "u-boot-dtb-tegra-${MACHINE}.${UBOOT_EXT}"
UBOOT_SYMLINK_tegra124 = "u-boot-dtb-tegra-${MACHINE}.${UBOOT_EXT}"
UBOOT_SYMLINK_tegra124m = "u-boot-dtb-tegra-${MACHINE}.${UBOOT_EXT}"
UBOOT_BINARY = "u-boot.${UBOOT_EXT}"
UBOOT_BINARY_mx7 = "u-boot-nand.${UBOOT_EXT}"
UBOOT_BINARY_tegra = "u-boot-dtb-tegra.${UBOOT_EXT}"
UBOOT_BINARY_tegra124 = "u-boot-dtb-tegra.${UBOOT_EXT}"
UBOOT_BINARY_tegra124m = "u-boot-dtb-tegra.${UBOOT_EXT}"

#we need some native tools for deployment
DEPENDS_append_mx6 = " imx-loader-native"
DEPENDS_append_mx7 = " mtd-utils-native imx-loader-native"
DEPENDS_append_tegra = " cbootimage-native tegrarcm-native"
DEPENDS_append_tegra124 = " cbootimage-native tegrarcm-native"
DEPENDS_append_tegra124m = " cbootimage-native tegrarcm-native"
DEPENDS_append_tegra2 = " mtd-utils-native"
DEPENDS_append_vf = " mtd-utils-native imx-loader-native"
#we need mkimage to produce the U-Boot scripts even if not building a uImage kernel
DEPENDS_append_arm = " virtual/bootloader u-boot-mkimage-native"
DEPENDS_remove_qemuarm = "virtual/bootloader"

imagedeploytools() {
    # default to nothing to deploy
    :
}

imagedeploytools_append_mx6() {
    IMAGE_ROOT="${IMAGE_ROOTFS}/../imx_flash/"
    rm -f ${IMAGE_ROOT}/imx_flash/imx_usb ${IMAGE_ROOT}/imx_flash/*.conf
    mkdir -p ${IMAGE_ROOT}

    # add imx-loader and its configuration files
    cp ${STAGING_BINDIR_NATIVE}/imx_usb ${IMAGE_ROOT}/
    ${BUILD_STRIP} ${IMAGE_ROOT}/imx_usb
    cp ${STAGING_ETCDIR_NATIVE}/imx-loader.d/imx_usb.conf ${IMAGE_ROOT}/
    cp ${STAGING_ETCDIR_NATIVE}/imx-loader.d/mx6_usb_work.conf ${IMAGE_ROOT}/
}

imagedeploytools_append_mx7() {
    IMAGE_ROOT="${IMAGE_ROOTFS}/../imx_flash/"
    rm -f ${IMAGE_ROOT}/mkfs.ubifs  ${IMAGE_ROOT}/imx_usb
    mkdir -p ${IMAGE_ROOT}

    # add the mkfs.ubifs binary
    cp ${STAGING_DIR_NATIVE}/usr/sbin/mkfs.ubifs ${IMAGE_ROOT}/
    ${BUILD_STRIP} ${IMAGE_ROOT}/mkfs.ubifs

    # add imx-loader and its configuration files
    cp ${STAGING_BINDIR_NATIVE}/imx_usb ${IMAGE_ROOT}/
    ${BUILD_STRIP} ${IMAGE_ROOT}/imx_usb
    cp ${STAGING_ETCDIR_NATIVE}/imx-loader.d/imx_usb.conf ${IMAGE_ROOT}/
    cp ${STAGING_ETCDIR_NATIVE}/imx-loader.d/mx7_usb_work.conf ${IMAGE_ROOT}/
}

imagedeploytools_append_tegra() {
    IMAGE_ROOT="${IMAGE_ROOTFS}/../tegra-uboot-flasher/"
    mkdir -p ${IMAGE_ROOT}

    # add cbootimage and tegrarcm
    cp ${STAGING_BINDIR_NATIVE}/cbootimage ${IMAGE_ROOT}/
    cp ${STAGING_BINDIR_NATIVE}/tegrarcm ${IMAGE_ROOT}/
    ${BUILD_STRIP} ${IMAGE_ROOT}/cbootimage
    ${BUILD_STRIP} ${IMAGE_ROOT}/tegrarcm
}

imagedeploytools_append_tegra2() {
    # add the mkfs.ubifs binary
    cp ${STAGING_DIR_NATIVE}/usr/sbin/mkfs.ubifs ${IMAGE_ROOT}/
    ${BUILD_STRIP} ${IMAGE_ROOT}/mkfs.ubifs
}

imagedeploytools_append_tegra124() {
    IMAGE_ROOT="${IMAGE_ROOTFS}/../tegra-uboot-flasher/"
    mkdir -p ${IMAGE_ROOT}

    # add cbootimage and tegrarcm
    cp ${STAGING_BINDIR_NATIVE}/cbootimage ${IMAGE_ROOT}/
    cp ${STAGING_BINDIR_NATIVE}/tegrarcm ${IMAGE_ROOT}/
    ${BUILD_STRIP} ${IMAGE_ROOT}/cbootimage
    ${BUILD_STRIP} ${IMAGE_ROOT}/tegrarcm
}

imagedeploytools_append_tegra124m() {
    IMAGE_ROOT="${IMAGE_ROOTFS}/../tegra-uboot-flasher/"
    mkdir -p ${IMAGE_ROOT}

    # add cbootimage and tegrarcm
    cp ${STAGING_BINDIR_NATIVE}/cbootimage ${IMAGE_ROOT}/
    cp ${STAGING_BINDIR_NATIVE}/tegrarcm ${IMAGE_ROOT}/
    ${BUILD_STRIP} ${IMAGE_ROOT}/cbootimage
    ${BUILD_STRIP} ${IMAGE_ROOT}/tegrarcm
}


imagedeploytools_append_vf() {
    IMAGE_ROOT="${IMAGE_ROOTFS}/../vf_flash/"
    rm -f ${IMAGE_ROOT}/mkfs.ubifs  ${IMAGE_ROOT}/imx_uart
    mkdir -p ${IMAGE_ROOT}

    # add the mkfs.ubifs binary
    cp ${STAGING_DIR_NATIVE}/usr/sbin/mkfs.ubifs ${IMAGE_ROOT}/
    ${BUILD_STRIP} ${IMAGE_ROOT}/mkfs.ubifs

    # add imx-loader and its configuration files
    cp ${STAGING_BINDIR_NATIVE}/imx_uart ${IMAGE_ROOT}/
    ${BUILD_STRIP} ${IMAGE_ROOT}/imx_uart*
    cp ${STAGING_ETCDIR_NATIVE}/imx-loader.d/vybrid_usb_work.conf ${IMAGE_ROOT}/
}

imagedeploy_kernel() {
    # put kernel, existing device-tree(s) into the bin directories
    cp -pP ${DEPLOY_DIR_IMAGE}/${KERNEL_IMAGETYPE}* ${IMAGE_ROOTFS}/../${MACHINE}_bin/
    ls ${DEPLOY_DIR_IMAGE}/*.dtb 2> /dev/null && cp -pP ${DEPLOY_DIR_IMAGE}/*.dtb ${IMAGE_ROOTFS}/../${MACHINE}_bin/
    mv ${IMAGE_ROOTFS}/../${MACHINE}_bin/${KERNEL_IMAGETYPE}-${MACHINE}.bin ${IMAGE_ROOTFS}/../${MACHINE}_bin/${KERNEL_IMAGETYPE}
}

fakeroot imagedeploy() {
    :
}

fakeroot imagedeploy_arm() {
    cd ${WORKDIR}
    cp -Lr ${BURNFLASH}/* ${IMAGE_DIR}/

    cd ${IMAGE_DIR}/${MACHINE}_bin
    ./mk-u-boot-scripts.sh
    cd ${WORKDIR}

    imagedeploy_kernel

    cp -P ${DEPLOY_DIR_IMAGE}/u-boot* ${IMAGE_ROOTFS}/../${MACHINE}_bin/
    if [ -e ${DEPLOY_DIR_IMAGE}/SPL ]
    then
        cp -P ${DEPLOY_DIR_IMAGE}/SPL* ${IMAGE_ROOTFS}/../${MACHINE}_bin/
    fi
    rm -f ${IMAGE_ROOTFS}/../${MACHINE}_bin/u-boot-hsmmc-${MACHINE}.${UBOOT_EXT}
    mv ${IMAGE_ROOTFS}/../${MACHINE}_bin/${UBOOT_SYMLINK} ${IMAGE_ROOTFS}/../${MACHINE}_bin/${UBOOT_BINARY}

    # add the rootfs version to the welcome banner
    echo -n "${IMAGE_NAME} ${PV}" >> ${IMAGE_ROOTFS}/etc/issue
    echo " ${TDX_VERDATE}" | sed 's/-//' >> ${IMAGE_ROOTFS}/etc/issue
    echo "" >> ${IMAGE_ROOTFS}/etc/issue

    # add additional tools if required
    imagedeploytools

    # remove intermediate file rootfs.lock
    rm -f ${IMAGE_DIR}/rootfs.lock

    #create tarball
    tar -cjf ${DEPLOY_DIR_IMAGE}/${IMAGE_NAME}_${TDX_VER_EXT}.tar.bz2 ${IMAGE_DIR}
}

fakeroot do_imagedeploy() {
    imagedeploy
}
addtask imagedeploy after do_rootfs before do_image_complete

# make sure to rerun do_rootfs before do_imagedeploy is executed
do_imagedeploy[vardep] = "do_rootfs"
