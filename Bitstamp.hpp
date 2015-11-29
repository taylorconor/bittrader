//
//  Bitstamp.hpp
//  bittrader
//
//  Created by Conor Taylor on 12/11/2015.
//  Copyright © 2015 Conor Taylor. All rights reserved.
//

#ifndef Bitstamp_hpp
#define Bitstamp_hpp

#include <iostream>
#include <fstream>
#include <vector>
#include <curl/curl.h>
#include <tuple>
#include "rapidjson/document.h"
#include "Bitstamp.hpp"

using namespace std;

#define PYTHON_LOC	string("../../bittrader/python")

struct Tick {
	int error;
	int ts;
	double ask;
	
	Tick(int ts, double ask) {
		this->ts = ts;
		this->ask = ask;
		this->error = 0;
	}
	Tick(int error) {
		this->error = error;
	}
};

class Bitstamp {
private:
	static size_t WriteCallback(void *contents, size_t size,
								size_t nmemb, void *userp);
	
	string get(string url);
	std::string exec(const char* cmd);
	
public:
	Tick tick();
	int buy(double amount, double price);
	int sell(double amount, double price);
	vector<int> *open_orders();
	
};

#endif /* Bitstamp_hpp */
