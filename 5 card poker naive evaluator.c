#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* outputString(int rank)
{
    switch (rank)
    {
        case 2:
            return "Deuce";
        case 3:
            return "Three";
        case 4:
            return "Four";
        case 5:
            return "Five";
        case 6:
            return "Six";
        case 7:
            return "Seven";
        case 8:
            return "Eight";
        case 9:
            return "Nine";
        case 10:
            return "Ten";
        case 11:
            return "Jack";
        case 12:
            return "Queen";
        case 13:
            return "King";
        default:
            return "Ace";
    }
}
char outputsuit(int rank)
{
    char suit=' ';
    switch (rank)
    {
        case 0:
            suit = 'c';
            break;
        case 1:
            suit = 'd';
            break;
        case 2:
            suit = 'h';
            break;
        default:
            suit = 's';
            break;
    }
    return suit;
}
char outputvalue(int rank)
{
    char value = ' ';
    if (rank < 10)
        return rank+'0';
    switch (rank)
    {
        case 10:
            value = 'T';
            break;
        case 11:
            value = 'J';
            break;
        case 12:
            value = 'Q';
            break;
        case 13:
            value = 'K';
            break;
        default:
            value = 'A';
            break;
    }
    return value;
}

void outputcards(int num)
{
    char card[3] = "";
    printf("%c",outputvalue(num % 15));
    printf("%c",outputsuit(num / 15));
}

int * checkHand(int hand[5])
{
    static int values[6];
    int cards[14]={0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    int suits[4]={0,0,0,0};
    int pairs = 0, top = 0, suit = 0, value = 0, topPair = 0, lowPair = 0, temp = 0, first = 0, consec = 0, previous = 0, trip = 0, suitCount = 0, flushSuit = 0;
    char handString[10000]="";
    for (int h = 0; h < 6; h++)
        values[h] = 0;

    for (int c = 4; c > 0; c--)
    {
        first = 0;
        for (int d = 1; d <= c; d++)
        {
            if (hand[d] % 15 < hand[first] % 15)
                first = d;
        }
        temp = hand[first];
        hand[first] = hand[c];
        hand[c] = temp;
    }

    flushSuit = hand[0] / 15;

    for (int i = 0; i < 5; i++)
    {
        suit = hand[i] / 15;
        value = hand[i] % 15;
        cards[value - 2]++;
        suits[suit]++;

        if (suit == flushSuit)
            suitCount++;

        if (suitCount == 5) //flush
        {
            values[0] = 5;
            top = hand[0]%15;
            values[1] = top;
        }
        if (value == previous - 1 || previous == 0 || (value == 2 && hand[0] % 15 == 14 && consec == 4)) //straight
        {
            previous = value;
            consec++;
            if (consec == 5)
            {
                if (suitCount == 5)
                {
                    values[0] = 8;
                    if (top != 5)
                        top = 4 + value;
                    if (top == 14) {
                        strcpy(handString, "a royal flush, ");
                        strcat(handString,outputString(14));
                        strcat(handString," to ");
                        strcat(handString,outputString(10));
                    }
                    else if (top == 5) {
                        strcpy(handString, "a straight flush, ");
                        strcat(handString,outputString(top));
                        strcat(handString," to ");
                        strcat(handString,outputString(14));
                    }
                    else {
                        strcpy(handString, "a straight flush, ");
                        strcat(handString,outputString(top-4));
                        strcat(handString," to ");
                        strcat(handString,outputString(top));
                    }
                    values[1] = top;
                    break;
                }
                if (values[0] < 4)
                {
                    values[0] = 4;
                    if (top != 5)
                        top = 4 + value;
                    values[1] = top;
                    if (top == 5){
                        strcpy(handString, "a straight, ");
                        strcat(handString,outputString(top));
                        strcat(handString," to ");
                        strcat(handString,outputString(14));
                    }
                    else{
                        strcpy(handString, "a straight, ");
                        strcat(handString,outputString(top-4));
                        strcat(handString," to ");
                        strcat(handString,outputString(top));
                    }
                    break;
                }
            }
        }
        else if (previous != value)
        {
            previous = 0;
            consec = 0;
        }
        if (suitCount == 5){
            strcpy(handString, "a flush, ");
            strcat(handString, outputString(top));
            strcat(handString, " high");
            break;
        }
        if (cards[value - 2] > 3)
        {
            //quads
            values[0] = 7;
            strcpy(handString, "a four of a kind, ");
            strcat(handString,outputString(value));
            strcat(handString,"s");
            values[1] = value;
            break;
        }
        else if (cards[value - 2] > 2)
        {
            //trips, check for full house
            if (values[0] < 3) {
                values[0] = 3;
                values[1] = value;
            }
            if (topPair == value && trip == 0)
            {
                topPair = lowPair;
            }
            trip = value;
            lowPair = 0;
            pairs--;
            if (topPair > 0)
            {
                values[0] = 6;
                values[1] = value;
                strcpy(handString, "a full house, ");
                strcat(handString,outputString(trip));
                strcat(handString,"s full of ");
                strcat(handString,outputString(topPair));
                strcat(handString,"s");
                break;
            }
        }
        else if (cards[value - 2] > 1)
        {
            //pair or two pair
            pairs++;
            if (values[0] < pairs) { values[0] = pairs; }
            if (topPair == 0) { topPair = value; }
            else if (lowPair == 0) { lowPair = value; }
            if (trip > 0)
            {
                values[0] = 6;
                values[1] = trip;
                break;
            }
        }
    }
    //populate hands with high cards
    int j = 0, k = 0;
    switch (values[0])
    {
        case 0:
            strcpy(handString, "high card, ");
            strcat(handString,outputString(hand[0] % 15));
            for (int i=0; i<5; i++){
                values[i+1]=hand[i] % 15;
            }
            break;
        case 1:
            values[1] = topPair;
            values[2] = topPair;
            j=3;
            for (int i=0; i<5; i++){
                if (hand[i] % 15 != topPair){
                    values[j]=hand[i] % 15;
                    j++;
                }
            }
            strcpy(handString, "a pair, ");
            strcat(handString,outputString(topPair));
            strcat(handString,"s");
            break;
        case 2:
            values[1] = topPair;
            values[2] = topPair;
            values[3] = lowPair;
            values[4] = lowPair;
            for (int i=0; i<5; i++){
                if ((hand[i] % 15 != topPair)&&(hand[i] % 15 != lowPair))
                    values[5]=hand[i] % 15;
            }
            strcpy(handString, "a two pair, ");
            strcat(handString,outputString(topPair));
            strcat(handString,"s and ");
            strcat(handString,outputString(lowPair));
            strcat(handString,"s");
            break;
        case 3:
            strcpy(handString, "a three of a kind, ");
            strcat(handString,outputString(trip));
            strcat(handString,"s");
            values[1] = trip;
            values[2] = trip;
            values[3] = trip;
            j=4;
            for (int i=0; i<5; i++){
                if (hand[i] % 15 != trip){
                    values[j]=hand[i] % 15;
                    j++;
                }
            }
            break;
    }
    //handString.Replace("Sixs","Sixes");
    for (int i=0;i<strlen(handString);i++){
        if (handString[i]=='x'){
            handString[i+1]='e';
            handString[i+2]='s';
            break;
        }
    }
    printf(handString);
    printf("\n");
    return values;
}

int main()
{
    printf(".  ..__.._..  ..___  .  ..__..  ..__   .___.  ..__..   .  ..__..___..__..__ \n|\\ |[__] | \\  /[__   |__|[__]|\\ ||  \\  [__ \\  /[__]|   |  |[__]  |  |  |[__)\n| \\||  |_|_ \\/ [___  |  ||  || \\||__/  [___ \\/ |  ||___|__||  |  |  |__||  \\\n\n");
    printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n@@@@@@##B80EZhJlx?*~:,.``````@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n#z?^!,-``..`                ````@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\nP.      vPm!               -`  ```````-jB@@@@@@@@@@@@@@@@@@@@@@@@\n$'     rW]vZ?             '`      vpm%j'  ``'=^?lhO8#@@@@@@@@@@@@@@\nQ,     u#  @W!            -     ,6#Q@r          ```,r}jZ0B@@@@@@@\n@r    !We]i$@@0          ``    r#8  @j                  ``_~riVZ#\n@2`   8@~    rt~         -   'm@QgE$@0                         `x\n@d'    _*l\\ /qPl_       `'   rt~   '8@~                        `o\n@B    ^%WWWWWWWWW^      -       ,@   @:                         =B\n@@?   `rKWWWWWU!       `'   `<zQ@#r                           `j@\n@@w`     =}qWx`        -  `G@@@@@@@h`                         ^#@\n@@D'       \\/         `'  `X##8Q#@@@n                        .q@@\n@@#!                  -     `~zE<oWX                         r#@@\n@@@]`         .!rv]x?='       @@:                           'b@@@\n@@@P.        nWWWWWWZ<                                      (@@@@\n@@@8-       =WWWWWWWn.                                     'Z@@@@\n@@@#<       'WWWWWWq<                 _jH'                 r#@@@@\n@@@@V`       !UWWWWl`             `^sB@@@Q:               -R@@@@@\n@@@@W.        'ipWH!           _|p#@@@@@@@B=             `i@@@@@@\n@@@@Q_          _v]`        ,n&@@@@@@@@@@@@#^            -N@@@@@@\n@@@@#!            -        ]#@@@@@@@@@@@@@@@#x          `]@@@@@@@\n@@@@@]`          '`       !@@@@@@@@@@@@@@@@@@@O'        ‘Q@@@@@\n@@@@@U'          -        `W@@@@@@@@@@@@@@@@@@@h       `l@@@@@@@@\n@@@@@&-         '`         `rK&Qgq}<OKD@@@@@@@@Z       _D@@@@@@@@\n@@@@@@^        `-                 !E@] ]0@@@@#X'      `n@@@@@@@@@\n@@@@@@l`       ..              ,vb@@@B!  '==,`        :Q@@@@@@@@@\n@@@@@@Qi=:~*?]]nVi*:'`            '=*]u,             `z@@@@@@@@@@\n@@@@@@@@@@@@@@@@@@@@#QEKVv~_``                       =B@@@@@@@@@@\n@@@@@@@@@@@@@@@@@@@@@@@@@@@@@#86XVx*:'`             `P@@@@@@@@@@@\n@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@#Qdh}r~_``     ^#@@@@@@@@@@@\n@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@B&Wwc]}Q@@@@@@@@@@@@\n");

    int fullDeck[52] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,           //clubs    (2-10 = 2-10, 11 = J, 12 = Q, 13 = K, 14 = A)
                        17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,   //diamonds (17-25 = 2-10, 26 = J, 27 = Q, 28 = K, 29 = A)
                        32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44,   //hearts   (32-40 = 2-10, 41 = J, 42 = Q, 43 = K, 44 = A)
                        47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59 }; //spades   (47-55 = 2-10, 56 = J, 57 = Q, 58 = K, 59 = A)
    int option = 0;
    printf("\nPress 0 to run an example, Press 1 to run tests\n");
    scanf("%d",&option);
    system("cls");
    if (option == 0){
        int player1Hand[5] = {59,52,28,40,13}; // [A♠, 7♠] + [K♦, 10♥, K♣]
        printf("Player 1 has ");
        outputcards(player1Hand[0]);
        printf(" ");
        outputcards(player1Hand[1]);
        printf("\n");
        int player2Hand[5] = {9,54,28,40,13};  // [9♣, 9♠] + [K♦, 10♥, K♣]
        printf("Player 2 has ");
        outputcards(player2Hand[0]);
        printf(" ");
        outputcards(player2Hand[1]);
        printf("\n\n");
        printf("Board: ");
        outputcards(player1Hand[2]);
        printf(" ");
        outputcards(player1Hand[3]);
        printf(" ");
        outputcards(player1Hand[4]);
        printf("\n\n");
        naiveEval(player1Hand, player2Hand);
    }
    else{
        testNaiveEval(fullDeck);
    }
    return 0;
}

void naiveEval(int hand1[5], int hand2[5])
{
    int *p , *q, p1Hand[6];
    printf("Player 1 has ");
    p = checkHand(hand1);
    for (int i = 0; i <6; i++ )
      p1Hand[i] = *(p + i);
    printf("Player 2 has ");
    q = checkHand(hand2);
    int winType = 1;
    printf("\n");
    for (int i=0; i<6; i++){
        if (p1Hand[i]>*(q + i)){
            winType = 2;
        }
        else if (p1Hand[i]<*(q + i)){
            winType = 0;
        }
    }
    switch (winType)
    {
        case 0:
            printf("Player 1 lost, Player 2 won");
            break;
        case 1:
            printf("Player 1 and Player 2 drew");
            break;
        case 2:
            printf("Player 1 won and Player 2 lost");
            break;
    }
    printf("\n");
}

int testCheckHand(int hand[5])
{
    int cards[14]={0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    int suits[4]={0,0,0,0};
    int rank = 0, pairs = 0, top = 0, suit = 0, value = 0, topPair = 0, lowPair = 0, temp = 0, first = 0, consec = 0, previous = 0, trip = 0, suitCount = 0, flushSuit = 0;

    //sort the given hand
    for (int c = 4; c > 0; c--){
        first = 0;
        for (int d = 1; d <= c; d++){
            if (hand[d] % 15 < hand[first] % 15)
                first = d;
        }
        temp = hand[first];
        hand[first] = hand[c];
        hand[c] = temp;
    }

    //rank the given hand
    flushSuit = hand[0] / 15;
    for (int i = 0; i < 5; i++){
        suit = hand[i] / 15;
        value = hand[i] % 15;
        cards[value - 2]++;
        suits[suit]++;

        if (suit == flushSuit)
            suitCount++;
        if (suitCount == 5)
            rank = 5; //flush
        if (value == previous - 1 || previous == 0 || (value == 2 && hand[0] % 15 == 14 && consec == 4)){
            previous = value;
            consec++;
            if (consec == 5){
                if (suitCount == 5){
                    rank = 8; //straight flush
                    break;
                }
                if (rank < 4){
                    rank = 4; //straight
                    break;
                }
            }
        }
        else if (previous != value){
            previous = 0;
            consec = 0;
        }
        if (cards[value - 2] > 3){
            rank = 7; //quads
            break;
        }
        else if (cards[value - 2] > 2){
            if (rank < 3)
                rank = 3; //trips
            if (topPair == value && trip == 0)
                topPair = lowPair;
            trip = value;
            lowPair = 0;
            pairs--;
            if (topPair > 0){
                rank = 6; //full house
                break;
            }
        }
        else if (cards[value - 2] > 1){
            pairs++;
            if (rank < pairs)
                rank = pairs; //pair or two pair
            if (topPair == 0)
                topPair = value;
            else if (lowPair == 0)
                lowPair = value;
            if (trip > 0){
                rank = 6; //full house
                break;
            }
        }
    }
    return rank;
}

void testNaiveEval(int deck[52]){
    int count = 0, iterations = 0, totalRanks[9] = {0,0,0,0,0,0,0,0,0};
    for (int i=14;i>1;i--){
        for (int j=14-count;j>1;j--){
            for (int k=14;k>1;k--){
                for (int l=14;l>1;l--){
                    for (int m=14;m>1;m--){
                        if (!(m==l&&l==k&&k==j&&j==i)){
                            if ((i>=j&&j>=k&&k>=l&&l>=m)){
                                int hand[5] = {i+45,j+30,k+15,l,m};
                                totalRanks[testCheckHand(hand)]++;
                                iterations++;
                            }
                        }
                    }
                }
            }
        }
        count++;
    }
    count = 0;
    for (int i=14;i>1;i--){
        count++;
        for (int j=14-count;j>1;j--){
            for (int k=14-count-1;k>1;k--){
                for (int l=14-count-2;l>1;l--){
                    for (int m=14-count-3;m>1;m--){
                        if (!(m==l&&l==k&&k==j&&j==i)){
                            if ((i>j&&j>k&&k>l&&l>m)){
                                int hand[5] = {i,j,k,l,m};
                                totalRanks[testCheckHand(hand)]++;
                                iterations++;
                            }
                        }
                    }
                }
            }
        }
    }
    printf("%d iterations representing %d distinct hands\n\n",iterations,iterations);
    printf("Straight flushes: Expected 10    Got: %d\n",totalRanks[8]);
    printf(" Four of a kinds: Expected 156   Got: %d\n",totalRanks[7]);
    printf("     Full houses: Expected 156   Got: %d\n",totalRanks[6]);
    printf("         Flushes: Expected 1277  Got: %d\n",totalRanks[5]);
    printf("       Straights: Expected 10    Got: %d\n",totalRanks[4]);
    printf(" Three of a kind: Expected 858   Got: %d\n",totalRanks[3]);
    printf("       Two Pairs: Expected 858   Got: %d\n",totalRanks[2]);
    printf("           Pairs: Expected 2860  Got: %d\n",totalRanks[1]);
    printf("      High cards: Expected 1277  Got: %d\n",totalRanks[0]);
}




