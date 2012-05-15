#!/bin/bash

BUILD_DIR=build
TAR_DIR="accords-1.0"
TAR_NAME="${TAR_DIR}.tar.gz"
TMP_SOURCE=$(mktemp)
TMP_DEST=$(mktemp)

mkdir -p ${BUILD_DIR} || (echo "Cannot create build dir ${BUILD_DIR}" && exit 1)

./configure && make dist || (echo "Cannot create tarball" && exit 2)

pushd ${BUILD_DIR}/
	tar xf ../${TAR_NAME} -C . || (echo "Cannot extract tarball" && exit 3)
	cd ${TAR_DIR} && ./configure || (echo "Cannot configure target" && exit 4)
popd

SOURCE_FILES=$(find . -type f -prune -not -path "./${BUILD_DIR}/*" -and -prune -not -path "./.git/*" -and -prune -not -name ".gitignore" -and -prune -not -name "${TAR_NAME}" > ${TMP_SOURCE})
DEST_FILES=$(find ./${BUILD_DIR}/ -type f | sed -e "s|${BUILD_DIR}\/${TAR_DIR}\/||g" > ${TMP_DEST})

echo "The following files might be missing from Makefile's DIST variable"
diff -uw ${TMP_SOURCE} ${TMP_DEST} | grep "^-"

rm ${TMP_SOURCE} || echo "Error while removing ${TMP_SOURCE}"
rm ${TMP_DEST} || echo "Error while removing ${TMP_DEST}"
