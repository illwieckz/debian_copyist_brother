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
ext1.ini ext2.ini ext3.ini ext4.ini ext5.ini\
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
	rm -f  ext1.ini
	rm -f  ext2.ini
	rm -f  ext3.ini
	rm -f  ext4.ini
	rm -f  ext5.ini
Brsane3.ini:
	ln -s  ../../../brscan/mk_package3/G4/Brsane3.ini           .
brsanenetdevice3.cfg:
	ln -s  ../../../brscan/mk_package3/G1/brsanenetdevice3.cfg  .
setupSaneScan3:
	ln -s  ../../../brscan/mk_package3/G1/setupSaneScan3        .
ext1.ini:
	cp -p  ../../../brscan/mk_package3/G5/ext1.ini           .
ext2.ini:
	cp -p  ../../../brscan/mk_package3/G5/ext2.ini           .
ext3.ini:
	cp -p  ../../../brscan/mk_package3/G5-2/ext3.ini           .
ext4.ini:
	cp -p  ../../../brscan/mk_package3/G6/ext4.ini           .
ext5.ini:
	cp -p  ../../../brscan/mk_package3/G7/ext5.ini           .
brscan3d:
	sh ../G1/makepackage 3  d
	sh ../G1/makepackage 3  r
# DO NOT DELETE
