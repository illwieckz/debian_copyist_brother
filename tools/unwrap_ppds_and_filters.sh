#! /bin/sh

if [ "${WEBBROTHER_WORKSPACE}" != '' ]
then
	WORKSPACE="${WEBBROTHER_WORKSPACE}"
else
	WORKSPACE="$(cd "$(dirname $0)"; pwd)/../material/"
fi

modified_cupswrapper_dir="${WORKSPACE}/abstract/cupswrappers_modified/"
unwrap_dir="${WORKSPACE}/abstract/generated_content_original/"
mkdir -p "${modified_cupswrapper_dir}"
mkdir -p "${unwrap_dir}"

cd "${unwrap_dir}";
mkdir -p "ppds" "filters";
for s in "${modified_cupswrapper_dir}"*
do
	echo "${s}"
done
chmod a+x ppds/* filters/*
cd - >/dev/null

#EOF
