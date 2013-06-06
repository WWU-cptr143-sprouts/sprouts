#include "headers/area.h"

ostream& operator<<(ostream& os, const Area& a)
{
    os << "{ ";

    for (int j = 0; j < a.size(); ++j)
    {
        os << "Con:" << *(a[j]->line);

        if (j+1 != a.size())
           os << ",";

        os << " ";
    }

    os << "}";

    return os;
}

/*
 * Until we test this, we shouldn't define it since when we use the standard
 * libraries it will call this equality operator.
*/
bool operator==(const Area& a,const Area& b)
{
    int i=0;
    bool clockWise =true;
    bool countWise =true;
    if(a.size()!=b.size()) return false;
    if(a.size()==0) return true;
    //find the point a common point between the two areas
    for(;(a[0]->line!=b[i]->line) && i<b.size();i++);
    //if we've reached the end and there are no matches
    if(i==b.size()-1 &&a[0]->line!=b[i]->line) return false;
    //run find the first element in clockwise roation that is not equal,
    //then do the same for counterclockwise.
    for(int j=0;j<a.size()&& clockWise;j++)
    {
        if(a[j]->line!=b[(i+j)%a.size()]->line)  clockWise =false;
    }
    for(int j=0;j<(a.size())&& countWise;j++)
    {
        if(a[j]->line!=b[
           static_cast<unsigned int>(abs(double(i)-j))%(a.size())]->line)  countWise =false;
    }
    // the areas match if at least one of the checks didnt find any differences
    return (clockWise || countWise);
}

bool operator!=(const Area& a,const Area& b) {return !(a==b);}
