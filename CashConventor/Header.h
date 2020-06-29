#pragma once

#include <iostream>
#include <windows.h>
#include <string>
#include <regex>
#include <winhttp.h>
#include <map>
#include <vector>
#include <math.h>
#include <commctrl.h>//for droplist
#include <stdio.h>
#include <sqlite3.h>
#include <ctime>

using namespace std;

bool dirExists(const string&); // function which check folder
map<string, double> GetValueFromHTTP();


enum clientElement { button, startMoney, droplist, aboutApp, rateHist};// user elements 

const regex regNum("^[0-9]*$");//regular expression which needs for input fild    
