#! /bin/sh

if [ "${WEBBROTHER_WORKSPACE}" != '' ]
then
	WORKSPACE="${WEBBROTHER_WORKSPACE}"
else
	WORKSPACE="$(cd "$(dirname $0)"; pwd)/../material/"
fi

new_files_dir="${WORKSPACE}/abstract/copied_content/"
inf_dir="${new_files_dir}inf/"
lpd_dir="${new_files_dir}lpd/"
printer_dir="${new_files_dir}printer/"
ppd_dir="${new_files_dir}ppd/"
mkdir -p "${new_files_dir}" "${inf_dir}" "${lpd_dir}" "${printer_dir}" "${ppd_dir}"

olpd_pwd="$(pwd)"
cd "${WORKSPACE}/extraction/opt/brother/Printers/"
cp -av * "${printer_dir}"
cd "${WORKSPACE}/extraction/usr/local/Brother/Printer/"
cp -av * "${printer_dir}"
cd "${WORKSPACE}/extraction/usr/local/Brother/inf/"
cp -av * "${inf_dir}"
cd "${WORKSPACE}/extraction/usr/local/Brother/lpd/"
cp -av * "${lpd_dir}"
cd "${WORKSPACE}/extraction/usr/share/ppd/Brother/"
cp -av * "${ppd_dir}"
cd "${old_pwd}"

#EOF
