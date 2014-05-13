#! /usr/bin/env bash

error_message="error: pass the directory created by the web_brother.sh tools that contains list and files"

[ "${1}" = '' ] && echo "${error_message}" && exit 1
! [ -d "${1}" ] && echo "${error_message}" && exit 1

work_dir="$(cd ${1}; pwd)"
file_list="${work_dir}/lists/printers_files_with_licenses.txt"

! [ -f "${file_list}" ] && echo "${error_message}" && exit 1

tmp_dir="/tmp/list_files.d"
mkdir -p "${tmp_dir}"

function get_archive_type {
	echo "${1}" | sed -e 's/.*\(rpm\|deb\|ppd.gz\|tar.gz\)/\1/'
}

function create_list {
	cat "${file_list}" | sed -e 's/\t.*//' | sort | uniq | while read archive_name
	do
		archive_license="$(cat "${file_list}" | sort | uniq | grep "^${archive_name}" | sed -e 's/.*\t//')"
		archive_type="$(get_archive_type ${archive_name})"

		if [ "${archive_type}" = 'deb' ]
		then
			embedded_data_archive_name="$(ar t "${work_dir}/files/${archive_name}" | grep '^data.')"
			if [ "$(get_archive_type "${embedded_data_archive_name}")" = 'tar.gz' ]
			then
				cd "${tmp_dir}"
				ar x "${work_dir}/files/${archive_name}" "${embedded_data_archive_name}"
				tar -tzf "${embedded_data_archive_name}" | grep -v '/$' | while read file_path
				do
					file_name=$(basename "${file_path}")
					file_type="$(get_archive_type "${file_name}")"
					# bad packagers put previous .deb and .rpm inside the .deb
					if [ "${file_type}" != 'rpm' -a "${file_type}" != 'deb' ]
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
			file_sum="$(cat "${work_dir}/files/${archive_name}" | gunzip | md5sum | sed -e 's/ -$//')"
			printf '%s\t%s\t%s\t%s\t%s\n' ${file_sum} ${file_name} ${archive_name} ${archive_license} ${file_path}
		fi
	done
}

create_list | tee "${work_dir}/lists/printers_extracted_files_with_metadata.txt"
rmdir "${tmp_dir}"

#EOF
