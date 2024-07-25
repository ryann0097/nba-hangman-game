#include "gameManager.h"
#include <iomanip>

string hangmanStages[] = {
    // Stage 0: No parts yet
    "\t\t\t\t\t    +---+\n"
    "\t\t\t\t\t    |   |\n"
    "\t\t\t\t\t        |\n"
    "\t\t\t\t\t        |\n"
    "\t\t\t\t\t        |\n"
    "\t\t\t\t\t        |\n"
    "\t\t\t\t\t =========\n",

    // Stage 1: Head
    "\t\t\t\t\t    +---+\n"
    "\t\t\t\t\t    |   |\n"
    "\t\t\t\t\t    O   |\n"
    "\t\t\t\t\t        |\n"
    "\t\t\t\t\t        |\n"
    "\t\t\t\t\t        |\n"
    "\t\t\t\t\t =========\n",

    // Stage 2: Head and body
    "\t\t\t\t\t    +---+\n"
    "\t\t\t\t\t    |   |\n"
    "\t\t\t\t\t    O   |\n"
    "\t\t\t\t\t    |   |\n"
    "\t\t\t\t\t        |\n"
    "\t\t\t\t\t        |\n"
    "\t\t\t\t\t =========\n",

    // Stage 3: Head, body, and one arm
    "\t\t\t\t\t    +---+\n"
    "\t\t\t\t\t    |   |\n"
    "\t\t\t\t\t    O   |\n"
    "\t\t\t\t\t   /|   |\n"
    "\t\t\t\t\t        |\n"
    "\t\t\t\t\t        |\n"
    "\t\t\t\t\t =========\n",

    // Stage 4: Head, body, and both arms
    "\t\t\t\t\t    +---+\n"
    "\t\t\t\t\t    |   |\n"
    "\t\t\t\t\t    O   |\n"
    "\t\t\t\t\t   /|\\  |\n"
    "\t\t\t\t\t        |\n"
    "\t\t\t\t\t        |\n"
    "\t\t\t\t\t =========\n",

    // Stage 5: Head, body, both arms, and one leg
    "\t\t\t\t\t    +---+\n"
    "\t\t\t\t\t    |   |\n"
    "\t\t\t\t\t    O   |\n"
    "\t\t\t\t\t   /|\\  |\n"
    "\t\t\t\t\t   /    |\n"
    "\t\t\t\t\t        |\n"
    "\t\t\t\t\t =========\n",

    // Stage 6: Full hangman
    "\t\t\t\t\t    +---+\n"
    "\t\t\t\t\t    |   |\n"
    "\t\t\t\t\t    O   |\n"
    "\t\t\t\t\t   /|\\  |\n"
    "\t\t\t\t\t   / \\  |\n"
    "\t\t\t\t\t        |\n"
    "\t\t\t\t\t =========\n"
};


void wordHangman::revealWord(){revealFullWord = true;}

std::string centerText(const std::string& text, int width) {
    int len = text.length();
    if (len >= width){return text;}
    int leftPadding = (width - len) / 2;
    int rightPadding = width - len - leftPadding;
    return std::string(leftPadding, ' ') + text + std::string(rightPadding, ' ');
}

std::string wordHangman::displayHangman() {
    std::ostringstream hangmanDisplay;
        int totalWidth = 80;

        std::ostringstream header;
        std::vector<std::string> headerComponents;

        if (position == 1) {
            headerComponents.push_back("Guard");
        } else if (position == 2) {
            headerComponents.push_back("Forward");
        } else {
            headerComponents.push_back("Center ");
        }

        if (champ == 1) headerComponents.push_back("NBA Champion");
        if (roy == 1) headerComponents.push_back("ROY");
        if (dpoy == 1) headerComponents.push_back("DPOY");
        if (mvp == 1) headerComponents.push_back("MVP");
        if (at75 == 1) headerComponents.push_back("75th TEAM");

        for (size_t i = 0; i < headerComponents.size(); ++i) {
            if (i != 0) header << " - ";
            header << headerComponents[i];
        }

        hangmanDisplay << centerText(header.str(), totalWidth) << "\n\n";

        int totalStages = 6;
        int stageIndex;
        if (maxTries == 0) {
            stageIndex = 0;
        } else {
            stageIndex = (wrongGuesses * totalStages) / maxTries;
        }
        hangmanDisplay << centerText(hangmanStages[stageIndex], totalWidth) << "\n\n";

        std::ostringstream wordDisplay;
        if (!revealFullWord) {
            for (const auto &i : guessingWord) {
                if (charInTheWord(i) && charAlreadyUsed(i)) {
                    wordDisplay << i << " ";
                } else if (isalpha(i)) {
                    wordDisplay << "_ ";
                } else {
                    wordDisplay << i << " ";
                }
            }
        } else {
            for (const auto &i : guessingWord) {
                wordDisplay << i << " ";
            }
        }

        hangmanDisplay << centerText(wordDisplay.str(), totalWidth) << "\n\n";

        std::ostringstream wrongDisplay;
        wrongDisplay << "Wrong guesses so far: < ";
        for (const auto &i : wrongLetters) {
            wrongDisplay << i << " ";
        }
        wrongDisplay << ">" << " --- Chances left: " << maxTries - wrongGuesses;

        hangmanDisplay << centerText(wrongDisplay.str(), totalWidth) << "\n";

        return hangmanDisplay.str();
    }

    bool wordHangman::charInTheWord(const char &guessingChar){
        char lowerChar = tolower(guessingChar);
        for(auto i : guessingWord){if(tolower(i) == lowerChar){return true;}}
            return false;
    }

    bool wordHangman::charAlreadyUsed(const char &guessingChar){
        char lowerChar = tolower(guessingChar);
        for(auto i : this->usedLetters){if(tolower(i) == lowerChar){return true;}}
            return false;
    }

    bool wordHangman::checkWin(){
        for (auto c : guessingWord){if (!isalpha(c)) continue; if (!charAlreadyUsed(c)) 
            return false;}
        return true;
    }

    bool wordHangman::checkLoss(const short &gameDiff){
        if(gameDiff == 1 && wrongGuesses == 10){return true;}
        else if(gameDiff == 2 && wrongGuesses == 8){return true;}
        else if(gameDiff == 3 && wrongGuesses == 6){return true;}
        return false;
    }

    void wordHangman::guess(){
        string guessingChar;
        std::getline(std::cin, guessingChar);
        currentGuess = guessingChar;
    }

    short wordHangman::guessValidate(const string &guessingChar){
    /**
     * RETORNO 1 - JÁ USADO
     * RETORNO 2 - ACERTO
     * RETORNO 3 - ERRO NORMAL
     * RETORNO 4 - DESISTÊNCIA
     * RETORNO 5 - TENTATIVA NÃO VALIDA
     * RETORNO 6 - ERRO DE CHUTE INTEIRO
    */

   //a primeira condicional é só pra um chute normal...
        if (guessingChar.size() == 1){
            if (isalpha(guessingChar[0]) != 0){if(charAlreadyUsed(guessingChar[0])){return 1;} 
            else{
                if(charInTheWord(guessingChar[0])){
                    usedLetters.push_back(guessingChar[0]);
                    rightGuesses++;
                    return 2;
                }
                else{
                    usedLetters.push_back(guessingChar[0]);
                    wrongLetters.push_back(guessingChar[0]);
                    wrongGuesses++;
                    return 3;
                }
            }
        } 
        else if(guessingChar == "#"){return 4;} 
        else{return 5;}
    } 
    //a segunda condicional é para um chute inteiro...
    else if(guessingChar.size() > 1){
        bool allAlpha = true;
        bool allCorrect = true;
        for (const auto &i : guessingChar) {
            if(isalpha(i) == 0 && i != ' '){allAlpha = false; break;}
            if(!charInTheWord(i)){allCorrect = false;}
            if (!charAlreadyUsed(i)) {
                usedLetters.push_back(i);
                if (!charInTheWord(i)){wrongLetters.push_back(i);
                if(difficulty == 1) {wrongGuesses++;} 
                else if(difficulty == 2){wrongGuesses += 2;} 
                else if(difficulty == 3){wrongGuesses = 6;}
            }
        }
    }
    if(!allAlpha){return 5;} 
    else if(allCorrect){rightGuesses++; return 2;} 
    else{return 6;}
}
return 5;
}

short wordHangman::getDifficulty(){return wordHangman::difficulty;}

// ================================================

void Player::setOffsetScoring(const short &pointsOffset){Score += pointsOffset;}
void Player::setPlayerName(const string &newName){playerName = newName;}
void Player::addPlayedWord(wordHangman* wordPlayed) {alreadyPlayedWords.push_back(wordPlayed);}

// ====================================================

void Database::createData(){
    string lineToBeRead;
    bool headerRow = true;

    while (std::getline(dataFile, lineToBeRead)){
        if(headerRow){headerRow = !headerRow; continue;}
        
        std::stringstream dataFlux(lineToBeRead);
        string name, position, champ, roy, dpoy, mvp, at75, difficulty;

        std::getline(dataFlux, name, ',');
        std::getline(dataFlux, position, ',');
        std::getline(dataFlux, champ, ',');
        std::getline(dataFlux, roy, ',');
        std::getline(dataFlux, dpoy, ',');
        std::getline(dataFlux, mvp, ',');
        std::getline(dataFlux, at75, ',');
        std::getline(dataFlux, difficulty, ',');

        short difLevel = std::stoi(difficulty);
        allWords.push_back(wordHangman(name, position, champ, roy, dpoy, mvp, at75, difLevel));
    }
}

void Database::saveData(Player& currentPlayer) {
    std::cout << "Salvando dados do jogador...\n";

    dataSave.open("./database/nba-save.bin", std::ios::binary | std::ios::out | std::ios::app);
    if (!dataSave.is_open()) {
        std::cerr << "Erro ao abrir o arquivo para escrita\n";
        return;
    }

    std::string playerName = currentPlayer.getName();

    std::ostringstream scoringStream, easyPontuationStream, midPontuationStream, hardPontuationStream;
    scoringStream << currentPlayer.getScoring();
    std::string scoringString = scoringStream.str();

    easyPontuationStream <<currentPlayer.getEasyWon() << "/" << currentPlayer.getEasyWon() + currentPlayer.getEasyLoss();
    midPontuationStream << currentPlayer.getMidWon() << "/" << currentPlayer.getMidWon() + currentPlayer.getMidLoss();
    hardPontuationStream << currentPlayer.getHardWon() << "/" << currentPlayer.getHardWon() + currentPlayer.getHardLoss();
    
    string easyString = easyPontuationStream.str(), midString = midPontuationStream.str(), hardString = hardPontuationStream.str();
    string underLine = "\n\t__________________________________________________________________";
    string dataToWrite = "\tName:" + playerName + ", Score:" + scoringString + "\n\tLevels - Easy:" + easyString + " | Medium: " + midString + " | Hard: " + hardString + underLine;

    size_t length = dataToWrite.size();
    dataSave.write(reinterpret_cast<const char*>(&length), sizeof(length));
    dataSave.write(dataToWrite.c_str(), length);
 
    dataSave.flush();
    dataSave.close();

}


std::string Database::readData() {
    std::ifstream dataLoad("./database/nba-save.bin", std::ios::binary | std::ios::in);
    if (!dataLoad.is_open()) {
        std::cerr << "Erro ao abrir o arquivo para leitura\n";
    }

    std::ostringstream scoreBoard;

    while (dataLoad.peek() != EOF) {
        // Ler o comprimento da string
        size_t dataLength;
        dataLoad.read(reinterpret_cast<char*>(&dataLength), sizeof(dataLength));
        if (dataLoad.eof()) break; // Check for EOF after reading the size

        // Ler a string completa
        std::string data(dataLength, '\0');
        dataLoad.read(&data[0], dataLength);
        if (dataLoad.eof()) break; // Check for EOF after reading the data

        scoreBoard << data << "\n";
    }

    dataLoad.close();
    return scoreBoard.str();
}
