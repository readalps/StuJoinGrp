#ifndef JOIN_GRP_H
#define JOIN_GRP_H

#include <stdio.h>
#include <set>

typedef unsigned char u8;

class CStudJoinGroup
{
public:
    void init()
    {
        /// fill G1 as [0,1,2,3,4]
        for (u8 idx = 0; idx < 5; ++idx)
            arrVal[idx] = idx;
        doneGrpSum = 1; // only G1 is done
        doneStuSum = 0; // no student joins 3 groups yet
        swapSum = 1; // just below lowest proper value 2
    }
    int scan();

private:
    void genG2();
    void refreshSets();
    bool allFit();
    bool isStuDone(u8 stu);
    bool isStuInGrp(u8 stu, u8 grp);
    void outputSolution(int sum);
    bool genNextGrp();
    u8 groups(u8 stu, std::set<u8>& setLack);
    bool fillBlanks(u8 head, std::set<u8>& setNew);
    bool checkNewGrpValid();
    bool checkNewGrpHead();
    void calcDoneStuSum();
    bool isStuValid(u8 stu);
    bool isStuFull(u8 stu);
    bool greaterThanLast(u8 grp);
    bool doAdjusting();
    bool adjust(bool& bReadjust);
    void backtrack();

    u8 arrVal[30]; // 6 groups of students
    u8 doneGrpSum;
    u8 doneStuSum;
    u8 swapSum;
    std::set<u8> setLowInG1;
    std::set<u8> setHighInG1;
    std::set<u8> setHighInG2;
    std::set<u8> setHighest;
};

#endif
