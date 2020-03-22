#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <iostream>

using namespace std;


class Date {
public:
  Date(const int& new_year, const int& new_month,const int& new_day)
    : year(new_year)
    , month(new_month)
    , day(new_day) { }

  int GetYear() const {
    return year;
  }
  int GetMonth() const {
    return month;
  }
  int GetDay() const {
    return day;
  }
private:
  const int year;
  const int month;
  const int day;
};

Date ParseDate(istream& date_stream);

bool operator<(const Date& lhs, const Date& rhs);

bool operator<=(const Date& lhs, const Date& rhs);

bool operator>(const Date& lhs, const Date& rhs);

bool operator>=(const Date& lhs, const Date& rhs);
bool operator==(const Date& lhs, const Date& rhs);

bool operator!=(const Date& lhs, const Date& rhs);

ostream& operator<<(ostream& stream, const Date& date);
