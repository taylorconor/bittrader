//
//  Trader.hpp
//  bittrader
//
//  Created by Conor Taylor on 13/11/2015.
//  Copyright © 2015 Conor Taylor. All rights reserved.
//

#ifndef Trader_hpp
#define Trader_hpp

#include <iostream>
#include <fstream>
#include <streambuf>
#include <vector>

#include "Bitstamp.hpp"
#include "Logger.hpp"
#include "TransactionPool.hpp"

using namespace std;

#define MOVING_AVERAGE_FREQ	80

class Trader {
private:
	Bitstamp api;
	Logger *logger;
	string path;
	vector<double> *ticks;
	const int frequency;
	double moving_average;
	TransactionPool transaction_pool;
	
	double get_moving_average(int offset);
	MovingAverageStats get_moving_average();
	void get_ticks(vector<double> *ticks, string path);
	void write_tick(double tick);

public:
	Trader(int frequency, string path);

	void update();
};

#endif /* Trader_hpp */
