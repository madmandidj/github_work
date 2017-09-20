cd ./UI
make
cd ..


cd ./CDRProcessor
make
cd ..


cd ./Feeder
make
cd ..


cd ./DataGenerator
make
./TestDataGenerator -n 10000000 -s 7 -o 6
cd ..


cd ./CDRProcessor
gnome-terminal -x ./TestProcMngr -n 40 -s 1000000 -o 100000
cd ..


cd ./Feeder
gnome-terminal -x ./TestFeedMngr -n 5
cd ..


cd ./UI
./TestUIMngr






