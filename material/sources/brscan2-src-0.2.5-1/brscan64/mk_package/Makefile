.PHONY: all clean depend 

all: brscan2

clean : clean2


brscan: \
../backend_brscan/.libs/libsane-brother.so.1.0.7 \
../libbrscandec/libbrscandec.so.1.0.0 \
../libbrcolm/libbrcolm.so.1.0.1 \
./Brsane.ini \
./brsanenetdevice.cfg \
../netconfig/brsaneconfig \
../libbrcolm/GrayCmData/BHL/* \
../libbrcolm/GrayCmData/BHL2/* \
../libbrcolm/GrayCmData/BHL2FB/* \
../libbrcolm/GrayCmData/BHLFB/* \
../libbrcolm/GrayCmData/BHMFB/* \
../libbrcolm/GrayCmData/BHminiFB/* \
../libbrcolm/GrayCmData/YL4/* \
../libbrcolm/GrayCmData/YL4FB/* \
../libbrcolm/GrayCmData/ZL2/* \
../libbrcolm/GrayCmData/ZL2FB/* \
../libbrcolm/GrayCmData/ZLe/* \
../libbrcolm/GrayCmData/ZLeFB/* \
./brscan.list \
./setupSaneScan
	rm -f brscan
	sh makepackage 1 r
	sh makepackage 1 d
	mkdir -p ../RELEASE
	cp -p linux-2.6-amd64/brscan-*.deb  ../RELEASE
	cp -p linux-2.6-x86_64/brscan-*.rpm ../RELEASE

brscan2: \
../backend_brscan2/.libs/libsane-brother2.so.1.0.7 \
../libbrscandec2/libbrscandec2.so.1.0.0 \
../libbrcolm2/libbrcolm2.so.1.0.1 \
./Brsane2.ini \
./ext1.ini \
./ext2.ini \
../libbrcolm2/GrayCmData/ALL/* \
../libbrcolm2/GrayCmData/AL/* \
./setupSaneScan2 \
./brsanenetdevice2.cfg \
./brscan2.list \
../netconfig/brsaneconfig2
	rm -f brscan2
	sh makepackage 2 d
	sh makepackage 2 r
	mkdir -p ../RELEASE
	cp -p linux-2.6-amd64/brscan2-*.deb ../RELEASE
	cp -p linux-2.6-x86_64/brscan2-*.rpm ../RELEASE


clean1:
	rm -fR  linux-2.6-amd64/brscan*
	rm -fR  linux-2.6-x86_64/brscan*
	rm -fR brscan
	rm -f  Brsane.ini            
	rm -f  brsanenetdevice.cfg   
	rm -f  setupSaneScan         

clean2:
	rm -fR  linux-2.6-x86_64/brscan2*
	rm -fR  linux-2.6-amd64/brscan2*
	rm -fR brscan2
	rm -f  Brsane2.ini           
	rm -f  ext1.ini     
	rm -f  ext2.ini     
	rm -f  brsanenetdevice2.cfg  
	rm -f  setupSaneScan2        

Brsane.ini brsanenetdevice.cfg setupSaneScan:
	ln -s  ../../brscan/mk_package/Brsane.ini            
	ln -s  ../../brscan/mk_package/brsanenetdevice.cfg   
	ln -s  ../../brscan/mk_package/setupSaneScan         

Brsane2.ini ext1.ini ext2.ini brsanenetdevice2.cfg setupSaneScan2:
	ln -s  ../../brscan/mk_package/Brsane2.ini           
	ln -s  ../../brscan/mk_package/ext1.ini            
	ln -s  ../../brscan/mk_package/ext2.ini            
	ln -s  ../../brscan/mk_package/brsanenetdevice2.cfg  
	ln -s  ../../brscan/mk_package/setupSaneScan2        

brscand:
	sh makepackage 1 d
	sh makepackage 1 r
brscan2d:
	sh makepackage 2  d
	sh makepackage 2  r
# DO NOT DELETE
