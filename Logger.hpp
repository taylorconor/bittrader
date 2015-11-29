//
//  Logger.hpp
//  bittrader
//
//  Created by Conor Taylor on 14/11/2015.
//  Copyright © 2015 Conor Taylor. All rights reserved.
//

#ifndef Logger_hpp
#define Logger_hpp

#include <iostream>
#include <sstream>
#include "time.h"

using namespace std;

class Logger {
private:
	static Logger *_instance;
	Logger();
	
	
	string timestr(int ts);
	
public:
	static Logger *instance();
	
	void log(string s);
	void log_ts(string s);
	void log_tick(int ts, double ask, double moving_average);
};

#endif /* Logger_hpp */
