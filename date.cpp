#include "date.h"


Date ParseDate(istream& date_stream) {
  int year;
  date_stream >> year;
  date_stream.ignore(1);

  int month;
  date_stream >> month;
  date_stream.ignore(1);

  int day;
  date_stream >> day;
  return Date(year, month, day);
}

bool operator<(const Date& lhs, const Date& rhs) {
  return vector<int>{lhs.GetYear(), lhs.GetMonth(), lhs.GetDay()} <
      vector<int>{rhs.GetYear(), rhs.GetMonth(), rhs.GetDay()};
}

bool operator<=(const Date& lhs, const Date& rhs) {
  return vector<int>{lhs.GetYear(), lhs.GetMonth(), lhs.GetDay()} <=
      vector<int>{rhs.GetYear(), rhs.GetMonth(), rhs.GetDay()};
}

bool operator>(const Date& lhs, const Date& rhs) {
  return vector<int>{lhs.GetYear(), lhs.GetMonth(), lhs.GetDay()} >
      vector<int>{rhs.GetYear(), rhs.GetMonth(), rhs.GetDay()};
}

bool operator>=(const Date& lhs, const Date& rhs) {
  return vector<int>{lhs.GetYear(), lhs.GetMonth(), lhs.GetDay()} >=
      vector<int>{rhs.GetYear(), rhs.GetMonth(), rhs.GetDay()};
}
bool operator==(const Date& lhs, const Date& rhs) {
  return vector<int>{lhs.GetYear(), lhs.GetMonth(), lhs.GetDay()} ==
      vector<int>{rhs.GetYear(), rhs.GetMonth(), rhs.GetDay()};
}

bool operator!=(const Date& lhs, const Date& rhs) {
  return vector<int>{lhs.GetYear(), lhs.GetMonth(), lhs.GetDay()} !=
      vector<int>{rhs.GetYear(), rhs.GetMonth(), rhs.GetDay()};
}

ostream& operator<<(ostream& stream, const Date& date) {
  stream << setw(4) << setfill('0') << date.GetYear() <<
      "-" << setw(2) << setfill('0') << date.GetMonth() <<
      "-" << setw(2) << setfill('0') << date.GetDay();
  return stream;
}
