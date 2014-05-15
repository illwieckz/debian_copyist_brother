
.PHONY: all clean depend 

all: brsaneconfig brsaneconfig2

clean: clean1 clean2

brscan: brsaneconfig

brscan2: brsaneconfig2


brsaneconfig: brsaneconfig.c ../backend_src/brother_advini.c ../backend_src/brother_advini.h resource.h
	sh mk_brsaneconfig 1


brsaneconfig2: brsaneconfig.c ../backend_src/brother_advini.c ../backend_src/brother_advini.h resource.h
	sh mk_brsaneconfig 2

clean1:
	rm -f brsaneconfig

clean2:
	rm -f brsaneconfig2

depend:
	makedepend brsaneconfig.c ../backend_src/brother_advini.c
