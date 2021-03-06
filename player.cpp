#include "player.hpp"
#include "common.hpp"
#include <climits>
/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish
 * within 30 seconds.
 */


/* 
* aldjfhlakjfhlakjsdhf
*/

Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;
    
    if (side == WHITE)
    {
        //std::cerr << 'd' << std::endl;
        pside = WHITE;
        oside = BLACK;
    } else {
        //std::cerr << 'e' << std::endl;
        pside = BLACK;
        oside = WHITE;
    }
    board = new Board();
    
    /*
     * TODO: Do any initialization you need to do here (setting up the board,
     * precalculating things, etc.) However, remember that you will only have
     * 30 seconds.
     */
}

/*
 * Destructor for the player.
 */
Player::~Player() {
    delete board;
}


int Player::heuristic(Board *b)
{

    int sum = 0;
    if (!testingMinimax)
    {
        // Greater value for corner pieces
        sum += 3 * (b->get(pside, 0, 0) + b->get(pside, 0, 7) + 
            b->get(pside, 7, 0) + b->get(pside, 7, 7));
        // Lower value for pieces adjacent to a corner
        sum -= 2 * (b->get(pside, 0, 1) + b->get(pside, 1, 0) + 
            b->get(pside, 6, 0) + b->get(pside, 0, 6) +
            b->get(pside, 7, 1) + b->get(pside, 1, 7) + 
            b->get(pside, 7, 6) + b->get(pside, 6, 7));
        // Lower value for pieces adjacent and diagonal to a corner
        sum -= 3 * (b->get(pside, 1, 1) + b->get(pside, 1, 6) + 
            b->get(pside, 6, 1) + b->get(pside, 6, 6));
        // Greater value for side pieces
        sum += 2 * (b->get(pside, 0, 2) + b->get(pside, 0, 3) +
            b->get(pside, 0, 4) + b->get(pside, 0, 5) + 
            b->get(pside, 7, 2) + b->get(pside, 7, 3) + 
            b->get(pside, 7, 4) + b->get(pside, 7, 5) +
            b->get(pside, 2, 0) + b->get(pside, 3, 0) + 
            b->get(pside, 4, 0) + b->get(pside, 5, 0) +
            b->get(pside, 2, 7) + b->get(pside, 3, 7) + 
            b->get(pside, 4, 7) + b->get(pside, 5, 7));

    }
    
    if (pside == WHITE) 
    {
        
        return b->countWhite() - b->countBlack() + sum;
    } 
    else 
    {
        //sum -= 5* (b->get(oside, 0, 0) + b->get(oside, 0, 7) + 
        //b->get(oside, 7, 0) + b->get(oside, 7, 7));
        return b->countBlack() - b->countWhite() + sum;
    }
}

void Player::printt(Board *b) {
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            if (b->get(WHITE, j, i)) std::cerr << 'w';
            else if (b->get(BLACK, j, i)) std::cerr << 'b';
            else std::cerr << '-';
        }
        std::cerr << std::endl;
    }
    
}

int Player::minimax(bool turn, int depth, Board *bd, int a, int b) {
    if (depth == 0)
    {
        int count = heuristic(bd);
        //printt(b);
        //std::cerr << "c " << count << std::endl;
        return count;
    }
    int best = 0;
    if (!turn)
    {
        //oside
        best = INT_MAX;
        if (!bd->hasMoves(oside))
        {
            Board *newb = bd->copy();
            return minimax(!turn, depth-1, newb, a, b);
        }
        
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                Move move(i, j);
                if (bd->checkMove(&move, oside)) {
                    Board *newb = bd->copy();
                    newb->doMove(&move, oside);
                    //printt(newb);
                    int v = minimax(!turn, depth-1, newb, a, b);
                    best = min(v, best);
                    b = min(b, best);
                    if (b <= a)
                    {
                        return best;
                    }
                }
            }
        }
        return best;
    } else {
        //pside
        best = INT_MIN;
        if (!bd->hasMoves(pside))
        {
            Board *newb = bd->copy();
            return minimax(!turn, depth-1, newb, a, b);
        }
        
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                Move move(i, j);
                if (bd->checkMove(&move, pside)) {
                    //std::cerr << move.getX() << " " << move.getY() << std::endl;
                    Board *newb = bd->copy();
                    newb->doMove(&move, pside);
                    int v = minimax(!turn, depth-1, newb, a, b);
                    best = max(v, best);
                    a = max(a, best);
                    if (b <= a)
                    {
                        return best;
                    }
                }
            }
        }
        return best;
    }
    return 0;
}

/*
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be
 * nullptr.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return nullptr.
 */
Move *Player::doMove(Move *opponentsMove, int msLeft) {
    /*
     * TODO: Implement how moves your AI should play here. You should first
     * process the opponent's opponents move before calculating your own move
     */
    Move *m = new Move(0,0);
    if (opponentsMove != nullptr) board->doMove(opponentsMove, oside);
    if (!board->hasMoves(pside)) return nullptr;
    int best = INT_MIN;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Move move(i, j);
            if (board->checkMove(&move, pside)) {
                //std::cerr << move.getX() << " " << move.getY() << std::endl;
                Board *newb = board->copy();
                newb->doMove(&move, pside);
                int v;
                if (testingMinimax) v = minimax(false, 1, newb, INT_MIN, INT_MAX);
                else v = minimax(false, 6, newb, INT_MIN, INT_MAX);
                //std::cerr << v << std::endl;
                if (v > best)
                {
                    best = v;
                    m->setX(i);
                    m->setY(j);
                }
            }
        }
    }
    if (!board->checkMove(m, pside)) return nullptr;
    board->doMove(m, pside);
    //std::cerr << m->getX() << " " << m->getY() << std::endl;
    
    return m;
}
