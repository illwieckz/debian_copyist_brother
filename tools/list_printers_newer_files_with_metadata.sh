#! /usr/bin/env bash

error_message="error: pass the directory created by the web_brother.sh tools that contains list and files"

if [ "${WEBBROTHER_WORKSPACE}" != '' ]
then
	WORKSPACE="${WEBBROTHER_WORKSPACE}"
else
	WORKSPACE="$(cd "$(dirname $0)"; pwd)/../material/"
fi

! [ -d "${WORKSPACE}" ] && echo "${error_message}" && exit 1

archives_list="${WORKSPACE}/lists/printers_archives_with_licenses.txt"

! [ -f "${archives_list}" ] && echo "${error_message}" && exit 1

function _newer_list {
	cat "${WORKSPACE}/lists/printers_files_with_metadata.txt" | cut -d "$(printf '\t')" -f 2 | sort | uniq | while read file_name
	do
		grep "$(printf '\t')${file_name}$(printf '\t')" "${WORKSPACE}/lists/printers_files_with_metadata.txt" | sort -k 3 | tail -n 1
	done
}

_newer_list | sort -k 3 | tee "${WORKSPACE}/lists/printers_newer_files_with_metadata.txt"

#EOF
