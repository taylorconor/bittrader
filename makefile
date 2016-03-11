build:
	g++ *.cpp -std=c++11 -lpthread -lcurl -o bittrader

clean:
	rm -f *.o
	rm -f bittrader
