#! /bin/sh

if [ "${WEBBROTHER_WORKSPACE}" != '' ]
then
	WORKSPACE="${WEBBROTHER_WORKSPACE}"
else
	WORKSPACE="$(cd "$(dirname $0)"; pwd)/../material/"
fi

new_cupswrapper_dir="${WORKSPACE}/abstract/cupswrappers_original/"
mkdir -p "${new_cupswrapper_dir}"

find "${WORKSPACE}/extraction" -type 'f' -name 'cupswrapper*' -exec cp -av {} "${new_cupswrapper_dir}" \;

#EOF
