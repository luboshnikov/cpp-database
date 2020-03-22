#pragma once

#include "date.h"
#include "condition_parser.h"
#include "node.h"

#include <algorithm>
#include <iterator>
#include <map>
#include <vector>
#include <set>
#include <sstream>
#include <functional>

using namespace std;

class Database {
public:
    void Add(const Date& date,const string& event);

    ostream& Print(ostream& os) const ;

    int RemoveIf(function<bool(const Date&, const string&)> predicate);
    vector<string> FindIf(function<bool(Date, string)> predicate) const;
    string Last(const Date& date) const ;
private:
    map<Date, set<string>> storage;
    map<Date, vector<string>> order_of_events;
};
