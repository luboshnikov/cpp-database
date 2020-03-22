#include "database.h"
#include "date.h"
#include "condition_parser.h"
#include "node.h"
#include "test_runner.h"

#include <iostream>
#include <stdexcept>
#include <sstream>

using namespace std;

string ParseEvent(istream& is) {
    string s;
    while (is.peek() == ' '){
        is.ignore(1);
    }
    getline(is, s);
    return s;
}

void TestAll();

int main() {
  TestAll();

  Database db;

  for (string line; getline(cin, line); ) {
    istringstream is(line);

    string command;
    is >> command;
    if (command == "Add") {
      const auto date = ParseDate(is);
      const auto event = ParseEvent(is);
      db.Add(date, event);
    } else if (command == "Print") {
      db.Print(cout);
    } else if (command == "Del") {
      auto condition = ParseCondition(is);
      auto predicate = [condition](const Date& date, const string& event) {
        return condition->Evaluate(date, event);
      };
      int count = db.RemoveIf(predicate);
      cout << "Removed " << count << " entries" << endl;
    } else if (command == "Find") {
      auto condition = ParseCondition(is);
      auto predicate = [condition](const Date& date, const string& event) {
        return condition->Evaluate(date, event);
      };

      const auto entries = db.FindIf(predicate);
      for (const auto& entry : entries) {
        cout << entry << endl;
      }
      cout << "Found " << entries.size() << " entries" << endl;
    } else if (command == "Last") {
      try {
          cout << db.Last(ParseDate(is)) << endl;
      } catch (invalid_argument&) {
          cout << "No entries" << endl;
      }
    } else if (command.empty()) {
      continue;
    } else {
      throw logic_error("Unknown command: " + command);
    }
  }

  return 0;
}

void TestDel () {
  istringstream empty_is("");
  auto empty_condition = ParseCondition(empty_is);
  auto empty_predicate = [empty_condition](const Date& date, const string& event) {
    return empty_condition->Evaluate(date, event);
  };
{
    Database db;
    db.Add({2018, 1, 1}, "e1");
    db.Add({2018, 1, 1}, "e2");
    db.Add({2018, 1, 1}, "e3");
    db.Add({2018, 1, 1}, "e4");
    istringstream is("date == 2018-01-01");
    auto condition = ParseCondition(is);
    auto predicate = [condition](const Date& date, const string& event) {
      return condition->Evaluate(date, event);
    };
    AssertEqual(db.RemoveIf(predicate), 4, "Db Del 1");
  }
  {
    Database db;
    db.Add({2019, 1, 1}, "e1");
    db.Add({2019, 1, 1}, "e2");
    db.Add({2018, 1, 7}, "e3");
    db.Add({2018, 1, 7}, "e4");
    istringstream is("date >= 2018-01-07 AND date <= 2020-01-01");
    auto condition = ParseCondition(is);
    auto predicate = [condition](const Date& date, const string& event) {
      return condition->Evaluate(date, event);
    };
    AssertEqual(db.RemoveIf(predicate), 4, "Db Del 2");
  }
  {
    Database db;
    db.Add({2019, 1, 1}, "e1");
    db.Add({2019, 1, 1}, "e2");
    db.Add({2018, 1, 7}, "e3");
    db.Add({2018, 1, 7}, "e4");
    AssertEqual(db.RemoveIf(empty_predicate), 4, "Db Del 3");
  }
  {
    Database db;
    db.Add({2019, 1, 1}, "e1");
    db.Add({2019, 1, 1}, "e2");
    db.Add({2018, 1, 7}, "e3");
    db.Add({2018, 1, 7}, "e4");
    istringstream is(R"(event == "e1")");
    auto condition = ParseCondition(is);
    auto predicate = [condition](const Date& date, const string& event) {
      return condition->Evaluate(date, event);
    };
    AssertEqual(db.RemoveIf(predicate), 1, "Db Del 4");
  }

  {
    Database db;
    db.Add({2019, 1, 1}, "e1");
    db.Add({2019, 1, 1}, "e2");
    db.Add({2018, 1, 7}, "e3");
    db.Add({2018, 1, 7}, "e4");
    istringstream is(R"(event == "e1" OR date == 2019-01-01)");
    auto condition = ParseCondition(is);
    auto predicate = [condition](const Date& date, const string& event) {
      return condition->Evaluate(date, event);
    };
    AssertEqual(db.RemoveIf(predicate), 2, "Db Del 5");
  }
}

void TestParseEvent() {
  {
    istringstream is("event");
    AssertEqual(ParseEvent(is), "event", "Parse event without leading spaces");
  }
  {
    istringstream is("   sport event ");
    AssertEqual(ParseEvent(is), "sport event ", "Parse event with leading spaces");
  }
  {
    istringstream is("  first event  \n  second event");
    vector<string> events;
    events.push_back(ParseEvent(is));
    events.push_back(ParseEvent(is));
    AssertEqual(events, vector<string>{"first event  ", "second event"}, "Parse multiple events");
  }
}

void TestDbAdd() {
    {
        Database db;
        db.Add({2017, 1, 1}, "new year");
        db.Add({2017, 1, 7}, "xmas");
        ostringstream out;
        db.Print(out);
        AssertEqual("2017-01-01 new year\n2017-01-07 xmas\n", out.str(), "straight ordering");
    }
    {
        Database db;
        db.Add({2017, 1, 1}, "new year");
        db.Add({2017, 1, 1}, "holiday");
        ostringstream out;
        db.Print(out);
        AssertEqual("2017-01-01 new year\n2017-01-01 holiday\n", out.str(), "several in one day");
    }
    {
        Database db;
        db.Add({2017, 1, 7}, "xmas");
        db.Add({2017, 1, 1}, "new year");
        ostringstream out;
        db.Print(out);
        AssertEqual("2017-01-01 new year\n2017-01-07 xmas\n", out.str(), "reverse ordering");
    }
    {
        Database db;
        db.Add({2017, 1, 1}, "new year");
        db.Add({2017, 1, 1}, "new year");
        db.Add({2017, 1, 1}, "xmas");
        db.Add({2017, 1, 1}, "new year");
        ostringstream out;
        db.Print(out);
        AssertEqual("2017-01-01 new year\n2017-01-01 xmas\n", out.str(), "uniq adding");
    }
}

void TestAll() {
  TestRunner tr;
  tr.RunTest(TestParseEvent, "TestParseEvent");
  tr.RunTest(TestParseCondition, "TestParseCondition");
  tr.RunTest(TestDel, "TestDel");
  tr.RunTest(TestDbAdd, "TestDbAdd");
}
