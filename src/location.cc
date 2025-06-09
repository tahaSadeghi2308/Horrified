#include "location.hpp"

using namespace std;

Place::Place(const string &_name) : name(_name){}

vector<shared_ptr<Place>> Place::getNeighbors() {
    return this->neighbors;
}

void Place::setNeighbors(const vector<shared_ptr<Place>> &neiList){
    this->neighbors = neiList;
}

string Place::getName(){ return this->name; }