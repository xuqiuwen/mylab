init:
	g++ -o ./build/init.out ./src/init.cpp ./src/DSMgr.cpp
	cd build; ./init.out
test:
	g++ -o ./build/test.out ./src/test.cpp ./src/BMgr.cpp ./src/DSMgr_fast.cpp
	cd build; ./test.out
clean:
	rm ./build/init.out ./build/test.out ./build/data.dbf