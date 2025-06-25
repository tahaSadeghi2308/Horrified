#ifndef PLACE_HPP
#define PLACE_HPP

#include <string>

class Place {
    std::string placeName;
public:
    explicit Place(const std::string& name);
    std::string getPlaceName();
};

#endif // PLACE_HPP