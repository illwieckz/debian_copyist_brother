Generated PPDs and Filters
==========================

304 cupswrappers writes 304 filters and 197 ppds, those cupswrappers are launched at ``post-inst`` time.
Some package genrate both filter and ppd, other embed the ppd and generate the filter.

Those script manage services and restart them, check packages dependencies, check connected printers… they can't be launched easily to extract content, so they was rewritted to delete useless parts and keep only the generation code.

You can find the original cupswrappers in the “[cupswrappers_original](../material/abstract/printers/cupswrappers_original)” directory, and the modified cupswrappers in the “[cupswrappers_modified](../material/abstract/printers/cupswrappers_modified)” directory.

After that, embedded ppds and filters were extracted to the “[generated_content/ppd](../material/abstract/printers/generated_content/ppd)” and the “[generated_content/filter](../material/abstract/printers/generated_content/filter)” directories. Those files are owned by Brother and distributed under GPLv2 license.
