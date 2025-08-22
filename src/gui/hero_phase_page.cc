#include "gui/hero_phase_page.hpp"
#include "system.hpp"

using namespace std;

HeroPhasePage::HeroPhasePage (
    const Font &f,
    System *s
        
) : font(f) , pad(20.f) , panelW(LEFT_PANEL_WIDTH - (2*pad)) , panelH((SCREEN_HEIGHT / 9) - 25),
    buttonColor({ 230, 225, 200, 255 }) , sys(s) , leftScroll(0.0f)
{
    gameMapImg = LoadTexture("../Horrified_Assets/map.png");
    coffins = LoadTexture("../Horrified_Assets/Items/Coffins/Coffin.png");
    smahsedCoffins = LoadTexture("../Horrified_Assets/Items/Coffins/SmashedCoffin.png");
    moveRec = { pad , pad , panelW , panelH };
    PickRec = { pad , moveRec.y + pad + panelH , panelW , panelH};
    GuidRec  = { pad , PickRec.y + pad + panelH , panelW , panelH};
    speciallRec = {pad , GuidRec.y + pad + panelH , panelW , panelH};
    AdvanceRec = {pad , speciallRec.y + pad + panelH , panelW , panelH };
    DefeatRec = {pad , AdvanceRec.y + pad + panelH , panelW , panelH };
    PerkRec = { pad , DefeatRec.y + pad + panelH , panelW ,panelH };
    exitANDsave = { pad , PerkRec.y + pad + panelH , panelW , panelH};
    Help = {pad , exitANDsave.y + pad + panelH , panelW , panelH };
    mainMenuBtn = { pad , Help.y + pad + panelH , panelW , panelH };
    exitOnly = { pad , mainMenuBtn.y + pad + panelH , panelW , panelH };
}

void HeroPhasePage::draw(shared_ptr<HeroBase> &cHero ,int &actions, PageNumbers &cPage) {
    static Rectangle Src = { 0, 0, (float)gameMapImg.width, (float)gameMapImg.height };
    static Rectangle Dest = {
            (float)LEFT_PANEL_WIDTH,
            (float)UP_PANEL_HEIGHT, 
            (float)(UP_PANEL_WIDTH),
            (float)(SCREEN_HEIGHT - UP_PANEL_HEIGHT)
    };
    static Vector2 origin = { 0, 0 };
    ClearBackground(BLACK);
    DrawTexturePro(gameMapImg, Src, Dest, origin, 0 , WHITE);
    drawLeftPanel();
    drawEvindence();
    drawCoffin();
    drawRightPanel(cHero);
    drawUpPanel(cHero,actions);
}

void HeroPhasePage::drawRightPanel(shared_ptr<HeroBase>& hero){
    Vector2 mouse = GetMousePosition();
    float pad = 5;
    float padBetweenItems = 55;

    auto items = hero->getHeroItems();
    Rectangle rightPanel {
        (float)SCREEN_WIDTH-RIGHT_PANEL_WIDTH, 0 ,
        (float)RIGHT_PANEL_WIDTH ,
        (float)SCREEN_HEIGHT
    };
    DrawRectangleRec(rightPanel , DARKGRAY);
 
    float Size = 50;
    float x = rightPanel.x + pad ,y = rightPanel.y + pad;
    float itemsBottom = y; // will track bottom of the last row of items
    for(auto& it : items){
        Texture2D itemTex = it.address;
        Vector2 pos = {x,y};
        float scale = Size/rightPanel.width;
        DrawTextureEx(itemTex,pos,0.0,scale,WHITE);
        if (y + Size > itemsBottom) itemsBottom = y + Size;
        x += (Size + padBetweenItems);
        if (x + Size + padBetweenItems  > rightPanel.x + rightPanel.width){
            x = rightPanel.x + pad;
            y += Size + padBetweenItems ;
        }
    }

    // No longer draw last perk here; moved to top panel per latest request
}

void HeroPhasePage::drawLeftPanel(){
    Rectangle rec {0 , 0 , (float)LEFT_PANEL_WIDTH , (float)SCREEN_HEIGHT};
    DrawRectangleRec(rec , DARKGRAY);

    // Scroll handling
    float scrollSpeed = 30.0f;
    leftScroll += GetMouseWheelMove() * scrollSpeed;

    // Build list of buttons (logical positions)
    vector<pair<string, Rectangle>> logicalButtons = { 
        {"MOVE", moveRec},
        {"PICK", PickRec},
        {"GUIDE", GuidRec},
        {"SPECIALL", speciallRec},
        {"ADVANCE", AdvanceRec},
        {"DEFEAT", DefeatRec},
        {"PERK", PerkRec},
        {"SAVE GAME", exitANDsave},
        {"HELP" , Help},
        {"MAIN MENU", mainMenuBtn},
        {"EXIT" , exitOnly}
    };

    // Compute content height
    float contentTop = moveRec.y;
    Rectangle last = exitOnly;
    float contentBottom = last.y + last.height;
    float contentHeight = contentBottom - contentTop;
    float viewportHeight = rec.height;
    float minScroll = min(0.0f, viewportHeight - contentHeight - contentTop);
    if (leftScroll > 0.0f) leftScroll = 0.0f;
    if (leftScroll < minScroll) leftScroll = minScroll;

    BeginScissorMode(rec.x, rec.y, rec.width, rec.height);

    float fontSize = 50;
    for (auto& button : logicalButtons){
        string text = button.first;
        Rectangle base = button.second;
        Rectangle targetRec = base;
        targetRec.y += leftScroll;

        if (targetRec.y + targetRec.height < rec.y || targetRec.y > rec.y + rec.height) continue;

        DrawRectangleRec(targetRec, buttonColor);
        Vector2 textSize = MeasureTextEx(font, text.c_str(), fontSize, 0);
        float centerX = (targetRec.width - textSize.x) / 2;
        float centerY = (targetRec.height - textSize.y) / 2;
        DrawTextEx(font, text.c_str(), {targetRec.x + centerX, targetRec.y + centerY}, fontSize, 0, BLACK);
    }

    EndScissorMode();
}

void HeroPhasePage::drawCoffin() {
    for(auto& cof : sys->getCoffins()){
        for(auto& loc : sys->getAllLocations()){
            if(cof == loc->getName())
            {
                Vector2 target = loc->getPosition();
                DrawTextureEx(coffins,{target.x , target.y - 50},0,0.075,WHITE);
            }
        }
    }
    for(auto& smash : sys->getSmashed()){
        for(auto& loc : sys->getAllLocations()){
            if(smash == loc->getName()){
                Vector2 target = loc->getPosition();
                DrawTextureEx(smahsedCoffins,{target.x , target.y - 50},0,0.075,WHITE);
            }
        }
    }
}

void HeroPhasePage::drawEvindence(){
    for(auto& evi : sys->getEvidence()){
        for(auto& loc : sys->getAllLocations()){
            if(evi == loc->getName()){
                Vector2 target = loc->getPosition();
                DrawCircleLinesV(target,60,GREEN);
            }
        }
    }
}

void HeroPhasePage::drawUpPanel(std::shared_ptr<HeroBase>& heroInfo, int actions){ // must override{
    const int fontSize = 35;
    const int pad = 10;  
    string terror = to_string(sys->getTerrorLevel());
    string act = to_string(actions);
    string heroTurn = heroInfo->getHeroName();// this part is totally by testing diffrent parts

    Rectangle rec = {(float)LEFT_PANEL_WIDTH,0,(float)UP_PANEL_WIDTH,(float)UP_PANEL_HEIGHT};
    DrawRectangleRec(rec, DARKGRAY);

    Vector2 basePos = { rec.x + 30, rec.y + (UP_PANEL_HEIGHT - fontSize) / 2 };
    DrawTextEx(font, "Terror level:", basePos, fontSize, 0, BLACK);

    Vector2 Terror = {basePos.x + MeasureTextEx(font, "Terror level:", fontSize, 0).x + pad , basePos.y};
    DrawTextEx(font, terror.c_str(), Terror, fontSize, 0, BLACK);

    Vector2 firstStart = { Terror.x + fontSize, Terror.y + fontSize + 5 };
    Vector2 firstEnd   = { Terror.x + fontSize, Terror.y - pad };
    DrawLineEx(firstStart, firstEnd, 4, WHITE);

    Vector2 action = {Terror.x + fontSize + (2*pad),basePos.y};
    DrawTextEx(font, "Actions:", action, fontSize, 0, BLACK);

    Vector2 actionPos = {action.x + MeasureTextEx(font, "Actions:", fontSize, 0).x + pad,action.y};
    DrawTextEx(font, act.c_str(), actionPos, fontSize, 0, BLACK);

    Vector2 secondStart = { actionPos.x + fontSize, actionPos.y + fontSize + 5 };
    Vector2 secondEnd   = { actionPos.x + fontSize, actionPos.y - pad };
    DrawLineEx(secondStart, secondEnd, 4, WHITE);

        // Hero Turn
    Vector2 turnLabel = { actionPos.x + fontSize + (  pad), basePos.y };
    DrawTextEx(font, "Hero Turn:", turnLabel, fontSize, 0, BLACK);
    Vector2 turnVal = { turnLabel.x + MeasureTextEx(font, "Hero Turn:", fontSize, 0).x + pad, turnLabel.y };
    DrawTextEx(font, heroTurn.c_str(), turnVal, fontSize, 0, BLACK);

    // Vertical separator line
    float lineX = turnVal.x + MeasureTextEx(font, heroTurn.c_str(), fontSize, 0).x + (2 * pad);
    float lineTop = turnLabel.y - 5.0f;
    float lineBottom = turnLabel.y + fontSize + 10.0f;
    DrawLineEx({ lineX, lineTop }, { lineX, lineBottom }, 2.0f, WHITE);

    // Perk (same line as Hero Turn, right of separator)
    std::string lastPerk = heroInfo->getLastPlayedName();
    if (lastPerk.empty()) lastPerk = "-";
    float infoFontSize = 24.0f;
    Vector2 perkLabel = { lineX + (2 * pad), turnLabel.y + 25 };
    DrawTextEx(font, "Perk:", perkLabel, infoFontSize, 0, BLACK);
    Vector2 perkVal = { perkLabel.x + MeasureTextEx(font, "Perk:", infoFontSize, 0).x + pad, perkLabel.y };
    DrawTextEx(font, lastPerk.c_str(), perkVal, infoFontSize, 0, BLACK);

    // Frenzy (above Hero Turn line)
    std::string frenzyName = "-";
    for (auto &m : sys->getAllMonsters()) {
        if (m && m->getIsFrenzed()) { frenzyName = m->getMonsterName(); break; }
    }
    Vector2 frenzyLabel = { perkLabel.x, turnLabel.y - 5 };
    DrawTextEx(font, "Frenzy:", frenzyLabel, infoFontSize, 0, BLACK);
    Vector2 frenzyVal = { frenzyLabel.x + MeasureTextEx(font, "Frenzy:", infoFontSize, 0).x + pad, frenzyLabel.y };
    DrawTextEx(font, frenzyName.c_str(), frenzyVal, infoFontSize, 0, BLACK);
}

void HeroPhasePage::drawMap() {
    Vector2 mouse = GetMousePosition();
    for (auto &p : sys->getAllLocations()) {
        p->draw(mouse);
    }
}

void HeroPhasePage::update(shared_ptr<HeroBase> &cHero ,int &actions , PageNumbers &cPage){
    Vector2 mouse = GetMousePosition();
    drawMap();
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) ) {
        Vector2 mouse = GetMousePosition();
        for (auto &p : sys->getAllLocations()) {
            if (p->isClicked(mouse)){
                selectedPlace = p;
                break;
            }
        }
    }    
    if (selectedPlace) {
        PlaceInfo(selectedPlace);
        if(GetKeyPressed() == KEY_BACKSPACE){
            selectedPlace = nullptr;
            scroll = 0;
        }
    }
   
    Vector2 logicalMouse = mouse;
    if (mouse.x >= 0 && mouse.x <= LEFT_PANEL_WIDTH) {
        logicalMouse.y -= leftScroll;
    }

    BeginScissorMode(0, 0, LEFT_PANEL_WIDTH, SCREEN_HEIGHT);
    if(CheckCollisionPointRec(logicalMouse, moveRec)){
        Rectangle hl = moveRec; hl.y += leftScroll;
        DrawRectangleLinesEx(hl, 8 ,DARKGREEN);
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            cPage = PageNumbers::MOVE_PAGE;
    }
    else if (CheckCollisionPointRec(logicalMouse,PickRec)){
        Rectangle hl = PickRec; hl.y += leftScroll;
        DrawRectangleLinesEx(hl, 8 ,DARKGREEN);
         if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
             cPage = PageNumbers::PICKUP_PAGE;
    }
    else if(CheckCollisionPointRec(logicalMouse , GuidRec)){
        Rectangle hl = GuidRec; hl.y += leftScroll;
        DrawRectangleLinesEx(hl, 8 ,DARKGREEN);
         if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            cPage = PageNumbers::GUIDE_PAGE;
    }
    else if(CheckCollisionPointRec(logicalMouse , AdvanceRec)){
        Rectangle hl = AdvanceRec; hl.y += leftScroll;
        DrawRectangleLinesEx(hl, 8 ,DARKGREEN);
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            cPage = PageNumbers::ADVANCED_PAGE;
    }
    else if(CheckCollisionPointRec(logicalMouse , DefeatRec)){
         Rectangle hl = DefeatRec; hl.y += leftScroll;
         DrawRectangleLinesEx(hl, 8 ,DARKGREEN);
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            cPage = PageNumbers::DEFEAT_PAGE;
    }
    else if(CheckCollisionPointRec(logicalMouse , this->exitANDsave)){
        Rectangle hl = this->exitANDsave; hl.y += leftScroll;
        DrawRectangleLinesEx(hl, 8 ,DARKGREEN);
         if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            cPage = PageNumbers::SAVE_MODE;
         }
    }
    else if(CheckCollisionPointRec(logicalMouse , speciallRec)){
         Rectangle hl = speciallRec; hl.y += leftScroll;
         DrawRectangleLinesEx(hl, 8 ,DARKGREEN);
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            cPage = PageNumbers::SPECIALACTION_PAGE;
    }
    else if(CheckCollisionPointRec(logicalMouse , PerkRec)){
        Rectangle hl = PerkRec; hl.y += leftScroll;
        DrawRectangleLinesEx(hl, 8 ,DARKGREEN);
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            cPage = PageNumbers::PLAYPERK_PAGE;
    }
    else if(CheckCollisionPointRec(logicalMouse , mainMenuBtn)){
        Rectangle hl = mainMenuBtn; hl.y += leftScroll;
        DrawRectangleLinesEx(hl, 8 ,DARKGREEN);
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            cPage = PageNumbers::WELCOME_PAGE;
        }
    }
    else if (CheckCollisionPointRec(logicalMouse , Help)){
        Rectangle hl = Help; hl.y += leftScroll;
        DrawRectangleLinesEx(hl, 8 ,DARKGREEN);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            cPage = PageNumbers::HELP_PAGE;
        }
    }
    else if(CheckCollisionPointRec(logicalMouse , exitOnly)){
        Rectangle hl = exitOnly; hl.y += leftScroll;
        DrawRectangleLinesEx(hl, 8 ,DARKGREEN);
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            cPage = PageNumbers::EXIT_PAGE;
        }
    }
    EndScissorMode();
}

void HeroPhasePage::PlaceInfo(shared_ptr<Place> selected){
    vector<Texture2D> temp;
    for(auto& hero : selected->getAllHeroes()){
        temp.push_back(hero->getAddress());
    }  
    for(auto& mon:selected->getMonsters()){
        temp.push_back(mon->getAddress());
    }
    for(auto& vill : selected->getVillagers()){
        temp.push_back(vill->getAddress());
    }  
    for(auto& item:selected->getItems()){
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

    for (size_t i = 0; i < temp.size(); ++i){
        if (saveX + Size > panel.x + panel.width){
            saveX = startX;
            saveY += rowHeight;
        }
        Vector2 pos = { saveX, saveY };
        DrawTextureEx(temp[i], pos, 0.0f, Size / (float)(temp[i]).width, WHITE);
        saveX += Size + padding;
    }
    EndScissorMode();
}