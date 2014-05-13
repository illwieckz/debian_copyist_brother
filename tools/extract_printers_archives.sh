#! /usr/bin/env bash

error_message="error: pass the directory created by the web_brother.sh tools that contains list and files"

if [ "${WEBBROTHER_WORKSPACE}" != '' ]
then
	WORKSPACE="${WEBBROTHER_WORKSPACE}"
else
	WORKSPACE="$(cd "$(dirname $0)"; pwd)/../material/"
fi

! [ -d "${WORKSPACE}" ] && echo "${error_message}" && exit 1

files_list="${WORKSPACE}/lists/printers_files_with_metadata.txt"
extraction_dir="${WORKSPACE}/extraction/"
ppd_dir='./usr/share/ppd/Brother/'
mkdir -p "${extraction_dir}"
mkdir -p "${extraction_dir}${ppd_dir}"

tools_dir="$(cd "$(dirname $0)"; pwd)/"

! [ -f "${files_list}" ] && echo "${error_message}" && exit 1

function _extract_all {
	cat "${files_list}" | sed -e 's/\t/ /g' | while read file_line
	do
		archive_name="$(echo ${file_line} | cut -d ' ' -f 3)"
		file_path="$(echo ${file_line} | cut -d ' ' -f 5)"
		"${tools_dir}extract_file_from_archive.sh" "${file_path}" "${archive_name}"
	done
}

_extract_all

#EOF
