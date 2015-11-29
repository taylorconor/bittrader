//
//  Bitstamp.cpp
//  bittrader
//
//  Created by Conor Taylor on 12/11/2015.
//  Copyright © 2015 Conor Taylor. All rights reserved.
//

#include "Bitstamp.hpp"

// allows curl to write responses to string rather than to stdout
size_t Bitstamp::WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}

// send get request (native)
string Bitstamp::get(string url) {
	CURL *curl;
	CURLcode res;
	string readBuffer;
	
	curl = curl_easy_init();
	if(curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		
		return readBuffer;
	}
	return 0;
}

// execute system command, return in string
std::string Bitstamp::exec(const char* cmd) {
	string s = string(cmd) + " > output.tmp";
	int status = system(s.c_str());
	if (status != 0) {
		return "ERROR";
	}
	
	ifstream t(PYTHON_LOC+"/output.tmp");
	string str((std::istreambuf_iterator<char>(t)),
					std::istreambuf_iterator<char>());
	return str;
}

// get tick information
Tick Bitstamp::tick() {
	string s = this->get("https://www.bitstamp.net/api/ticker/");
	rapidjson::Document d;
	d.Parse(s.c_str());
	if (s == "" || !d.HasMember("ask")) {
		Tick t(1);
		return t;
	}
	string ask = d["ask"].GetString();
	string ts = d["timestamp"].GetString();

	Tick t(stoi(ts), stod(ask));
	return t;
}

// submit buy request
int Bitstamp::buy(double amount, double price) {
	char r_amount[20], r_price[20];
	sprintf(r_amount, "%.8f", amount);
	sprintf(r_price, "%.2f", price);
	
	string cmd = "cd "+PYTHON_LOC+" && python buy.py "+string(r_amount)+" "+string(r_price);
	
	string id = exec(cmd.c_str());
	if (id == "ERROR")
		return -1;
	
	return atoi(id.c_str());
}

// submit sell request
int Bitstamp::sell(double amount, double price) {
	char r_amount[20], r_price[20];
	sprintf(r_amount, "%.8f", amount);
	sprintf(r_price, "%.2f", price);
	
	string cmd = "cd "+PYTHON_LOC+" && python sell.py "+string(r_amount)+" "+string(r_price);
	
	string id = exec(cmd.c_str());
	if (id == "ERROR")
		return -1;
	
	return atoi(id.c_str());
}

vector<int> *Bitstamp::open_orders() {
	string cmd = "cd "+PYTHON_LOC+" && python orders.py";
	string res = exec(cmd.c_str());
	vector<int> *orders = new vector<int>();
	string num;
	for (int i = 0, done = false; i < res.length() && !done; i++) {
		switch (res[i]) {
		case '\n':
				done = true;
		case ',':
				orders->push_back(atoi(num.c_str()));
				num = "";
				break;
		default:
				num = num + res[i];
		}
	}
	return orders;
}
