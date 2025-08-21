#include "gui.hpp"
#include "gui/page_base.hpp"
#include "gui/hero_phase_page.hpp"
#include "gui/move_page.hpp"
#include "gui/pickup_page.hpp"
#include "gui/guide_page.hpp"
#include "gui/welcome_page.hpp"
#include "gui/monster_page.hpp"
#include "gui/player_setup_page.hpp"
#include "gui/hero_selection_page.hpp"
#include "gui/advanced_page.hpp"
#include "gui/defeat_page.hpp"
#include "gui/perk_page.hpp"
#include "gui/load_match.hpp"
#include "gui/end_game_page.hpp"
#include <filesystem>
#include <array>

using namespace std;
namespace fs = std::filesystem;

Gui::Gui(System *s,const int width,const int height):sys(s),scroll(0.0f),SCREEN_WIDTH(width),SCREEN_HEIGHT(height)
{
    UP_PANEL_WIDTH = SCREEN_WIDTH - LEFT_PANEL_WIDTH - RIGHT_PANEL_WIDTH;
    pageNumber = PageNumbers::WELCOME_PAGE;
    isEnd = -1;
    round = 0;
    actions = 1000;
    GameFont = LoadFont("../Horrified_Assets/Melted.ttf");
    sys->setFont(GameFont);
//    this->playerPriority.push_back("archaeologist"); //for test (needs welcom page)
//    this->playerPriority.push_back("mayor");
    float pad = 20;
    float panelH = (SCREEN_HEIGHT / 9) - 25 , panelW = LEFT_PANEL_WIDTH - (2*pad);
    moveRec = { pad , pad , panelW , panelH };
    PickRec = { pad , moveRec.y + pad + panelH , panelW , panelH};
    GuidRec  = { pad , PickRec.y + pad + panelH , panelW , panelH};
    speciallRec = {pad , GuidRec.y + pad + panelH , panelW , panelH};
    AdvanceRec = {pad , speciallRec.y + pad + panelH , panelW , panelH };
    DefeatRec = {pad , AdvanceRec.y + pad + panelH , panelW , panelH };
    PerkRec = { pad , DefeatRec.y + pad + panelH , panelW ,panelH };
    exitANDsave = { pad , PerkRec.y + pad + panelH , panelW , panelH};
    Help = {pad , exitANDsave.y + pad + panelH , panelW , panelH };
    selectedSaveFolderNumber = -1;
    pages = {
        { PageNumbers::HERO_PHASE_PAGE , make_shared<HeroPhasePage>(GameFont , s)},
        { PageNumbers::MOVE_PAGE , make_shared<MovePage>(GameFont , s)},
        { PageNumbers::PICKUP_PAGE , make_shared<PickupPage>(GameFont , s)},
        { PageNumbers::GUIDE_PAGE , make_shared<GuidePage>(GameFont , s)},
        { PageNumbers::WELCOME_PAGE , make_shared<WelcomePage>()},
        { PageNumbers::MONSTERPHASE_PAGE , make_shared<MonsterPhasePage>(GameFont , s)},
        { PageNumbers::PLAYER_SETUP_PAGE , make_shared<PlayerSetupPage>(this->playerPriority)},
        { PageNumbers::HERO_SELECTION_PAGE , make_shared<HeroSelectionPage>(this->playerPriority, this->mainPri, s)},
        { PageNumbers::ADVANCED_PAGE , make_shared<AdvancedPage>(GameFont , s)},
        { PageNumbers::DEFEAT_PAGE , make_shared<DefeatPage>(GameFont , s)} ,
        { PageNumbers::PLAYPERK_PAGE , make_shared<PerkPage>(GameFont , s , doNextPhase)},
        { PageNumbers::LOAD_MATCH_PAGE , make_shared<LoadMatchPage>(selectedSaveFolderNumber)},
        { PageNumbers::END_GAME_PAGE , make_shared<EndGamePage>(GameFont , s)}
    };
}

void Gui::run() {
    while (!WindowShouldClose()) {
        PageNumbers prevPage = pageNumber;
        isEnd = sys->isEndGame();
        if (this->pageNumber == PageNumbers::EXIT_PAGE) {
            delete sys;
            sys = nullptr;
            break;
        }
        if (isEnd != -1) {
            auto endPage = static_pointer_cast<EndGamePage>(pages[PageNumbers::END_GAME_PAGE]);
            endPage->setEndCode(isEnd);
            pageNumber = PageNumbers::END_GAME_PAGE;
        }
        if (this->pageNumber == PageNumbers::SAVE_MODE){
            sys->saveState();
            this->saveState();
            this->pageNumber = PageNumbers::HERO_PHASE_PAGE;
        }
        if (currentHero == nullptr && !mainPri.empty()) {
            if (!mainPri.empty()) {
                string currentHeroName = mainPri[round % mainPri.size()].second;
                for (auto& h : sys->getAllHerosAvailable()) {
                    if (h->getHeroName() == currentHeroName) { currentHero = h; break; }
                }
                if (currentHero != nullptr) {
                    if (actions == 1000)
                        actions = currentHero->getActionCount();
                    doNextPhase = true;
                }
            }
        }
        BeginDrawing();
        if (this->pageNumber == PageNumbers::HERO_PHASE_PAGE){
            ClearBackground(BLACK);
            pages[this->pageNumber]->draw(currentHero , actions , pageNumber);
            pages[this->pageNumber]->update(currentHero , actions , pageNumber);
        }
        else if (this->pageNumber == PageNumbers::SPECIALACTION_PAGE) {
            if (currentHero->getHeroName() == "archaeologist" || currentHero->getHeroName() == "scientist"){
                ClearBackground(BLACK);
                pages[PageNumbers::HERO_PHASE_PAGE]->draw(currentHero , actions , pageNumber);
                currentHero->speciallAction(sys , this->pageNumber , actions , SCREEN_WIDTH , SCREEN_HEIGHT);
            } else {
                this->pageNumber = PageNumbers::HERO_PHASE_PAGE;
            }

        }
        else if (this->pageNumber == PageNumbers::WELCOME_PAGE || this->pageNumber == PageNumbers::PLAYER_SETUP_PAGE || this->pageNumber == PageNumbers::HERO_SELECTION_PAGE || this->pageNumber == PageNumbers::LOAD_MATCH_PAGE || this->pageNumber == PageNumbers::END_GAME_PAGE) {
            ClearBackground(BLACK);
            pages[this->pageNumber]->draw(currentHero , actions , pageNumber);
            pages[this->pageNumber]->update(currentHero , actions , pageNumber);
        }
        else if (this->pageNumber != PageNumbers::MONSTERPHASE_PAGE) {
            ClearBackground(BLACK);
            pages[PageNumbers::HERO_PHASE_PAGE]->draw(currentHero , actions , pageNumber);
            pages[this->pageNumber]->draw(currentHero , actions , pageNumber);
            pages[this->pageNumber]->update(currentHero , actions , pageNumber);
        }
        else if (pageNumber == PageNumbers::MONSTERPHASE_PAGE) {
            ClearBackground(BLACK); 
            pages[this->pageNumber]->draw(currentHero , actions , pageNumber);

            if (pageNumber == HERO_PHASE_PAGE) {
                currentHero = nullptr;
                round++;
            }
        }
        
        if (actions <= 0 && isEnd == -1) {
            if(doNextPhase) {
                pageNumber = PageNumbers::MONSTERPHASE_PAGE;
                actions = 1000;
            } else {
                currentHero = nullptr;
                round++;
            }
        }
        EndDrawing();

        if (prevPage == PageNumbers::LOAD_MATCH_PAGE && pageNumber == PageNumbers::HERO_PHASE_PAGE) {
            if (selectedSaveFolderNumber >= 0) {
                cout << "in loading page!!!\n";
                sys->loadState(selectedSaveFolderNumber);
                cout << "loaded system\n";
                this->loadState(selectedSaveFolderNumber);
                cout << "loaded gui\n";
            }
        }
    }
}
void Gui::drawLeftPanel()
{
    Rectangle rec {0 , 0 , (float)LEFT_PANEL_WIDTH , (float)SCREEN_HEIGHT};
    DrawRectangleRec(rec , DARKGRAY);

    vector<pair<string, Rectangle>> buttons = { {"MOVE", moveRec},{"PICK", PickRec},{"GUIDE", GuidRec},{"SPECIALL", speciallRec},
        {"ADVANCE", AdvanceRec},
        {"DEFEAT", DefeatRec},
        {"PERK", PerkRec},
        {"SAVE & EXIT", exitANDsave},
        {"HELP" , Help}
    };

    float fontSize = 50;
    for (auto& button : buttons)
    {
        string text = button.first;
        Rectangle targetRec = button.second;

        DrawRectangleRec(targetRec, buttonColor);
        Vector2 textSize = MeasureTextEx(GameFont, text.c_str(), fontSize, 0);
        float centerX = (targetRec.width - textSize.x) / 2;
        float centerY = (targetRec.height - textSize.y) / 2;
        DrawTextEx(GameFont, text.c_str(), {targetRec.x + centerX, targetRec.y + centerY}, fontSize, 0, BLACK);
    }
}

void Gui::drawRightPanel(shared_ptr<HeroBase>& hero)
{
    Vector2 mouse = GetMousePosition();
    float pad = 5;
    float padBetweenItems = 55;

    auto items = hero->getHeroItems();
    Rectangle rightPanel {(float)SCREEN_WIDTH-RIGHT_PANEL_WIDTH , 0 , (float)RIGHT_PANEL_WIDTH , (float)SCREEN_HEIGHT};
    DrawRectangleRec(rightPanel , DARKGRAY);
 
    float Size = 50;
    float x = rightPanel.x + pad ,y = rightPanel.y + pad;
    for(auto& it : items)
    {
        Texture2D itemTex = it.address;
        Vector2 pos = {x,y};
        float scale = Size/rightPanel.width;
        DrawTextureEx(itemTex,pos,0.0,scale,WHITE);

        x += Size + padBetweenItems;
        if (x + Size + padBetweenItems  > rightPanel.x + rightPanel.width)
        {
            x = rightPanel.x + pad;
            y += Size + padBetweenItems ;
        }
    }    
}



void Gui::drawCoffin()
{
    for(auto& cof : sys->getCoffins())
    {
        for(auto& loc : sys->getAllLocations())
        {
            if(cof == loc->getName())
            {
                Vector2 target = loc->getPosition();
                DrawTextureEx(coffins,{target.x , target.y - 50},0,0.075,WHITE);
            }
        }
    }
    for(auto& smash : sys->getSmashed())
    {
        for(auto& loc : sys->getAllLocations())
        {
            if(smash == loc->getName())
            {
                Vector2 target = loc->getPosition();
                DrawTextureEx(smahsedCoffins,{target.x , target.y - 50},0,0.075,WHITE);
            }
        }
    }
}

void Gui::drawEvindence()
{
    for(auto& evi : sys->getEvidence())
    {
        for(auto& loc : sys->getAllLocations())
        {
            if(evi == loc->getName())
            {
                Vector2 target = loc->getPosition();
                DrawCircleLinesV(target,60,RED);
            }
        }
    }
}


void Gui::drawUpPanel(std::shared_ptr<HeroBase>& heroInfo, int actions) // must override
{
    const int fontSize = 40;
    const int pad = 10;  
    string terror = to_string(sys->getTerrorLevel());
    string act = to_string(actions);
    string heroTurn = heroInfo->getHeroName();// this part is totally by testing diffrent parts

    Rectangle rec = {(float)LEFT_PANEL_WIDTH,0,(float)UP_PANEL_WIDTH,(float)UP_PANEL_HEIGHT};
    DrawRectangleRec(rec, GameColor);

    Vector2 basePos = { rec.x + 30, rec.y + (UP_PANEL_HEIGHT - fontSize) / 2 };
    DrawTextEx(GameFont, "Terror level:", basePos, fontSize, 0, BLACK);

    Vector2 Terror = {basePos.x + MeasureTextEx(GameFont, "Terror level:", fontSize, 0).x + pad , basePos.y};
    DrawTextEx(GameFont, terror.c_str(), Terror, fontSize, 0, BLACK);

    Vector2 firstStart = { Terror.x + fontSize, Terror.y + fontSize + 5 };
    Vector2 firstEnd   = { Terror.x + fontSize, Terror.y - pad };
    DrawLineEx(firstStart, firstEnd, 4, WHITE);

    Vector2 action = {Terror.x + fontSize + (2*pad),basePos.y};
    DrawTextEx(GameFont, "Actions:", action, fontSize, 0, BLACK);

    Vector2 actionPos = {action.x + MeasureTextEx(GameFont, "Actions:", fontSize, 0).x + pad,action.y};
    DrawTextEx(GameFont, act.c_str(), actionPos, fontSize, 0, BLACK);

    Vector2 secondStart = { actionPos.x + fontSize, actionPos.y + fontSize + 5 };
    Vector2 secondEnd   = { actionPos.x + fontSize, actionPos.y - pad };
    DrawLineEx(secondStart, secondEnd, 4, WHITE);

    Vector2 turn = {action.x + fontSize + (16*pad) , basePos.y};
    DrawTextEx(GameFont, "Hero Turn:", turn , fontSize, 0, BLACK);
    Vector2 turnPos = {turn.x + MeasureTextEx(GameFont, "Hero turn :", fontSize, 0).x , turn.y};
    DrawTextEx(GameFont, heroTurn.c_str(),turnPos, fontSize, 0, BLACK);
}


void Gui::drawMap() {
    Vector2 mouse = GetMousePosition();

    for (auto &p : sys->getAllLocations()) 
    {
        p->draw(mouse);
    }
}



void Gui::handleInput()
{
    Vector2 mouse = GetMousePosition();
    if(pageNumber == PageNumbers::HERO_PHASE_PAGE)
    {
        drawMap();
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) ) {
            Vector2 mouse = GetMousePosition();
            for (auto &p : sys->getAllLocations()) 
            {
                if (p->isClicked(mouse)) 
                {
                    selectedPlace = p;
                    break;
                }
            }
        }    
        if (selectedPlace) 
            {
                PlaceInfo(selectedPlace);
                if(GetKeyPressed() == KEY_BACKSPACE)
                {
                    selectedPlace = nullptr;
                    scroll = 0;
                }
            }
    }
    if(CheckCollisionPointRec(mouse, moveRec))
    {
        DrawRectangleLinesEx(moveRec, 8 ,DARKGREEN);
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        pageNumber = PageNumbers::MOVE_PAGE;
    }
    else if (CheckCollisionPointRec(mouse,PickRec))
    {
        DrawRectangleLinesEx(PickRec, 8 ,DARKGREEN);
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        pageNumber = PageNumbers::PICKUP_PAGE;
    }
    else if(CheckCollisionPointRec(mouse , GuidRec))
    {
        DrawRectangleLinesEx(GuidRec, 8 ,DARKGREEN);
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        pageNumber = PageNumbers::GUIDE_PAGE;
    }
    else if(CheckCollisionPointRec(mouse , AdvanceRec))
    {
        DrawRectangleLinesEx(AdvanceRec, 8 ,DARKGREEN);
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        pageNumber = PageNumbers::ADVANCED_PAGE;
    }
    else if(CheckCollisionPointRec(mouse , DefeatRec))
    {
         DrawRectangleLinesEx(DefeatRec, 8 ,DARKGREEN);
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        pageNumber = PageNumbers::DEFEAT_PAGE;
    }
    else if(CheckCollisionPointRec(mouse , PerkRec))
    {
        DrawRectangleLinesEx(PerkRec, 8 ,DARKGREEN);
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        pageNumber = PageNumbers::PLAYPERK_PAGE;
    }

}

void Gui::movingAsset(shared_ptr<Place>& destination)
{
    Vector2 pos = destination -> getPosition();
    float w = 120, h = 120;
    Rectangle rect = {pos.x - w/2, pos.y - h/2, w, h}; // we should go back and up for 1/2 of the squere
    float expand = 7 * (sinf(GetTime() * 4 ) + 1); // get it with testing diffrent options 

    Rectangle fadingRec = {rect.x - expand , rect.y - expand , rect.width + 2 * expand , rect.height + 2 * expand};

    DrawRectangleLinesEx(fadingRec, 3, Fade(YELLOW, 0.4f));
    DrawRectangleLinesEx(rect, 2, YELLOW);
}

void Gui::PlaceInfo(shared_ptr<Place> selected)
{

    vector<Texture2D> temp;

    for(auto& hero : selected->getAllHeroes())
    {
        temp.push_back(hero->getAddress());
    }  
    for(auto& mon:selected->getMonsters())
    {
        temp.push_back(mon->getAddress());
    }
    for(auto& vill : selected->getVillagers())
    {
        temp.push_back(vill->getAddress());
    }  
    for(auto& item:selected->getItems())
    {
       temp.push_back(item.address);
    }  
    // creating panel
    float panelWidth = 800;
    float panelHeight = 600;
    Rectangle panel = { (SCREEN_WIDTH - panelWidth) / 2.0f, (SCREEN_HEIGHT - panelHeight) / 2.0f, panelWidth, panelHeight };
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Color{ 0, 0, 0, 100 }); // could change
    DrawRectangleRec(panel, DARKGRAY);

    // all sizes that we need
    float Size = 170.0f;
    float padding = 20.0f;
    float rowHeight = Size + 2 * padding;
    int itemsNum = (panelWidth - 2 * padding) / rowHeight; // two padding for left and right 
    int Rows = (temp.size() + itemsNum - 1) / itemsNum; // ceil doesnt work so i used manual ceil(if we go to next line we need the next line)
    float ContentHeight = Rows * rowHeight;
    float showingHeight = panel.height - 60.0f; 
    //add scrolling
    float scrollSpeed = 20.0f;
    scroll += GetMouseWheelMove() * scrollSpeed;
    float maxScroll = 0.0f;
    float minScroll = 0.0f;
    if (ContentHeight > showingHeight)
        minScroll = showingHeight - ContentHeight; 

    if (scroll > maxScroll) 
    scroll = maxScroll;
    if (scroll < minScroll) 
    scroll = minScroll;

    float startX = panel.x + 20.0f;
    float startY = panel.y + 40.0f + scroll; 
    float saveX = startX;
    float saveY = startY;

    BeginScissorMode(panel.x, panel.y, panel.width, panel.height); 

    for (size_t i = 0; i < temp.size(); ++i)
    {
        if (saveX + Size > panel.x + panel.width)
        {
            saveX = startX;
            saveY += rowHeight;
        }
        Vector2 pos = { saveX, saveY };
        DrawTextureEx(temp[i], pos, 0.0f, Size / (float)(temp[i]).width, WHITE);
        saveX += Size + padding;
    }
    EndScissorMode();
}

void Gui::saveState() {
    vector<string> dirNames;
    for (const auto& entry : fs::directory_iterator("../data/after_game/")) {
        if (entry.is_directory()) {
            dirNames.push_back(entry.path().filename().string());
        }
    }

    sort(
        dirNames.begin(),
        dirNames.end(),
        [](const string& a, const string& b) -> bool { return stoi(a) > stoi(b); }
    );

    fs::path folderPath = fs::path("../data/after_game") / dirNames.front();
    /*
     * saving pattern:
     * 0 : page_number
     * 1 : current remaining actions count
     * 2 : round
     * 3 : mainpri data
     * 4 : doNextPhase
     * */
    ofstream file(folderPath / "util.txt");
    if (file.is_open()) {
        file << 0 << "\n"; // should be hero phase page
        file << this->actions << "\n";
        file << this->round << '\n';
        for (auto& [ playerName , heroName] : this->mainPri){
            file << playerName << "_" << heroName << "_";
        }
        file << '\n';
        file << (int)(this->doNextPhase) << '\n';
        file << sys->getTerrorLevel() << '\n';
        file << '\n';
        file.close();
    }
}

void Gui::loadState(const int folderNumber) {
    constexpr int DATA_ROW_COUNT {6};
    fs::path folderPath = fs::path("../data/after_game") / to_string(folderNumber);

    // load data safely
    array<string, DATA_ROW_COUNT> loadedData{};
    ifstream file(folderPath / "util.txt");
    if (file.is_open()) {
        for (int i {}; i < DATA_ROW_COUNT && file.good(); i++) {
            getline(file , loadedData[i]);
        }
        file.close();
    }

    auto safeStoi = [] (const string &s, int fallback) -> int {
        try { return stoi(s); } catch (...) { return fallback; }
    };

    pageNumber = static_cast<PageNumbers>(safeStoi(loadedData[0], static_cast<int>(PageNumbers::HERO_PHASE_PAGE)));
    actions = safeStoi(loadedData[1], actions);
    round = safeStoi(loadedData[2], round);

    this->mainPri.clear();
    vector<string> temp;
    stringstream stream(loadedData[3]);
    string x;
    while (getline(stream , x , '_')) if (!x.empty()) temp.push_back(x);
    for (size_t i = 0; i + 1 < temp.size(); i += 2) {
        this->mainPri.emplace_back(temp[i], temp[i + 1]);
    }
    doNextPhase = static_cast<bool>(safeStoi(loadedData[4], 1));
    sys->setTerrorLevel(safeStoi(loadedData[5], 20));
}

void Gui::MovePhase(std::shared_ptr<HeroBase>& hero, int &actions)
{
    static bool isThereVillager = false;
    static std::shared_ptr<Place> targetPlace;
    static std::vector<std::shared_ptr<Villager>> selectedVillagers;
    Vector2 mouse = GetMousePosition();

    auto neis = hero->getCurrentPlace()->getNeighbors();

    for (auto& place : neis)
    {
        movingAsset(place);
    }

    if (!isThereVillager && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        for (auto& place : neis)
        {
            if (place->isClicked(mouse))
            {
                targetPlace = place;
                auto villagers = hero->getCurrentPlace()->getVillagers();
                if (!villagers.empty())
                {
                    isThereVillager = true;
                    selectedVillagers.clear();
                }
                else
                {
                    sys->moveHero(hero, targetPlace);
                    actions--;
                    pageNumber = PageNumbers::HERO_PHASE_PAGE;
                    return;
                }   
            }
        }
    }

    if (isThereVillager)
    {
        float panelW = 800, panelH = 600, pad = 20, Size = 170;
        Rectangle panel = {(SCREEN_WIDTH - panelW) / 2.0f,(SCREEN_HEIGHT - panelH) / 2.0f,panelW, panelH};
        DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, {0,0,0,100});
        DrawRectangleRec(panel, DARKGRAY);

        float x = panel.x + pad;
        float y = panel.y + pad;
        
        for (auto& vill : hero->getCurrentPlace()->getVillagers())
        {
            Texture2D villTex = vill->getAddress();
            Vector2 pos = { x,y }; 
            DrawTextureEx(villTex, pos , 0, Size/villTex.width, WHITE);

            Rectangle villRec = {x, y,villTex.width * (Size/villTex.width),villTex.height * (Size/villTex.width)};

            if (CheckCollisionPointRec(mouse, villRec) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                auto it = find(selectedVillagers.begin(),selectedVillagers.end(),vill);
                if (it == selectedVillagers.end())
                    selectedVillagers.push_back(vill);
                else
                    selectedVillagers.erase(it);
            }
            if (find(selectedVillagers.begin(),selectedVillagers.end(),vill) != selectedVillagers.end())
            {
                DrawRectangleLinesEx(villRec, 3, YELLOW);
            }

            x += Size + pad;
            if (x + Size > panel.x + panel.width)
            {
                x = panel.x + pad;
                y += Size + pad;
            }
        }

        if (IsKeyPressed(KEY_BACKSPACE))
        {
            sys->moveHero(hero, targetPlace);
            for (auto& vill : selectedVillagers)
            {
                sys->moveVillager(vill, targetPlace);

                if(vill->getSafeZone() == vill->getVillagerLoc())
                {
                    sys->killVillager(vill);
                    hero->addPerkCard(sys->getRandomPerk());
                }
            }
            selectedVillagers.clear();
            isThereVillager = false;
            actions--;
            pageNumber = PageNumbers::HERO_PHASE_PAGE;
        }

        return; 
    }

    if (IsKeyPressed(KEY_BACKSPACE))
    {
        pageNumber = PageNumbers::HERO_PHASE_PAGE;
    }
}

void Gui::pickUpPhase(shared_ptr<HeroBase>& hero ,int &actions)
{
    static bool isPicked = false;
    Vector2 mouse = GetMousePosition();

    float panelW = 800, panelH = 600, pad = 20, Size = 170;
    Rectangle panel = {(SCREEN_WIDTH - panelW) / 2,(SCREEN_HEIGHT - panelH) / 2 ,panelW, panelH};
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, {0,0,0,100});
    DrawRectangleRec(panel, DARKGRAY);

    float x = panel.x + pad;
    float y = panel.y + pad;

    auto current = hero->getCurrentPlace();

    for(auto item : current -> getItems())
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
                hero->addHeroItems(item);
                hero->getCurrentPlace()->removeItem(item);
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
            pageNumber = PageNumbers::HERO_PHASE_PAGE;
            if(isPicked)
            {
                actions--;
                isPicked = false;
            }
        }
}

void Gui::guidePhase(shared_ptr<HeroBase>& hero ,int &actions)
{
    Vector2 mouse =GetMousePosition();
    static int option = -1;
    static shared_ptr<Villager> selectedVill; 
    static shared_ptr<Place> choosenPlace;

    if(option == 1)
    {
        vector<shared_ptr<Place>> possiblePlace = hero ->getCurrentPlace()-> getNeighbors();   
        
        for (auto& place : possiblePlace)
        {
            movingAsset(place);
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
        if(choosenPlace)
        {
            float panelW = 800, panelH = 600, pad = 20, Size = 170;
            Rectangle panel = {(SCREEN_WIDTH - panelW) / 2.0f,(SCREEN_HEIGHT - panelH) / 2.0f,panelW, panelH};
            DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, {0,0,0,100});
            DrawRectangleRec(panel, DARKGRAY);
        
            float x = panel.x + pad;
            float y = panel.y + pad;

            for(auto& vill : choosenPlace->getVillagers())
            {
                Texture2D villTex = vill->getAddress();
                Vector2 pos = { x,y }; 
                DrawTextureEx(villTex, pos , 0, Size/villTex.width, WHITE);

                Rectangle villRec = {x, y,villTex.width * (Size/villTex.width),villTex.height * (Size/villTex.width)};

                if(CheckCollisionPointRec(mouse , villRec) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    selectedVill = vill;
                }

                if(vill == selectedVill)
                DrawRectangleLinesEx(villRec, 3, YELLOW);

                x += Size + pad;
                if (x + Size > panel.x + panel.width)
                {
                    x = panel.x + pad;
                    y += Size + pad;
                }
            }
        }
        if(IsKeyPressed(KEY_BACKSPACE))
        {
            if(selectedVill)
            {
                sys->moveVillager(selectedVill , hero->getCurrentPlace());
                if(selectedVill->getSafeZone() == selectedVill->getVillagerLoc())
                {
                    sys->killVillager(selectedVill);
                    hero->addPerkCard(sys->getRandomPerk());
                }
                selectedVill = nullptr; 
                pageNumber = PageNumbers::HERO_PHASE_PAGE;
                actions--;
                option = -1;
            }
            else
            {
                choosenPlace = nullptr;
                option = -1;
            }
        }
    }
    else if(option == 2)
    {
        static bool selected = false;
        vector<shared_ptr<Place>> possiblePlace = hero ->getCurrentPlace()-> getNeighbors();
        if(selectedVill && selected)
        {
            for (auto& place : possiblePlace )
            {
               movingAsset(place);
            }
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                for (auto& place : possiblePlace)
                {
                    if (place->isClicked(mouse))
                    {
                        sys->moveVillager(selectedVill,place);
                        if(selectedVill->getSafeZone() == selectedVill->getVillagerLoc())
                        {
                            sys->killVillager(selectedVill);
                            hero->addPerkCard(sys->getRandomPerk());
                        }
                        pageNumber = PageNumbers::HERO_PHASE_PAGE;
                        actions--;
                        selectedVill = nullptr;
                        selected = false;
                        option = -1;
                    }
                }
            }
        }
        else
        {
            float panelW = 800, panelH = 600, pad = 20, Size = 170;
            Rectangle panel = {(SCREEN_WIDTH - panelW) / 2.0f,(SCREEN_HEIGHT - panelH) / 2.0f,panelW, panelH};
            DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, {0,0,0,100});
            DrawRectangleRec(panel, DARKGRAY);
            
            float x = panel.x + pad;
            float y = panel.y + pad;

            auto villInPlace = hero->getCurrentPlace()->getVillagers();

            for(auto& vill : villInPlace)
            {
                Texture2D villTex = vill->getAddress();
                Vector2 pos = { x,y }; 
                DrawTextureEx(villTex, pos , 0, Size/villTex.width, WHITE);

                Rectangle villRec = {x, y,villTex.width * (Size/villTex.width),villTex.height * (Size/villTex.width)};

                if(CheckCollisionPointRec(mouse , villRec) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    selectedVill = vill;
                }

                if(vill == selectedVill)
                DrawRectangleLinesEx(villRec, 3, YELLOW);

                x += Size + pad;
                if (x + Size > panel.x + panel.width)
                {
                    x = panel.x + pad;
                    y += Size + pad;
                }
            }
                if(IsKeyPressed(KEY_BACKSPACE))
                {
                    if(hero->getCurrentPlace()->getVillagers().empty() || !selectedVill)
                    {
                        pageNumber = PageNumbers::HERO_PHASE_PAGE;
                        option = -1;
                    }
                    else
                        selected = true;
                }
            
        }
    }
    else
    {
        float panelW = 840 , panelH = 100  , padding = 10;
        float fontSize = 35 ;
        Rectangle panel = {(SCREEN_WIDTH - panelW) / 2 ,(SCREEN_HEIGHT - panelH) / 2 , panelW , panelH};
        string bring = "You can bring a villager to your location.";
        string put = "You can put a villager from your place to a neighbor location.";
        DrawRectangleRec(panel,DARKGRAY);
        Color bringHover = BLACK;
        Color putHover = BLACK;
        Vector2 bringPos = {panel.x + padding,panel.y + padding };
        Vector2 putPos = {panel.x + padding,panel.y + fontSize + 20};
        
        Rectangle bringRect = { bringPos.x, bringPos.y, panelW - 20, fontSize };
        Rectangle putRect = { putPos.x , putPos.y , panelW  , fontSize };
        if(CheckCollisionPointRec(mouse , bringRect))
        {
            bringHover = YELLOW;
            if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                option = 1;
            }
        }
        else if(CheckCollisionPointRec(mouse , putRect))
        {
            putHover = YELLOW;
            if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                option = 2;
            }
        }

        DrawTextEx(GameFont, bring.c_str(), bringPos , fontSize , 0.0 ,bringHover);
        DrawTextEx(GameFont, put.c_str(), putPos , fontSize , 0.0 ,putHover);

        if(IsKeyPressed(KEY_BACKSPACE))
        {
            pageNumber = PageNumbers::HERO_PHASE_PAGE;
        }
    }
}

void Gui::advancedPhase(std::shared_ptr<HeroBase>& hero,int &actions)
{
    Vector2 mouse = GetMousePosition();
    static int option = -1;
    static bool showErr = false;
    static float time = 0.0;
    static string errText;
    static bool checkRedPower = true;
    if(option == 1)
    {
        bool isCoffin {false};

        for(auto coffin : sys ->getCoffins())
            if (hero->getCurrentPlace()->getName() == coffin) isCoffin = true;

            if(!isCoffin)
            {
                showErr = true;
                time = GetTime();
                errText = "There is no coffin in your current place"; 
                if(sys->getCoffins().empty())
                    errText = "He has been defeated";
                option = -1;
            }

            else 
            {
                vector<Item> redItems;
                int itemsPowerSum = 0;
                for(auto i : hero->getHeroItems()) {
                    if (i.color == card::Color::R) { 
                        redItems.push_back(i); 
                        itemsPowerSum += i.power;

                        if(hero->getHeroName() == "scientist")
                            ++itemsPowerSum;  
                    }
                }
                if ( itemsPowerSum < 6 && checkRedPower) 
                {
                    showErr = true;
                    time = GetTime();
                    errText = "You dont have enough item to advance dracula"; 
                    option = -1;
                }
                else 
                {
                    checkRedPower = false ;
                    float panelW = 800, panelH = 600, pad = 20, Size = 170;
                    Rectangle panel = {(SCREEN_WIDTH - panelW) / 2,(SCREEN_HEIGHT - panelH) / 2,panelW, panelH};
                    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, { 0, 0, 0, 100 });
                    DrawRectangleRec(panel, DARKGRAY);
                    float x = panel.x + pad;
                    float y = panel.y + pad;

                    static int redPower = 0;
                      
                    for (auto& item : hero->getHeroItems())
                    {
                        if (item.color != card::Color::R) continue;

                        Texture2D itemTexure = item.address;
                        float scale = Size / itemTexure.width;
                        Rectangle rec = { x, y, itemTexure.width * scale, itemTexure.height * scale };
                        DrawTextureEx(itemTexure, { x, y }, 0, scale, WHITE);
            
                        if (CheckCollisionPointRec(mouse, rec))
                        {
                            DrawRectangleLinesEx(rec, 2, YELLOW);
                            if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                            {
                                redPower += item.power;
                                if(hero->getHeroName() == "scientist")
                                    ++redPower;

                                hero->deleteItem(item.name);
                                sys->addItem(item);
                                              
                                if (redPower >= 6)
                                {
                                    option = -1;
                                    pageNumber = PageNumbers::HERO_PHASE_PAGE;
                                    sys->destroyClue("coffin", hero->getCurrentPlace()->getName());
                                    redPower = 0;
                                    checkRedPower = true;
                                    actions--;
                                    return;
                                }
                            }                     
                        }
                        x += Size + pad;
                        if (x + Size > panel.x + panel.width)
                        {
                            x = panel.x + pad;
                            y += Size + pad;
                        }
                    }
                }
            }
    }
    else if(option == 2)
    {
        if(hero->getCurrentPlace()->getName() == "precinct") {
            bool check{true};
            vector<Item> validItems;
            for(auto& evi:sys->getEvidence())
            {
                for(auto& item:hero->getHeroItems())
                {
                    if(item.place == evi)
                    {
                        check=false;
                        validItems.push_back(item);
                    }
                }
            }
        
            if(check)
            {
                showErr = true;
                time = GetTime();
                errText = "You dont have a item that came from ";
                for(auto& left :sys->getEvidence())
                    errText += left + ',';
                if(sys->getEvidence().empty())
                    errText = "He has been defeated";
                option = -1;
            }
            else
            {
                float panelW = 800, panelH = 600, pad = 20, Size = 170;
                    Rectangle panel = {(SCREEN_WIDTH - panelW) / 2,(SCREEN_HEIGHT - panelH) / 2,panelW, panelH};
                    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, { 0, 0, 0, 100 });
                    DrawRectangleRec(panel, DARKGRAY);
                    float x = panel.x + pad;
                    float y = panel.y + pad;
                      
                    for (auto& item : validItems)
                    {
                        Texture2D itemTexture = item.address;
                        float scale = Size / itemTexture.width;
                        Rectangle rec = { x, y, itemTexture.width * scale, itemTexture.height * scale };
                        DrawTextureEx(itemTexture, { x, y }, 0, scale, WHITE);
            
                        if (CheckCollisionPointRec(mouse, rec))
                        {
                            DrawRectangleLinesEx(rec, 2, YELLOW);
                            if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                            {
                                hero->deleteItem(item.name);
                                sys->addItem(item);
                                sys->destroyClue("evidence", item.place );
                                actions--;              
                                option = -1;
                                pageNumber = PageNumbers::HERO_PHASE_PAGE;
                                break;
                            }                     
                        }
                        x += Size + pad;
                        if (x + Size > panel.x + panel.width)
                        {
                            x = panel.x + pad;
                            y += Size + pad;
                        }
                    }
                }    
            }
        else
        {
            showErr = true;
            time = GetTime();
            errText = "Your Not in precinct";
            option = -1;
        }
    
    }

    else
    {
        float panelW = 800, panelH = 600, pad = 20, Size = 250;
        Rectangle panel = {(SCREEN_WIDTH - panelW) / 2.0f,(SCREEN_HEIGHT - panelH) / 2.0f,panelW, panelH};
        DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, {0,0,0,100});
        DrawRectangleRec(panel, DARKGRAY);
        Vector2 textSize = MeasureTextEx(GameFont,"Which one you want to advance" , 25, 0.0);
        DrawTextEx(GameFont,"Which one you want to advance" , { panel.x + ((panel.width - textSize.x )/ 2)  , panel.y + pad} , 25 , 0.0 , BLACK );

        float x = panel.x + (pad * 4);
        float y = panel.y + (pad * 4);

        float scale = Size/panel.width;

        Rectangle checkInvisible;
        Rectangle checkDracula;

        int smallingRec = 50;

        for(auto& mon:sys->getAllMonsters())
        {
            Texture2D png = mon->getAddress();
            DrawTextureEx(png , { x , y } , 0.0 , Size / panel.width , WHITE);
            if(mon->getMonsterName() == "dracula")  { checkDracula = {x,y, png.width*scale - 50 , png.height*scale - smallingRec}; x += Size + (pad*5) ;}
            else { checkInvisible = {x,y, (png.width*scale) - smallingRec  , (png.height*scale) - smallingRec }; x += Size + (pad*5) ;}
        }

        if (CheckCollisionPointRec(mouse, checkDracula ) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            option = 1;
        else if (CheckCollisionPointRec(mouse, checkInvisible ) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
           option = 2;
        if(IsKeyPressed(KEY_BACKSPACE))
        {
            pageNumber = PageNumbers::HERO_PHASE_PAGE;
        }
    }
    if(showErr)
    {
        if (GetTime() - time < 2.0)
        {
            float panelW = 500 , panelH = 75, pad = 20, Size = 250;
            Rectangle err = {0 , 20 , (float)SCREEN_WIDTH , panelH };
            DrawRectangleRec(err, RED);
            Vector2 textSize = MeasureTextEx(GameFont,errText.c_str() , 25, 0 );
            DrawTextEx(GameFont,errText.c_str() , {(SCREEN_WIDTH - textSize.x) / 2 + pad , err.y + pad} , 25, 0.0 , WHITE);
        }
        else
        {
            time = 0;
            showErr = false;
            errText = "";
        }
    }
}

void Gui::defeatPhase(std::shared_ptr<HeroBase>& hero , int &actions){
    static int option = -1;
    Vector2 mouse = GetMousePosition();
    static bool showErr = false;
    static float time = 0.0;
    static string errText;
    static bool PowerCheck = true;
    if(option == 1)
    {
        if (sys->foundCluesCount("coffin") != 4)
        {
            showErr = true;
            time = GetTime();
            errText = "You didnt smashed all the coffins"; 
            option = -1;
        }
        else
        {
            bool isInSamePlace {false};
            for(auto monst : sys->getAllMonsters()){
                if (monst->getMonsterName() == "dracula"){
                    if (monst->getCurrentLocation() == hero->getCurrentPlace()) { isInSamePlace = true; break; }
                }
            }

            if(!isInSamePlace)
            {
                showErr = true;
                time = GetTime();
                errText = "Your not in the same place with Dracula"; 
                option = -1;
            }
            else 
            {
                vector<Item> YellowItems;
                int itemsPowerSum = 0;
                for(auto i : hero->getHeroItems()) 
                {
                    if (i.color == card::Color::Y) 
                    { 
                        YellowItems.push_back(i); 
                        itemsPowerSum += i.power;
                        if(hero->getHeroName() == "scientist") 
                            ++itemsPowerSum; 
                    }
                }
                if ( itemsPowerSum < 9 && PowerCheck ) 
                {
                    showErr = true;
                    time =GetTime();
                    errText = "You dont have enough Yellow item to kill dracula"; 
                    option = -1;
                }
                else 
                {
                    PowerCheck = false;
                    float panelW = 800, panelH = 600, pad = 20, Size = 170;
                    Rectangle panel = {(SCREEN_WIDTH - panelW) / 2,(SCREEN_HEIGHT - panelH) / 2,panelW, panelH};
                    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, { 0, 0, 0, 100 });
                    DrawRectangleRec(panel, DARKGRAY);
                    float x = panel.x + pad;
                    float y = panel.y + pad;

                    static int YellowPower = 0;
                      
                    for (auto& item : hero->getHeroItems())
                    {
                        if (item.color != card::Color::Y) continue;

                        Texture2D itemTexure = item.address;
                        float scale = Size / itemTexure.width;
                        Rectangle rec = { x, y, itemTexure.width * scale, itemTexure.height * scale };
                        DrawTextureEx(itemTexure, { x, y }, 0, scale, WHITE);
            
                        if (CheckCollisionPointRec(mouse, rec))
                        {
                            DrawRectangleLinesEx(rec, 2, YELLOW);
                            if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                            {
                                YellowPower += item.power;
                                if(hero->getHeroName() == "scientist")
                                    ++YellowPower;

                                hero->deleteItem(item.name);
                                sys->addItem(item);
                                              
                                if (YellowPower >= 9)
                                {
                                    option = -1;
                                    pageNumber = PageNumbers::HERO_PHASE_PAGE;
                                    for(auto& mon : sys->getAllMonsters())
                                    {
                                        if(mon->getMonsterName() == "dracula")
                                        sys->killMonster(mon);
                                    }
                                    YellowPower = 0;
                                    PowerCheck = true;
                                    actions--;
                                    return;
                                }
                            }                     
                        }
                        x += Size + pad;
                        if (x + Size > panel.x + panel.width)
                        {
                            x = panel.x + pad;
                            y += Size + pad;
                        }
                    }
                }
            }
        }
    }
    else if(option == 2)
    {
        if (sys->foundCluesCount("evidence") != 5)
        {
            showErr = true;
            time = GetTime();
            errText = "You didnt collect all the evidence"; 
            option = -1;
        }
        else
        {
            bool isInSamePlace {false};
            for(auto monst : sys->getAllMonsters()){
                if (monst->getMonsterName() == "invisibleMan"){
                    if (monst->getCurrentLocation() == hero->getCurrentPlace()) { isInSamePlace = true; break; }
                }
            }

            if(!isInSamePlace)
            {
                showErr = true;
                time = GetTime();
                errText = "Your not in the same place with InvisibleMan"; 
                option = -1;
            }
            else 
            {
                vector<Item> redItems;
                int itemsPowerSum = 0;
                for(auto i : hero->getHeroItems()) 
                {
                    if (i.color == card::Color::R) 
                    { 
                        redItems.push_back(i); 
                        itemsPowerSum += i.power;  
                        if(hero->getHeroName() == "scientist")
                            ++itemsPowerSum;
                    }
                }
                if ( itemsPowerSum < 9 && PowerCheck) 
                {
                    showErr = true;
                    time =GetTime();
                    errText = "You dont have enough red item to kill invisibleMan"; 
                    option = -1;
                }
                else 
                {
                    PowerCheck = false;
                    float panelW = 800, panelH = 600, pad = 20, Size = 170;
                    Rectangle panel = {(SCREEN_WIDTH - panelW) / 2,(SCREEN_HEIGHT - panelH) / 2,panelW, panelH};
                    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, { 0, 0, 0, 100 });
                    DrawRectangleRec(panel, DARKGRAY);
                    float x = panel.x + pad;
                    float y = panel.y + pad;

                    static int redPower = 0;
                      
                    for (auto& item : hero->getHeroItems())
                    {
                        if (item.color != card::Color::R) continue;

                        Texture2D itemTexure = item.address;
                        float scale = Size / itemTexure.width;
                        Rectangle rec = { x, y, itemTexure.width * scale, itemTexure.height * scale };
                        DrawTextureEx(itemTexure, { x, y }, 0, scale, WHITE);
            
                        if (CheckCollisionPointRec(mouse, rec))
                        {
                            DrawRectangleLinesEx(rec, 2, YELLOW);
                            if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                            {
                                redPower += item.power;
                                if(hero->getHeroName() == "scientist")
                                    ++redPower;

                                hero->deleteItem(item.name);
                                sys->addItem(item);              
                                if (redPower >= 9)
                                {
                                    option = -1;
                                    pageNumber = PageNumbers::HERO_PHASE_PAGE;
                                    for(auto& mon : sys->getAllMonsters())
                                    {
                                        if(mon->getMonsterName() == "invisibleMan")
                                        sys->killMonster(mon);
                                    }
                                    redPower = 0;
                                    PowerCheck = true;
                                    actions--;
                                    return;
                                }
                            }                     
                        }
                        x += Size + pad;
                        if (x + Size > panel.x + panel.width)
                        {
                            x = panel.x + pad;
                            y += Size + pad;
                        }
                    }
                }
            }
        }
    }
    else
    {
        float panelW = 800, panelH = 600, pad = 20, Size = 250;
        Rectangle panel = {(SCREEN_WIDTH - panelW) / 2.0f,(SCREEN_HEIGHT - panelH) / 2.0f,panelW, panelH};
        DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, {0,0,0,100});
        DrawRectangleRec(panel, DARKGRAY);
        Vector2 textSize = MeasureTextEx(GameFont,"Which one you want to advance" , 25, 0.0);
        DrawTextEx(GameFont,"Which one you want to advance" , { panel.x + ((panel.width - textSize.x )/ 2)  , panel.y + pad} , 25 , 0.0 , BLACK );

        float x = panel.x + (pad * 4);
        float y = panel.y + (pad * 4);

        float scale = Size/panel.width;

        Rectangle checkInvisible;
        Rectangle checkDracula;

        int smallingRec = 50;

        for(auto& mon:sys->getAllMonsters())
        {
            Texture2D png = mon->getAddress();
            DrawTextureEx(png , { x , y } , 0.0 , Size / panel.width , WHITE);
            if(mon->getMonsterName() == "dracula")  { checkDracula = {x,y, png.width*scale - 50 , png.height*scale - smallingRec}; x += Size + (pad*5) ;}
            else { checkInvisible = {x,y, (png.width*scale) - smallingRec  , (png.height*scale) - smallingRec }; x += Size + (pad*5) ;}
        }

        if (CheckCollisionPointRec(mouse, checkDracula ) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            option = 1;
        else if (CheckCollisionPointRec(mouse, checkInvisible ) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
           option = 2;
        if(IsKeyPressed(KEY_BACKSPACE))
        {
            pageNumber = PageNumbers::HERO_PHASE_PAGE;
        }
    }
    if(showErr)
    {
        if (GetTime() - time < 2.0)
        {
            float panelW = 500 , panelH = 75, pad = 20, Size = 250;
            Rectangle err = {0 , 20 , (float)SCREEN_WIDTH , panelH };
            DrawRectangleRec(err, RED);
            Vector2 textSize = MeasureTextEx(GameFont,errText.c_str() , 25, 0 );
            DrawTextEx(GameFont,errText.c_str() , {(SCREEN_WIDTH - textSize.x) / 2 + pad , err.y + pad} , 25, 0.0 , WHITE);
        }
        else
        {
            time = 0;
            showErr = false;
            errText = "";
        }
    }
}

void Gui::playPerkPhase( std::shared_ptr<HeroBase>& hero , int &actions , bool &doMonsterPhase)
{
    static int count = 0;
    static bool found = false;
    static string selectedPerk;
    Vector2 mouse = GetMousePosition();
    if(found)
    {
        if (selectedPerk == "Visit_from_the_Detective")
        {
           for(auto& loc : sys->getAllLocations())
           {
                Vector2 pos = loc->getPosition();
                float radius = 25.0f; 
                movingAsset(loc);
                if (CheckCollisionPointCircle(mouse, pos, radius))
                {
                    DrawCircleLines(pos.x, pos.y, radius + 4.0f, YELLOW);
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                    {
                        for(auto& mon : sys->getAllMonsters())
                        {
                            if(mon->getMonsterName() == "invisibleMan")
                            {
                                sys->moveMonster(mon,loc);
                                found = false;
                                selectedPerk = "" ;
                                hero->deletePerk("Visit_from_the_Detective");
                                pageNumber = PageNumbers::HERO_PHASE_PAGE;
                                break;
                            }
                        }
                    }
                }
            }
        }
        else if (selectedPerk == "Break_of_Dawn")
        {
            doMonsterPhase = false;
            for(int i {} ; i < 2; i++){
                Item temp = sys->getRandomItem();
                hero->getCurrentPlace()->addItem(temp);
            }
            found = false;
            selectedPerk = "" ;
            hero->deletePerk("Break_of_Dawn");
            pageNumber = PageNumbers::HERO_PHASE_PAGE;
        }
        else if (selectedPerk == "Overstock") {
            for (auto her : sys->getAllHeros()) {
                Item temp = sys->getRandomItem();
                sys->putItemInPlace(her->getCurrentPlace()->getName() , temp);
            }
            found = false;
            selectedPerk = "" ;
            hero->deletePerk("Overstock");
            pageNumber = PageNumbers::HERO_PHASE_PAGE;
        }
        else if (selectedPerk == "Late_into_the_Night") {
            actions += 2;
            found = false;
            selectedPerk = "" ;
            hero->deletePerk("Late_into_the_Night");
            this->pageNumber = PageNumbers::HERO_PHASE_PAGE; 
        }
        else if (selectedPerk == "Repel") 
        {
            static string nameMon;
            {
                auto monsters = sys->getAllMonsters();
                if (!monsters.empty() && monsters[0] != nullptr) {
                    nameMon = monsters[0]->getMonsterName();
                } else {
                    pageNumber = PageNumbers::HERO_PHASE_PAGE;
                    hero->deletePerk("Repel");
                    found = false;
                    selectedPerk = "" ;
                    count = 0;
                    return;
                }
            }

           if(count < 2)
            {
                for(auto& mon : sys->getAllMonsters())
                {    
                    if(nameMon == mon->getMonsterName()) {
                        float x = 0, y = 20 , pad = 20;
                        float panelH = 75;
                        Rectangle rec = {x,y,(float)SCREEN_WIDTH,panelH};
                        DrawRectangleRec(rec , BLACK);
                        string name = mon->getMonsterName();
                        Vector2 size = MeasureTextEx(GameFont,name.c_str(),25,0);
                        DrawTextEx(GameFont,name.c_str(),{((SCREEN_WIDTH-size.x)/2)+pad , y + pad },25,0,WHITE);
                        auto currentLoc = mon->getCurrentLocation();
                        if (!currentLoc) continue;
                        auto neis = currentLoc->getNeighbors();
                        for (auto& place : neis)
                        {
                            movingAsset(place);
                        }

                        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                        {
                            for (auto& place : neis)
                            {
                                if (place->isClicked(mouse))
                                {
                                    sys->moveMonster(mon, place);
                                    count++;   
                                }
                            }
                        }
                    }
                }        
            } 
            else
            {
                if(count >= 4)
                {
                    pageNumber = PageNumbers::HERO_PHASE_PAGE;
                    hero->deletePerk("Repel");
                    found = false;
                    selectedPerk = "" ;
                    count = 0;
                    return;
                }
                for(auto& mon : sys->getAllMonsters()) {
                    if(mon->getMonsterName() != nameMon) {
                        float x = 0, y = 20 , pad = 20;
                        float panelH = 75;
                        Rectangle rec = {x,y,(float)SCREEN_WIDTH,panelH};
                        DrawRectangleRec(rec , BLACK);
                        string name = mon->getMonsterName();
                        Vector2 size = MeasureTextEx(GameFont,name.c_str(),25,0);
                        DrawTextEx(GameFont,name.c_str(),{((SCREEN_WIDTH-size.x)/2)+pad , y + pad },25,0,WHITE);
                        auto currentLoc = mon->getCurrentLocation();
                        if (!currentLoc) continue;
                        auto neis = currentLoc->getNeighbors();

                        for (auto& place : neis)
                        {
                            movingAsset(place);
                        }

                        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                        {
                            for (auto& place : neis)
                            {
                                if (place->isClicked(mouse))
                                {
                                    sys->moveMonster(mon, place);
                                    count++;   
                                }
                            }
                        }
                    }
                }
            }
        }
        else if (selectedPerk == "Hurry") 
        {
            static bool isThereVillager = false;
            static shared_ptr<Place> targetPlace;
            static vector<shared_ptr<Villager>> selectedVillagers;

            if(count < 2)
            {
                float x = 0, y = 20 , pad = 20;
                float panelH = 75;
                Rectangle rec = {x,y,(float)SCREEN_WIDTH,panelH};
                DrawRectangleRec(rec , BLACK);
                string name = hero->getHeroName();
                Vector2 size = MeasureTextEx(GameFont,name.c_str(),25,0);
                DrawTextEx(GameFont,name.c_str(),{((SCREEN_WIDTH-size.x)/2)+pad , y + pad },25,0,WHITE);
                
                
                auto neis = hero->getCurrentPlace()->getNeighbors();

                for (auto& place : neis)
                {
                    movingAsset(place);
                }

                if (!isThereVillager && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    for (auto& place : neis)
                    {
                        if (place->isClicked(mouse))
                        {
                            targetPlace = place;
                            auto villagers = hero->getCurrentPlace()->getVillagers();
                            if (!villagers.empty())
                            {
                                isThereVillager = true;
                                selectedVillagers.clear();
                            }
                            else
                            {
                                sys->moveHero(hero, targetPlace);
                                count++;
                                //pageNumber = PageNumbers::HERO_PHASE_PAGE;
                            }   
                        }
                    }
                }

                if (isThereVillager)
                {
                    float panelW = 800, panelH = 600, pad = 20, Size = 170;
                    Rectangle panel = {(SCREEN_WIDTH - panelW) / 2.0f,(SCREEN_HEIGHT - panelH) / 2.0f,panelW, panelH};
                    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, {0,0,0,100});
                    DrawRectangleRec(panel, DARKGRAY);

                    float x = panel.x + pad;
                    float y = panel.y + pad;
                    
                    for (auto& vill : hero->getCurrentPlace()->getVillagers())
                    {
                        Texture2D villTex = vill->getAddress();
                        Vector2 pos = { x,y }; 
                        DrawTextureEx(villTex, pos , 0, Size/villTex.width, WHITE);

                        Rectangle villRec = {x, y,villTex.width * (Size/villTex.width),villTex.height * (Size/villTex.width)};

                        if (CheckCollisionPointRec(mouse, villRec) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                        {
                            auto it = find(selectedVillagers.begin(),selectedVillagers.end(),vill);
                            if (it == selectedVillagers.end())
                                selectedVillagers.push_back(vill);
                            else
                                selectedVillagers.erase(it);
                        }
                        if (find(selectedVillagers.begin(),selectedVillagers.end(),vill) != selectedVillagers.end())
                        {
                            DrawRectangleLinesEx(villRec, 3, YELLOW);
                        }

                        x += Size + pad;
                        if (x + Size > panel.x + panel.width)
                        {
                            x = panel.x + pad;
                            y += Size + pad;
                        }
                    }

                    if (IsKeyPressed(KEY_BACKSPACE))
                    {
                        sys->moveHero(hero, targetPlace);
                        count++;
                        for (auto& vill : selectedVillagers)
                        {
                            sys->moveVillager(vill, targetPlace);

                            if(vill->getSafeZone() == vill->getVillagerLoc())
                            {
                                sys->killVillager(vill);
                                hero->addPerkCard(sys->getRandomPerk());
                            }
                        }
                        selectedVillagers.clear();
                        isThereVillager = false;
                        //pageNumber = PageNumbers::HERO_PHASE_PAGE;
                    } 
                }
            }
            else
            {
                if(count >= 4)
                {
                    pageNumber = PageNumbers::HERO_PHASE_PAGE;
                    hero->deletePerk("Hurry");
                    found = false;
                    selectedPerk = "" ;
                    count = 0;
                    return;
                }
                
                for(auto& second : sys->getAllHeros())
                {
                
                    if(second->getHeroName() != hero->getHeroName())
                    {
                    float x = 0, y = 20 , pad = 20;
                    float panelH = 75;
                    Rectangle rec = {x,y,(float)SCREEN_WIDTH,panelH};
                    DrawRectangleRec(rec , BLACK);
                    string name = second->getHeroName();
                    Vector2 size = MeasureTextEx(GameFont,name.c_str(),25,0);
                    DrawTextEx(GameFont,name.c_str(),{((SCREEN_WIDTH-size.x)/2)+pad , y + pad },25,0,WHITE);
                    

                    auto neis = second->getCurrentPlace()->getNeighbors();

                    for (auto& place : neis)
                    {
                        movingAsset(place);
                    }

                    if (!isThereVillager && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                    {
                        for (auto& place : neis)
                        {
                            if (place->isClicked(mouse))
                            {
                                targetPlace = place;
                                auto villagers = second->getCurrentPlace()->getVillagers();
                                if (!villagers.empty())
                                {
                                    isThereVillager = true;
                                    selectedVillagers.clear();
                                }
                                else
                                {
                                    sys->moveHero(second, targetPlace);
                                    count++;
                                    //pageNumber = PageNumbers::HERO_PHASE_PAGE;
                                }   
                            }
                        }
                    }

                    if (isThereVillager)
                    {
                        float panelW = 800, panelH = 600, pad = 20, Size = 170;
                        Rectangle panel = {(SCREEN_WIDTH - panelW) / 2.0f,(SCREEN_HEIGHT - panelH) / 2.0f,panelW, panelH};
                        DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, {0,0,0,100});
                        DrawRectangleRec(panel, DARKGRAY);

                        float x = panel.x + pad;
                        float y = panel.y + pad;
                        
                        for (auto& vill : second->getCurrentPlace()->getVillagers())
                        {
                            Texture2D villTex = vill->getAddress();
                            Vector2 pos = { x,y }; 
                            DrawTextureEx(villTex, pos , 0, Size/villTex.width, WHITE);

                            Rectangle villRec = {x, y,villTex.width * (Size/villTex.width),villTex.height * (Size/villTex.width)};

                            if (CheckCollisionPointRec(mouse, villRec) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                            {
                                auto it = find(selectedVillagers.begin(),selectedVillagers.end(),vill);
                                if (it == selectedVillagers.end())
                                    selectedVillagers.push_back(vill);
                                else
                                    selectedVillagers.erase(it);
                            }
                            if (find(selectedVillagers.begin(),selectedVillagers.end(),vill) != selectedVillagers.end())
                            {
                                DrawRectangleLinesEx(villRec, 3, YELLOW);
                            }

                            x += Size + pad;
                            if (x + Size > panel.x + panel.width)
                            {
                                x = panel.x + pad;
                                y += Size + pad;
                            }
                        }

                        if (IsKeyPressed(KEY_BACKSPACE))
                        {
                            sys->moveHero(second, targetPlace);
                            count++;
                            for (auto& vill : selectedVillagers)
                            {
                                sys->moveVillager(vill, targetPlace);

                                if(vill->getSafeZone() == vill->getVillagerLoc())
                                {
                                    sys->killVillager(vill);
                                    second->addPerkCard(sys->getRandomPerk());
                                }
                            }
                            selectedVillagers.clear();
                            isThereVillager = false;
                            //pageNumber = PageNumbers::HERO_PHASE_PAGE;
                        } 
                    } 
                }   
            }
        }
    }
}
    else
    {
        float panelW = 900, panelH = 800, pad = 20, Size = 120;
        Rectangle panel = {(SCREEN_WIDTH - panelW) / 2.0f,(SCREEN_HEIGHT - panelH) / 2.0f,panelW, panelH};
        DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, {0,0,0,100});
        DrawRectangleRec(panel, DARKGRAY);
        
        float x = panel.x + pad;
        float y = panel.y + pad;

        for(auto& perk : hero->getHeroPerks())
        {
            Texture2D png = perk.address;
            Vector2 pos {x,y};
            float scale = Size/panel.width;
            DrawTextureEx(png , pos,0,scale,WHITE);
            Rectangle pickPerk = {x,y,scale*png.width,scale*png.height};

            if(CheckCollisionPointRec(mouse,pickPerk))
            {
                DrawRectangleLinesEx(pickPerk,3,YELLOW);
                if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    found = true;
                    selectedPerk = perk.name;
                }
            }
            x += Size + pad;
            if(x + Size > panel.x + panel.width)
            {
                x = panel.x + pad;
                y += Size + pad;
            }
        }
        if(IsKeyPressed(KEY_BACKSPACE))
        {
            pageNumber = PageNumbers::HERO_PHASE_PAGE;
        }
    }
}


Gui::~Gui()
{
    UnloadTexture(gameMap);
    UnloadFont(GameFont);
    UnloadTexture(coffins);
    UnloadTexture(smahsedCoffins);
    CloseWindow();
}