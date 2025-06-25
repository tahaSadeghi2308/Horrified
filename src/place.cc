#include "place.hpp"

using namespace std;

Place::Place(const string& name) : placeName(name) {}

string Place::getPlaceName() { return this->placeName; }