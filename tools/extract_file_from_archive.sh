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
extraction_dir="${WORKSPACE}/extraction/"
ppd_dir='./usr/share/ppd/Brother/'
mkdir -p "${extraction_dir}"
mkdir -p "${extraction_dir}${ppd_dir}"

! [ -f "${archives_list}" ] && echo "${error_message}" && exit 1

tmp_dir="/tmp/list_files.d/"
mkdir -p "${tmp_dir}"

function _get_archive_type {
	echo "${1}" | sed -e 's/.*\(rpm\|deb\|ppd.gz\|tar.gz\)/\1/'
}

function _extract_file_from_archive {
	file_path="${1}"
	archive_name="${2}"
	archive_license="$(cat "${archives_list}" | sort | uniq | grep "^${archive_name}" | sed -e 's/.*\t//')"
	archive_type="$(_get_archive_type ${archive_name})"

	if [ "${archive_type}" = 'deb' ]
	then
		embedded_data_archive_name="$(ar t "${WORKSPACE}/files/${archive_name}" | grep '^data.')"
		if [ "$(_get_archive_type "${embedded_data_archive_name}")" = 'tar.gz' ]
		then
			(cd "${tmp_dir}"; ar x "${WORKSPACE}/files/${archive_name}" "${embedded_data_archive_name}")
			(cd "${extraction_dir}"; tar -xzf "${tmp_dir}${embedded_data_archive_name}" "${file_path}")
			printf 'extracted %s from %s (%s)\n' ${file_path} ${archive_name} ${archive_license}
			rm "${tmp_dir}${embedded_data_archive_name}"
		fi
	elif [ "${archive_type}" = 'ppd.gz' ]
	then
		complete_file_path="${extraction_dir}${ppd_dir}${file_path}"
		cat "${WORKSPACE}/files/${archive_name}" | gunzip > "${complete_file_path}"
		printf 'extracted %s from %s (%s)\n' ${file_path} ${archive_name} ${archive_license}
	fi
}

_extract_file_from_archive "${1}" "${2}"

rmdir "${tmp_dir}"

#EOF
