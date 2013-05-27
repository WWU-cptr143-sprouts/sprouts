#include "headers/area.h"

ostream& operator<<(ostream& os, const Area& a)
{
    os << "{ ";

    for (int j = 0; j < a.size(); ++j)
    {
        os << "Connection " << a[j]->line << ":" << *(a[j]->line);

        if (j+1 != a.size())
           os << ",";

        os << " ";
    }

    os << "}";

    return os;
}
