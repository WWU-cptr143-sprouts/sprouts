#include "headers/structs.h"

ostream& operator<<(ostream& os, const Coord& coord)
{
    os << "(" << coord.x << "," << coord.y << ")";
    return os;
}

ostream& operator<<(ostream& os, const Connection& con)
{
    if (con.exists())
        os << "Node " << con.dest << " via " << con.line << ":{ " << *con.line << " }";
    else
        os << "default";

    return os;
}

ostream& operator<<(ostream& os, const Line& line)
{
    for (int i = 0; i < line.size(); i++)
    {
        os << line[i];

        if (i+1 != line.size())
            os << " ";
    }
    
    return os;
}
