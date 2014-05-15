#! /bin/sh

if [ "${WEBBROTHER_WORKSPACE}" != '' ]
then
	WORKSPACE="${WEBBROTHER_WORKSPACE}"
else
	WORKSPACE="$(cd "$(dirname $0)"; pwd)/../material/"
fi

files_dir="${WORKSPACE}/files/"
sources_dir="${WORKSPACE}/sources/"
mkdir -p "${sources_dir}"

tmp_dir='/tmp/extract_sources/'
mkdir -p "${tmp_dir}"

for archive in "${files_dir}"*.tar.gz
do
	echo "archive: ${archive}"
	case "$(basename ${archive})" in
		brcups_ink4_src_1.1.2-x.tar.gz)
			(cd "${tmp_dir}"; tar --strip-components=1 -xzvf "${archive}")
		;;
		brother-colorlaser2-gpl-src-1.0.3-1.tar.gz)
			(cd "${tmp_dir}"; tar --strip-components=1 -xzvf "${archive}")
		;;
		brother-laser3-cups-driver-2.0.4-1.tar.gz)
			(cd "${tmp_dir}"; tar --strip-components=1 -xzvf "${archive}")
		;;
		brmfc-src-1.0.0-1.i386.tar.gz)
			mkdir -p "${sources_dir}/brmfc-src-1.0.0-1"
			(cd "${sources_dir}/brmfc-src-1.0.0-1"; tar -xzvf "${archive}")
		;;
		brother-laser1-src-1.0.2-1.tar.gz)
			mkdir -p "${sources_dir}/brother-laser1-src-1.0.2-1"
			(cd "${sources_dir}/brother-laser1-src-1.0.2-1"; tar --strip-components=1 -xzvf "${archive}")
		;;
		ink3_GPL_src_101-1.tar.gz)
			(cd "${sources_dir}"; tar --strip-components=1 -xzvf "${archive}")
		;;
		jd622l101code.tar.gz)
			mkdir -p "${sources_dir}/jd622l101code"
			(cd "${sources_dir}/jd622l101code"; tar -xzvf "${archive}")
		;;
		jd623l101code.tar.gz)
			mkdir -p "${sources_dir}/jd623l101code"
			(cd "${sources_dir}/jd623l101code"; tar -xzvf "${archive}")
		;;
		jd662l101code.tar.gz)
			mkdir -p "${sources_dir}/jd662l101code"
			(cd "${sources_dir}/jd662l101code"; tar -xzvf "${archive}")
		;;
		jd663l101code.tar.gz)
			mkdir -p "${sources_dir}/jd663l101code"
			(cd "${sources_dir}/jd663l101code"; tar -xzvf "${archive}")
		;;
		*)
			(cd "${sources_dir}"; tar -xzvf "${archive}")
		;;
	esac
done

for archive in "${tmp_dir}"*.tar.gz
do
	(cd "${sources_dir}"; tar -xzvf "${archive}")
done

#EOF
