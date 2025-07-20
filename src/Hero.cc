#include "Hero.hpp"

using namespace std;

void HeroBase::addPerkCard(const Perk &perk) {
    this->heroPerks.push_back(perk);
}

HeroBase::HeroBase (
    const int &_num, 
    std::string_view _name,
    const Perk &perk
) : actionCount(_num) , heroName(_name) {
    this->addPerkCard(perk);
    this->last_played_perk_ = "Nothing";
}

int HeroBase::getActionCount() { return this->actionCount; }

void HeroBase::setActionCount(const int &num) {
    this->actionCount = num; 
}

std::string HeroBase::getHeroName() { return this->heroName; }

Archaeologist::Archaeologist(const int &_num , std::string_view _name , const Perk &perk) :
    HeroBase(_num , _name , perk) 
    {
        setAddress("../../Horrified_Assets/Heros/Archaeologist.png");
    }

Mayor::Mayor(const int &_num , std::string_view _name , const Perk &perk) :
    HeroBase(_num , _name , perk) 
    {
        setAddress("../../Horrified_Assets/Heros/Mayor.png");
    }

void HeroBase::setCurrentPlace(shared_ptr<Place> _place) {
    this->currentPlace = _place;
}

void HeroBase::deleteItem(std::string_view itemName) {
    bool isFound {};
    for (int i {}; i < this->heroItems.size(); i++) {
        if (heroItems[i].name == itemName) {
            this->heroItems.erase(heroItems.begin() + i);
        }
    }
}

shared_ptr<Place> HeroBase::getCurrentPlace() { return this->currentPlace; }

std::vector<Item>& HeroBase::getHeroItems() { return this->heroItems; }

void HeroBase::addHeroItems(Item _item)
{
    heroItems.push_back(_item);
}

void HeroBase::deletePerk(string_view perkName) {
    bool isFound {};
    for (int i {}; i < this->heroPerks.size() && !isFound; i++) {
        if (this->heroPerks[i].name == perkName){
            this->heroPerks.erase(heroPerks.begin() + i);
            isFound = true;
        }
    }
}

void HeroBase::speciallAction(System* sys,int& pageNumber, int &actions,const int SCREEN_WIDTH,const int SCREEN_HEIGHT)
{
    Vector2 mouse =GetMousePosition(); 
    static shared_ptr<Place> choosenPlace;
    static bool isPicked = false;

    if(choosenPlace)
    {
        float panelW = 800, panelH = 600, pad = 20, Size = 170;
        Rectangle panel = {(SCREEN_WIDTH - panelW) / 2,(SCREEN_HEIGHT - panelH) / 2 ,panelW, panelH};
        DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, {0,0,0,100});
        DrawRectangleRec(panel, DARKGRAY);

        float x = panel.x + pad;
        float y = panel.y + pad;

        for(auto item : choosenPlace -> getItems())
        {
            Texture2D itemTex = item.address;
            Vector2 pos = { x,y};
            DrawTextureEx(itemTex,pos,0.0,Size/panel.width,WHITE);

            Rectangle itemRec = {x,y,itemTex.width * (Size/itemTex.width), itemTex.height * (Size/itemTex.width) }; 
            
            if (CheckCollisionPointRec(mouse, itemRec))
            {
                DrawRectangleLinesEx(itemRec, 2, YELLOW);
                if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    addHeroItems(item);
                    choosenPlace->removeItem(item);
                    isPicked = true;
                }
            }

            x += Size + pad;
            if (x + Size > panel.x + panel.width)
            {
                x = panel.x + pad;
                y += Size + pad;
            }
        }


        if(IsKeyPressed(KEY_BACKSPACE))
            {
                if(isPicked)
                {
                    actions--;
                    isPicked = false;
                    choosenPlace = nullptr;
                    pageNumber = PageNumbers::HERO_PHASE_PAGE;
                }
                else
                {
                    choosenPlace = nullptr;
                }
            }
        }
    else
    {
        vector<shared_ptr<Place>> possiblePlace = currentPlace -> getNeighbors();   
        
        for (auto& place : possiblePlace)
        {
            Vector2 pos = place->getPosition();
            float radius = 25.0f; // now just for test 
            DrawCircleV(pos, radius, BLUE);
            if (CheckCollisionPointCircle(mouse, pos, radius))
            DrawCircleLines(pos.x, pos.y, radius + 4.0f, YELLOW);
        }
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            for (auto& place : possiblePlace)
            {
                if (place->isClicked(mouse))
                {
                    choosenPlace = place;
                }
            }
        }
        if(IsKeyPressed(KEY_BACKSPACE))
        {
            pageNumber = PageNumbers::HERO_PHASE_PAGE;
        }
    }
}
