#! /usr/bin/env bash

# FILENAME: web_brother.sh
# AUTHOR: Thomas DEBESSE
# VERSION: 20140322
# LICENSE: ISC
# DEPENDENCIES: bash, wget, sed, dos2unix

#############################################################################
##
##  Constants, Configuration, Initialisation
##

BROTHER_TRADEMARK='Brother™'
BROTHER_PAGES_DIR='http://welcome.solutions.brother.com/bsc/public_s/id/linux/en/'
BROTHER_REPOSITORY='http://www.brother.com/pub/bsc/linux/dlf/'
BROTHER_LICENSE_DIR='http://www.brother.com/agreement/'
BROTHER_LICENSE_FILE='agree.html'

WEBARCHIVE_DOMAIN='http://web.archive.org'
WEBARCHIVE_ROOT='/web/20140319074031/'

USE_WEBARCHIVE='true'

# You can override default workspace directory via "WEBBROTHER_WORKSPACE" env variable

if [ "${WEBBROTHER_WORKSPACE}" != '' ]
then
	WORKSPACE="${WEBBROTHER_WORKSPACE}"
else
	WORKSPACE="$(cd "$(dirname $0)"; pwd)/../material/"
fi

FILES_DIR="${WORKSPACE}/files/"
LISTS_DIR="${WORKSPACE}/lists/"
PAGES_DIR="${WORKSPACE}/pages/"
LICENSES_DIR="${WORKSPACE}/licenses/"
mkdir -p "${WORKSPACE}"
mkdir -p "${FILES_DIR}"
mkdir -p "${LISTS_DIR}"
mkdir -p "${PAGES_DIR}"
mkdir -p "${LICENSES_DIR}"

#############################################################################
##
##  PRECIOUS MAGICAL REGEXP HERE
##

function _parse_sources_archives_with_licenses {
	# for each line containing file (tar.gz)
	# -> extract archive name
	print_page sources | grep 'dlfile=' | grep '\.tar.gz' | sed -e 's#^.*/linux/dlf/\(.*\)\&lang=\(.*\)"><b>.*#\1\t\2#' \
		| sort | uniq
}

function _parse_printers_archives_with_licenses {
	# for each line containing file (deb or ppd)
	# -> extract archive name
	print_page printers | grep 'dlfile=' | grep '\.deb\|\.ppd.gz' | sed -e 's#^.*/linux/dlf/\(.*\)\&lang=\(.*\)">.*#\1\t\2#' \
		| sort | uniq

}

function _parse_labelers_archives_with_licenses {
	# for each line containing file (deb or ppd)
	# -> extract archive name
	print_page labelers | grep 'dlfile=' | grep '\.deb\|\.ppd.gz' | sed -e 's#^.*/linux/dlf/\(.*\)\&lang=\(.*\)">.*#\1\t\2#' \
		| sort | uniq
}

function _parse_scanners_archives_with_licenses {
	# for each line containing file (deb)
	# -> extract archive name
	print_page scanners | grep 'dlfile=' | grep '\.deb' | sed -e 's#^.*/linux/dlf/\(.*\)\&lang=\(.*\)"><b>.*#\1\t\2#' \
		| sort | uniq
}

function _parse_pcfaxes_archives_with_licenses {
	# for each line containing file (deb)
	# -> extract archive name
	print_page pcfaxes | grep 'dlfile=' | grep '\.deb' | sed -e 's#^.*/linux/dlf/\(.*\)\&lang=\(.*\)"><b>.*#\1\t\2#' \
		| sort | uniq
}

function _parse_printers_variant_models {
	# for each line containing both variant and original model name
	# -> extract variant model name
	print_page printers | grep '^<p><a name=' | grep ' / ' | sed -e 's#^<p><a name="[A-Za-z0-9-]* / \([A-Za-z0-9-]*\)"></a><b>.*</b></p>#\1#' \
		| sort | uniq
}

function _parse_printers_alternative_models {
	# for each advertising itself as an alternative model
	# -> extract model name
	print_page printers | grep '^<div id="AltDriver"><p><a name=' | sed -e 's#^<div id="AltDriver"><p><a name="\([A-Za-z0-9-]*\)"></a><b>.*</b></p>#\1#' \
		| sort | uniq
}

function _parse_printers_original_model_from_variant {
	# for each line containing both variant and original model name and containing the given variant model name
	# -> extract the original model name
	print_page printers | grep '^<p><a name=' | grep " / ${1}" | sed -e 's#<p><a name="\([A-Za-z0-9-]*\) / '"${1}"'"></a><b>.*</b></p>$#\1#' \
		| sort | uniq
}

function _parse_printers_original_model_from_alternative {
	# for each line advertising itself as an alternative model and containing the given alternative model name
	# -> get the second line containing the original model name
	# -> extract model name
	print_page printers | grep '<div id="AltDriver"><p><a name="'"${1}"'"></a><b>' -A 2 | grep '^<td>Please use ' \
		| sed -e 's#^<td>Please use <a href=".*"><b>\([A-Za-z0-9-]*\)</a> driver.</td></table></div>$#\1#' \
		| sort | uniq
}

function _parse_printers_primary_models {
	# for each line containing model name
	# -> drop variant name
	# -> extract model name 
	print_page printers | grep '^<p><a name=' | sed -e 's# / [A-Za-z0-9-]*##;s#^<p><a name="\([A-Za-z0-9-]*\)"></a><b>.*</b></p>#\1#' \
		| sort | uniq
}

function _parse_labelers_primary_models {
	# for each line containing model name
	# -> drop variant name
	# -> extract model name 
	print_page labelers | grep '^<p><a name=' | sed -e 's# / [A-Za-z0-9-]*##;s#^<p><a name="\([A-Za-z0-9-]*\)"></a><b>.*</b></p>#\1#' \
		| sort | uniq
}

function _parse_printer_archives {
	# for each line containing model name
	# -> drop variant name for line if exists
	# -> from this line to line containing "Page Top" words used as end marker (drop before, drop after, maximum 100 lines between)
	# ---> for each line containing a file (deb, ppd)
	# -----> extract archive name
	# -----> for each line containing archive name
	# -------> convert end of line char with tab char
	# -------> drop trailing '\t' char
	print_page printers | sed -e 's#\([A-Za-z0-9-]*\) / [A-Za-z0-9-]*#\1#' | grep -A 100 '^<p><a name="'"${1}"'"></a><b>' \
		| grep -m 1 -B 100 '<div align=right><p><a href=".*#top">Page Top</a></p><p>&nbsp;</p></div>' \
		| grep '\.deb\|\.ppd.gz' | sed -e 's#^.*/linux/dlf/\(.*\)\&lang=.*#\1#' | sort \
		| sed -e ':a;N;$!ba;s/\n/\t/g' | sed -e 's/\t$//' \
		| sort | uniq
}

function _parse_labeler_archives {
	# for each line containing model name
	# -> drop variant name for line if exists
	# -> from this line to line containing "Page Top" words used as end marker (drop before, drop after, maximum 100 lines between)
	# ---> for each line containing a file (deb, ppd)
	# -----> extract archive name
	# -----> for each line containing archive name
	# -------> convert end of line char with tab char
	# -------> drop trailing '\t' char
	print_page labelers | sed -e 's#\([A-Za-z0-9-]*\) / [A-Za-z0-9-]*#\1#' | grep -A 100 '^<p><a name="'"${1}"'"></a><b>' \
		| grep -m 1 -B 100 '<div align=right><p><a href=".*#top">Page Top</a></p><p>&nbsp;</p></div>' \
		| grep '\.deb\|\.ppd.gz' | sed -e 's#^.*/linux/dlf/\(.*\)\&lang=.*#\1#' | sort \
		| sed -e ':a;N;$!ba;s/\n/\t/g' | sed -e 's/\t$//' \
		| sort | uniq
}

#############################################################################
##
## Some useful tricks
##

function _true { echo true; true; }

function _false { echo false; false;}

function _actions { echo $_actions; }

function _declare {
	if [ "${_actions}" = '' ]
	then
		_actions="${1}"
	else
		_actions="${_actions} ${1}"
	fi
}

function _describe {
	if [ "${1}" = 'print_help' ]
	then 
		echo "${2}"
		true
	else
		false
	fi
}

function _error {
	if [ "${1}" = '' ]
	then 
		echo "ERR: ${2}" 2>/dev/null
		false
	else
		true
	fi
}

_declare print_help
function print_help {
	_describe "${1}" 'print this help' && return
	printf "${0}:\n\tparse information from ${BROTHER_TRADEMARK} webpages, with many actions ($(_actions))\n\n"
	for action in $(_actions)
	do
		printf "${action}:\n\t$(${action} print_help)\n\n"
	done
}

#############################################################################
##
##  Cache handling
##

function _cache {
	# howto: _cache cache_file command
	if [ -f "${1}" ]
	then
		cat "${1}"
	else
		eval "${@:2:${#@}}" | tee "${1}"
	fi
}

_declare pre_cache
function pre_cache {
	_describe "${1}" 'explicitly precache informations, by default caching is done just in time' && return
	list_archives_with_licenses all > /dev/null
	list_models all > /dev/null
	list_primary_models_with_archives all > /dev/null
	list_archives_urls all > /dev/null
	list_licenses_urls all > /dev/null
	for license in $(list_licenses all)
	do
		print_license "${license}" >/dev/null
	done
	list_cache
}

_declare list_cache
function list_cache {
	_describe "${1}" 'list cached files' && return
	find "${WORKSPACE}" -name '*.txt' -o -name '*.html' | sort
}

_declare drop_cache
function drop_cache {
	_describe "${1}" 'delete cached files' && return
	find "${WORKSPACE}" -name '*.txt' -exec rm -v {} \;
	find "${WORKSPACE}" -name '*.html' -exec rm -v {} \;
}

#############################################################################
##
##  Page handling
##

_declare get_page_url
function get_page_url {
	_describe "${1}" "get page by type (sources printers scanners pcfaxes)" && return
	_error "${1}" 'missing type' || return
	case "${1}" in
		sources)
			echo "${BROTHER_PAGES_DIR}download_src.html"
		;;
		printers)
			echo "${BROTHER_PAGES_DIR}download_prn.html"
		;;
		labelers)
			echo "${BROTHER_PAGES_DIR}download_esp.html"
		;;
		scanners)
			echo "${BROTHER_PAGES_DIR}download_scn.html"
		;;
		pcfaxes)
			echo "${BROTHER_PAGES_DIR}download_pcf.html"
		;;
		*)
			_error '' 'bad type' || return
		;;
	esac
}

function _download_page {
	if [ "${USE_WEBARCHIVE}" = 'true' ]
	then
		wget -q -O - "${WEBARCHIVE_DOMAIN}${WEBARCHIVE_ROOT}${1}" | sed -e 's#\(<a href=".*\)\&amp;#\1\&#g;s#/web/[0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9]/##' | dos2unix
	else
		wget -q -O - "${1}" | dos2unix
	fi
}

_declare print_page
function print_page {
	_describe "${1}" "print page by type (sources printers scanners pcfaxes)" && return
	_error "${1}" 'missing type' || return
	case "${1}" in
		sources|printers|labelers|scanners|pcfaxes)
			brother_page="$(get_page_url "${1}")"
			page="${PAGES_DIR}/${1}.html"
			_cache "${page}" _download_page "${brother_page}"
		;;
		*)
			_error '' 'bad type' || return
		;;
	esac
}

#############################################################################
##
##  Licenses handling
##

function _parse_archives_with_licenses {
	case "${1}" in
		sources|printers|labelers|scanners|pcfaxes)
			"_parse_${1}_archives_with_licenses"
		;;
	esac
}

_declare list_archives_with_licenses
function list_archives_with_licenses {
	_describe "${1}" "list all files with licenses by type (sources printers scanners pcfaxes all)" && return
	_error "${1}" 'missing type' || return
	case "${1}" in
		sources|printers|labelers|scanners|pcfaxes)
			_cache "${LISTS_DIR}${1}_archives_with_licenses.txt" "_parse_archives_with_licenses '${1}'"
		;;
		all)
			_cache "${LISTS_DIR}${1}_archives_with_licenses.txt" '(list_archives_with_licenses sources; list_archives_with_licenses printers; list_archives_with_licenses labelers; list_archives_with_licenses scanners; list_archives_with_licenses pcfaxes) | sort | uniq'
		;;
		*)
			_error '' 'bad type' || return
		;;
	esac
}

_declare list_licenses
function list_licenses {
	_describe "${1}" 'list licenses (sources printers scanners pcfaxes all)' && return
	_error "${1}" 'missing type' || return
	case "${1}" in
		sources|printers|labelers|scanners|pcfaxes)
			_cache "${LISTS_DIR}${1}_licenses.txt" "list_archives_with_licenses ${1} | cut -f 2 | sort | uniq"
		;;
		all)
			_cache "${LISTS_DIR}${1}_licenses.txt" '(list_licenses sources; list_licenses printers; list_licenses labelers; list_licenses scanners; list_licenses pcfaxes) | sort | uniq'
		;;
		*)
			_error '' 'bad type' || return
		;;
	esac
}

_declare is_license
function is_license {
	_describe "${1}" 'check if this license is used' && return
	_error "${1}" 'missing license name' || return
	list_licenses all | grep "^${1}$" >/dev/null && _true || _false
}

_declare get_license_url
function get_license_url {
	_describe "${1}" 'get license url by name' && return
	_error "${1}" 'missing license name' || return
	is_license "${1}" >/dev/null || _error '' 'bad license name' || return
	echo "${BROTHER_LICENSE_DIR}${1}/${BROTHER_LICENSE_FILE}"
}

_declare print_license
function print_license {
	_describe "${1}" 'print license by name' && return
	_error "${1}" 'missing license name' || return
	is_license "${1}" >/dev/null || _error '' 'bad license name' || return
	_cache "${LICENSES_DIR}${1}.html" "wget -q -O - '$(get_license_url "${1}")' | dos2unix"
}

function _list_licenses_urls {
	for license in $(list_licenses "${1}")
	do
		get_license_url "${license}"
	done
}

_declare list_licenses_urls
function list_licenses_urls {
	_describe "${1}" 'list licenses urls (sources printers scanners pcfaxes all)' && return
	_error "${1}" 'missing type' || return
	case "${1}" in
		sources|printers|labelers|scanners|pcfaxes)
			_cache "${LISTS_DIR}${1}_licenses_urls.txt" "_list_licenses_urls ${1} | sort | uniq"
		;;
		all)
			_cache "${LISTS_DIR}${1}_licenses_urls.txt" "(list_licenses_urls sources; list_licenses_urls printers; list_licenses_urls labelers; list_licenses_urls scanners; list_licenses_urls pcfaxes) | sort | uniq"
		;;
		*)
			_error '' 'bad type' || return
		;;
	esac
}

#############################################################################
##
##  Files handling
##

_declare list_archives
function list_archives {
	_describe "${1}" "get files by type (sources printers scanners pcfaxes all)" && return
	# _error by list_archives_with_licenses
	list_archives_with_licenses "${1}" | cut -f 1
}

_declare get_file_license
function get_file_license {
	_describe "${1}" 'get file by archive name' && return
	_error "${1}" 'missing archive name' || return
	list_archives_with_licenses all | grep "^${1}$(printf '\t')" | cut -f 2
}

_declare get_file_url
function get_file_url {
	_describe "${1}" 'get file url by name' && return
	_error "${1}" 'missing archive name' || return
	is_file "${1}" >/dev/null || _error '' 'bad archive name' || return
	echo "${BROTHER_REPOSITORY}${1}"
}

function _list_archives_urls {
	for file in $(list_archives "${1}")
	do
		get_file_url "${file}"
	done
}

_declare list_archives_urls
function list_archives_urls {
	_describe "${1}" "list files urls by type (sources printers scanners pcfaxes all)" && return
	_error "${1}" 'missing type' || return
	case "${1}" in
		sources|printers|labelers|scanners|pcfaxes)
			_cache "${LISTS_DIR}${1}_archives_urls.txt" "_list_archives_urls ${1} | sort | uniq"
		;;
		all)
			_cache "${LISTS_DIR}${1}_archives_urls.txt" '(list_archives_urls sources; list_archives_urls printers; list_archives_urls labelers; list_archives_urls scanners; list_archives_urls pcfaxes) | sort | uniq'
		;;
		*)
			_error '' 'bad type' || return
		;;
	esac
}

_declare is_file
function is_file {
	_describe "${1}" 'check if file exists by name' && return
	_error "${1}" 'missing archive name' || return
	list_archives all | grep "^${1}$" >/dev/null && _true || _false
}

#############################################################################
##
##  Models handling
##

# internals has no many checks because can't be called directly, checks are in caller
function _parse_primary_models {
	case "${1}" in
		printers|labelers)
			"_parse_${1}_primary_models"
		;;
		scanners)
			# STUB
			printf ''
		;;
		pcfaxes)
			# STUB
			printf ''
		;;
	esac
}

_declare list_primary_models
function list_primary_models {
	# all?
	# NOT sources
	_describe "${1}" "list models that have driver by type (printers scanners pcfaxes)" && return
	_error "${1}" 'missing type' || return
	case "${1}" in
		printers|labelers|scanners|pcfaxes)
			_cache "${LISTS_DIR}${1}_primary_models.txt" "_parse_primary_models '${1}'"
		;;
		*)
			_error '' 'bad type' || return
		;;
	esac
}

function _parse_secondary_models_with_primary {
	case "${1}" in
		printers)
			for model in $(_parse_printers_variant_models)
			do
				printf "${model}\t"
				_parse_printers_original_model_from_variant "${model}"
			done
			for model in $(_parse_printers_alternative_models)
			do
				printf "${model}\t"
				_parse_printers_original_model_from_alternative "${model}"
			done
		;;
		labelers)
			# labelers don't have secondary models
			printf ''
		;;
		scanners)
			# STUB
			printf ''
		;;
		pcfaxes)
			# STUB
			printf ''
		;;

	esac
}

_declare list_secondary_models_with_primary
function list_secondary_models_with_primary {
	_describe "${1}" 'list models that uses driver from another model, by type (printers scanners pcfaxes)' && return
	_error "${1}" 'missing type' || return
	case "${1}" in
		printers|labelers|scanners|pcfaxes)
			_cache "${LISTS_DIR}${1}_secondary_models_with_primary.txt" "_parse_secondary_models_with_primary '${1}'"
		;;
		*)
			_error '' 'bad type' || return
		;;
	esac
}

_declare list_secondary_models
function list_secondary_models {
	_describe "${1}" 'list models that uses driver from another model, by type (printers scanners pcfaxes)' && return
	_error "${1}" 'missing type' || return
	case "${1}" in
		printers|labelers|scanners|pcfaxes)
			_cache "${LISTS_DIR}${1}_secondary_models.txt" "list_secondary_models_with_primary '${1}' | cut -f 1 | sort | uniq"
		;;
		*)
			_error 'bad_type' || return
		;;
	esac
}

_declare list_models
function list_models {
	_describe "${1}" 'list models' && return
	_error "${1}" 'missing type (printers scanners pcfaxes all)' || return
	case "${1}" in
		printers|labelers|scanners|pcfaxes)
			_cache "${LISTS_DIR}${1}_models.txt" "(list_primary_models '${1}'; list_secondary_models '${1}') | sort | uniq"
		;;
		all)
			_cache "${LISTS_DIR}${1}_models.txt" '(list_models printers; list_models labelers; list_models scanners; list_models pcfaxes) | sort | uniq'
		;;
		*)
			_error '' 'bad type' || return
		;;
	esac
}

# not all because a secondary fax model can be a primary printer model
_declare is_primary_model
function is_primary_model {
	_describe "${1}" 'check if model is a primary model by type and by name (printers scanners pcfaxes) (model name)' && return
	_error "${1}" 'missing type' || return
	_error "${2}" 'missing model name' || return
	case "${1}" in
		printers|labelers|scanners|pcfaxes)
			list_primary_models "${1}" | grep "^${2}$" >/dev/null && _true || _false
		;;
		*)
			_error '' 'bad type' || return
		;;
	esac
}

_declare is_secondary_model
function is_secondary_model {
	_describe "${1}" 'check if model is a secondary model by type and by name (printers scanners pcfaxes) (model name)' && return
	_error "${1}" 'missing type' || return
	_error "${2}" 'missing model name' || return
	case "${1}" in
		printers|labelers|scanners|pcfaxes)
			list_secondary_models "${1}" | grep "^${2}$" >/dev/null && _true || _false
		;;
		*)
			_error '' 'bad type' || return
		;;
	esac
}

_declare is_model
function is_model {
	_describe "${1}" 'check if model is a true model by type and by name (printers scanners pcfaxes all) (model name)' && return
	_error "${1}" 'missing type' || return
	_error "${2}" 'missing model name' || return
	case "${1}" in
		printers|labelers|scanners|pcfaxes|all)
			list_models "${1}" | grep "^${2}$" >/dev/null && _true || _false
		;;
		*)
			_error '' 'bad type'
		;;
	esac
}

_declare get_primary_model
function get_primary_model {
	_describe "${1}" 'get model that has a driver for this model by type and by name (printers scanners pcfaxes) (model name)' && return
	_error "${1}" 'missing type' || return
	_error "${2}" 'missing model name' || return
	is_model "${1}" "${2}" >/dev/null || _error '' "bad ${1} model name" || return
	is_primary_model "${1}" "${2}" >/dev/null && echo "${2}"
	case "${1}" in
		printers|labelers|scanners|pcfaxes)
			list_secondary_models_with_primary "${1}" | grep "^${2}$(printf '\t')" | cut -f 2
		;;
		*)
			_error '' 'bad type' || return
		;;
	esac
}

function _parse_primary_models_with_archives {
	case "${1}" in
		printers|labelers)
			for model in $(list_primary_models "${1}")
			do
				printf "${model}\t"
				"_parse_$(echo ${1} | sed -e 's/s$//')_archives" "${model}"
			done
		;;
		scanners)
			# STUB
			printf ''
		;;
		pcfaxes)
			# STUB
			printf ''
		;;
	esac
}

_declare list_primary_models_with_archives
function list_primary_models_with_archives {
	_describe "${1}" 'list models with files by type (printers scanners pcfaxes all)' && return
	_error "${1}" 'missing type' || return
	case "${1}" in
		printers|labelers|scanners|pcfaxes)
			_cache "${LISTS_DIR}${1}_primary_models_with_archives.txt" _parse_primary_models_with_archives "${1}"
		;;
		all)
			_cache "${LISTS_DIR}${1}_primary_models_with_archives.txt" '(list_primary_models_with_archives printers; list_primary_models_with_archives labelers; list_primary_models_with_archives scanners; list_primary_models_with_archives pcfaxes) | sort'
		;;
		*)
			_error '' 'bad type' || return
		;;
	esac
}

# TODO: sources?
_declare list_model_archives
function list_model_archives {
	_describe "${1}" 'get files by type and by model (printers scanners pcfaxes all) (model name)' && return
	_error "${1}" 'missing type' || return
	_error "${2}" 'missing model name' || return
	is_model "${1}" "${2}" >/dev/null || _error '' "bad ${1} model name" || return
	case "${1}" in
		printers|labelers|scanners|pcfaxes)
			list_primary_models_with_archives "${1}" | grep "$(get_primary_model "${1}" "${2}")" | cut -f 2- | sed -e 's/\t/\n/'
		;;
		all)
			(	for type in printers labelers scanners pcfaxes
				do
						is_model "${type}" "${2}" >/dev/null && list_model_archives "${type}" "${2}"
				done
			) | sort | uniq
		;;
		*)
			_error '' 'bad type' || return
		;;
	esac
}

# TODO: sources?
_declare list_model_archives_urls
function list_model_archives_urls {
	_describe "${1}" 'list files url by type (printers scanners pcfaxes all)' && return
	_error "${1}" 'missing type' || return
	case "${1}" in
		printers|labelers|scanners|pcfaxes|all)
			for file in $(list_model_archives "${1}" "${2}")
			do
				get_file_url "${file}"
			done
		;;
		*)
			_error '' 'bad type' || return
		;;
	esac
}

#############################################################################
##
##  Downloading
##

function _download {
	wget -nd -c -P "${FILES_DIR}" "${@}" # -nv
}

_declare download_file
function download_file {
	_describe "${1}" 'download file by name' && return
	_error "${1}" 'missing archive name' || return
	_download "$(get_file_url ${1})"
}

_declare download_model_archives
function download_model_archives {
	_describe "${1}" 'get files by type and by model (printers scanners pcfaxes all) (model name)' && return
	_error "${1}" 'missing type' || return
	_error "${2}" 'missing model name' || return
	is_model "${1}" "${2}" >/dev/null || _error '' "bad ${1} model name" || return
	case "${1}" in
		printers|labelers|scanners|pcfaxes|all)
			_download $(list_model_archives_urls "${1}" "${2}")
		;;
		*)
			_error '' 'bad type' || return
		;;
	esac
}

_declare download_archives
function download_archives {
	_describe "${1}" 'download files by type (sources printers scanners pcfaxes all)' && return
	_error "${1}" 'missing type' || return
	case "${1}" in
		sources|printers|labelers|scanners|pcfaxes)
			_download $(list_archives_urls "${1}")
		;;
		all)
			download_archives sources
			download_archives printers
			download_archives labelers
			download_archives scanners
			download_archives pcfaxes
		;;
		*)
			_error '' 'bad type' || return
		;;
	esac
}

#############################################################################
##
##  Main
##

function _main {
	valid_action=false
	for action in $(_actions)
	do
		if [ "${1}" = "${action}" ]
		then
			valid_action=true
			${@} && return 0 || return 1
		fi
	done
	if ! $valid_action
	then
			_error "${1}" 'missing action' && _error '' 'bad action'
			print_help > /dev/stderr
	fi
}

#############################################################################
##
##  Execution
##

_main $@

#EOF
