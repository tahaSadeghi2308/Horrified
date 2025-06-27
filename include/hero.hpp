#ifndef HERO_HPP
#define HERO_HPP

#include <string>
#include <string_view>

class HeroBase {
    std::string name;
    std::string currentPlace;
public:
    explicit HeroBase(std::string_view _name);
    std::string getHeroName() const;
    std::string getCurrentPlace() const;
    void setCurrentPlace(std::string_view _newPlace);
};

#endif // HERO_HPP