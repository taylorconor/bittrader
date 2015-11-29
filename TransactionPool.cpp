//
//  TransactionPool.cpp
//  bittrader
//
//  Created by Conor Taylor on 17/11/2015.
//  Copyright © 2015 Conor Taylor. All rights reserved.
//

#include "TransactionPool.hpp"

TransactionPool::TransactionPool() {}
TransactionPool::TransactionPool(MovingAverageStats avg) {
	this->prev_avg = avg.prev_avg;
	this->avg_max = avg.avg_max;
	for (int i = 0; i < POOL_SIZE; i++) {
		pool.push_back(Transaction());
	}
	this->logger = Logger::instance();
	this->open_orders = new vector<int>();
	this->update_open_orders();
}

void TransactionPool::update_open_orders() {
	thread t(&TransactionPool::t_update_open_orders, this);
	t.detach();
}

void TransactionPool::t_update_open_orders() {
	while (true) {
		vector<int> *old_orders = open_orders;
		open_orders = api.open_orders();
		delete old_orders;
		int remaining = UPDATE_FREQUENCY;
		while (remaining > 0)
			remaining = sleep(remaining);
	}
}

// check if the given orderid is in the list of open orders
bool TransactionPool::is_in_open_orders(int orderid) {
	if (find(open_orders->begin(), open_orders->end(), orderid) != open_orders->end()) {
		return true;
	}
	return false;
}

// process new tick
void TransactionPool::update(double ask, double moving_average) {
	bool update_max_avg = true;
	
	for (int i = 0; i < pool.size(); i++) {
		switch (pool[i].state()) {
			case NO_ACTION:
				// decide when best to buy
				if (moving_average < avg_max-10) {
					avg_max = moving_average;
					bool success = pool[i].buy(TRANSACTION_AMOUNT, ask);
					if (success)
						update_max_avg = false;
				}
				break;
			case BUY_PENDING:
				// wait for buy confirmation
				if (is_in_open_orders(pool[i].buy_id())) {
					logger->log("buy confirmed! id="+to_string(pool[i].buy_id()));
					pool[i].buy_confirm();
				}
				break;
			case BUY_CONFIRM:
				// decide when best to sell
				if (ask > pool[i].buy_price()+8) {
					pool[i].sell_when_ready();
				}
				break;
			case SELL_WHEN_READY:
				// sell immediately if ticker starts to fall
				if (ask > pool[i].buy_price()+8
					&& prev_avg > moving_average) {
					pool[i].sell(ask);
				}
				break;
			case SELL_PENDING:
				// wait for sell confirmation
				if (is_in_open_orders(pool[i].sell_id())) {
					logger->log("sell confirmed! id="+to_string(pool[i].sell_id()));
				} else {
					break;
				}
			case SELL_CONFIRM:
				// reset the transaction so it can be used again
				pool[i].reset();
				break;
		}
	}

	if (update_max_avg && moving_average > avg_max) {
		avg_max = moving_average;
	}
	
	prev_avg = moving_average;
	prev_ask = ask;
}