//
//  Logger.cpp
//  bittrader
//
//  Created by Conor Taylor on 14/11/2015.
//  Copyright © 2015 Conor Taylor. All rights reserved.
//

#include "Logger.hpp"

// initialise to null here to avoid a linker error
Logger *Logger::_instance = nullptr;

Logger::Logger() {}
Logger *Logger::instance() {
	if (_instance == NULL) {
		_instance = new Logger();
	}
	return _instance;
}

// convert timestamp to string
string Logger::timestr(int ts) {
	time_t tt = ts;
	struct tm *ptm = localtime(&tt);
	char buf[30];
	strftime(buf, 30, "%d/%m/%y %H:%M:%S",  ptm);
	return string(buf);
}

// plain log
void Logger::log(string s) {
	stringstream stream;
	stream << s << endl;
	cout << stream.str();
}

// plain log with timestamp
void Logger::log_ts(string s) {
	int ts = (int)time(0);
	string tstr = timestr(ts);
	log(tstr + ": " + s);
}

// log a tick update
void Logger::log_tick(int ts, double ask, double moving_average) {
	stringstream s;
	s << timestr(ts) << ":\n"
		<< "\task: " << ask << "\n"
		<< "\tmoving average: " << moving_average << "\n";
	log(s.str());
}