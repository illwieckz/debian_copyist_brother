.PHONY: all clean depend 

all: brscan brscan2

clean : clean1 clean2


brscan: \
../backend_brscan/.libs/libsane-brother.so.1.0.7 \
../libbrscandec/libbrscandec.so.1.0.0 \
../libbrcolm/libbrcolm.so.1.0.0 \
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
	rm -rf brscan
	mkdir brscan
	sh makepackage 1  d
	sh makepackage 1  r
	cp -p linux-2.6-intel/brscan-*.*  ../RELEASE

brscan2: \
../backend_brscan2/.libs/libsane-brother2.so.1.0.7 \
../libbrscandec2/libbrscandec2.so.1.0.0 \
../libbrcolm2/libbrcolm2.so.1.0.0 \
./Brsane2.ini \
./ext1.ini \
../libbrcolm2/GrayCmData/ALL/* \
../libbrcolm2/GrayCmData/AL/* \
./setupSaneScan2 \
./brsanenetdevice2.cfg \
./brscan2.list \
../netconfig/brsaneconfig2
	rm -rf brscan2
	sh makepackage 2  d
	sh makepackage 2  r
	mkdir brscan2
	cp -p linux-2.6-intel/brscan2-*.* ../RELEASE


clean1:
	rm -fR  linux-2.6-intel/brscan*
	rm -fR brscan
clean2:
	rm -fR  linux-2.6-intel/brscan2*
	rm -fR brscan2



brscand:
	sh makepackage 1 d
	sh makepackage 1 r

brscan2d:
	sh makepackage 2  d
	sh makepackage 2  r


