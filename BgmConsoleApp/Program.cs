using System;
using System.Collections.Generic;
using System.Linq;

namespace BgmConsoleApp
{
    // Comments prefixed // JH are not original!
    class Program
    {
        /* Now for some definitions */
        private const int TRUE = 1;
        private const int FALSE = 0;
        private const int WHITE = 1;
        private const int BLACK = 2;
        private const int PROGRAM = 11;
        private const int INTERACTIVE = 12;
        private const int NOOFPARAMETERS = 81;

        /* Now, the global variables */
        static int[] bboard;
        static int[] wboard;
        static int[] bmen;
        static int[] wmen;
        //int seed;
        static int[] board;
        static int[] opboard;
        static int[] men;
        static int[] opmen;
        static int[] parametervalues;
        static int[] human;
        static int[] storemove;
        static int[] storemen;
        static int[] hitman;
        static int[] allmoves;
        static int hit;
        static int[] noofmen;
        static int turnnumber;
        static int bestscore;
            //   int availablemove;
            //   int noofpointsread;
        static int[] pointsread;
           //    int* valofbackfour;
          //     int* blotcoeff;
           //    int* buildercoeff;
           //    int* valofophit;
          //     int charr;
         static int starter;
          //     int nonstarter;
         static int nooflegal;
          //     int ophomepoints;
          //     int* valofhit;
          //     int* valofpoint;
          //     int* valofboard;
          //     int* valofsix;
          //     int* valofconsec;
          ///     int* valofimmedblock;
            //   int homepoints;
        static int[] typeofgame;

        static int movetype;
        static int movefound;
        static int gametype;
        static int makingpoint;
        static int sigma = FALSE;
        static int externaldice = FALSE;
        static bool fast = false;
        static int debug = TRUE;
        static bool terse = false;
        static char terminator;

        static Random rand;

        /* Now, a list of routines ... this doesn't happen in C# */

        /// <summary>
        /// IN THE COMMENTS THE WORD "MOVE" WILL MEAN THE SHIFTING OF ONE
        /// MAN BY THE DISTANCE INDICATED ON ONE DICE.  THE WORD "PLAY" WILL MEAN
        /// THE COMBINATION OF 2 OR 4 MOVES THAT ARE MADE IN ONE TURN.
        /// </summary>
        /// <param name="args"></param>
        static void Main(string[] args)
        {
            // Initialise lots of things
            int[] cbboard = new int[26];
            int[] callmoves = new int[5];
            int[] cwboard = new int[26];
            int[] cbmen = new int[16];
            int[] cwmen = new int[16];
            int[] cstoremove = new int[5];
            int[] cstoremen = new int[5];
            int[] chitman = new int[5];
            int[] chuman = new int[3];          // Keep track of who is human or not.
            int[] cnoofmen = new int[3];
            int[] cpointsread = new int[5];
            int[] ctypeofgame = new int[5];

            int[] cparametervalues = new int[] {0,   /* TYPE 1 (RUNNING GAME)   */

-35,-12,20,55,80,100,        /* VALOFPOINT  1 -- 6      */
72,44,28,20,15,15,           /* VALOFPOINT  7 -- 12     */
15,15,17,30,56,72,           /* VALOFPOINT  13 -- 18    */
100,80,50,30,2,2,            /* VALOFPOINT  19 -- 24    */
0,20,40,70,120,200,340,      /* VALOFBOARD  0 -- 6      */
0,5,12,20,50,90,160,         /* VALOFSIX    0 -- 6      */
0,10,30,70,145,220,350,350,  /* VALOFCONSEC 0 -- 7      */
0,10,20,40,80,160,320,       /* VALOFIMMEDBLOCK 0 -- 6  */
15,50,67,93,156,305,800,     /* VALOFHIT  0 -- 6        */
-8,-6,-4,-2,                 /* VALOFBACKFOUR  1 -- 4   */
13,12,3,2,10,3,              /* BLOTCOEFF  0 -- 5       */
3,6,30,                      /* BUILDERCOEFF  1 -- 3    */
15,50,67,93,156,305,800,     /* VALOFOPHIT  0 -- 6      */

0,                           /* TYPE 2 (PURELY BLOCKING GAME) */

2,10,30,55,80,105,
125,90,60,30,13,0,
0,0,0,0,0,100,
100,100,80,50,20,0,
0,0,0,0,0,0,0,
0,10,20,38,80,170,336,
0,10,30,70,150,250,400,400,
0,10,22,50,150,350,700,
0,0,0,0,-10,-40,-100,
-32,-24,-16,-8,
0,0,0,1,10,3,
0,6,30,
0,0,0,0,0,0,0,

0,                           /* TYPE 3 (DEFENSIVE POSITION) */

1,10,30,80,130,160,
100,50,30,10,0,0,
0,0,0,15,35,70,
140,130,80,30,10,1,
0,20,40,70,120,200,340,
0,4,10,17,40,70,130,
0,10,30,80,175,275,420,420,
0,10,20,40,80,160,320,
15,40,60,130,240,380,800,
-2,-2,-2,-2,
13,2,3,2,8,3,
3,6,30,
15,50,67,93,156,305,800,

300,                         /* TYPE 4 (OPPONENT HAS DEFENSIVE POSITION) */

2,13,30,55,80,100,
72,56,38,29,21,15,
15,0,0,0,0,0,
0,0,0,0,0,0,
0,20,40,70,120,200,340,
0,5,12,20,50,90,160,
0,10,30,70,110,150,190,190,
0,10,20,40,80,160,320,
15,50,67,93,156,305,800,
-32,-24,-16,-8,
30,30,3,2,10,3,
3,6,30,
15,40,60,130,240,380,800,

0,                           /* TYPE 5 (EARLY BACK GAME) */

-50,-10,3,80,240,300,
180,100,60,20,0,0,
0,0,0,0,30,50,
70,100,200,300,315,330,
0,20,40,70,120,200,340,
0,5,12,20,50,90,160,
0,10,30,80,175,275,420,420,
0,0,0,0,0,0,0,
-100,-100,-50,0,0,0,0,
0,0,0,0,
-10,5,3,2,10,3,
3,6,30,
0,0,0,0,0,0,0,

0,                           /* TYPE 6 (OPPONENT PLAYS EARLY BACK GAME) */

50,50,50,55,80,100,
90,60,30,20,15,15,
15,0,0,0,0,0,
0,0,0,0,0,0,
0,20,40,70,120,200,340,
0,5,12,20,50,90,160,
0,10,30,70,145,220,350,350,
0,10,20,40,80,160,320,
15,50,67,93,156,305,800,
-32,-24,-16,-8,
13,8,3,2,10,3,
3,6,30,
0,0,60,130,240,380,800,

2500,                        /* TYPE 7 (BRINGING MEN IN) */

300,250,200,150,100,50,
10,-80,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,0,
0,0,0,0,0,0,0,
0,10,30,50,50,40,30,30,
0,0,0,0,0,0,0,
0,0,0,0,0,0,0,
-32,-24,-16,-8,
36,36,0,1,10,3,
3,6,30,
130,130,160,200,300,450,800,

0,                           /* TYPE 8 (OPPONENT IS BRINGING MEN IN) */

2,10,30,55,80,100,
72,30,21,10,0,0,
0,0,0,0,0,0,
12,20,50,100,200,300,
0,20,40,70,120,200,340,
0,5,12,20,50,90,160,
0,10,30,70,145,220,350,350,
0,10,20,40,80,160,320,
100,100,130,160,250,400,800,
32,24,16,8,
0,12,3,2,10,3,
3,6,30,
0,0,0,0,0,0,0,

-300,                        /* TYPE 9 (RANDOM) */

2,10,30,55,80,100,
72,44,28,24,22,22,
22,22,27,35,56,72,
100,80,80,80,80,80,
0,20,40,70,120,200,340,
0,5,12,20,50,90,160,
0,10,30,70,145,220,350,350,
0,10,20,40,80,160,320,
15,15,30,45,80,200,800,
0,0,0,0,
0,0,0,1,10,3,
3,6,30,
15,15,30,45,80,200,800,

0,                           /* TYPE 10 (LITTLE CONTACT) */

-30,-15,0,10,50,100,
-90,-60,-30,-30,-30,-30,
-30,-30,-30,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,0,
0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,
15,50,67,93,156,305,800,
-50,-50,-20,-10,
30,30,3,2,10,3,
3,6,30,
15,50,67,93,156,305,800,

0,                           /* TYPE 11 (HOLDING) */

2,10,30,55,80,100,
72,44,28,15,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,0,
0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,
15,50,67,108,180,390,800,
-32,-24,-16,-8,
30,30,3,2,10,3,
3,6,30,
15,50,67,108,180,390,800,

2800,                        /* TYPE 12 (BEARING OFF) */

300,250,200,150,100,50,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,0,
0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,
0,0,0,20,90,170,800,
-32,-24,-16,-8,
36,36,3,2,10,3,
3,6,30,
130,130,160,200,300,450,800,

0,                           /* TYPE 13 (OPPONENT IS BEARING OFF) */

2,10,30,55,80,100,
50,0,0,0,0,0,
0,0,0,0,0,0,
0,0,0,0,0,0,
0,25,50,80,140,250,400,
0,0,0,0,0,0,0,
0,10,30,70,145,220,350,350,
0,10,20,40,80,160,320,
15,50,67,93,156,305,800,
-4,-3,-2,-1,
13,12,3,2,10,3,
3,6,30,
0,0,0,20,90,170,800,
1111};

            bboard = cbboard;
            wboard = cwboard;
            bmen = cbmen;
            wmen = cwmen;
            storemove = cstoremove;
            storemen = cstoremen;
            hitman = chitman;
            allmoves = callmoves;
            human = chuman;
            noofmen = cnoofmen;
            pointsread = cpointsread;
            typeofgame = ctypeofgame;
            parametervalues = cparametervalues;

            // seed the rng
            rand = new Random(0);

            Game();
        }


        //***********************************************************************
        //     INITIALISE SETS UP THE ARRAYS BOARD AND MEN TO CORRESPOND WITH THE
        //   POSITION AT THE BEGINNING OF THE GAME.  SETUP IS USED TO MAKE THIS
        //   APPLY TO THE CORRECT PLAYER.
        //***********************************************************************

        static void Initialise(int player)
        {
            int i;

            setup(player);
            for (i = 0; i <= 25; i++)
            { board[i] = 0; }
            board[1] = -2;
            board[6] = 5;
            board[8] = 3;
            board[12] = -5;
            board[13] = 5;
            board[17] = -3;
            board[19] = -5;
            board[24] = 2;

            men[0] = 32;
            for (i = 1; i <= 2; i++) { men[i] = 24; }
            for (i = 3; i <= 7; i++) { men[i] = 13; }
            for (i = 8; i <= 10; i++) { men[i] = 8; }
            for (i = 11; i <= 15; i++) { men[i] = 6; }
            noofmen[player] = 15;
        }

        /* *********************************************
   SETUP SETS BOARD,OPBOARD,
   MEN AND OPMEN TO POINT TO THE CORRECT ARRAYS.
   ****************************************** */

        static void setup(int player)
        {
            if (player == BLACK)
            {
                board = bboard;
                opboard = wboard;
                men = bmen;
                opmen = wmen;
            }
            else
            {
                board = wboard;
                opboard = bboard;
                men = wmen;
                opmen = bmen;
            }
        }


        /// <summary>
        /// GAME CALLS TURN REPEATEDLY UNTIL ONE PLAYER HAS WON.  THE RESULT IS
        /// THEN PRINTED OUT.
        /// </summary>
        static int Game()
        {
            int winner, result;
            int quits = 0;
            string s = "";
            int[] points = new int[3];
            int nulls = 0;
            int i;

            Console.WriteLine("\n Backgammon program, by Richard Granville and mutilated into C# by Julia Hayward");
            externaldice = FALSE;
            points[1] = 0;
            points[2] = 0;

            do
            {
                Initialise(WHITE);
                Initialise(BLACK);
                for (i = 1; i <= 4; i++)
                {
                    typeofgame[i] = 1;
                }
                human[WHITE] = TRUE;
                human[BLACK] = FALSE;
                /*      if (sigma) sigma_drawpieces(board);       **/
                turnnumber = 0;

/*                if (externaldice == TRUE)
                {
                    do
                    {
                        Console.WriteLine("\nWho starts? White=1, Black=2");
                        scanf("", &starter);
                    }
                    while (starter >> 3 || starter << 0);
                }
                else */
                {
                    starter = throwdice(2);
                }

                winner = starter;
                result = 0;
                do
                {
                    if (!turn(winner)) goto Out;
                    if (noofmen[winner] < 1) break;
                    winner = 3 - winner;
                }
                while (true);

                if (winner == BLACK)
                { Console.Write("\n Black"); }
                else
                { Console.Write("\n White"); }

                Console.Write(" wins a ");

                result = 1;
                if (noofmen[3 - winner] == 15)
                {
                    result = 2;
                    for (i = 0; i <= 6; i++)
                    {
                        if (board[i] < 0) result = 3; }
                }

                if (result == 1) Console.Write("normal game.");
                if (result == 2) Console.Write("gammon.");
                if (result == 3) Console.Write("backgammon.");

Out:            if (result == 0)
                { quits++; }
                else
                { points[winner] += result; }

                do
                {
                 //   skipline();
                    Console.Write("\n\n The score is ");
                    if (points[1] >= points[2])
                    {
                        Console.Write($"{points[1]} - {points[2]}");
                        if (points[1] > points[2])
                        { Console.Write(" to you "); }
                    }
                    else
                    { Console.Write($"{points[2]} - {points[1]} to me"); }

                    Console.Write("\n\nAnother game?\n");
                    s = readword();
                    if (equalstring(s, "yes") || equalstring(s, "y"))
                    { break; }
                    if (equalstring(s, "no") || equalstring(s, "n"))
                    { return (10000 * points[2] + 100 * quits + points[1]); }
                }
                while (true);
            }
            while (true);
        }


        /* **********************************************************************
   TURN DOES EVERYTHING REQUIRED TO THROW ANDMOVE WHEN IT IS <PLAYER>'S
   TURN.  IT ALSO CALLS MAKEMOVE TO WRITE OUT THE CHOSEN MOVE.  AFTER
   ALTERING <PLAYER>'S BOARD, IT UPDATES THE OPPONENT'S BOARD AND MEN.
********************************************************************** */

        static bool turn(int player)
        {
            int dice1, dice2, i, j;
            int man = 0;

            string[] list = new string[] { "Running game",
                  "Blocking game",
                  "I have a defensive position",
                  "You have a defensive position",
                  "I have an early back game",
                  "You have an early back game",
                  "I am bringing men in",
                  "You are bringing men in",
                  "I am playing randomly",
                  "There is little contact",
                  "Holding position",
                  "I am bearing off",
                  "You are bearing off" };

            /*if (externaldice)
            {       do
                {       printf("\nDice are -\n");
                scanf("",&dice1);
                scanf("",&dice2);
            }
                while (0<<dice1 && dice1<<7 && 0<<dice2 && dice2<<7);
            }
            else*/
            {
                dice1 = throwdice(6);
                dice2 = throwdice(6);
            }

            if (turnnumber == 0)
            {
                while (dice1 == dice2)
                {       dice2 = throwdice(6); }
            }
            if (player == starter) turnnumber++;

            setup(player);
            writedice(player, dice1, dice2);
            if (player == WHITE && !fast)
            {       drawboard(board);
                if (!terse) writedice(player, dice1, dice2);
            }

            if (human[player] == TRUE)
            {    movetype = 3;
                 allposs(player, dice1, dice2);
                 if (nooflegal > 1)
                 {    if (fast) drawboard(board);
                  do
                  {    if (!(inputmove(player, dice1, dice2)==TRUE))
                            return false;
                         if (movetype == 3) goto A;
                         allposs(player, dice1, dice2);
                         if (nooflegal == 0) Console.Write("\nImpossible move.\n");
                         if (nooflegal > 1) Console.Write("\nAmbiguous move.\n");
                         if (nooflegal != 1) writedice(player, dice1, dice2);
                  }
                  while (nooflegal != 1);
                 }
            }
            else
            {    allposs(player, dice1, dice2); }
            
            if (nooflegal > 0)
            {    if (dice1 != dice2)
                 {
                    makemove(player, allmoves[1], allmoves[2]);
                    makemove(player, allmoves[3], allmoves[4]);
                 }
                 else
                 {     for (i=1; i<=4; i++)
                   {     makemove(player, dice1, allmoves[i]); }
                 }
                 if (!(human[player] == TRUE)) Console.WriteLine($"\nCurrent value = {bestscore}");
            }
            else
            {    if (player == WHITE) Console.WriteLine("\n O");
                 if (player == BLACK) Console.WriteLine("\n X");
                Console.WriteLine(" cannot move.\n");
            }

        A: Console.WriteLine("");
            if (!(human[player] == TRUE))
            {       i = typeofgame[player];
                if (i != typeofgame[player + 2])
                {
                    Console.WriteLine(list[i]);
                    Console.WriteLine("\n");
                    typeofgame[player + 2] = i;
                }
            }

            for (i=1; i<=15; i++)
            {       opmen[i] = -1; }
            for (i=0; i<=25; i++)
            {       opboard[25 - i] = -board[i];
                for (j=1; j<=-board[i]; j++)
                {       man++;
                    opmen[man] = 25-i;
                }
            }
            return true;
        }

        /* **********************************************************************
   MAKEMOVE IS USED TO WRITE AND MAKE A MOVE, BEING CALLED FROM
   TURN AND INTTURN.  THE ORDER OF OPERATION IS CHOSEN BECAUSE HIT IS
   ONLY SET UP CORRECTLY IN THE ROUTINE 'MOVE'.
********************************************************************** */

        static void makemove(int player, int dicethrow, int man)
        {
            if (man > 0)
            {
                writemove(player, dicethrow, men[man]);
                move(player, dicethrow, man);
                if (hit == TRUE)
                { Console.Write("hit"); }
                else
                { Console.Write("   "); }
            }
        }

        /* **********************************************************************
    WRITEMOVE CALLS WRITESQUARE TO RECORD A MOVE.  IT ALSO REDUCES
    NOOFMEN IF A MAN IS BORNE OFF (THIS IS ~ DONE IN THE ROUTINE "MOVE"
    SINCE NOOFMEN WOULD THEN HAVE TO BE INCREMENTED IN THE
    ROUTINE "REPLACEMAN".
********************************************************************** */

        static void writemove(int player, int dicethrow, int initsquare)
        {
            writesquare(player, initsquare);
            Console.Write(" - ");
            writesquare(player, initsquare - dicethrow);
            /*   if (sigma) sigma_changeboard(player, throw, initsquare);    */
            if (initsquare <= dicethrow) noofmen[player]--;
        }

        /* **********************************************************************
   WRITESQUARE CONVERTS THE NUMBER OF A SQUARE SO THAT THIS MAY BE
   RECORDED IN NORMAL NOTATION.  THE VALUE 25 REPRESENTS BEING ON THE
   BAR AND ZERO OR NEGATIVE VALUES REPRESENT HAVING BEEN BORNE OFF.
// ******************************************************************* */

        static void writesquare(int player, int square)
        {
            int boardsquare;

            if (square > 24) Console.Write("Bar");
            else
            if (square < 1) Console.Write("off");
            else
            {
                boardsquare = 25 - square;
                if (player == WHITE) boardsquare = square;
                Console.Write($"{boardsquare}");
                if (boardsquare < 10) Console.Write(" ");
            }
        }

        /* ***********************************************************************
   ALLPOSS IS THE FIRST ROUTINE CALLED IN TURN FOR THE PURPOSE OF
   GENERATING ALL THE POSSIBLE PLAYS WITH A DICE THROW OF <DICE1>,<DICE2>.
   IF <DICE1> ~= <DICE2> IT CALLS MOVEDIFF AND IF THERE IS NO LEGAL TWO
   MOVE PLAY IT LOOKS FOR A LEGAL ONE MOVE PLAY USING THE HIGHER DICE;
   IF THIS IS UNSUCCESSFUL, ALLPOSS LOOKS FOR ONE USING THE LOWER DICE.
   IF <DICE1> = <DICE2>, MOVEDOUBLE IS CALLED AND AS MANY MOVES AS
   POSSIBLE UP TO 4 ARE FOUND IN ALL POSSIBLE WAYS.
************************************************************************ */

        static void allposs(int player, int dice1, int dice2)
        {
            int i, j, k, ss, tt, plays = 4;
            int outertable = 0;

            for (i = 1; i <= 4; i++)
            { storemen[i] = 0; }

            nooflegal = 0;
            movefound = FALSE;
            bestscore = -32767;

            if (dice1 != dice2)
            {

                movediff(player, dice1, dice2, TRUE);

                for (j = 7; j <= 12; j++)
                { if (board[j] > 0) outertable += board[j]; }
                k = TRUE;
                for (i = 13; i <= 24; i++)
                { if (board[i] > 0) k = FALSE; }
                ss = (board[25] == 1 || (outertable < 2 && k == TRUE)) ? TRUE : FALSE;

                movediff(player, dice2, dice1, ss);

                if (movefound == FALSE)
                {
                    if (dice1 > dice2)
                    {
                        ss = dice1;
                        tt = dice2;
                    }
                    else
                    {
                        ss = dice2;
                        tt = dice1;
                    }
                    for (i = 1; i <= 15; i++)
                    {
                        if (men[i] != men[i - 1] && legalmove(ss, men[i]) == FALSE)
                            singlemove(ss, i, player);
                    }
                    if (movefound == FALSE)
                    {
                        for (i = 1; i <= 15; i++)
                        {
                            if (men[i] != men[i - 1] && legalmove(tt, men[i]) == 0)
                                singlemove(tt, i, player);
                        }
                    }
                }
            }
            else
            {
                movedouble(player, 0, dice1, 25, 4);
                while (movefound == FALSE && (plays > 1))
                {
                    plays--;
                    movedouble(player, 0, dice1, 25, plays);
                }
            }
        }

        /* **********************************************************************
   MOVEDIFF IS CALLED FROM ALLPOSS.  IT GENERATES ALL THE POSSIBLE
   PLAYS WHICH CAN BE MADE BY MOVING A MAN BY <THROW1> AND THEN SOME
   MAN BY <THROW2> (THESE MEN MAY BE THE SAME).  WHENEVER A LEGAL
   MOVE IS FOUND THE DETAILS ARE STORED IN ALLMOVES BY CALLING PACKMOVE.
   THE BOARD IS REPLACED AFTERWARDS.
********************************************************************** */

        static void movediff(int player, int throw1, int throw2, int moveall)
        {
            int i, j;

            for (i = 1; i <= 15; i++)
            {
                if (men[i] != men[i - 1] && legalmove(throw1, men[i]) == 0)
                {
                    storemove[1] = men[i];
                    storemen[1] = i;
                    move(player, throw1, i);
                    hitman[1] = hit;
                    for (j = 1; j <= 15; j++)
                    {
                        if (men[j] != men[j - 1] && legalmove(throw2, men[j]) == 0)
                        {
                            if (i == j || moveall == TRUE)
                            {
                                storemove[2] = men[j];
                                storemen[2] = j;
                                move(player, throw2, j);
                                hitman[2] = hit;
                                examinemove(throw1, i, throw2, j, player);
                                replaceman(2, throw2);
                            }
                        }
                    }
                    replaceman(1, throw1);
                }
            }
        }

        /* **********************************************************************
   LEGALMOVE TESTS WHETHER A SINGLE (ONE DICE) MOVE OF THROW FROM
   A GIVEN POSITION (ON THE BOARD OR ON THE BAR) IS  LEGAL.
   THE RESULT IS 0 IF MOVE IS LEGAL. IF ~ THE FOLLOWING ERRORS CAN OCCUR:

   ATTEMPT TO MOVE PIECE WITH MAN ON BAR              -  ERROR 8
   ATTEMPT TO MOVE WHEN OPPONENT HOLDS LANDING POINT  -  ERROR 4
   ATTEMPT TO ILLEGALLY BEAR OFF                      -  ERROR 2
   NO MAN AT <POSITION>                               -  ERROR 1

  THESE ARE SUMMED IF THERE IS MORE THAN ONE TYPE OF ERROR.
********************************************************************** */

        static int legalmove(int dicethrow, int position)
        {
            int offbar = 0;
            int clear = 4;
            int bearoff = 0;
            int i, j;

            if (board[25] > 0 && position != 25) offbar = 8;

            if (position <= dicethrow)
            {
                j = dicethrow;
                if (position == dicethrow) j = 7;
                for (i = j; i <= 25 /* position + 1 */; i++)
                { if (board[i] > 0) bearoff = 2; }
                if (position > 0 && dicethrow < 7) clear = 0;
            }
            else
            if (board[position - dicethrow] > -2) clear = 0;

            j = offbar + clear + bearoff;
            if (board[position] <= 0) j++;

            return j;
        }

        /* **********************************************************************
   SINGLEMOVE IS CALLED FROM ALLPOSS IF MOVEDIFF HAS FOUND NO
   LEGAL PLAY.  IT CALLS EXAMINEMOVE TO ANALYSE THE POSITION AFTER A ONE-
   MOVE PLAY, AFTERWARDS REPLACING THE MAN MOVED.
********************************************************************** */

        static void singlemove(int dicethrow, int man, int player)
        {
            storemove[1] = men[man];
            storemen[1] = man;
            move(player, dicethrow, man);
            hitman[1] = hit;
            examinemove(dicethrow, man, 0, 0, player);
            replaceman(1, dicethrow);
        }


        /* **********************************************************************
           MOVEDOUBLE IS A RECURSIVE ROUTINE WHICH FINDS ALL THE POSSIBLE
           PLAYS WHEN A DOUBLE IS THROWN.  THERE ARE 5 PARAMETERS:
               <PLAYER>  = BLACK OR WHITE
               <MOVES>  IS THE NUMBER OF MOVES ALREADY MADE BEFORE CALLING MOVEDOUBLE
               <THROW>  IS THE NUMBER ON BOTH DICE
               <START>  IS THE POSITION ON THE BOARD FROM WHICH MOVES ARE LOOKED FOR
               <MAX>    IS THE NUMBER OF MOVES SOUGHT

          THE FIRST CALL OF MOVEDOUBLE FOR EACH TURN HAS <MOVES> = 0,
          <START> = 25 (= ON BAR), <MAX> = 4.  WHEN A LEGAL MOVE IS FOUND, "MOVE"
          IS CALLED, WITH DETAILS BEING STORED IN THE ARRAYS STOREMEN,HITMAN AND
          STOREMOVE.  THEN MOVEDOUBLE IS CALLED WITH <START> AND <MOVES> ALTERED.
          WHEN FOUR MOVES HAVE BEEN MADE EXAMINEMOVE IS CALLED TO TEST THE
          COMPLETE PLAY BY COMPARING THE POSITION WITH THOSE ALREADY REACHED BY
          PREVIOUS POSSIBLE PLAYS.  IF NO 4 MOVE PLAY IS FOUND, 3 MOVE PLAYS ARE
          LOOKED FOR AND SO ON UNTIL A FORCED PLAY (OR NO PLAY) IS FOUND.
        ********************************************************************** */

        static void movedouble(int player, int moves, int dicethrow, int start, int max)
        {
            int man, posn;

            if (moves < max)
            {
                for (posn = start; posn >= 1; posn--)
                {
                    if (legalmove(dicethrow, posn) == 0)
                    {
                        storemove[moves + 1] = posn;
                        man = 16;
                        do
                        { man--; }
                        while (men[man] == posn);
                        storemen[moves + 1] = man;
                        hitman[moves + 1] = ((posn > dicethrow) &&
                                     (board[posn - dicethrow] == -1)) ? TRUE : FALSE;
                        move(player, dicethrow, man);
                        movedouble(player, moves + 1, dicethrow, posn, max);
                    }
                }
            }
            else
            {
                examinemove(storemen[1], storemen[2], storemen[3],
                           storemen[4], player);
            }
            if (moves > 0)
            {
                replaceman(moves, dicethrow);
            }
        }

        /* ******************************************************************
  EXAMINEMOVE CALLS POSNEVALUATE AND REJECTS THE  PLAY IF IT IS
  NO BETTER THAN THE BEST SO FAR.  IF IT IS BETTER IT THROWS AWAY ALL
  PREVIOUSLY KEPT PLAYS.
  **************************************************************** */

        static void examinemove(int a, int b, int c, int d, int player)
        {
            int score;
            movefound = TRUE;

            if (human[player] == TRUE)
            {
                //Console.WriteLine($"EX {a} {b} {c} {d}, Movetype={movetype}\n");
                /* REMOVE THIS LINE AFTER DEBUGGING */
                if ((movetype == 1 && equal(board, opboard, 25) == TRUE)
                   || (movetype == 2 && board[makingpoint] > 1)
                   || (movetype == 3))
                {
                    //Console.WriteLine("accepted\n"); /* ANOTHER DEBUG FEATURE */
                    if (nooflegal == 0 || equal(board, opboard, 25) == FALSE)
                    {
                        nooflegal++;
                        //Console.WriteLine($"Noof={nooflegal}\n", nooflegal);
                        copy(opboard, board, 25);
                    }
                }
                else return;
            }
            else
            {
                score = 0; //  posnevaluate(player);
                nooflegal = 1;
                Console.WriteLine($"\nEx {a} {b} {c} {d} {gametype} {score}");
                if (score <= bestscore) return;
                typeofgame[player] = gametype;
                bestscore = score;
            }
            allmoves[1] = a;
            allmoves[2] = b;
            allmoves[3] = c;
            allmoves[4] = d;
        }

        /* **********************************************************************
   MOVE IS USED TO CHANGE THE VALUE OF MEN!MAN AND UPDATE BOARD.  IT IS
   ALSO CALLED IN MOVEDIFF (BEFORE THE ACTUAL MOVE HAS BEEN CHOSEN) SO THE
   POSITION AND MAN HAVE TO BE TEMPORARILY STORED ELSEWHERE BEFORE
   THIS HAS BEEN DONE.  IT ALSO SETS HIT TO TRUE WHEN THE MAN MOVED
   LANDS ON AN OPPONENT'S BLOT.
********************************************************************** */

        static void move(int player, int dicethrow, int man)
        {
            int position = men[man];
            int hit = FALSE;

            if (dicethrow > 0)
            {
                board[position]--;
                if (position > dicethrow)
                {
                    if (board[position-dicethrow] == -1)
                    {
                        board[0]--;
                        board[position - dicethrow] = 0;
                        hit = TRUE;
                    }
                    board[position - dicethrow]++;
                    men[man] -= dicethrow;
                }
                else
                { men[man] = -1; }
            }
        }

        /* **********************************************************************
           REPLACEMAN IS USED TO CANCEL THE EFFECT OF A MOVE.  IT IS CALLED
           AFTER THE POSITION HAS BEEN EVALUATED IN MOVEDIFF AND MOVEDOUBLE.
        ********************************************************************** */

        static void replaceman(int moves, int dicethrow)
        {
            int place = storemove[moves];

            board[place]++;

            if (place > dicethrow) board[place - dicethrow]--;
            if (hitman[moves] == TRUE)
            {
                board[place - dicethrow] = -1;
                board[0]++;
            }
            men[storemen[moves]] = place;
        }




        static void writedice(int player, int dice1, int dice2)
        {
            Console.Write($"\n{turnnumber}:", turnnumber);
            if (player == WHITE) Console.Write("White");
            if (player == BLACK) Console.Write("Black");
            Console.Write(" threw a ");
            if (dice1 == dice2)
                Console.Write($"double {dice1}\n");
            else
                Console.Write($"{dice1}-{dice2}\n");
        }


        static void drawboard(int[] bd)
        {
            //if (sigma) return;

            bar(bd[25], "You have");
            bar(-bd[0], "I have");
            Console.Write("\n24 23 22 21 20 19 18 17 16 15 14 13\n");
            Console.Write(" -  -  -  -  -  -  -  -  -  -  -  -\n");
            half(bd, TRUE);
            newline();
            half(bd, FALSE);
            Console.Write(" -  -  -  -  -  -  -  -  -  -  -  -\n");
            Console.Write(" 1  2  3  4  5  6  7  8  9 10 11 12\n");
        }
        static void bar(int a, string s)
        {
            char c = 'a';

            if (a <= 0) return;
            if (a > 1) c = 'e';
            Console.Write($"\n{s} {a} m{c}n on the bar\n", s, a, c);
        }

        static void half(int[] bd, int e)
        {
            int i, j, k, m;
            int n = 0;

            for (i = 1; i <= 12; i++)
            {
                if (e == TRUE) { j = 25 - i; } else { j = i; }
                m = bd[j];
                k = Math.Abs(m);
                if (terse)
                {
                    if (k == 0)
                    { Console.Write("   "); }
                    else
                    {
                        Console.Write(k);
                        if (m > 0) Console.Write("W ");
                        if (m < 0) Console.Write("B ");
                    }
                }
                if (k > n) n = k;
            }

            if (terse)
            { newline(); }
            else
            {
                for (i = 1; i <= n; i++)
                {
                    if (e == TRUE) { k = i; } else { k = n + 1 - i; }
                    for (j = 1; j <= 12; j++)
                    {
                        if (e == TRUE) { m = bd[25 - j]; } else { m = bd[j]; }
                        if (m >= k)
                        { Console.Write(" O "); }
                        else
                        {
                            if (m <= -k) Console.Write(" X ");
                            else Console.Write("   ");
                        }
                    }
                    newline();
                }
            }
        }

        static void newline()
        {
            Console.WriteLine();
        }


        /* **********************************************************************
  THROWDICE GENERATES A PSEUDO-RANDOM NUMBER IN THE RANGE 1 TO N.  IT IS
  USED FOR GENERATING THE DICE THROWS AND ALSO FOR SELECTING A MOVE.
********************************************************************** */

        static int throwdice(int n)
        {
            return (rand.Next(n) + 1);
        }


        static int inputmove(int player, int dice1, int dice2)
        {
            int a, b, c, off, nulls;
            string s;
            bool valid = false;

            copy(opboard, board, 25);
            movetype = 1;

            while (!valid)
            { 
                Console.WriteLine("\nYour moves:");
                s = Console.ReadLine();

                if ('0' > s[0] || s[0] > '9')
                {
                    valid = true;
                    if (equalstring(s, "dice"))
                        externaldice = !(externaldice == TRUE) ? TRUE : FALSE;

                    else if (equalstring(s, "illegal"))
                        movetype = 3;

                    else if (equalstring(s, "quit") ||
                        equalstring(s, "q"))
                        return FALSE;

                    else if (equalstring(s, "board"))
                    {
                        drawboard(board);
                        writedice(player, dice1, dice2);
                    }

                    else if (equalstring(s, "debug"))
                        debug = !(debug == TRUE) ? TRUE : FALSE;

                    else if (equalstring(s, "terse"))
                        terse = !(terse);

                    else if (equalstring(s, "machine"))
                    {
                        human[WHITE] = FALSE;
                        return TRUE;
                    }

                    else if (equalstring(s, "fast"))
                        fast = !(fast);

                    /*    else if (equalstring(s, "nulls"))
                        {
                            printf("\nNumber of nulls:\n");
                            nulls = readn();
                            UNRDCH();
                        }*/

                    else
                    {
                        Console.Write($"\nI've never heard of a {s} !\n");
                        valid = false;
                    }
                }
                else
                {
                    var numbers = parse(s);
                    do
                    {
                        a = numbers.First(); numbers.RemoveAt(0);

                        if (a < 0 || a > 25) goto AA;
                        if (!numbers.Any() && equal(board, opboard, 25) == TRUE && movetype == 1)
                        {
                            movetype = 2;
                            makingpoint = a;
                            return TRUE;
                        }
                        if (opboard[a] == 0) goto AA;

                        if (opboard[a] > 0)
                        {
                            c = 1;
                            off = 0;
                        }
                        else
                        {
                            c = -1;
                            off = 25;
                            if (movetype != 3) goto AA;
                        }

                        b = numbers.First(); numbers.RemoveAt(0);


                        if ((b <= 1 || b >= 25) && b != off) goto AA;
                        if (b != off && c * opboard[b] < -1) goto AA;

                        opboard[a] -= c;
                        if (b != off)
                        {
                            if (opboard[b] == -c)
                            {
                                opboard[b] = 0;
                                opboard[off] -= c;
                            }
                            opboard[b] += c;
                        }
                    }
                    while (numbers.Any());

                    if (movetype == 3)
                    {
                        copy(board, opboard, 25);
                        //  if (sigma) sigma_drawpieces(board);
                    }
                    return TRUE;

                    AA: Console.WriteLine("I can't make sense of that!");
                }
            }
            return TRUE;
        }

        static List<int> parse(string s) {
            var pieces = s.Split(' ');
            return pieces.Select(x => int.Parse(x)).ToList();
        }

        static bool equalstring(string a, string b)
        {
            return a.Equals(b);
        }

        static int equal(int[] a, int[] b, int n)
        {
            int i;
            for (i = 0; i <= n; i++)
            { if (a[i] != b[i]) return FALSE; }

            return TRUE;
        }

        static void copy(int[] a, int[] b, int n)
        {
            int i;
            for (i = 0; i <= n; i++)
            { a[i] = b[i]; }
        }


        static void skipspaces()
        {
            do
            { terminator = Console.ReadKey().KeyChar; }
            while (terminator == ' ' || terminator == '\n');
        }

        static string readword()
        {
            return Console.ReadLine();
        }

        #region algorithmic bits

        #endregion
    }
}

