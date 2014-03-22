web_brother.sh
==============

The `web_brother.sh` tool helps to query Brother™ web pages: check licenses, list models, list driver files and urls…
Currently this tools only handles complete printers information (some stubs remaining for scanners and pcfaxes).
This tool is self-documented.


```sh
luser@debian tools $ ./web_brother.sh list_models printers | wc -l
391
```

```sh
luser@debian tools $ ./web_brother.sh list_secondary_models print_help
list models that uses driver from another model, by type (printers scanners pcfaxes)
```

```sh
luser@debian tools $ ./web_brother.sh list_secondary_models printers | head -n 2
DCP-115C
DCP-117C
```

```sh
luser@debian tools $ ./web_brother.sh get_primary_model printers DCP-115C
MFC-210C
```

```sh
luser@debian tools $ ./web_brother.sh list_model_files printers DCP-115C
cupswrapperMFC210C-1.0.2-3.i386.deb
mfc210clpr-1.0.2-1.i386.deb
```

This tool cache all computed informations

```sh
luser@debian tools $ time ./web_brother.sh list_primary_models_with_files printers | head -n 2
DCP-110C	cupswrapperDCP110C-1.0.2-3.i386.deb	dcp110clpr-1.0.2-1.i386.deb
DCP-130C	dcp130ccupswrapper-1.0.1-1.i386.deb	dcp130clpr-1.0.1-1.i386.deb

real	0m1.200s

luser@debian tools $ time ./web_brother.sh list_primary_models_with_files printers | head -n 2
DCP-110C	cupswrapperDCP110C-1.0.2-3.i386.deb	dcp110clpr-1.0.2-1.i386.deb
DCP-130C	dcp130ccupswrapper-1.0.1-1.i386.deb	dcp130clpr-1.0.1-1.i386.deb

real	0m0.024s
```

You can explicitly precache (by default this tool caches just in time) or drop the cache.

```sh
luser@debian tools $ ./web_brother.sh drop_cache >/dev/null
luser@debian tools $ ./web_brother.sh pre_cache >/dev/null
```

This tool allows you to download files by type or by model, but this functionnality will be moved to another tool.

The tool is self-documented, for more information, invoke 'print_help' keyword

```sh
luser@debian tools $ ./web_brother.sh print_help | head -n 2
./web_brother.sh:
	parse information from Brother™ webpages, with many actions (print_help pre_cache list_cache drop_cache get_page_url print_page list_files_with_licenses list_licenses is_license get_license_url print_license list_licenses_urls list_files get_file_license get_file_url list_files_urls is_file list_primary_models list_secondary_models_with_primary list_secondary_models list_models is_primary_model is_secondary_model is_model get_primary_model list_primary_models_with_files list_model_files list_model_files_urls download_file download_model_files download_files)

luser@debian tools $ ./web_brother.sh list_primary_models_with_files print_help
list models with files by type (printers scanners pcfaxes all)
```
