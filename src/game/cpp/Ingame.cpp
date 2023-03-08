#include "Ingame.h"

int checkWinner( Player *player1, Player *player2){
    if( player1->getHealth() <= 0){
        return 2;
    }
    if( player2->getHealth() <= 0){
        return 1;
    }
    return 0;
}