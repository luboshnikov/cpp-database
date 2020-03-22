#pragma once

#include "date.h"
#include <memory>

using namespace std;

enum class LogicalOperation {
    And,
    Or
};

enum class Comparison {
    Less, //
    LessOrEqual, //
    Greater, //
    GreaterOrEqual, //
    Equal, //
    NotEqual, //
};

class Node {
public:
    virtual bool Evaluate(const Date& date,const string& event) const = 0;
};

class EmptyNode : public Node {
public:
    bool Evaluate(const Date& date,const string& event) const override {
        return true;
    }
};

class DateComparisonNode : public Node {
public:
    DateComparisonNode(const Comparison& cmp,const Date& date)
        : cmp_node(cmp)
        , date_node(date) {}
    bool Evaluate(const Date& date,const string& event) const override {
        switch (cmp_node) {
        case Comparison::Less:
            return date < date_node;
        case Comparison::LessOrEqual:
            return date <= date_node;
        case Comparison::Greater:
            return date > date_node;
        case Comparison::GreaterOrEqual:
            return date >= date_node;
        case Comparison::Equal:
            return date == date_node;
        case Comparison::NotEqual:
            return date != date_node;
        }
    }
private:
    const Comparison cmp_node;
    const Date date_node;
};

class EventComparisonNode : public Node {
public:
    EventComparisonNode(const Comparison& cmp, const string& event)
        : cmp_node(cmp)
        , event_node(event) {}

    bool Evaluate(const Date& date,const string& event) const override {
        switch (cmp_node) {
        case Comparison::Less:
            return event < event_node;
        case Comparison::LessOrEqual:
            return event <= event_node;
        case Comparison::Greater:
            return event > event_node;
        case Comparison::GreaterOrEqual:
            return event >= event_node;
        case Comparison::Equal:
            return event == event_node;
        case Comparison::NotEqual:
            return event != event_node;
        }
    }
private:
    const Comparison cmp_node;
    const string event_node;
};

class LogicalOperationNode : public Node {
public:
    LogicalOperationNode(const LogicalOperation& lo , const shared_ptr<Node> left, const shared_ptr<Node> right)
        : lo_node(lo)
        , left_node(left)
        , right_node(right) {}
    bool Evaluate(const Date& date,const string& event) const override {
        switch (lo_node){
        case LogicalOperation::And:
            return left_node->Evaluate(date, event) && right_node->Evaluate(date, event);
        case LogicalOperation::Or:
            return left_node->Evaluate(date, event) || right_node->Evaluate(date, event);
        }
    }
private:
    const LogicalOperation lo_node;
    const shared_ptr<Node> left_node;
    const shared_ptr<Node> right_node;
};

void TestDateComparisonNode();
void TestEventComparisonNode();
void TestLogicalOperationNode();
