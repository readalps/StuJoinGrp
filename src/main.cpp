#include "StuJoinGrp.h"

void intro()
{
    printf(" Suppose 10 students, denoted by number 0-9, to join some groups such that\n");
    printf(" each group can contain at most 5 students and \n");
    printf(" for any 2 students, they are both in at least 1 group.\n");
    printf(" Please list all kinds of solutions with as few groups as possible.\n\n");

    getchar();
    printf(" Step1. If student A only joins one group then he has at most 4 group-mates; \n");
    printf(" and if he joins two groups then he has at most 8 group-mates.\n");
    printf(" So for any specific student, to make sure that he has all other 9 students as his group-mates, \n");
    printf(" he must join at least 3 groups. And there are 10 of them, \n");
    printf(" for any possible group set with x groups, we have 5x >= 30, that is, x >= 6.\n\n");

    getchar();
    printf(" Step2. Now we try to list all the six-group solutions with 1st group [0,1,2,3,4].\n\n");
}

int main()
{
    intro();
    CStudJoinGroup obj;
    obj.init();
    obj.scan();
    return 0;
}
