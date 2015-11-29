//
//  Transaction.hpp
//  bittrader
//
//  Created by Conor Taylor on 17/11/2015.
//  Copyright © 2015 Conor Taylor. All rights reserved.
//

#ifndef Transaction_hpp
#define Transaction_hpp

#include <iostream>
#include <vector>

#include "Bitstamp.hpp"
#include "Logger.hpp"

using namespace std;

enum TransactionState {
	NO_ACTION, BUY_PENDING, BUY_CONFIRM,
	SELL_WHEN_READY, SELL_PENDING, SELL_CONFIRM
};

class Transaction {
private:
	double _buy_price;
	double _sell_price;
	int buy_id;
	int sell_id;
	double amount;
	Bitstamp api;
	TransactionState _state;
	Logger *logger;
	
public:
	Transaction();
	TransactionState state();
	bool buy(double amount, double price);
	bool sell(double price);
	double buy_price();
	double sell_price();
	void reset();
	void sell_when_ready();
};

#endif /* Transaction_hpp */
