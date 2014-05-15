.PHONY: all clean depend 

all: brscan3

clean : clean3


brscan3: \
../../backend_brscan3/.libs/libsane-brother3.so.1.0.7 \
../../libbrscandec2/libbrscandec2.so.1.0.0 \
./Brsane3.ini \
setupSaneScan3 \
brsanenetdevice3.cfg \
brscan3.list \
../../netconfig3/brsaneconfig3
	rm -f brscan3
	sh ../G1/makepackage 3 d
	sh ../G1/makepackage 3 r
	mkdir -p ../RELEASE
	cp -p linux-2.6-amd64/brscan3-*.deb ../RELEASE
	cp -p linux-2.6-x86_64/brscan3-*.rpm ../RELEASE



clean3:
	rm -fR  linux-2.6-x86_64/brscan3*
	rm -fR  linux-2.6-amd64/brscan3*
	rm -fR brscan3
	rm -f  Brsane3.ini           
	rm -f  brsanenetdevice3.cfg  
	rm -f  setupSaneScan3      


Brsane3.ini:
	ln -s  ../../../brscan/mk_package3/G1/Brsane3.ini           .
brsanenetdevice3.cfg:
	ln -s  ../../../brscan/mk_package3/G1/brsanenetdevice3.cfg  .
setupSaneScan3:
	ln -s  ../../../brscan/mk_package3/G1/setupSaneScan3        .

brscan3d:
	sh ../G1/makepackage 3  d
	sh ../G1/makepackage 3  r
# DO NOT DELETE
