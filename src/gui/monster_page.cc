#include "gui/monster_page.hpp"
using namespace std;


MonsterPhasePage::MonsterPhasePage(
    const Font &f,
    System *s
): GameFont(f) , sys(s)
{}

void MonsterPhasePage::Reset_For_Next_Phase()
{
    intialize = true; 
    strikeMap.clear();
    time = 0;
    title = true;
    moveOnce = true;
    diceOnce = true;
    status = -1;
    attackOnce = true;
    dices.clear();
    option = 0;
    diceTime = 0;
    newCard = true;
}

void MonsterPhasePage::draw(shared_ptr<HeroBase> &cHero ,int &actions , PageNumbers &cPage)
{
    if(newCard)
    {
        currentCard  = sys->getRandomMonstCard();
        for (auto monst : sys->getAllMonsters()){
            if (monst != nullptr) {
                monst->putItem(currentCard.itemCount);
                monst->doEvent(currentCard.name , cHero);
                break;
            }
        }
        newCard = false;
    }

    if (title) 
    {
        
        time += GetFrameTime(); 
        Texture2D t = currentCard.address;
        float scale = 0.3;
        Vector2 position  // base position (texture position) 
        {
            (SCREEN_WIDTH - t.width * scale) / 2,
            (SCREEN_HEIGHT - t.height * scale) / 2
        };
        
        DrawTextureEx(t, position, 0, scale, WHITE);
        
        const char* titleText = "MONSTER PHASE BEGAN"; // dont using cstr anymore
        int fontSize = 60;
        Vector2 textSize = MeasureTextEx(GameFont, titleText, fontSize, 2);
        Vector2 textPos  
        {
            (SCREEN_WIDTH - textSize.x) / 2,
            position.y - 90
        };
        
        Color titleColor = RED;
        titleColor.a = (unsigned char)(255 * (time / 5)); // with alpha we can fade

        DrawTextEx(GameFont, titleText, textPos, fontSize, 2, titleColor);
        
        const char* subtitle = "MONSTERS ARE OUT BECAREFULL";
        fontSize = 30;
        textSize = MeasureTextEx(GameFont, subtitle, fontSize, 1);
        textPos =
        {
            (SCREEN_WIDTH - textSize.x) / 2,
            position.y + t.height * scale + 20
        };
        DrawTextEx(GameFont, subtitle, textPos, fontSize, 1, titleColor);
        
        if (time > 5) 
        {
            title = false;
            time = 0;
        }
    } 
    else 
    {
        if (intialize) 
        {
            strikeMap.clear();

            for (auto st : currentCard.strikePriorities) {
                if (st == "du") {
                    for (auto m : sys->getAllMonsters()) {
                        if (m){
                            if (m->getMonsterName() == "dracula") {
                                pair p {"du", m};
                                strikeMap.push_back(p);
                            }
                        }
                    }
                } else if (st == "inm") {
                    for (auto m : sys->getAllMonsters()) {
                        if (m){
                            if (m->getMonsterName() == "invisibleMan") {
                                pair p {"inm", m};
                                strikeMap.push_back(p);
                            }
                        }
                    }
                } else {
                    for (auto m : sys->getAllMonsters()) {
                        if (m) {
                            if (m->getIsFrenzed()) {
                                pair p {"fz", m};
                                strikeMap.push_back(p);
                            }
                        }
                    }
                }
            }
            intialize = false;
        }

        if (strikeMap.empty()) 
        {
            // reseting all the variables it could be a function in second form
            Reset_For_Next_Phase();
            cPage = PageNumbers::HERO_PHASE_PAGE;
            return;
        }

        auto& s = strikeMap.front(); 
            
        if (moveOnce)
        {
            s.second->move(currentCard.move, s.first);
            moveOnce = false;
        }

        if (diceOnce) 
        {
            dices.clear();
            for (int i = 0; i < currentCard.dice; i++) {
                dices.push_back(sys->rollDice());
            }
            diceOnce = false;
        }
        
        if (!dices.empty()) 
        {
            auto& d = dices.front();

            const char* monsterName = s.second->getMonsterName().c_str();
            Vector2 textSize = MeasureTextEx(GameFont, monsterName, 40, 2);
            Vector2 textPos = {(SCREEN_WIDTH - textSize.x) / 2, 50};
            DrawTextEx(GameFont, monsterName, textPos, 40 , 2, RED);

            if (attackOnce) 
            {
                status = s.second->attack(d, s.second, cHero);
                attackOnce = false;
            }

            string roll = "DICE SIGN IS :  ";
    
                for(auto di : dices)
                {
                    roll += di;
                    roll += "   ";
                }

            Vector2 rollSize = MeasureTextEx(GameFont , roll.c_str() , 40 , 0 );
            Vector2 rollPos {textPos.x - 50  , textPos.y + 100};
            DrawTextEx(GameFont,roll.c_str(),rollPos,40,0,WHITE);
            
            diceTime += GetFrameTime();
            
            if (status == -1 || status == 4) 
            {
                time += GetFrameTime();
    
                float panelWidth = 800;
                float panelHeight = 200;
                Rectangle panel 
                {
                    (SCREEN_WIDTH - panelWidth) / 2,
                    (SCREEN_HEIGHT - panelHeight) / 2,
                    panelWidth,
                    panelHeight
                };
                
                DrawRectangleRec(panel, DARKGRAY);
                
                const char* resultText;
                Color textColor = WHITE;
                
                if (status == -1) 
                {
                    resultText = "Lucky! Nothing happened this time.";
                    textColor = GREEN;
                } 
                else
                {
                    resultText = "Monster power activated!";
                    textColor = PURPLE;
                }
                
                textSize = MeasureTextEx(GameFont, resultText, 40, 0);
                Vector2 textPos 
                {
                    panel.x + ((panel.width - textSize.x) / 2),
                    panel.y + ((panel.height - textSize.y) / 2)
                };
                
                DrawRectangleLinesEx(panel, 3, textColor);
                DrawTextEx(GameFont, resultText, textPos, 40 , 0 , textColor);
                
                if (time > 2) 
                {
                    dices.pop_front();
                    attackOnce = true;
                    time = 0;
                    diceTime = 0;
                }
            } 
            else if (status == 2 || status == 3) {
                if (diceTime < 2) 
                {
                    shared_ptr<HeroBase> attacedHero;
                        if (status == 2) attacedHero = cHero;
                        else {
                            for (auto h : sys->getAllHerosAvailable()) {
                                if (h->getHeroName() != cHero->getHeroName()) attacedHero = h;
                            }
                        }
                        float panelWidth = 600;
                        float panelHeight = 200;
                        Rectangle panel 
                        {
                            (SCREEN_WIDTH - panelWidth) / 2,
                            (SCREEN_HEIGHT - panelHeight) / 2,
                            panelWidth,
                            panelHeight
                        };
                        
                        string attackText = attacedHero->getHeroName();
                        attackText += " is under attack!";
                        textSize = MeasureTextEx(GameFont, attackText.c_str(), 30 , 0);
                        Vector2 textPos 
                        {
                            panel.x + (panel.width - textSize.x) / 2,
                            panel.y + (panel.height - textSize.y) / 2
                        };
                        DrawRectangleRec(panel, DARKGRAY);
                        DrawRectangleLinesEx(panel, 3, RED);
                

                    DrawTextEx(GameFont, attackText.c_str() , textPos, 30, 0, RED);

                } 
                else
                {
                    if (option == 1) {
                        float panelW = 800, panelH = 600, pad = 20, Size = 170;
                        Rectangle panel = {(SCREEN_WIDTH - panelW) / 2, (SCREEN_HEIGHT - panelH) / 2, panelW, panelH};
                        DrawRectangleRec(panel, DARKGRAY);
                        DrawRectangleLinesEx(panel, 3, GOLD);
                        
                        const char* panelTitle = "Choose an item for protection";
                        int fontSize = 30;
                        Vector2 textSize = MeasureTextEx(GameFont, panelTitle, fontSize, 2);
                        Vector2 textPos = {panel.x + (panel.width - textSize.x) / 2, panel.y + 20};
                        DrawTextEx(GameFont, panelTitle, textPos, fontSize, 2, WHITE);
                        
                        float x = panel.x + pad;
                        float y = panel.y + pad + 60;
                        
                        for (auto& item : cHero->getHeroItems()) {
                            Texture2D itemTexure = item.address;
                            float scale = Size / itemTexure.width;
                            Rectangle rec = {x, y, itemTexure.width * scale, itemTexure.height * scale};
                            DrawTextureEx(itemTexure, {x, y}, 0, scale, WHITE);
                            
                            if (CheckCollisionPointRec(GetMousePosition(), rec)) {
                                DrawRectangleLinesEx(rec, 3, YELLOW);
                                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                                    cHero->deleteItem(item.name);
                                    sys->addItem(item);          
                                    dices.pop_front();
                                    attackOnce = true;
                                    time = 0;
                                    diceTime = 0;
                                    return;
                                }                     
                            }
                            x += Size + pad;
                            if (x + Size > panel.x + panel.width) {
                                x = panel.x + pad;
                                y += Size + pad;
                            }
                        }
                    } 
                    else 
                    {
                        shared_ptr<HeroBase> attacedHero;
                        if (status == 2) attacedHero = cHero;
                        else {
                            for (auto h : sys->getAllHerosAvailable()) {
                                if (h->getHeroName() != cHero->getHeroName()) attacedHero = h;
                            }
                        }
                        
                        if (attacedHero->getHeroItems().empty()) {
                            sys->increaseTerrorLevel();
                            for (auto loc : sys->getAllLocations()) {
                                if (loc->getName() == "hospital") {
                                    sys->moveHero(attacedHero, loc);
                                    break;
                                }
                            }
                            Reset_For_Next_Phase();
                            cPage = PageNumbers::HERO_PHASE_PAGE;
                            return;
                        }
                        else 
                        {
                            float panelWidth = 500;
                            float panelHeight = 300;
                            Rectangle panel = {
                                (SCREEN_WIDTH - panelWidth) / 2,
                                (SCREEN_HEIGHT - panelHeight) / 2,
                                panelWidth,
                                panelHeight
                            };
                            
                            DrawRectangleRec(panel, DARKGRAY);
                            DrawRectangleLinesEx(panel, 3, GOLD);
                            
                            const char* question = "Hero under attack!";
                            int fontSize = 30;
                            Vector2 textSize = MeasureTextEx(GameFont, question, fontSize, 2);
                            Vector2 textPos = {panel.x + (panel.width - textSize.x) / 2, panel.y + 30};
                            DrawTextEx(GameFont, question, textPos, fontSize, 2, RED);
                            
                            const char* subtitle = "Use an item for protection?";
                            fontSize = 20;
                            textSize = MeasureTextEx(GameFont, subtitle, fontSize, 1);
                            textPos = {panel.x + (panel.width - textSize.x) / 2, panel.y + 80};
                            DrawTextEx(GameFont, subtitle, textPos, fontSize, 1, WHITE);
                            
                            Rectangle yesButton = {panel.x + 50, panel.y + 150, 150, 60};
                            Color yesHover = CheckCollisionPointRec(GetMousePosition(), yesButton) ? GREEN : Color{0,0,0,0} ;
                            DrawRectangleRec(yesButton, yesHover);
                            DrawRectangleLinesEx(yesButton, 2, WHITE);
                            
                            const char* yesText = "YES";
                            fontSize = 25;
                            textSize = MeasureTextEx(GameFont, yesText, fontSize, 2);
                            textPos = {yesButton.x + (yesButton.width - textSize.x) / 2, yesButton.y + (yesButton.height - textSize.y) / 2};
                            DrawTextEx(GameFont, yesText, textPos, fontSize, 2, WHITE);
                            
                            Rectangle noButton = {panel.x + 300, panel.y + 150, 150, 60};
                            Color noHover = CheckCollisionPointRec(GetMousePosition(), noButton) ? RED : Color{0,0,0,0};
                            DrawRectangleRec(noButton, noHover);
                            DrawRectangleLinesEx(noButton, 2, WHITE);
                            
                            const char* noText = "NO";
                            fontSize = 25;
                            textSize = MeasureTextEx(GameFont, noText, fontSize, 2);
                            textPos = {noButton.x + (noButton.width - textSize.x) / 2, noButton.y + (noButton.height - textSize.y) / 2};
                            DrawTextEx(GameFont, noText, textPos, fontSize, 2, WHITE);
                            
                            if (CheckCollisionPointRec(GetMousePosition(), yesButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                                option = 1;

                            if (CheckCollisionPointRec(GetMousePosition(), noButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) 
                            {
                                sys->increaseTerrorLevel();
                                for (auto loc : sys->getAllLocations()) {
                                    if (loc->getName() == "hospital") {
                                        sys->moveHero(attacedHero, loc);
                                        break;
                                    }
                                }
                                Reset_For_Next_Phase();
                                sys->increaseTerrorLevel();
                                cPage = PageNumbers::HERO_PHASE_PAGE;
                                return;
                            }
                        }
                    }
                }
            } 
            else 
            {
                time += GetFrameTime();
                
                float panelWidth = 600;
                float panelHeight = 200;
                Rectangle panel 
                {
                    (SCREEN_WIDTH - panelWidth) / 2,
                    (SCREEN_HEIGHT - panelHeight) / 2,
                    panelWidth,
                    panelHeight
                };
                
                DrawRectangleRec(panel, DARKGRAY);
                DrawRectangleLinesEx(panel, 3, RED);
                
                const char* message = "We lost a villager!";
                textSize = MeasureTextEx(GameFont, message, 35,0);
                Vector2 textPos 
                {
                    panel.x + (panel.width - textSize.x) / 2,
                    panel.y + (panel.height - textSize.y) / 2
                };
                DrawTextEx(GameFont, message, textPos, 35, 0 , RED);
                
                if (time > 2)
                {
                    sys->increaseTerrorLevel();
                    Reset_For_Next_Phase();
                    cPage = PageNumbers::HERO_PHASE_PAGE;
                    return;
                }
            }
        } 
        else 
        {
            strikeMap.pop_front();
            diceOnce = true;
            moveOnce = true;
            attackOnce = true;
            option = 0;
            diceTime = 0;
            time = 0;
            return;
        }
    }
}


void MonsterPhasePage::update(shared_ptr<HeroBase> &cHero ,int &actions ,PageNumbers &cPage)
{
    //noting
}
