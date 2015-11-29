//
//  Trader.cpp
//  bittrader
//
//  Created by Conor Taylor on 13/11/2015.
//  Copyright © 2015 Conor Taylor. All rights reserved.
//

#include "Trader.hpp"

Trader::Trader(int frequency, string path) : frequency(frequency) {
	this->path = path;
	this->ticks = new vector<double>();
	get_ticks(this->ticks, path);
	this->logger = Logger::instance();
	this->transaction_pool = TransactionPool(get_moving_average());
}

// calculate moving average for a single offset from the end
double Trader::get_moving_average(int offset) {
	int size = (int)ticks->size();
	// fail if there's not enough ticks in the history to calculate
	// the moving average for this offset
	if (size < offset+MOVING_AVERAGE_FREQ) {
		return -1;
	}
	
	double sum = 0;
	for (int i = size-1-offset; i > size-MOVING_AVERAGE_FREQ-1-offset; i--) {
		sum += ticks->at(i);
	}
	return sum/MOVING_AVERAGE_FREQ;
}

// calculate a full moving average (only called on init since
// it's expensive)
MovingAverageStats Trader::get_moving_average() {
	MovingAverageStats a;
	for (int i = 0; i < MOVING_AVERAGE_FREQ; i++) {
		double avg = get_moving_average(i);
		if (avg < 0)
			continue;
		if (avg > a.avg_max) {
			a.avg_max = avg;
		}
		if (i == MOVING_AVERAGE_FREQ-1) {
			a.prev_avg = avg;
		}
	}
	return a;
}

// write a new tick back to the log file
void Trader::write_tick(double tick) {
	std::ofstream out;
	out.open(path, std::ios::app);
	out << "," << tick;
	out.close();
}

// retrieve all ticks from the log file
void Trader::get_ticks(vector<double> *ticks, string path) {
	ifstream t(path);
	string str((istreambuf_iterator<char>(t)), istreambuf_iterator<char>());
	
	string buf = "";
	for (char &c : str) {
		if (c == ',') {
			ticks->push_back(stod(buf));
			buf = "";
		} else {
			buf += c;
		}
	}
	ticks->push_back(stod(buf));
}

// retrieve and process a new tick
void Trader::update() {
	Tick t = api.tick();
	if (t.error) {
		return;
	}
	ticks->push_back(t.ask);
	
	moving_average = get_moving_average(0);
	transaction_pool.update(t.ask, moving_average);
	
	// keep the vector at a constant size
	if (ticks->size() > MOVING_AVERAGE_FREQ) {
		vector<double>(ticks->end()-MOVING_AVERAGE_FREQ, ticks->end()).swap(*ticks);
	}
	
	write_tick(t.ask);
	logger->log_tick(t.ts, t.ask, moving_average);
}