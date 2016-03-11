//
//  TransactionPool.hpp
//  bittrader
//
//  Created by Conor Taylor on 17/11/2015.
//  Copyright © 2015 Conor Taylor. All rights reserved.
//

#ifndef TransactionPool_hpp
#define TransactionPool_hpp

#include <stdio.h>
#include <vector>
#include <thread>
#include <algorithm>
#include <unistd.h>
#include "Transaction.hpp"
#include "Bitstamp.hpp"
#include "Logger.hpp"

#define POOL_SIZE			4
#define TRANSACTION_AMOUNT	25
#define UPDATE_FREQUENCY	5

struct MovingAverageStats {
	double prev_avg;
	double avg_max;
	MovingAverageStats() {
		prev_avg = avg_max = 0;
	}
};

class TransactionPool {
private:
	vector<Transaction> pool;
	Bitstamp api;
	Logger *logger;
	vector<int> *open_orders;
	double prev_avg;
	double prev_ask;
	
	void update_open_orders();
	void t_update_open_orders();
	bool is_in_open_orders(int orderid);
	
public:
	TransactionPool();
	TransactionPool(MovingAverageStats avg);
	
	double avg_max;
	
	void update(double ask, double moving_average);
};

#endif /* TransactionPool_hpp */
