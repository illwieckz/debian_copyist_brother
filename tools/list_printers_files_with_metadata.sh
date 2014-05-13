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

tmp_dir="/tmp/list_files.d"
mkdir -p "${tmp_dir}"

function get_archive_type {
	echo "${1}" | sed -e 's/.*\(rpm\|deb\|ppd.gz\|tar.gz\)/\1/'
}

function create_list {
	cat "${archives_list}" | sed -e 's/\t.*//' | sort | uniq | while read archive_name
	do
		archive_license="$(cat "${archives_list}" | sort | uniq | grep "^${archive_name}" | sed -e 's/.*\t//')"
		archive_type="$(get_archive_type ${archive_name})"

		if [ "${archive_type}" = 'deb' ]
		then
			embedded_data_archive_name="$(ar t "${WORKSPACE}/files/${archive_name}" | grep '^data.')"
			if [ "$(get_archive_type "${embedded_data_archive_name}")" = 'tar.gz' ]
			then
				cd "${tmp_dir}"
				ar x "${WORKSPACE}/files/${archive_name}" "${embedded_data_archive_name}"
				tar -tzf "${embedded_data_archive_name}" | grep -v '/$' | while read file_path
				do
					file_name=$(basename "${file_path}")
					file_type="$(get_archive_type "${file_name}")"
					# bad packagers put previous .deb and .rpm inside the .deb
					if [ "${file_type}" != 'rpm' -a "${file_type}" != 'deb' -a "${file_name}" != 'changelog.Debian.gz' -a "${file_name}" != 'copyright' ]
					then
						file_sum="$(tar -Oxzf "${embedded_data_archive_name}" "${file_path}" | md5sum | sed -e 's/ -$//')"
						printf '%s\t%s\t%s\t%s\t%s\n' ${file_sum} ${file_name} ${archive_name} ${archive_license} ${file_path}
					fi
				done
				rm "${embedded_data_archive_name}"
				cd - >/dev/null
			fi
		elif [ "${archive_type}" = 'ppd.gz' ]
		then
			file_name="$(basename "${archive_name}" | sed -e 's/.gz$//')"
			file_path="./${file_name}"
			file_sum="$(cat "${WORKSPACE}/files/${archive_name}" | gunzip | md5sum | sed -e 's/ -$//')"
			printf '%s\t%s\t%s\t%s\t%s\n' ${file_sum} ${file_name} ${archive_name} ${archive_license} ${file_path}
		fi
	done
}

create_list | tee "${WORKSPACE}/lists/printers_files_with_metadata.txt"
rmdir "${tmp_dir}"

#EOF
