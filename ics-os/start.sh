cd contrib
cd reversi
rm reversi.exe
make
make install
cd ..
cd ..
make clean
sudo make install
qemu-system-x86_64 -fda ics-os-floppy.img