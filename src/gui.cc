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

Gui::Gui(System *s,const int width,const int height):sys(s),SCREEN_WIDTH(width),SCREEN_HEIGHT(height)
{
    pageNumber = PageNumbers::WELCOME_PAGE;
    isEnd = -1;
    round = 0;
    actions = 1000;
    GameFont = LoadFont("../Horrified_Assets/Melted.ttf");
    sys->setFont(GameFont);
    selectedSaveFolderNumber = -1;
    showSavePopup = false;
    savePopupStart = 0.0;
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
            showSavePopup = true;
            savePopupStart = GetTime();
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
        // draw save popup toast if active
        if (showSavePopup) {
            double elapsed = GetTime() - savePopupStart;
            if (elapsed < 3.0) {
                float panelW = 500 , panelH = 75;
                Rectangle toast = { (SCREEN_WIDTH - panelW) / 2.0f , 20 , panelW , panelH };
                DrawRectangleRec(toast, DARKGREEN);
                const char *msg = "you saved game success fully";
                Vector2 textSize = MeasureTextEx(GameFont, msg , 25, 0 );
                DrawTextEx(GameFont, msg , { toast.x + (panelW - textSize.x) / 2.0f , toast.y + (panelH - textSize.y) / 2.0f } , 25, 0.0 , WHITE);
            } else {
                showSavePopup = false;
                savePopupStart = 0.0;
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



Gui::~Gui()
{
    CloseWindow();
    if(sys != nullptr)
    {
        delete sys;
        sys = nullptr;
    }
}