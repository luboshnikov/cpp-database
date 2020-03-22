#include "database.h"
#include <string>

void Database::Add(const Date& date,const string& event){
    if (storage[date].find(event) == storage[date].end()) {
            order_of_events[date].push_back(event);
        }
    storage[date].insert(event);
}

ostream& Database::Print(ostream& os) const {
    for(auto i = order_of_events.begin(); i != order_of_events.end(); i++){
        for(auto j : i->second){
            os << i->first << ' ' << j << endl;
        }
    }
    return os;
}

int Database::RemoveIf(function<bool(const Date&, const string&)> predicate){ // при добавлении в одну дату нескольких событий вылетает
    int k = 0;
    for(auto it = order_of_events.begin(); it != order_of_events.end(); ++it){ ///////////////////////////
        const Date d = it->first;
        auto it_vector = stable_partition(begin(it->second), end(it->second), [predicate ,d](const string& event) {
            return predicate(d, event);
            });
        k += it_vector - it->second.begin();
        it->second.erase(it->second.begin(), it_vector);
        if(it->second.empty()){
            storage.erase(it->first);
        } else {
            storage[it->first].clear();
            for (auto r : it->second){
                storage[it->first].insert(r);
            }
        }
    }
    for (auto it = order_of_events.cbegin(); it != order_of_events.cend() /* not hoisted */; /* no increment */)
    {
      if (it->second.size() == 0)
      {
        order_of_events.erase(it++);    // or "it = m.erase(it)" since C++11
      }
      else
      {
        ++it;
      }
    }
    //order_of_events.erase(it_map, order_of_events.end());
    return k;
}
vector<string> Database::FindIf(function<bool(Date, string)> predicate) const {
    vector<string> result;
    for(auto it = order_of_events.begin(); it != order_of_events.end(); it++){
        auto it_vector = it->second.begin();
        const Date d = it->first;
        while (it_vector != it->second.end()){
            it_vector = find_if(it_vector, it->second.end(), [predicate, d](const string& ev){
                    return predicate(d, ev);
                    });
            if(it_vector == it->second.end())
            {
                continue;
            }
            ostringstream ss;
            ss << d;
            ss << " ";
            ss << *it_vector;
            result.push_back(ss.str());
            it_vector++;
        }
    }
    return result;
}

string Database::Last(const Date& date) const {
    auto it = order_of_events.upper_bound(date);
    if(it == order_of_events.begin()){
        return "No entries";
    }
    auto it2 = prev(prev(it)->second.end());
    ostringstream ss;
    ss << prev(it)->first;
    ss << " ";
    ss << *it2;
    return ss.str();
}
