#include "gameManager.h"
#include <cctype>
#include <iomanip>
#include <map>
#include <cstdlib>
#include <unistd.h>

void cleaningTheMess() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

string GameManager::gameScreenFormat(const string &r1, const string &r2, const string &r3, const string &r4){
    std::ostringstream stringForReturn;

    stringForReturn << "\n\n";
    stringForReturn << std::setw(10) << "\t" << "+----==================[" << r1 << "]===================----+";

    if(!r2.empty()){stringForReturn << "\n" << r2 << std::endl;}
    if(!r3.empty() && !r2.empty()){stringForReturn << r3 << std::endl;}

    if(r2.empty() && r3.empty()){stringForReturn << "\n\n"; }
    stringForReturn << "    " << r4;

    return stringForReturn.str();
}

// >======================== [ GAME_MANAGER FUNCTIONS ] ========================<

void GameManager::loadWords(Database &currentData) {
    for (auto &it : currentData.allWords) {
        if (it.getDifficulty() == 3) {
            HardWords.push_back(new wordHangman(it));
        } else if (it.getDifficulty() == 2) {
            MidWords.push_back(new wordHangman(it));
        } else {
            EasyWords.push_back(new wordHangman(it));
        }
    }
}

void GameManager::randomWord() {
    std::vector<wordHangman*>* selectedWords = nullptr; 
    std::vector<short> WordsReference;

    /**
     * A função não sabe, até que seja chamada, com qual vetor in-class irá trabalhar. Logo, atribuimos um ponteiro para
     * que ela possar "atribuir" um dos vetores para que ela seja possa trabalhar com o vetor.
    */

    if (gameDifficulty == 1) {
        selectedWords = &EasyWords;
    } else if (gameDifficulty == 2) {
        selectedWords = &MidWords;
    } else if (gameDifficulty == 3) {
        selectedWords = &HardWords;
    }

    /**
     * Se o vetor não for nulo, poderemos começar a verificação. Percorremos inicialmente, num laço for, o vetor atribuído a 
     * selectedWords. Verificamos a categoria selecionada e baseamos nossa busca a partir dela. Ao final, "salvamos" o indice 
     * de onde foi encontrada a palavra e o adicionamos para "WordsReference".
    */

    if (selectedWords != nullptr) {
        for (size_t i = 0; i < selectedWords->size(); ++i) {
            wordHangman* word = (*selectedWords)[i];
            bool categoryVerifier = false;

            // Verificação da categoria do jogo
            switch (gameCategory) {
                case 1:
                    categoryVerifier = word->champ;
                    break;
                case 2:
                    categoryVerifier = (word->position == 1);
                    break;
                case 3:
                    categoryVerifier = (word->position == 2);
                    break;
                case 4:
                    categoryVerifier = (word->position == 3);
                    break;
                case 5:
                    categoryVerifier = word->roy;
                    break;
                case 6:
                    categoryVerifier = word->dpoy;
                    break;
                case 7:
                    categoryVerifier = word->mvp;
                    break;
                case 8:
                    categoryVerifier = word->at75;
                    break;
                default:
                    break;
            }

            if (categoryVerifier) {
                WordsReference.push_back(i);
            }
        }
    }

    /*
      * Aqui é feita a randomização do vetor de índices para que tenhamos um valor "aleatório".
      * Com o índice aleatório, atribuímos o elemento para o ponteiro "currentWord" in-class.
    */

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(WordsReference.begin(), WordsReference.end(), g);

    if (!WordsReference.empty()) {
        short number = WordsReference[0];
        currentWord = {(*selectedWords)[number]};
    }
}


// >>>>>>>> gameloops

void GameManager::process_events(){

    // std::cout << "proc: " << gameStateStrings[gameState] << "\n"; [DISCARD]:Debugging stuff 
    if(gameState == GameState::READ_NAME){
        string Name;
        std::getline(std::cin,Name) ;
        currentPlayer->setPlayerName(Name);
    }
    else if(gameState == GameState::READ_GUESS){
        currentWord->guess();
    }
    else if(gameState == GameState::READ_OPTION){
        setMenu();
    }
    else if(gameState == GameState::READ_CAT){
        setCat();
    }
    else if(gameState == GameState::READ_DIF){
        setDiff();
    }
    else if(gameState == GameState::NEW_CHALLENGE){
        currentData->createData();
        loadWords(*currentData); // charging the words
    }
    else if(gameState == GameState::RANDOM){
        randomWord();
    }
    else if(gameState == GameState::RULES){
        std::cin >> uniqueEntry;
    }
    else if(gameState == GameState::SCORE_BOARD){
        std::cin >> uniqueEntry;
    }
    else if(gameState == GameState::WIN){
        if(gameDifficulty == 1){ currentPlayer->incrementEasyWon(); currentPlayer->setOffsetScoring(1); }
        else if(gameDifficulty == 2){ currentPlayer->incrementMidWon(); currentPlayer->setOffsetScoring(2); }
        else{ currentPlayer->incrementHardWon(); currentPlayer->setOffsetScoring(3); }
    }
    else if(gameState == GameState::LOSS){
        if(gameDifficulty == 1){ currentPlayer->incrementEasyLost();}
        else if(gameDifficulty == 2){ currentPlayer->incrementMidLost();}
        else{ currentPlayer->incrementHardLost(); }
        currentWord->revealWord();
    }
    else if(gameState == GameState::REVEAL_WORD){
        std::cin >> uniqueEntry;
    }
    else if(gameState == GameState::EXIT_CONFIRMATION){
        setEnter();
    }
    else if(gameState == GameState::GAME_SAVING){
        currentData->saveData(*currentPlayer);
    }
}

void GameManager::update(){

    
    // std::cout << "up: " << gameStateStrings[gameState] << "\n";
    if(gameState == GameState::START){
        if(!init){init = true;}
        else{gameState = GameState::READ_NAME;}
    }
    else if(gameState == GameState::READ_NAME){
        gameState = GameState::MENU;
    }
    else if(gameState == GameState::MENU){
        gameState = GameState::READ_OPTION;
    }
    else if(gameState == GameState::READ_OPTION){
        if(menuOption == 1){gameState = GameState::NEW_CHALLENGE; }
        else if(menuOption == 2){gameState = GameState::RULES; }
        else if(menuOption == 3){gameState = GameState::SCORE_BOARD; }
        else if(menuOption == 4){gameState = GameState::EXIT_CONFIRMATION; }
        else{gameState = GameState::INV_MENU_OPT;}
    }
    else if(gameState == GameState::INV_MENU_OPT){
        gameState = GameState::READ_OPTION;
    }
    else if(gameState == GameState::NEW_CHALLENGE){ // [ IN-CHALLENGE FLUX ]        
        gameState = GameState::GAME;
    }
    else if(gameState == GameState::GAME){
        gameState = GameState::DIF_CHOOSE;
    }
    else if(gameState == GameState::DIF_CHOOSE){
        gameState = GameState::READ_DIF;
    }
    else if(gameState == GameState::READ_DIF){
        if((gameDifficulty == 1 || gameDifficulty == 2 || gameDifficulty == 3)){
            gameState = GameState::CAT_CHOOSE;
        }
        else{gameState = GameState::INV_DIF;}
    }
    else if(gameState == GameState::INV_DIF){
        gameState = GameState::READ_DIF;
    }
    else if(gameState == GameState::CAT_CHOOSE){
        gameState = GameState::READ_CAT;
    }
    else if(gameState == GameState::READ_CAT){
        if(gameCategory >= 1 && gameCategory <= 8 ){gameState = GameState::RANDOM;}
        else{gameState = GameState::INV_CAT;}
    }
    else if(gameState == GameState::INV_CAT){
        gameState = GameState::READ_CAT;
    }
    else if (gameState == GameState::RANDOM){
        bool wordAlreadyPlayed = false;

        for(const auto &i : currentPlayer->alreadyPlayedWords){
            if(i == currentWord){ wordAlreadyPlayed = true; break;}
        }
        if(wordAlreadyPlayed){gameState = GameState::RANDOM;}
        else{gameState = GameState::IN_GAME;}
    }

    else if(gameState == GameState::IN_GAME){
        sleep(1);
        gameState = GameState::READ_GUESS;
    }
    else if(gameState == GameState::READ_GUESS){
        string guessCopy = currentWord->currentGuess;
        switch(currentWord->guessValidate(guessCopy)){
        case 1:
            gameState = GameState::ALREADY_USED;
            break;
        case 2:
            gameState = GameState::RIGHT_GUESS;
            break;
        case 3:
            gameState = GameState::WRONG_GUESS;
            break;
        case 4:
            gameState = GameState::GAME_END;
            break;
        case 5:
            gameState = GameState::INV_GUESS;
            break;
        case 6:
            gameState = GameState::FULL_GUESS_WRONG;
            break;
        }
    }
    else if(gameState == GameState::RIGHT_GUESS){
        gameState = GameState::CHECK_WIN_OR_LOSS;
    }
    else if(gameState == GameState::WRONG_GUESS){
        gameState = GameState::CHECK_WIN_OR_LOSS;
    }
    else if(gameState == GameState::FULL_GUESS_WRONG){
        gameState = GameState::CHECK_WIN_OR_LOSS;
    }

    else if(gameState == GameState::INV_GUESS){
        gameState = GameState::READ_GUESS;
    }
    else if(gameState == GameState::ALREADY_USED){
        gameState = GameState::READ_GUESS;
    }
    else if(gameState == GameState::CHECK_WIN_OR_LOSS){
        if(currentWord->checkWin()){gameState = GameState::WIN;}
        else if(currentWord->checkLoss(gameDifficulty)){gameState = GameState::LOSS;}
        else{gameState = GameState::READ_GUESS;}
    }
    else if(gameState == GameState::WIN){
        sleep(1);
        gameState = GameState::GAME_END;
    }
    else if(gameState == GameState::LOSS){
        sleep(1);
        gameState = GameState::REVEAL_WORD;
    }
    else if(gameState == GameState::REVEAL_WORD){
        if(uniqueEntry == '1'){gameState = GameState::GAME_END;}        
    }
    else if(gameState == GameState::RULES){
        if(uniqueEntry == '1'){gameState = GameState::MENU;}
    }
    else if(gameState == GameState::SCORE_BOARD){
        if(uniqueEntry == '1'){gameState = GameState::MENU;}
    }
    else if(gameState == GameState::EXIT_CONFIRMATION){
        if(uniqueEntry == tolower('Y')){
            gameState = GameState::GAME_SAVING;
        } else { gameState = GameState::MENU; }
    }
    else if(gameState == GameState::GAME_SAVING){
        gameState = GameState::EXIT;
    }
    else if(gameState == GameState::GAME_END){
        gameState = GameState::MENU;
    }
}

void GameManager::render(){

    // std::cout << "rend: " << gameStateStrings[gameState] << "\n";

    std::ostringstream gameMenu;
    gameMenu << "\n\tPlease choose an option:\n\t[1] - Start a new challenge.\n\t[2] - Show the game rules;\n\t[3] - Show scoreboard.\n\t[4] - Quit game.\n";

    std::ostringstream gameRules;
    gameRules << "\n\tHi " << currentPlayer->getName() << ", here are the game rules:\n"
    << "\t[1] You need to guess the secret word or phrase the game has chosen\n"
    << "\tby suggesting letters.\n"
    << "\t[2] We will display a row of dashes, representing each letter of the\n"
    << "\tsecret word/phrase you're trying to guess.\n"
    << "\t[3] Each correct guess earns you 1 point.\n"
    << "\t[4] Each wrong guess you lose a chance, and, depending of the level,\n"
    << "\tI draw one component of a hanged stick figure (the hangman!)\n"
    << "\t[5] The chances you have will be displayed for you during the game.\n"
    << "\t[6] If you can guess the secret word/phrase before the hangman is\n"
    << "\tcomplete, you add 2 extra points to your overall score.\n"
    << "\t[7] After a guessing round (challenge) is complete, you may try another\n"
    << "\tsecret word/phrase or quit the game.\n";

    std::ostringstream gameDifficulties;
    gameDifficulties << "\n\t[1] - Easy\n"
    << "\t[2] - Medium\n"
    << "\t[3] - Hard\n";

    std::ostringstream gameCategories;
    gameCategories << "\n\t[1] - Guard\n"
    << "\t[2] - Foward\n"
    << "\t[3] - [error] : couldn't find category \n"
    << "\t[4] - Champion\n"
    << "\t[5] - Rookie of The Year\n"
    << "\t[6] - Defensive Player of the Year\n"
    << "\t[7] - MVP\n"
    << "\t[8] - 75 Anniversary Team\n";

    enum ALERT_MSG_ID{
        INV_OPT = 0,
        RIGHT_GUESS,
        WRONG_GUESS,
        WIN,
        LOSS
    };
//se ganhar faltando uma palavra
    std::map<enum ALERT_MSG_ID, string> ALERT_MSGS{
        {INV_OPT, "    [Diesel] : What the hell are u trying to do?! Try again, dawg!"},
        {RIGHT_GUESS, "    [Diesel] : WHOA! What a bucket, lil' bro... keep COOKINNNNN', young fella.\n"},
        {WRONG_GUESS, "    [Diesel] : Uff... what a BRICK, bro. Shoulda do better next time.\n"},
        {WIN, "    [Diesel] : YEA! My man it's doing his stuff, he is doing his stuff.\n"},
        {LOSS, "   [Diesel] : No way... your FG was like what? Zero percent?\n"}
    };

    if(gameState == GameState::START){
        cleaningTheMess();
        std::cout << gameScreenFormat("WELCOME", ">> Hangman v1.0 <<", " - Copyright UFRN 2024 - ", "");
        sleep(1);
    }
    else if(gameState == GameState::READ_NAME){
        cleaningTheMess();
        std::cout << gameScreenFormat("New Player", "", "", "Please, enter yout name > ");
        sleep(1);
    }
    else if(gameState == GameState::MENU){
        cleaningTheMess();
        std::cout << gameScreenFormat("Main menu", gameMenu.str(), "", "Enter your option number and hit the damn \"Enter\" > ");
        sleep(1);
    }
    else if(gameState == GameState::INV_MENU_OPT){
        cleaningTheMess();
        std::cout << gameScreenFormat("Main menu", gameMenu.str(), ALERT_MSGS[ALERT_MSG_ID::INV_OPT], "Enter your option number and hit the damn \"Enter\" > ");
        sleep(1);
    }
    else if(gameState == GameState::NEW_CHALLENGE){
        cleaningTheMess();
        std::cout << gameScreenFormat("New Challenge", "", "", "Alright, big fella. You must be prepared for your challenge.");
        sleep(1);
    }
    else if(gameState == GameState::DIF_CHOOSE){
        cleaningTheMess();
        std::cout << gameScreenFormat("Challenge Difficulty", gameDifficulties.str(), "", "Enter your option number and hit the damn \"Enter\" > ");
        sleep(1);
    }
    else if(gameState == GameState::INV_DIF){
        cleaningTheMess();
        std::cout << gameScreenFormat("Challenge Difficulty", gameDifficulties.str(), ALERT_MSGS[ALERT_MSG_ID::INV_OPT], "Enter your option number and hit the damn \"Enter\" > ");
        sleep(1);
    }
    else if(gameState == GameState::CAT_CHOOSE){
        cleaningTheMess();
        std::cout << gameScreenFormat("Challenge Category", gameCategories.str(), "", "Enter your option number and hit the damn \"Enter\" > ");
        sleep(1);
    }
    else if(gameState == GameState::INV_CAT){
        cleaningTheMess();
        std::cout << gameScreenFormat("Challenge Category", gameCategories.str(), ALERT_MSGS[ALERT_MSG_ID::INV_OPT], "Enter your option number and hit the damn \"Enter\" > ");
        sleep(1);
    }
    else if(gameState == GameState::READ_GUESS){
        cleaningTheMess();
        std::cout << gameScreenFormat("Hangman Challenge", currentWord->displayHangman(), "", "Guess a letter [enter '#' to quit] > ");
        sleep(1);
    }
    else if(gameState == GameState::RIGHT_GUESS){
        cleaningTheMess();
        std::cout << gameScreenFormat(" Hangman Challenge ", currentWord->displayHangman(), ALERT_MSGS[ALERT_MSG_ID::RIGHT_GUESS], "Guess a letter [enter '#' to quit] > ");
        sleep(2);
    }
    else if(gameState == GameState::WRONG_GUESS || gameState == GameState::FULL_GUESS_WRONG){
        cleaningTheMess();
        std::cout << gameScreenFormat(" Hangman Challenge ", currentWord->displayHangman(), ALERT_MSGS[ALERT_MSG_ID::WRONG_GUESS], "Guess a letter [enter '#' to quit] > ");
        sleep(2);
    }
    else if(gameState == GameState::INV_GUESS){
        cleaningTheMess();
        std::cout << gameScreenFormat("Hangman Challenge", currentWord->displayHangman(), ALERT_MSGS[ALERT_MSG_ID::INV_OPT], "Guess a letter [enter '#' to quit] > ");
        sleep(1);
    }
    else if(gameState == GameState::WIN){
        cleaningTheMess();
        std::cout << gameScreenFormat("Hangman Challenge", currentWord->displayHangman(), "", ALERT_MSGS[ALERT_MSG_ID::WIN]);
        sleep(3);
    }
    else if(gameState == GameState::LOSS){
        cleaningTheMess();
        std::cout << gameScreenFormat("Hangman Challenge", currentWord->displayHangman(), "", ALERT_MSGS[ALERT_MSG_ID::LOSS]);
        sleep(2);
    }
    else if(gameState == GameState::REVEAL_WORD){
        cleaningTheMess();
        std::cout << gameScreenFormat("Hangman Challenge", currentWord->displayHangman(), "", "    Hell nah, bro. It was the right word...");
        sleep(3);
    }
    else if(gameState == GameState::RULES){
        cleaningTheMess();
        std::cout << gameScreenFormat("Rules", gameRules.str(), "","Press \"1\" to continue > ");
        sleep(1);
    }
    else if(gameState == GameState::SCORE_BOARD){
        cleaningTheMess();
        std::cout << gameScreenFormat("Scoreboard",  currentData->readData(), "", "Press \"1\" to continue > ");
        sleep(1);
    }
    else if(gameState == GameState::EXIT_CONFIRMATION){
        cleaningTheMess();
        std::cout << gameScreenFormat("Attention", "", "", "    You really wanna quit? [Y/N] > ");
        sleep(1);
    }
    else if(gameState == GameState::EXIT){
        cleaningTheMess();
        std::cout << gameScreenFormat("Farewell", "", "", "    Ok, dawg. See ya later.\n\n");
        sleep(1);
    }
    else if(gameState == GameState::GAME_SAVING){
        cleaningTheMess();
        std::cout << gameScreenFormat("Saving data", "", "", "    It won't take much of your time...");
        sleep(1);
    }
}

// =================================

