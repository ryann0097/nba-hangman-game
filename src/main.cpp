#include <iostream>
#include "gameManager.h"

int main() {
    
    GameManager newGameInstance;

    while(!newGameInstance.quitGame()){
        newGameInstance.process_events();
        newGameInstance.update();
        newGameInstance.render();
    }
    
    return 0;
}
