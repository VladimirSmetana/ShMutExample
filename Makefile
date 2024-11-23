all: writer reader

writer: writer.cpp common.h
	g++ -std=c++17 -Wall -o writer writer.cpp -lpthread -lrt

reader: reader.cpp common.h
	g++ -std=c++17 -Wall -o reader reader.cpp -lpthread -lrt

clean:
	rm -f writer reader
	shm_unlink /singleton_shm