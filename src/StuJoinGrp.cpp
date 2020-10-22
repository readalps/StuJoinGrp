#include "StuJoinGrp.h"
#include <memory.h>

void CStudJoinGroup::genG2()
{
    u8 arrHigh[4] = {5, 6, 7, 8};
    memcpy(&arrVal[5], &arrVal[0], 5 - swapSum);
    memcpy(&arrVal[10 - swapSum], arrHigh, swapSum);
    printf("\n From \n G1:[0,1,2,3,4]\n by swapping last %d students, we get \n G2:[%d,%d,%d,%d,%d]\n",
        (int)swapSum, (int)arrVal[5], (int)arrVal[6], (int)arrVal[7], (int)arrVal[8], (int)arrVal[9]);
    ++doneGrpSum;
    refreshSets();
}

void CStudJoinGroup::refreshSets()
{
    setLowInG1.clear();
    setHighInG1.clear();
    setHighInG2.clear();
    setHighest.clear();
    for (u8 idx = 0; idx < 5 - swapSum; ++idx)
        setLowInG1.insert(arrVal[idx]);
    for (u8 idx = 0; idx < swapSum; ++idx) {
        setHighInG1.insert(arrVal[5 - swapSum + idx]);
        setHighInG2.insert(arrVal[10 - swapSum + idx]);
    }
    for (u8 val = 9; val > arrVal[9]; --val)
        setHighest.insert(val);
}

bool CStudJoinGroup::isStuInGrp(u8 stu, u8 grp)
{
    if (stu < arrVal[grp * 5] || stu > arrVal[grp * 5 + 4])
        return false;
    for (u8 pos = grp * 5; pos < grp * 5 + 5; ++pos)
        if (stu == arrVal[pos])
            return true;
    return false;
}

bool CStudJoinGroup::isStuDone(u8 stu)
{
    u8 times = 0;
    short flags = 0;
    for (u8 grp = 0; grp < 6; ++grp)
        if (isStuInGrp(stu, grp)) {
            times++;
            for (u8 pos = 0; pos < 5; ++pos)
                flags |= (1 << arrVal[grp * 5 + pos]);
        }
    return (times == 3 && flags == 0x03FF);
}

bool CStudJoinGroup::allFit()
{
    for (u8 stu = doneStuSum - 1; stu < 10; ++stu)
        if (!isStuDone(stu))
            return false;
    return true;
}

void CStudJoinGroup::outputSolution(int sum)
{
    printf("\n # Solution %d:\n", sum);
    for (u8 grp = 0; grp < 6; ++grp)
        printf("\tG%d:[%d,%d,%d,%d,%d]\n", (int)grp + 1, (int)arrVal[grp * 5], (int)arrVal[grp * 5 + 1], (int)arrVal[grp * 5 + 2], (int)arrVal[grp * 5 + 3], (int)arrVal[grp * 5 + 4]);
    printf("\n");
    getchar();
}

bool CStudJoinGroup::fillBlanks(u8 head, std::set<u8>& setNew)
{
    setNew.insert(head);
    u8 stu = head + 1;
    while (setNew.size() < 5) {
        if (stu > 9) {
            break;
        }
        if (setNew.find(stu) == setNew.end()) {
            if (!isStuFull(stu)) {
                setNew.insert(stu);
            }
        }
        stu++;
    }
    return (setNew.size() == 5);
}

bool CStudJoinGroup::genNextGrp()
{
    u8 head = doneStuSum;
    if (head >= 5) {
        return false;
    }
    if (doneGrpSum > 0) {
        u8 lastHead = arrVal[doneGrpSum * 5 - 5];
        if (head < lastHead) {
            printf(" When generating G%d, DoneStuSum is lower than last group head %d,\n so change current head.\n", 
                (int)doneGrpSum + 1, (int)head, (int)lastHead);
            head = lastHead;
        }
    }
    std::set<u8> setNew;
    if (setLowInG1.find(head) != setLowInG1.end()) {
        setNew = setHighest;
        setNew.insert(head);
        u8 blanks = 5 - setNew.size();
        /// G1:[0,1,2,  3,4] G2:[0,1,2,  5,6] => G3:[0,b1,7,8,9] b1 need to be 1 or 3
        /// G3:[0,1,  7,8,9] => G4:[2,b1,7,8,9] b1 need to be 3
        if (blanks == 1) { 
            setNew.insert(head + 1);
        }
        /// G1:[0,1,  2,3,4] G2:[0,1,  5,6,7] => G3:[0,b1,b2,8,9] b1,b2 need to be 1,2 or 2,5
        else if (blanks == 2) {
            if (setLowInG1.find(head + 1) != setLowInG1.end()) {
                setNew.insert(head + 1);
                setNew.insert(*setHighInG1.begin());
            }
            else {
                setNew.insert(*setHighInG1.begin());
                setNew.insert(*setHighInG2.begin());
            }

        }
        else { // blanks == 3 // [0,  1,2,3,4] [0,  5,6,7,8] => [0,b1,b2,b3,9] b1,b2,b3 need to be 1,2,5
            u8 h1 = *setHighInG1.begin();
            setNew.insert(h1);
            setNew.insert(h1 + 1);
            setNew.insert(*setHighInG2.begin());
        }
    }
    else { // head must be in high subset of G1 since [5,6,7,8,9] cannot be a proper group
        std::set<u8> setLackMate;
        if (groups(head, setLackMate) != 2) {
            for (u8 idx = 0; idx < 5; ++idx) {
                setNew.insert(head + idx);
            }
        }
        else {
            setNew = setLackMate;
            if (!fillBlanks(head, setNew)) {
                return false;
            }
        }
    }
    u8 pos = doneGrpSum * 5;
    for (std::set<u8>::iterator it = setNew.begin(); it != setNew.end(); ++it)
        arrVal[pos++] = *it;
    pos = doneGrpSum * 5;
    printf(" Try new group G%d:[%d,%d,%d,%d,%d]\n", (int)(doneGrpSum + 1), (int)arrVal[pos], 
        (int)arrVal[pos + 1], (int)arrVal[pos + 2], (int)arrVal[pos + 3], (int)arrVal[pos + 4]);
    return true;
}

u8 CStudJoinGroup::groups(u8 stu, std::set<u8>& setLack)
{
    u8 times = 0;
    short flags = 0;
    for (u8 grp = 0; grp < doneGrpSum; ++grp) {
        if (isStuInGrp(stu, grp)) {
            times++;
            for (u8 pos = 0; pos < 5; ++pos) {
                flags |= (1 << arrVal[grp * 5 + pos]);
            }
        }
    }
    short lackFlags = 0x03FF - flags;
    for (u8 stu = 0; stu < 10; ++stu) {
        if ((1 << stu) & lackFlags) {
            setLack.insert(stu);
        }
    }
    return times;
}

bool CStudJoinGroup::greaterThanLast(u8 grp)
{
    bool ret = false;
    for (u8 pos = grp * 5; pos < grp * 5 + 5; ++pos) {
        if (arrVal[pos] > arrVal[pos - 5]) {
            return true;
        }
        if (arrVal[pos] < arrVal[pos - 5]) {
            return false;
        }
    }
    return false;
}

bool CStudJoinGroup::adjust(bool& bReadjust)
{
    bReadjust = false;
    /// to change last 4 students for current group
    u8 head = arrVal[doneGrpSum * 5];
    /// concerning head in low subset of G1
    if (setLowInG1.find(head) != setLowInG1.end()) {
        u8 blanks = 4 - setHighest.size();
        if (blanks == 1) {
            u8& val2 = arrVal[doneGrpSum * 5 + 1];
            if (setLowInG1.find(val2) != setLowInG1.end()) {
                val2 = *setHighInG1.begin();
                return true;
            }
            return false;
        }
        else if (blanks == 2) {
            u8& val2 = arrVal[doneGrpSum * 5 + 1];
            if (setLowInG1.find(val2) != setLowInG1.end()) {
                val2 = *setHighInG1.begin();
                arrVal[doneGrpSum * 5 + 2] = *setHighInG2.begin();
                return true;
            }
            return false;
        }
        return false;
    }
    /// head is in high subset of G1
    /// [3,4,5,6,7] to [3,4,5,6,8] and [3,4,5,8,9] to [3,4,6,7,8]
    u8 pos = doneGrpSum * 5;
    for (u8 idx = 0; idx < 4; ++idx) {
        if (arrVal[pos + 4 - idx] < 9 - idx) {
            arrVal[pos + 4 - idx]++;
            for (u8 step = pos + 4 - idx; step < pos + 4; ++step) {
                arrVal[step + 1] = arrVal[step] + 1;
            }
            bool bFullChk = true;
            for (u8 cur = pos + 1; cur < pos + 5; ++cur) {
                if (isStuFull(arrVal[cur])) {
                    bFullChk = false;
                    break;
                }
            }
            if (!bFullChk || !greaterThanLast(doneGrpSum) || !checkNewGrpHead()) {
                bReadjust = true;
                return false;
            }
            return true;
        }
    }
    return false;
}

void CStudJoinGroup::calcDoneStuSum()
{
    u8 stu = 0;
    for (; stu < 10; ++stu) {
        if (!isStuFull(stu)) {
            break;
        }
    }
    doneStuSum = stu;
}

bool CStudJoinGroup::checkNewGrpHead()
{
    u8 head = doneGrpSum * 5;
    u8 stu = arrVal[head];
    std::set<u8> setLack;
    u8 grpSum = groups(stu, setLack);
    if (grpSum != 2) {
        return true;
    }
    for (std::set<u8>::const_iterator it = setLack.begin(); it != setLack.end(); ++it) {
        u8 stuLack = *it;
        if (stuLack < stu) {
            return false;
        }
        bool bMatch = false;
        for (u8 pos = head + 1; pos < head + 5; ++pos) {
            if (arrVal[pos] == stuLack) {
                bMatch = true;
                break;
            }
        }
        if (!bMatch) {
            return false;
        }
    }
    return true;
}

bool CStudJoinGroup::checkNewGrpValid()
{
    u8 head = doneGrpSum * 5;
    if (setLowInG1.find(arrVal[head]) == setLowInG1.end()) {
        if (!greaterThanLast(doneGrpSum)) {
            printf(" G%d is lower than last group.\n", (int)doneGrpSum + 1);
            return false;
        }
    }
    for (u8 pos = head; pos != head + 5; ++pos) {
        if (!isStuValid(arrVal[pos])) {
            printf(" Student %d is invalid in G%d.\n", (int)arrVal[pos], (int)doneGrpSum + 1);
            calcDoneStuSum();
            return false;
        }
    }
    doneGrpSum++;
    calcDoneStuSum();
    return true;
}

bool CStudJoinGroup::isStuValid(u8 stu)
{
    u8 times = 0;
    short flags = 0;
    for (u8 grp = 0; grp <= doneGrpSum; ++grp) {
        if (isStuInGrp(stu, grp)) {
            times++;
            for (u8 pos = 0; pos < 5; ++pos)
                flags |= (1 << arrVal[grp * 5 + pos]);
        }
    }
    return (times < 3 || (times == 3 && flags == 0x03FF));
}

bool CStudJoinGroup::isStuFull(u8 stu)
{
    u8 times = 0;
    short flags = 0;
    for (u8 grp = 0; grp < doneGrpSum; ++grp) {
        if (isStuInGrp(stu, grp)) {
            times++;
            for (u8 pos = 0; pos < 5; ++pos)
                flags |= (1 << arrVal[grp * 5 + pos]);
        }
    }
    return (times == 3 && flags == 0x03FF);
}

void CStudJoinGroup::backtrack()
{
    while (doneGrpSum >= 2) { // G1 does not backtrack
        doneGrpSum--;
        calcDoneStuSum();
        printf(" Backward to G%d.\n", (int)doneGrpSum + 1);
        if (doneGrpSum == 1) {
            break; // will try a new swap from G1 to G2
        }
        if (doAdjusting()) {
            break;
        }
    }
}

bool CStudJoinGroup::doAdjusting()
{
    bool bPass = false;
    while (true) {
        bool bReadjust = true;
        if (!adjust(bReadjust)) {
            if (bReadjust) {
                continue;
            }
            break;
        }
        u8 pos = doneGrpSum * 5;
        printf(" Adjusted to G%d:[%d,%d,%d,%d,%d]\n", (int)(doneGrpSum + 1), (int)arrVal[pos], 
            (int)arrVal[pos + 1], (int)arrVal[pos + 2], (int)arrVal[pos + 3], (int)arrVal[pos + 4]);
        if (checkNewGrpValid()) {
            bPass = true;
            break;
        }
    }
    return bPass;
}

int CStudJoinGroup::scan()
{
    int nSolutionSum = 0;
    while (true)
    {
        if (doneGrpSum == 1)
        {
            if (++swapSum == 5)
                break;
            genG2();
        }
        if (doneGrpSum != 6)
        {
            if (!genNextGrp()) {
                backtrack();
                continue;
            }
            if (checkNewGrpValid()) {
                continue;
            }
            if (!doAdjusting()) {
                backtrack();
            }
        }
        else
        {
            if (allFit()) {
                outputSolution(++nSolutionSum);
                doneGrpSum = 5;
            }
            backtrack();
        }
    }
    return nSolutionSum;
}
