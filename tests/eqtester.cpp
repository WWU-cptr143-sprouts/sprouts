/*
*
*   Test the area eqality opperator
*
*/
#include "tests.h"
void eqtester()
{
    bool passOne = false;
    vector<int> dumby;
    dumby.push_back(2);
    dumby.push_back(1);
    dumby.push_back(3);

    dumby.push_back(5);
    Line linea,lineb,linec,lined,linee;
    Connection cona,conb,conc,cond,cone;
    Connection othera,otherb,otherc,otherd,othere;
    Area areaa,areab,areac;
    cona.line=&linea;
    conb.line=&lineb;
    conc.line=&linec;
    cond.line=&lined;
    //cone.line=&linee;
    //
    othera.line=&lined;
    otherb.line=&linec;
    otherc.line=&lineb;
    otherd.line=&linea;
    //
    //othere.line=&linee;
    //area a=a->b->c_>d
    areaa.push_back(&cona);
    areaa.push_back(&conb);
    areaa.push_back(&conc);
    areaa.push_back(&cond);
    //setup area b
    areab.push_back(&otherd);
    areab.push_back(&otherc);
    areab.push_back(&otherb);
    areab.push_back(&othera);
    passOne = (areaa==areab);
    cout << "forward test:" << passOne << endl;

    areab.clear();

    areab.push_back(&othera);
    areab.push_back(&otherb);
    areab.push_back(&otherc);
    areab.push_back(&otherd);

    passOne = (areaa==areab);
    cout << "backward test:" << passOne << endl;
}
