//
//  main.cpp
//  bittrader
//
//  Created by Conor Taylor on 12/11/2015.
//  Copyright © 2015 Conor Taylor. All rights reserved.
//

#include <iostream>
#include <thread>
#include "Trader.hpp"

#define FREQUENCY	5
#define TICKER_LOC	"/Users/Conor/Desktop/tick_history.txt"

int main(int argc, const char * argv[]) {
	Trader *trader = new Trader(FREQUENCY, TICKER_LOC);
	while (1) {
		thread t(&Trader::update, trader);
		t.detach();
		int remaining = FREQUENCY;
		while (remaining > 0)
			remaining = sleep(remaining);
	}
    return 0;
}
