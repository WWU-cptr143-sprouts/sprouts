#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include "stdafx.h"
#include "include/bezier.h"
#include "include/draw.h"
#include "include/string.h"
#include "include/textbox.h"

Game::Game(bool initial_turn, bool mesier_game, int num_nodes, SproutClips * clips)
{
  gameWinState = 0; //Sets the game to no victory
  mesier = mesier_game; //Set Mesier.
  turn = initial_turn; //Set initial turn.
  count = num_nodes; //Set number of nodes.
  nodeNumbers = count; //Tell it how many nodes the game has

  // create the TextBox
  SDL_Color color = {0,0,0};
  Point boxPlace = {205,4};
  text = new TextBox(clips->font, color, TEXTBOX_TYPE_MOVE, boxPlace, this);

  sproutClips = clips;

  // TODO: why are we using malloc instead of the new operator?
  nodes = NULL; //Initialize Node pointer to NULL.
  nodes = (Sprout **)malloc(4 * count * sizeof(Sprout*)); //Allocate 4*count Node pointers.

  connectable = NULL; //Initialize Connectable.
  connectable = (int **)malloc(4 * count * sizeof(int *)); //Allocate 4*count int pointers (first dimension).

  if(connectable == NULL) //If it didn't work...
  {
    std::cout << "Out of memory!" << std::endl; //We are out of memory and must quit.
    exit(1);
  }
  else
  {
    //It worked...
    for(int i = 0; i < 4 * count; i++) //Now we allocate memory to each pointer allocated before.
    {
      connectable[i] = NULL; //Initialize.
      connectable[i] = (int *)malloc(4 * count * sizeof(int)); //Allocate 4*count ints to each pointer established before.

      if(connectable[i] == NULL) //If one failes we have to quit.
      {
        std::cout << "Out of memory!" << std::endl;
        exit(1);
      }
    }
  }

  //This is identical to the above, it just allocates Bezier pointers instead of ints.
  connection = NULL;
  connection = (Bezier ***)malloc(4 * count * sizeof(Bezier **));

  if(connection == NULL)
  {
    std::cout << "Out of memory!" << std::endl;
    exit(1);
  }
  else
  {
    for(int i = 0; i < 4 * count; i++)
    {
      connection[i] = NULL;
      connection[i] = (Bezier **)malloc(4 * count * sizeof(Bezier *));

      if(connection[i] == NULL)
      {
        std::cout << "Out of memory!" << std::endl;
        exit(1);
      }
    }
  }

  //Initialization loop for all allocated arrays.
  for(int i = 0; i < 4 * count; i++) //Loop through rows.
  {
    for(int j = 0; j < 4 * count; j++) //Loop through columns.
    {
      connectable[i][j] = -1; //Initialize everything in Connectable to -1.
      connection[i][j] = NULL; //Initialize all Bezier pointers to NULL.
    }
    nodes[i] = NULL; //Initialize all Node pointers to Null;
  }

  for(int i = 0; i < count; i++)
  {
    for(int j = 0; j < count; j++)
    {
      if(j <= i)
        connectable[i][j] = 0;
      else
        connectable[i][j] = 1;
    }
    nodes[i] = new Sprout(i, -1, -1, sproutClips, nodes, this);
  }
}

// Game copy constructor.  Exactly the same as the main constructor except it copies data from a game reference passed to it.
Game::Game(Game *current)
{
  mesier = current->mesier;
  turn = current->turn;
  count = current->count;

  text = current->text;

  gameWinState = current->gameWinState;
  nodes = NULL;
  nodes = (Sprout **)malloc(4 * count * sizeof(Sprout *));

  connectable = NULL;
  connectable = (int **)malloc(4 * count * sizeof(int *));

  if(connectable == NULL)
  {
    std::cout << "Out of memory!" << std::endl;
    exit(1);
  }
  else
  {
    for(int i = 0; i < 4 * count; i++)
    {
      connectable[i] = NULL;
      connectable[i] = (int *)malloc(4 * count * sizeof(int *));
      if(connectable[i] == NULL)
      {
        std::cout << "Out of memory!" << std::endl;
        exit(1);
      }
    }
  }

  connection = NULL;
  connection = (Bezier ***)malloc(4 * count * sizeof(Bezier *));
  if(connection == NULL)
  {
    std::cout << "Out of memory!" << std::endl;
    exit(1);
  }
  else
  {
    for(int i = 0; i < 4 * count; i++)
    {
      connection[i] = NULL;
      connection[i] = (Bezier **)malloc(4 * count * sizeof(Bezier *));
      if(connection[i] == NULL)
      {
        std::cout << "Out of memory!" << std::endl;
        exit(1);
      }
    }
  }

  for(int i = 0; i < 4 * count; i++)
  {
    for(int j = 0; j < 4 * count; j++)
    {
      connectable[i][j] = current->connectable[i][j];
      if(current->connection[i][j] != NULL)
        connection[i][j] = new Bezier(current->connection[i][j]);
      else
        connection[i][j] = NULL;
    }
    if(current->nodes[i] != NULL)
      //nodes[i] = new Sprout(NULL);
      nodes[i] = new Sprout(current->nodes[i]);
    else
      nodes[i] = NULL;
  }
}

// freeConnections: Count the number of free connections a Node has.
// Input: Node number
// Output: Number of free connections.
int Game::freeConnections(int node)
{
	int free = 3; // Start with 3 and decrement when we find something.

	for(int i = 0; i < node; i++) // Loop until row = column (divides our half matrix).
	{
		if(connectable[node][i] != -1) // If it is a useable node...
			free -= connectable[node][i]; // Subtract what we find (0, 1, 2, 3).
	}

	for(int i = node + 1; i < 4 * count; i++) // Now loop from row = column + 1 to the end of the matrix.
	{
		if(connectable[i][node] != -1)
			free -= connectable[node][i]; // Subtract what we find.
	}

	return free; // Return the number of free connections.
}

void Game::output_console(void)
{
  std::cout << "  ";
  int z = 0;

  if(4 * count < 10)
    z = 4 * count;
  else
    z = 10;

  for(int j = 0; j < z; j++)
    std::cout << j;

  std::cout << std::endl << std::endl;

  for(int i = 0; i < z; i++)
  {
    std::cout << i << " ";

    for(int j = 0; j < z; j++)
    {
      if(connectable[i][j] != -1)
        std::cout << connectable[i][j];
      else
        std::cout << " ";
    }

    std::cout << std::endl;
  }
}

// TODO: use ifstream instead of FILE
void Game::output_file(FILE *fle, char *move)
{
  fputs(move, fle);
  fputs(",", fle);

  for(int j = 0; j < 4 * count; j++)
  {
    fputs(to_string(j).c_str(), fle);
    fputs(",", fle);
  }

  fputs("\n", fle);

  for(int i = 0; i < 4 * count; i++)
  {
    fputs(to_string(i).c_str(),fle);
    fputs(",", fle);

    for(int j = 0; j < 4 * count; j++)
    {
      if(connectable[i][j] != -1)
      {
        fputs(to_string(connectable[i][j]).c_str(), fle);
      }

      fputs(",", fle);
    }

    fputs("\n", fle);
  }

  fputs("\n", fle);
}

void Game::move(char *move)
{
  int to = -1, from = -1, creates = -1, near = -1, error = 0;
  bool clockwise = true;
  std::list<int> encap;

  error = parsemove(move, &from, &to, &creates, &near, &clockwise, &encap);
  parseerror(error);

  std::cout << "Move: " << move << std::endl;
  std::cout << from << " Creates " << creates << " To " << to;

  if(near != -1)
    std::cout << " Near " << near;

  std::cout << std::endl;

  if(error == 0)
    applyMove(from, to, creates, encap, clockwise, near);
  else
    std::cout << "Error: " << error << std::endl;
}

int Game::get_connected(int from, int to)
{
  return connectable[std::min(from, to)][std::max(from, to)];
}

int Game::get_connectable(int from, int to)
{
  return connectable[std::max(from, to)][std::min(from, to)];
}

//Game destructor.
Game::~Game()
{
  int i = 0;

  // Free the nodes memory
  for (i = 0; nodes[i] != NULL; i++)
    delete nodes[i];

  // Free the bezier memory
  for (int j = 0; j < i; j++)
    for (int k = 0; k < i; k++)
      if (connection[j][k] != NULL)
        delete connection[j][k];

  free(nodes);

  // loop through and free the 2nd dimension memory
  for (i = 0; i < 4*count-1; i++)
  {
    free(connectable[i]);
    free(connection[i]);
  }

  free(connectable);
  free(connection);
  delete text;
}
