//
//  Transaction.cpp
//  bittrader
//
//  Created by Conor Taylor on 17/11/2015.
//  Copyright © 2015 Conor Taylor. All rights reserved.
//

#include "Transaction.hpp"

Transaction::Transaction() {
	this->logger = Logger::instance();
}

// get transaction state
TransactionState Transaction::state() {
	return _state;
}

// submit buy request on this transaction
bool Transaction::buy(double amount, double price) {
	int id = api.buy(amount, price);
	if (id < 0)
		return false;
	this->_buy_id = id;
	this->amount = amount;
	this->_buy_price = price;
	this->_state = BUY_PENDING;
	logger->log_ts("Placing buy order for $"+to_string(amount)+" at $"+to_string(price)+"/BTC. id="+to_string(this->_buy_id));
	return true;
}

// submit sell request on this transaction
bool Transaction::sell(double price) {
	int id = api.sell(this->amount, price);
	if (id < 0)
		return false;
	this->_sell_id = id;
	this->_sell_price = price;
	this->_state = SELL_PENDING;
	logger->log_ts("Placing sell order for $"+to_string(this->amount)+" at $"+to_string(price)+"/BTC. id="+to_string(this->_sell_id));
	return true;
}

// get the buy price (only valid if _state >= BUY_PENDING)
double Transaction::buy_price() {
	return this->_buy_price;
}

// get the sell price (only valid if _state >= SELL_PENDING)
double Transaction::sell_price() {
	return this->_sell_price;
}

// get the buy id (only valid if _state >= BUY_PENDING)
int Transaction::buy_id() {
	return _buy_id;
}

// get the sell id (only valid if _state >= SELL_PENDING)
int Transaction::sell_id() {
	return _sell_id;
}

// reset the transaction to before any requests have been made
void Transaction::reset() {
	_state = NO_ACTION;
}

void Transaction::buy_confirm() {
	if (_state == BUY_PENDING)
		_state = BUY_CONFIRM;
}

// state update special case
void Transaction::sell_when_ready() {
	if (_state == BUY_CONFIRM)
		_state = SELL_WHEN_READY;
}
