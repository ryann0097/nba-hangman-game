/**
 * @file GameMan.hpp
 * @brief Declaração das classes e funções para o jogo da forca com palavras relacionadas à NBA.
 */

#ifndef GAMEMAN_HPP_
#define GAMEMAN_HPP_

#include <iostream>
#include <algorithm>
#include <random>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <set>

using std::string;
using std::tolower;

/**
 * @enum GameState
 * @brief Enumeração representando os diferentes estados do jogo.
 */
enum GameState {
    START = 0, 
    READ_NAME, // Estados de entrada
    INV_NAME,
    READ_OPTION,
    INV_MENU_OPT,
    READ_GUESS,
    ALREADY_USED,
    INV_GUESS,
    READ_DIF,
    INV_DIF,
    READ_CAT,
    INV_CAT,
    MENU,
    RULES,
    NEW_CHALLENGE,
    DIF_CHOOSE, 
    CAT_CHOOSE, // Melhor aumentar as categorias...
    RANDOM,
    GAME,
    IN_GAME,
    RIGHT_GUESS,
    WRONG_GUESS,
    REVEAL_WORD,
    FULL_GUESS_WRONG,
    CHECK_WIN_OR_LOSS,
    WIN,
    LOSS,
    GAME_END,
    SCORE_BOARD,
    EXIT_CONFIRMATION,
    EXIT,
    GAME_SAVING
};

/**
 * @class wordHangman
 * @brief Classe representando uma palavra a ser adivinhada no jogo da forca.
 */
class wordHangman {
private: 
    short difficulty = 0; ///< Nível de dificuldade da palavra.
    short wrongGuesses = 0; ///< Número de palpites errados feitos.
    short rightGuesses = 0; ///< Número de palpites corretos feitos.
    short maxTries = 0; ///< Número máximo de tentativas permitidas.
    std::vector<char> usedLetters; ///< Letras que já foram usadas.
    std::vector<char> wrongLetters; ///< Letras que foram adivinhadas erradas.
    bool revealFullWord = false; ///< Flag para revelar a palavra inteira.

public:
    string currentGuess = "\0"; ///< Palpite atual da palavra.
    string guessingWord; ///< Palavra a ser adivinhada.
    short position = 0; ///< Posição do jogador no jogo.
    short champ = 0; ///< Status de campeão do jogador.
    short roy = 0; ///< Status de Rookie of the Year do jogador.
    short dpoy = 0; ///< Status de Defensive Player of the Year do jogador.
    short mvp = 0; ///< Status de MVP do jogador.
    short at75 = 0; ///< Status de top 75 do jogador.

    /**
     * @brief Operador de comparação para verificar a igualdade de dois objetos wordHangman.
     * @param other Outro objeto wordHangman para comparar.
     * @return True se ambos os objetos forem iguais, false caso contrário.
     */
    bool operator==(const wordHangman& other) const {
        return guessingWord == other.guessingWord &&
        position == other.position &&
        champ == other.champ &&
        roy == other.roy &&
        dpoy == other.dpoy &&
        mvp == other.mvp &&
        at75 == other.at75;
    }

    /**
     * @brief Construtor padrão para wordHangman.
     */
    wordHangman(){}

    /**
     * @brief Construtor parametrizado para wordHangman.
     * @param newName Nome da palavra.
     * @param newPosition Posição do jogador.
     * @param newChamp Status de campeão.
     * @param newRoy Status de Rookie of the Year.
     * @param newDpoy Status de Defensive Player of the Year.
     * @param newMvp Status de MVP.
     * @param newAt75 Status de top 75.
     * @param newDifficulty Nível de dificuldade da palavra.
     */
    wordHangman(const std::string &newName, const std::string &newPosition, 
                const std::string &newChamp, const std::string &newRoy, 
                const std::string &newDpoy, const std::string &newMvp, 
                const std::string &newAt75, const short &newDifficulty) {
        guessingWord = newName;
        champ = (newChamp == "YES") ? 1 : 0;
        roy = (newRoy == "YES") ? 1 : 0;
        dpoy = (newDpoy == "YES") ? 1 : 0;
        mvp = (newMvp == "YES") ? 1 : 0;
        at75 = (newAt75 == "YES") ? 1 : 0;
        difficulty = newDifficulty;

        if (newPosition == "Guard") {
            position = 1;
        } else if (newPosition == "Forward") {
            position = 2;
        } else if (newPosition == "Center") {
            position = 3;
        }

        if(difficulty == 1){maxTries = 10;}
        else if(difficulty == 2){maxTries = 8;}
        else{maxTries = 6;}
    }

    /**
     * @brief Destrutor para wordHangman.
     */
    ~wordHangman(){}

    /**
     * @brief Exibe o estado atual da forca.
     * @return Uma string representando a forca.
     */
    string displayHangman();

    /**
     * @brief Verifica se um caractere está na palavra.
     * @param guessingChar O caractere a ser verificado.
     * @return True se o caractere estiver na palavra, false caso contrário.
     */
    bool charInTheWord(const char &guessingChar);

    /**
     * @brief Verifica se um caractere já foi usado.
     * @param guessingChar O caractere a ser verificado.
     * @return True se o caractere já foi usado, false caso contrário.
     */
    bool charAlreadyUsed (const char &guessingChar);

    /**
     * @brief Verifica se o jogador ganhou.
     * @return True se o jogador ganhou, false caso contrário.
     */
    bool checkWin();

    /**
     * @brief Verifica se o jogador perdeu.
     * @param gameDiff O nível de dificuldade do jogo.
     * @return True se o jogador perdeu, false caso contrário.
     */
    bool checkLoss(const short &gameDiff);

    /**
     * @brief Revela a palavra.
     */
    void revealWord();

    /**
     * @brief Processa um palpite feito pelo jogador.
     */
    void guess();

    /**
     * @brief Valida um palpite feito pelo jogador.
     * @param guessingChar O palpite feito pelo jogador.
     * @return Um valor curto indicando o resultado da validação.
     */
    short guessValidate(const string &guessingChar);

    /**
     * @brief Obtém o nível de dificuldade da palavra.
     * @return O nível de dificuldade.
     */
    short getDifficulty();
};

/**
 * @class Player
 * @brief Classe representando um jogador no jogo da forca.
 */
class Player {
private:
    string playerName; ///< Nome do jogador.
    short Score = 0; ///< Pontuação do jogador.
    short easyLevelsWon = 0; ///< Número de níveis fáceis vencidos.
    short midLevelsWon = 0; ///< Número de níveis médios vencidos.
    short hardLevelsWon = 0; ///< Número de níveis difíceis vencidos.
    short easyLevelsLost = 0; ///< Número de níveis fáceis perdidos.
    short midLevelsLost = 0; ///< Número de níveis médios perdidos.
    short hardLevelsLost = 0; ///< Número de níveis difíceis perdidos.

public:
    std::vector<wordHangman*> alreadyPlayedWords; ///< Palavras já jogadas pelo jogador.

    /**
     * @brief Construtor padrão para Player.
     */
    Player(){}

    /**
     * @brief Destrutor para Player.
     */
    ~Player(){}

    /**
     * @brief Define a pontuação do jogador.
     * @param pointsOffset O valor a ser adicionado à pontuação.
     */
    void setOffsetScoring(const short &pointsOffset);

    /**
     * @brief Define o nome do jogador.
     * @param name O nome a ser definido.
     */
    void setPlayerName(const string &name);

    /**
     * @brief Adiciona uma palavra jogada ao histórico do jogador.
     * @param wordPlayed A palavra que foi jogada.
     */
    void addPlayedWord(wordHangman *wordPlayed);

    /**
     * @brief Incrementa o número de níveis fáceis vencidos.
     */
    void incrementEasyWon(){easyLevelsWon++;}

    /**
     * @brief Incrementa o número de níveis médios vencidos.
     */
    void incrementMidWon(){midLevelsWon++;}

    /**
     * @brief Incrementa o número de níveis difíceis vencidos.
     */
    void incrementHardWon(){hardLevelsWon++;}

    /**
     * @brief Incrementa o número de níveis fáceis perdidos.
     */
    void incrementEasyLost(){easyLevelsLost++;}

    /**
     * @brief Incrementa o número de níveis médios perdidos.
     */
    void incrementMidLost(){midLevelsLost++;}

    /**
     * @brief Incrementa o número de níveis difíceis perdidos.
     */
    void incrementHardLost(){hardLevelsLost++;}

    /**
     * @brief Obtém o número de níveis fáceis vencidos.
     * @return O número de níveis fáceis vencidos.
     */
    short getEasyWon() const {return easyLevelsWon;}

    /**
     * @brief Obtém o número de níveis médios vencidos.
     * @return O número de níveis médios vencidos.
     */
    short getMidWon() const {return midLevelsWon;}

    /**
     * @brief Obtém o número de níveis difíceis vencidos.
     * @return O número de níveis difíceis vencidos.
     */
    short getHardWon() const {return hardLevelsWon;}

    /**
     * @brief Obtém o número de níveis fáceis perdidos.
     * @return O número de níveis fáceis perdidos.
     */
    short getEasyLoss() const {return easyLevelsLost;}

    /**
     * @brief Obtém o número de níveis médios perdidos.
     * @return O número de níveis médios perdidos.
     */
    short getMidLoss() const {return midLevelsLost;}

    /**
     * @brief Obtém o número de níveis difíceis perdidos.
     * @return O número de níveis difíceis perdidos.
     */
    short getHardLoss() const {return hardLevelsLost;}

    /**
     * @brief Obtém a pontuação do jogador.
     * @return A pontuação do jogador.
     */
    short getScoring() const {return Score;}

    /**
     * @brief Obtém o nome do jogador.
     * @return O nome do jogador.
     */
    string getName() const {return playerName;}
};

/**
 * @class Database
 * @brief Classe para manipulação do banco de dados de palavras.
 */
class Database {
private:
    std::ifstream dataFile; ///< Arquivo de leitura de dados.
    std::ofstream dataSave; ///< Arquivo de gravação de dados.

public:
    std::vector<wordHangman>allWords; ///< Todas as palavras do banco de dados.

    /**
     * @brief Construtor para Database.
     * Abre o arquivo de banco de dados.
     */
    Database() : dataFile("./database/nba-database.csv") {
        if(!dataFile.is_open()){std::cerr << "Erro ao abrir o arquivo\n";}
    }

    /**
     * @brief Destrutor para Database.
     * Fecha o arquivo de banco de dados.
     */
    ~Database() {
        if(dataFile.is_open()){dataFile.close();}
    }

    /**
     * @brief Cria os dados a partir do arquivo de banco de dados.
     */
    void createData();

    /**
     * @brief Lê dados do arquivo de banco de dados.
     * @return Uma string representando os dados lidos.
     */
    string readData();

    /**
     * @brief Salva os dados do jogador.
     * @param currentPlayer O jogador cujos dados devem ser salvos.
     */
    void saveData(Player& currentPlayer);
};

/**
 * @class GameManager
 * @brief Classe que gerencia a lógica e os estados do jogo.
 */
class GameManager{
private:
    GameState gameState = GameState::START; ///< Estado atual do jogo.
    short gameDifficulty = 1; ///< Nível de dificuldade do jogo.
    short gameCategory = 0; ///< Categoria do jogo.
    short menuOption = 0; ///< Opção de menu selecionada.
    char uniqueEntry = '\0'; ///< Entrada única para certas telas.
    bool init = false;
    wordHangman* currentWord; ///< Palavra atual sendo adivinhada.
    Player* currentPlayer = nullptr; ///< Jogador atual.
    Database* currentData = nullptr; ///< Banco de dados atual.

public:
    std::vector<wordHangman*> EasyWords; ///< Palavras fáceis no jogo.
    std::vector<wordHangman*> MidWords; ///< Palavras médias no jogo.
    std::vector<wordHangman*> HardWords; ///< Palavras difíceis no jogo.

    /**
     * @brief Construtor para GameManager.
     * Inicializa o jogador e o banco de dados.
     */
    GameManager(){
        currentPlayer = new Player();
        currentData = new Database();
    }

    /**
     * @brief Destrutor para GameManager.
     * Deleta o jogador e o banco de dados.
     */
    ~GameManager(){
        delete currentPlayer;
        delete currentData;
    }

    /**
     * @brief Formata a tela do jogo.
     * @param r1 Conteúdo da linha 1.
     * @param r2 Conteúdo da linha 2.
     * @param r3 Conteúdo da linha 3.
     * @param r4 Conteúdo da linha 4.
     * @return Uma string formatada representando a tela do jogo.
     */
    string gameScreenFormat(const string &r1, const string &r2, const string &r3, const string &r4);

    /**
     * @brief Exibe um alerta no jogo.
     * @param alertType Tipo de alerta a ser exibido.
     * @return Uma string representando o alerta.
     */
    string gameAlert(const string &alertType);

    /**
     * @brief Define o menu para o jogo.
     */
    void setMenu(){std::cin >> menuOption;}

    /**
     * @brief Define a ação de entrada para o jogo.
     */
    void setEnter(){std::cin >> uniqueEntry;}

    /**
     * @brief Define o nível de dificuldade para o jogo.
     */
    void setDiff(){std::cin >> gameDifficulty;}

    /**
     * @brief Define a categoria para o jogo.
     */
    void setCat(){std::cin >> gameCategory;}

    /**
     * @brief Carrega palavras do banco de dados.
     * @param currentData O banco de dados atual para carregar as palavras.
     */
    void loadWords(Database &currentData);

    /**
     * @brief Seleciona uma palavra aleatória para o jogo.
     */
    void randomWord();

    /**
     * @brief Encerra o jogo.
     * @return True se o estado do jogo for EXIT, false caso contrário.
     */
    bool quitGame(){return gameState == EXIT;}

    /**
     * @brief Processa eventos no jogo.
     */
    void process_events();

    /**
     * @brief Atualiza o estado do jogo.
     */
    void update();

    /**
     * @brief Renderiza a tela do jogo.
     */
    void render();
};

#endif
