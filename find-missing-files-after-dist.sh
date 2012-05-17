#!/bin/bash

SOURCE_DIR=$(pwd)
ORIG_DIR=$(mktemp -d)
BUILD_DIR=$(mktemp -d)
TMP_SOURCE=$(mktemp)
TMP_DEST=$(mktemp)

pushd ${ORIG_DIR}
	make -C ${SOURCE_DIR} distclean || echo "No distclean"
	${SOURCE_DIR}/configure && make dist && make distclean || (echo "Cannot create tarball" && exit 2)
	VERSION=$(grep PACKAGE_VERSION ${SOURCE_DIR}/configure | grep '=' | cut -d'=' -f2 | sed -e "s/'//g")
popd

TAR_DIR="accords-${VERSION}"
TAR_NAME="${TAR_DIR}.tar.gz"

pushd ${BUILD_DIR}
	tar xf ${ORIG_DIR}/${TAR_NAME} -C . || (echo "Cannot extract tarball" && exit 3)
	${TAR_DIR}/configure || (echo "Cannot configure target" && exit 4)
popd

SOURCE_FILES=$(find ${SOURCE_DIR} -type f -prune -not -path "${SOURCE_DIR}/.git/*" -and -prune -not -name ".gitignore" | sed -e "s|${SOURCE_DIR}/||g" | sort > ${TMP_SOURCE})
DEST_FILES=$(find ${BUILD_DIR}/${TAR_DIR}/ -type f | sed -e "s|${BUILD_DIR}/${TAR_DIR}/||g" | sort > ${TMP_DEST})

echo "The following files might be missing from Makefile's DIST variable"
diff -uw ${TMP_SOURCE} ${TMP_DEST} | grep "^-"

rm ${TMP_SOURCE} || echo "Error while removing ${TMP_SOURCE}"
rm ${TMP_DEST} || echo "Error while removing ${TMP_DEST}"
rm -rf ${ORIG_DIR}
rm -rf ${BUILD_DIR}
