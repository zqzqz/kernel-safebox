
sudo rmmod ksafeboox
make clean
make all
# cd ../client
# make clean
# make
# cd -
sudo insmod ksafebox.ko
sudo dmesg -C
echo "=================== list directory ==================="
ls ../box
cat ../box/tmp
# cd -
# ./client
# cd -
sudo rmmod ksafebox
echo "=================== kernel log ==================="
sudo dmesg -k
