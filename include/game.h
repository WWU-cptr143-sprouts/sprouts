#ifndef H_GAME
#define H_GAME

#include <list>
#include "Sprout.h"

class Bezier; //Prototype Bezier so we can use it here.
class Sprout; //Prototype Node.
class TextBox;
struct SproutClips;  // Prototype struct

class Game{
    // 2D array of integers
    int **connectable;
    // Messier mode. TODO: What is that?
    bool mesier;
    // Whose turn, false = left
    bool turn;
    SproutClips * sproutClips;
    int gameWinState;
    int nodeNumbers;

public:
    // TODO: why are these public?

    // The number of initial nodes in the game
    int count;
    // 2D array of pointers to Beziers
    Bezier ***connection;
    // Array of node pointers
    Sprout **nodes;
    TextBox * text;

    // Makes a game with turn, messier, and number of initial nodes
    Game(bool, bool, int, SproutClips *);
    // Copy constructor
    Game(Game *);
    // Number of connections a node has available to it
    int freeConnections(int);
    int parsemove(char *move, int *from, int *to, int *creates, int *near, bool *clockwise, std::list<int> *encap);
    bool setConnected(std::list<int> toConnect,std::list<int> canConnect);
    int findConnected(int from, int current);
    bool notEnregioned(int node, std::list<int> encapsulated);
    std::list<int> findBorders(int from, int to, int prev,std::list<int>);
    bool applyMove(int from, int to, int creates, std::list<int> encapsulated, bool clockwise, int near);
    void output_console(void);
    void output_file(FILE *, char *);
    void move(char *);
    int get_connectable(int from, int to);
    int get_connected(int from, int to);
    void parseerror(int error);
    int checkWin();
    std::list<std::string> possibleMoves();
    int checkMoveState();
    int numberNodes();
    int playerTurn();
    bool checkRegion(int created, int from, int to);

    ~Game();
};

#endif
