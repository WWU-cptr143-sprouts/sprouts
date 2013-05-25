#include "headers/structs.h"

ostream& operator<<(ostream& os, const Coord& coord)
{
    cout << "(" << coord.x << "," << coord.y << ")";
    return os;
}

ostream& operator<<(ostream& os, const Connection& con)
{
    if (con.exists())
        cout << "Node " << con.dest << " via " << con.line << ":{ " << *con.line << " }";
    else
        cout << "default";

    return os;
}

ostream& operator<<(ostream& os, const Line& line)
{
    for (int i = 0; i < line.size(); i++)
    {
        cout << line[i];

        if (i+1 != line.size())
            cout << " ";
    }
    
    return os;
}
