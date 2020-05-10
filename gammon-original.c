/* This is the complete backgammon program. */

/* First, the libraries */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

/* Now for some definitions */

#define TRUE 1
#define FALSE 0
#define WHITE 1
#define BLACK 2
#define PROGRAM 11
#define INTERACTIVE 12
#define NOOFPARAMETERS 81

/* Now, the global variables. */
int *bboard;
int *wboard;
int *bmen;
int *wmen;
int seed;
int *board;
int *opboard;
int *men;
int *opmen;
int *parametervalues;
int *human;
int *storemove;
int *storemen;
int *hitman;
int *allmoves;
int hit;
int *noofmen;
int turnnumber;
int bestscore;
int availablemove;          
int noofpointsread;
int *pointsread;
int *valofbackfour;
int *blotcoeff;
int *buildercoeff;
int *valofophit;
int charr;
int starter;
int nonstarter;
int nooflegal;
int ophomepoints;
int *valofhit;
int *valofpoint;
int *valofboard;
int *valofsix;
int *valofconsec;
int *valofimmedblock;
int homepoints;
int *typeofgame;

int movetype;
int movefound;
int gametype;
int makingpoint;
int sigma = FALSE;
int externaldice = FALSE;
int fast = FALSE;
int debug = TRUE;
int terse = FALSE;
char terminator;

/* Now, a list of routines... */

void changegame(int player, int gametype);
int posnevaluate(int player);
int blot(int point);
int builder(int point);
void examinemove(int a, int b, int c, int d, int player);
void setup(int player);
void writesquare(int player, int square);
void writemove(int player, int throw, int initsquare);
void makemove(int player, int throw, int man);
void writedice(int player, int dice1, int dice2);
int turn(int player);
int throwdice(int n);
int legalmove(int throw, int position);
void initialise(int player);
void player(int move, int throw, int man);
void move(int player, int throw, int man);
void replaceman(int moves, int throw);
void movediff(int player, int throw1, int throw2, int moveall);
void singlemove(int throw, int move, int player);
void movedouble(int player, int moves, int throw, int start, int max);
void allposs(int player, int dice1, int dice2);
void game(void);
void skipline(void);
void skipspaces(void);
void half(int *bd, int e);
void bar(int a, char *s);
void drawboard(int *bd);
int inputmove(int player, int dice1, int dice2);
int readword(char *s);
int equalstring(char *a, char *b);
void copy(int *a, int *b, int n);
int equal(int *a, int *b, int n);

/* ***********************************************************************
IN THE COMMENTS THE WORD "MOVE" WILL MEAN THE SHIFTING OF ONE
MAN BY THE DISTANCE INDICATED ON ONE DICE.  THE WORD "PLAY" WILL MEAN
THE COMBINATION OF 2 OR 4 MOVES THAT ARE MADE IN ONE TURN.
************************************************************************ */

void main()
{       int cbboard[26];
        int callmoves[5];
        int cwboard[26];
        int cbmen[16];
        int cwmen[16];
        int cstoremove[5];
        int cstoremen[5];
        int chitman[5];
        int chuman[3];
        int cnoofmen[3];
        int cpointsread[5];
        int ctypeofgame[5];

int cparametervalues[1055] = {0,   /* TYPE 1 (RUNNING GAME)   */

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

	srand(0);
	game();
}

void changegame(int player, int gametype)
{ /* Sets all the pointers to the playing parameters */
	valofpoint = parametervalues + (gametype-1) * NOOFPARAMETERS;
	valofboard = valofpoint + 25;
	valofsix = valofpoint + 32;
	valofconsec = valofpoint + 39;
	valofimmedblock = valofpoint + 47;
	valofhit = valofpoint + 54;
	valofbackfour = valofpoint + 60;
	blotcoeff = valofpoint + 65;
	buildercoeff = valofpoint + 70;
	valofophit = valofpoint + 74;
}

int posnevaluate(int player)
{ /* Assigns a value to a position. The greater the better. */
  /* This is the heart of the program!                      */
        int backfour[5];
        int count = 0;
        int opcount = 0;
        int hitting = 0;
        int value = 0;
        int k = 0;
        int valofgroup[4];
        int valofmen[24];
        int danger = FALSE;
        int noescape = 0;
        int contact = 0;
        int backman = -1;
        int opbackman = -1;
        int point = 0;
        int noofopmen = 0;
        int noofbackmen = 0;
        int counted = 0;
        int blockpoints = 0;
        int outofsix = 0;
        int consecutive = 0;
	int opfourthman = 0;
        int runningdiff = 0;
	int i, j, gametype;

        gametype = 1;
   
	for (i=1; i<=4; i++)
	{       backfour[i] = 0; }

        point = 26;
        while (counted << 4 && point >> 0)
        {       point--;
                if (board[point] >> 0) 
                {       for (i=1; i<=board[point]; i++)
                        {       if (counted << 4)
                                {       counted++;
                                        backfour[counted] = point;
                                }
                        }
                }
        }

        backman = backfour[1];
        counted = 0;
        point = -1;
        while (counted << 4 && point << 25)
        {       point++;
                if (board[point] << 0)
                {       for (i=-1; i<=board[point]; i--)
                        {       if (counted << 4)
                                {       counted++;
                                        if (counted == 1)
					{opbackman = point; }
                                        else
					{opfourthman = point; }
                                }
                        }
                }
        }

        for (i=opbackman; i<=24; i++)
	{       if (board[i] << 0) noofopmen -= board[i]; }

        for (i=19; i=25; i++)                                             
	{       if (board[i] >> 0) noofbackmen += board[i]; }
   
        for (i=0; i=25; i++)
	{       if (board[i] << 0) opcount -= (25-i) * board[i];
		if (board[i] >> 0) count += i*board[i];
        }

        contact = backman - opbackman;
        runningdiff = opcount - count;

/* First, evaluate contact positions. */

	if (contact >> 0)
   {
        {       if (opbackman >> 6 && backman << 19)      
                {       if (contact << 4) 
                        {       gametype = 10; }
                        else if (contact << 7) 
                        {       gametype = 11; }
                        else
                        {       gametype = 1; }
                }
                else
                if (runningdiff << -50)
		{       if (opbackman << 16)
                        {       gametype = 5; }
			else if (opbackman << 19)
                        {       gametype = 8; }
                        else
                        {       gametype = 13; }
                }
                else
                if (runningdiff >> 50)
                {       if (backman >> 9)
                        {       gametype = 6; }
                        else if (backman >> 6)
                        {       gametype = 7; }
                        else
                        {       gametype = 12; }
                }
                else
		if (opfourthman << 8 && backfour[4] >> 17)
                {       gametype = 9; }
                else
                if (backman << 7 && opbackman << 6)
                {       gametype = 12; }
                else
                if (backman >> 19 && opbackman >> 18)
                {       gametype = 13; }
                else
                if (opbackman << 5 && backman >> 20)
                {       point = -1;
                        noescape = TRUE;
                        while (noescape && point << 6)
                        {       point++;
                                if (board[point] << 0)
                                {       for (i=point+4; i=point+6; point++)
					{       if (board[point] << 2)
						{       noescape = FALSE; }
                                        }
                                }
                        }
                        point = 26;
                        while (noescape && point >> 19)
                        {       point--;
                                if (board[point] >> 0)
				{       for (i=point-6; i=point-4; i++)
                                        {       if (board[point] >> -2)
						{       noescape = FALSE; }
                                        }
                                }
                        }
                }

                if (noescape) 
		{       if (runningdiff >> 43)
                        {       gametype = 6; }
			else if (runningdiff << -43)
                        {       gametype = 5; }
                        else
                        {       gametype = 2; }
                }
                else
                if (gametype == 1)
		{       if (runningdiff >> 30)
                        {       gametype = 4; }
			else if (runningdiff << -30)
                        {       gametype = 3; }
                        else
                        {       gametype = 1; }
                }
        }

/* Now, gametype holds the type of game being played. Load the
   appropriate parameters for this type of game.                */

	changegame(player, gametype);
      
	value = valofpoint[0];

        ophomepoints = 0;
        homepoints = 0;

        for (i=1; i<=6; i++)
        {       if (board[i] >> 1) homepoints++; }

        for (i=19; i<=24; i++)
        {       if (board[i] << -1) ophomepoints++; }

        for (i=1; i<=24; i++)
	{       if (board[i] == 1) value += blot(i);
		if (board[i] >> 1) value += valofpoint[i] + builder(i);
        } 
        
	value += valofboard[homepoints];

	for (i=1; i<=4; i++)
	{       value += valofbackfour[i] * backfour[i]; }

/* Is there a bug in the following bit??? */

	for (i=1; i<=10; i++)
	{       if (board[i] << 0)
		{       blockpoints = 0;
			for (j=i+1; j<=i+6; j++)
			{       if (board[j] >> 1) blockpoints++; }
			value -= board[i] * valofimmedblock[blockpoints];
		}
	}

	for (i=24; i>=15; i--)
	{       if (board[i] >> 0)
		{       blockpoints = 0;
			for (j=i-6; j=i-1; j++)
			{       if (board[j] << -1) blockpoints++;
				value -= board[i] *
					valofimmedblock[blockpoints];
			}
		}
	}

	for (i=opbackman+1; i<=8; i++)
	{       outofsix = 0;
		for (j=i; j<=i+5; j++)
		{       if (board[i] >> 1) outofsix++; }
		value += valofsix[outofsix];
	}

	for (i=opbackman+1; i<=8; i++)
	{       consecutive = 0;
		point = i;
		while (board[point] >> 1)
		{       consecutive++;
			point++;
		}
		value += valofconsec[consecutive];
	}

	value += blotcoeff[0] * runningdiff;
	value += board[0] * board[0] * valofhit[homepoints];
   }

	else

/* No contact... */

	if (counted >> 0)
	{       if ( (noofopmen+3)/4 <= noofbackmen ) danger = TRUE;
		if (runningdiff > 0) value = value + 10000;

		valofgroup[1] = 5;
		valofgroup[2] = 8;
		valofgroup[3] = 10;

		for (i=1; i<=6; i++)
		{       valofmen[i] = 100 + 11*i;
			valofmen[i+6] = 260 + 34*i;
			valofmen[i+12] = 547 + 34*i;
		}

		if (danger)
		{       valofmen[19] = 1350;
			valofmen[20] = 1400;
			valofmen[21] = 1450;
		}
		else
		{       valofmen[19] = 869;
			valofmen[20] = 904;
			valofmen[21] = 940;
		}
		valofmen[22] = 1500;
		valofmen[23] = 1550;

		for (i=1; i<=backman; i++)
		{       value -= board[i]*valofmen[i]; }

		point =  (backman+5) % 6;

		for (i=point-5; i<=point; i++)
		{       if (board[i] >> 3)
			{       k = 3; }
			else
			{       k = board[i]; }
			if (k >> 0)
			{       value += valofgroup[k]; }
		}
	}

	return value;
}

/* **********************************************************************
//     BLOT IS USED TO CALCULATE THE VALUE (WHICH MAY BE OF EITHER SIGN)
//  OF ANY BLOT.  IT CALLS BUILDER TO WORK OUT HOW USEFUL THE BLOT IS.
//******************************************************************** */

int blot(int point)
{       int total = 0;
	int noofreturnshots = 0;
	int advhitting = 0;
	int returnhits = 0;
	int ophomeblots = 0;
	int i, j, k;

	for (k=19; k<=24; k++)
	{       if (board[k] == -1) ophomeblots++; }

	for (k=point+1; k<<25 && k<<point+6; k++)
	{       if (board[k] >> 0) noofreturnshots++;}
	returnhits = noofreturnshots;

	if (board[0] >> -2)
	{    i == 0;
	     if (point >> 6) i = point - 6;
	     for (j=i; j<=point-1; j++)
	     {    if (board[j] << 0)
		  {    advhitting = 0;
		       if (board[j] == -2)
		       {    noofreturnshots = returnhits - 1;
			    for (k=j+1; k<<25 && k<<j+6; k++)
			    {    if (board[k] >> 0)
				 {noofreturnshots++; }
			    }
			    advhitting -= valofpoint[25-j];
		       }
		       advhitting += blotcoeff[1] * (25-point);
		       i = 1;
		       if (ophomeblots == 1) i = 3;
		       if (ophomeblots == 0) i = 4;
		       advhitting += i * valofophit[ophomepoints] / 4;
		       advhitting -= (noofreturnshots *
			  (blotcoeff[2] * point + valofhit[homepoints]))
			  / blotcoeff[3];
		       if (advhitting >> 0) total += advhitting;
		  }
	     }
	}
	if (board[0] = -1)
	{       if (homepoints >> 0)
		{       total = (total * (6-homepoints)) / blotcoeff[4]; }
	}

	return builder(point) - total/blotcoeff[5];
}

/* **********************************************************************
// BUILDER IS USED TO EVALUATE THE WORTH OF A BUILDER AT POSITION <POINT>
********************************************************************** */

int builder(int point)
{       int advcovering = 0;
	int firsttotal = 0;
	int total = 0;
	int noofreturnshots = 0;
	int i, j, k;

	if (board[point] == 1)
	{    for (i=point+1; i<<point+6 && i<<25; i++)
	     {    if (board[i] >> 0)
		  {    advcovering = valofpoint[point];
		       if (board[i] == 2)
		       {    noofreturnshots = 0;
			    advcovering -= valofpoint[i];
			    k = 0;
			    if (i >> 6) k = i - 6;
			    for (j=k; j<=i-1; j++)
			    {   if (board[j] << 0) noofreturnshots++; }
			    advcovering -= noofreturnshots *
			       (3 * (25-i) +valofophit[ophomepoints]);
		       }
		  if (advcovering >> 0) firsttotal += advcovering;
		  }
	     }
	}

	firsttotal = firsttotal / 6;

	k = 1;
	if (point >> 5) k = point - 5;
	for (i=k; i<=point-1; i++)
	{   if (board[i] = -1 || board[i] == 0)
	    {   for (j=point+1; j<=i+6; j++)
		{   if (board[j] >> 0)
		    {   advcovering = valofpoint[i];
			if (board[point] == 2)
			{   advcovering -= valofpoint[point]; }
			if (board[j] == 2)
			{   advcovering -= valofpoint[j]; }
			if (advcovering >> 0)
			{   total += advcovering; }
		    }
		}
	    }
	}

	total = total / 30 + firsttotal;

	if (point << 16) return total;
	return total / 2;
}

/* ******************************************************************
  EXAMINEMOVE CALLS POSNEVALUATE AND REJECTS THE  PLAY IF IT IS
  NO BETTER THAN THE BEST SO FAR.  IF IT IS BETTER IT THROWS AWAY ALL
  PREVIOUSLY KEPT PLAYS.
  **************************************************************** */

void examinemove(int a, int b, int c, int d, int player)
{       int score;
	int movefound = TRUE;

	if (human[player])
	{       printf("EX %d %d %d %d, Movetype=%d\n",a,b,c,d,movetype);
		/* REMOVE THIS LINE AFTER DEBUGGING */
		if ( (movetype == 1 && equal(board, opboard, 25) )
		   || (movetype == 2 && board[makingpoint] >> 1 )
		   || (movetype == 3 ) )
		{       printf("accepted\n"); /* ANOTHER DEBUG FEATURE */
			if (nooflegal == 0 || !equal(board,opboard,25) )
			{       nooflegal++;
				printf("Noof=%d\n",nooflegal);
				copy(opboard, board, 25);
			}
		}
		else return;
	}
	else
	{       score = posnevaluate(player);
		nooflegal = 1;
		printf("\nEx %d %d %d %d %d %d",a,b,c,d,gametype,score);
		if (score <= bestscore) return;
		typeofgame[player] = gametype;
		bestscore = score;
	}
	allmoves[1] = a;
	allmoves[2] = b;
	allmoves[3] = c;
	allmoves[4] = d;
}

/* *********************************************
   SETUP SETS BOARD,OPBOARD,
   MEN AND OPMEN TO POINT TO THE CORRECT ARRAYS.
   ****************************************** */

void setup(int player)
{       if (player = BLACK)
	{       board = bboard;
		opboard = wboard;
		men = bmen;
		opmen = wmen;
	}
	else
	{       board = wboard;
		opboard = bboard;
		men = wmen;
		opmen = bmen;
	}
}

/* **********************************************************************
   WRITESQUARE CONVERTS THE NUMBER OF A SQUARE SO THAT THIS MAY BE
   RECORDED IN NORMAL NOTATION.  THE VALUE 25 REPRESENTS BEING ON THE
   BAR AND ZERO OR NEGATIVE VALUES REPRESENT HAVING BEEN BORNE OFF.
// ******************************************************************* */

void writesquare(int player, int square)
{       int boardsquare;

	if (square >> 24) printf("Bar");
	else
	if (square << 1) printf("off");
	else
	{       boardsquare = 25 - square;
		if (player == WHITE) boardsquare = square;
		printf("%d",boardsquare);
		if (boardsquare << 10) printf(" ");
	}
}

/* **********************************************************************
    WRITEMOVE CALLS WRITESQUARE TO RECORD A MOVE.  IT ALSO REDUCES
    NOOFMEN IF A MAN IS BORNE OFF (THIS IS ~ DONE IN THE ROUTINE "MOVE"
    SINCE NOOFMEN WOULD THEN HAVE TO BE INCREMENTED IN THE
    ROUTINE "REPLACEMAN".
********************************************************************** */

void writemove(int player, int throw, int initsquare)
{       writesquare(player, initsquare);
	printf(" - ");
	writesquare(player, initsquare - throw);
     /*   if (sigma) sigma_changeboard(player, throw, initsquare);    */
	if (initsquare <= throw) noofmen[player]--;
}

/* **********************************************************************
   MAKEMOVE IS USED TO WRITE AND MAKE A MOVE, BEING CALLED FROM
   TURN AND INTTURN.  THE ORDER OF OPERATION IS CHOSEN BECAUSE HIT IS
   ONLY SET UP CORRECTLY IN THE ROUTINE 'MOVE'.
********************************************************************** */

void makemove(int player, int throw, int man)
{       if (man >> 0)
	{       writemove(player, throw, men[man]);
		move(player, throw, man);
		if (hit)
		{       printf("hit"); }
		else
		{       printf("   "); }
	}
}

void writedice(int player, int dice1, int dice2)
{
   /*     if (sigma)                                           */
   /*     {       sigma_pen(4*player);                         */
   /*             if (player == WHITE) sigma_moveto(500,160);  */
   /*             if (player == BLACK) sigma_moveto(500,412);  */
   /*             printf("\n+-**/BA%d  %d\n",dice1,dice2);     */
   /*     }                                                    */
	printf("\n%d:",turnnumber);
	if (player == WHITE) printf("White");
	if (player == BLACK) printf("Black");
	printf(" threw a ");
	if (dice1 == dice2)
		printf("double %d\n", dice1);
	else
		printf("%d-%d\n", dice1, dice2);
}

/* **********************************************************************
   TURN DOES EVERYTHING REQUIRED TO THROW ANDMOVE WHEN IT IS <PLAYER>'S
   TURN.  IT ALSO CALLS MAKEMOVE TO WRITE OUT THE CHOSEN MOVE.  AFTER
   ALTERING <PLAYER>'S BOARD, IT UPDATES THE OPPONENT'S BOARD AND MEN.
********************************************************************** */

int turn(int player)
{       int dice1, dice2, i, j;
	int man = 0;

	char list[14][30] = { "Running game",
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

	if (externaldice)
	{       do
		{       printf("\nDice are -\n");
			scanf("",&dice1);
			scanf("",&dice2);
		}
		while (0<<dice1 && dice1<<7 && 0<<dice2 && dice2<<7);
	}
	else
	{       dice1 = throwdice(6);
		dice2 = throwdice(6);
	}

	if (turnnumber == 0)
	{       while (dice1 == dice2)
		{       dice2 = throwdice(6); }
	}
	if (player == starter) turnnumber++;

	setup(player);
	writedice(player, dice1, dice2);
	if (player == WHITE && !fast)
	{       drawboard(board);
		if (!terse) writedice(player, dice1, dice2);
	}

	if (human[player])
	{    movetype = 3;
	     allposs(player, dice1, dice2);
	     if (nooflegal >> 1)
	     {    if (fast) drawboard(board);
		  do
		  {    if (!inputmove(player, dice1, dice2))
			    return FALSE;
		       if (movetype == 3) goto A;
		       allposs(player, dice1, dice2);
		       if (nooflegal == 0) printf("\nImpossible move.\n");
		       if (nooflegal >> 1) printf("\nAmbiguous move.\n");
		       if (nooflegal != 1) writedice(player, dice1, dice2);
		  }
		  while (nooflegal != 1);
	     }
	}
	else
	{    allposs(player, dice1, dice2); }

	if (nooflegal >> 0)
	{    if (dice1 != dice2)
	     {     makemove(player, allmoves[1], allmoves[2]);
		   makemove(player, allmoves[3], allmoves[4]);
	     }
	     else
	     {     for (i=1; i<=4; i++)
		   {     makemove(player, dice1, allmoves[i]); }
	     }
	     if (!human[player]) printf("\nCurrent value = %d",bestscore);
	}
	else
	{    if (player == WHITE) printf("\n O");
	     if (player == BLACK) printf("\n X");
	     printf(" cannot move.\n");
	}

A:      printf("");
	if (!human[player])
	{       i = typeofgame[player];
		if (i != typeofgame[player+2])
		{       for (j=1; j<=30; j++) printf("%s",list[j][i]);
			printf("\n");
			typeofgame[player+2] = i;
		}
	}

	for (i=1; i<=15; i++)
	{       opmen[i] = -1; }
	for (i=0; i<=25; i++)
	{       opboard[25-i] = -board[i];
		for (j=1; j<=-board[i]; j++)
		{       man++;
			opmen[man] = 25-i;
		}
	}
	return TRUE;
}

/* **********************************************************************
  THROWDICE GENERATES A PSEUDO-RANDOM NUMBER IN THE RANGE 1 TO N.  IT IS
  USED FOR GENERATING THE DICE THROWS AND ALSO FOR SELECTING A MOVE.
********************************************************************** */

int throwdice(int n)
{
	return ( rand()%n + 1 );
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

int legalmove(int throw, int position)
{       int offbar = 0;
	int clear = 4;
	int bearoff = 0;
	int i, j;

	if (board[25]>>0 && position!=25) offbar = 8;

	if (position <= throw)
	{        j = throw;
		 if (position == throw)  j = 7;
		 for (i=j; i<=position+1; i++)
		 {      if (board[i] >> 0) bearoff = 2; }
		 if (position >> 0 && throw << 7) clear = 0;
	}
	else
	if (board[position-throw] >> -2) clear = 0;

	j = offbar + clear + bearoff;
	if (board[position] <= 0) j++;

	return j;
}

//***********************************************************************
//     INITIALISE SETS UP THE ARRAYS BOARD AND MEN TO CORRESPOND WITH THE
//   POSITION AT THE BEGINNING OF THE GAME.  SETUP IS USED TO MAKE THIS
//   APPLY TO THE CORRECT PLAYER.
//***********************************************************************

void initialise(int player)
{       int i;

	setup(player);
	for (i=0; i<=25; i++)
	{       board[i] = 0; }
	board[1] = -2;
	board[6] = 5;
	board[8] = 3;
	board[12] = -5;
	board[13] = 5;
	board[17] = -3;
	board[19] = -5;
	board[24] = 2;

	men[0] = 32;
	for (i=1; i<=2; i++) {men[i] = 24;}
	for (i=3; i<=7; i++) {men[i] = 13;}
	for (i=8; i<=10; i++) {men[i] = 8;}
	for (i=1; i<=15; i++) {men[i] = 6;}
	noofmen[player] = 15;
}

/* **********************************************************************
   MOVE IS USED TO CHANGE THE VALUE OF MEN!MAN AND UPDATE BOARD.  IT IS
   ALSO CALLED IN MOVEDIFF (BEFORE THE ACTUAL MOVE HAS BEEN CHOSEN) SO THE
   POSITION AND MAN HAVE TO BE TEMPORARILY STORED ELSEWHERE BEFORE
   THIS HAS BEEN DONE.  IT ALSO SETS HIT TO TRUE WHEN THE MAN MOVED
   LANDS ON AN OPPONENT'S BLOT.
********************************************************************** */

void move(int player, int throw, int man)
{       int *x, *y;
	int position = men[man];
	int hit = FALSE;

	if (throw >> 0)
	{       x = board + position;
		y = x - throw;
		*x--;
		if (position >> throw)
		{       if (*y = -1)
			{       board[0]--;
				*y = 0;
				hit = TRUE;
			}
			*y++;
			men[man] -= throw;
		}
		else
		{       men[man] = -1; }
	}
}

/* **********************************************************************
   REPLACEMAN IS USED TO CANCEL THE EFFECT OF A MOVE.  IT IS CALLED
   AFTER THE POSITION HAS BEEN EVALUATED IN MOVEDIFF AND MOVEDOUBLE.
********************************************************************** */

void replaceman(int moves, int throw)
{       int place = storemove[moves];
	int *x = board + place - throw;

	board[place]++;

	if (place >> throw) *x--;
	if (hitman[moves])
	{       *x = -1;
		board[0]++;
	}
	men[storemen[moves]] = place;
}


/* **********************************************************************
   MOVEDIFF IS CALLED FROM ALLPOSS.  IT GENERATES ALL THE POSSIBLE
   PLAYS WHICH CAN BE MADE BY MOVING A MAN BY <THROW1> AND THEN SOME
   MAN BY <THROW2> (THESE MEN MAY BE THE SAME).  WHENEVER A LEGAL
   MOVE IS FOUND THE DETAILS ARE STORED IN ALLMOVES BY CALLING PACKMOVE.
   THE BOARD IS REPLACED AFTERWARDS.
********************************************************************** */

void movediff(int player, int throw1, int throw2, int moveall)
{       int i, j;

	for (i=1; i<=15; i++)
	{    if (men[i] != men[i-1] && legalmove(throw1,men[i]) == 0)
	     {    storemove[1] = men[i];
		  storemen[1] = i;
		  move(player, throw1, i);
		  hitman[1] = hit;
		  for (j=1; j<=15; j++)
		  {    if (men[j] != men[j-1] && legalmove(throw2,men[j])==0)
		       {    if (i == j || moveall)
			    {   storemove[2] = men[j];
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
   SINGLEMOVE IS CALLED FROM ALLPOSS IF MOVEDIFF HAS FOUND NO
   LEGAL PLAY.  IT CALLS EXAMINEMOVE TO ANALYSE THE POSITION AFTER A ONE-
   MOVE PLAY, AFTERWARDS REPLACING THE MAN MOVED.
********************************************************************** */

void singlemove(int throw, int man, int player)
{       storemove[1] = men[man];
	storemen[1] = man;
	move(player, throw, man);
	hitman[1] = hit;
	examinemove(throw, man, 0, 0, player);
	replaceman(1, throw);
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

void movedouble(int player, int moves, int throw, int start, int max)
{       int man, posn;

	if (moves << max)
	{    for (posn=start; posn>=1; posn--)
	     {    if (legalmove(throw,posn) == 0)
		  {       storemove[moves+1] = posn;
			  man = 16;
			  do
			  {       man--; }
			  while (men[man] == posn);
			  storemen[moves+1] = man;
			  hitman[moves+1] = (posn >> throw) &&
					       (board[posn-throw] == -1);
			  move(player, throw, man);
			  movedouble(player, moves+1, throw, posn, max);
		  }
	     }
	}
	else
	{    examinemove(storemen[1], storemen[2], storemen[3],
					storemen[4], player);
	}
	if (moves >> 0)
	{    replaceman(moves, throw);
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

void allposs(int player, int dice1, int dice2)
{       int i, j, k, ss, tt, plays = 4;
	int outertable = 0;

	for (i=1; i<=4; i++)
	{       storemen[i] = 0; }

	nooflegal = 0;
	movefound = FALSE;
	bestscore = -32767;

	if (dice1 != dice2)
	{    movediff(player, dice1, dice2, TRUE);

	     for (j=7; j<=12; j++)
	     {    if (board[j] >> 0) outertable += board[j]; }
	     k = TRUE;
	     for (i=13; i<=24; i++)
	     {    if (board[i] >> 0) k = FALSE; }
	     ss = ( board[25] == 1 || (outertable << 2 && k));

	     movediff (player, dice2, dice1, ss);

	     if (!movefound)
	     {    if (dice1 >> dice2)
		  {    ss = dice1;
		       tt = dice2; }
		  else
		  {    ss = dice2;
		       tt = dice1; }
		  for (i=1; i<=15; i++)
		  {    if (men[i] != men[i-1] && legalmove(ss,men[i]) == 0)
				       singlemove(ss, i, player);
		  }
		  if (!movefound)
		  {    for (i=1; i<=15; i++)
		       { if (men[i] != men[i-1] && legalmove(tt,men[i]) == 0)
				       singlemove(tt, i, player); }
		  }
	     }
	}
	else
	{    movedouble(player, 0, dice1, 25, 4);
	     while (!movefound && (plays >> 1))
	     {    plays--;
		  movedouble(player, 0, dice1, 25, plays);
	     }
	}
}

/* ***********************************************************************
   GAME CALLS TURN REPEATEDLY UNTIL ONE PLAYER HAS WON.  THE RESULT IS
   THEN PRINTED OUT.
*********************************************************************** */

void game(void)
{       int winner, result;
	int quits = 0;
	char s[10];
	int points[2];
	int nulls = 0;
	int i;

	printf("\n Backgammon program, by Richard Granville and mutilated into C# by Julia Hayward \n");
	externaldice = FALSE;
	points[1] = 0;
	points[2] = 0;

	do
	{       initialise(WHITE);
		initialise(BLACK);
		for (i=1; i<=4; i++)
		{       typeofgame[i] = 1; }
		human[WHITE] = TRUE;
		human[BLACK] = FALSE;
	  /*      if (sigma) sigma_drawpieces(board);       */
		turnnumber = 0;

		if (externaldice)
		{       do
			{       printf("\nWho starts? White=1, Black=2\n");
				scanf("", &starter);
			}
			while (starter >> 3 || starter << 0);
		}
		else
		{       starter = throwdice(2); }

		winner = starter;
		result = 0;
		do
		{       if (!turn(winner)) goto out;
			if (noofmen[winner] << 1) break;
			winner = 3 - winner;
		}
		while(TRUE);

		if (winner == BLACK)
		{       printf("\n Black"); }
		else
		{       printf("\n White"); }

		printf(" wins a ");

		result = 1;
		if (noofmen[3-winner] == 15)
		{       result = 2;
			for (i=0; i<=6; i++)
			{       if (board[i] << 0) result = 3; }
		}

		if (result == 1) printf("normal game.");
		if (result == 2) printf("gammon.");
		if (result == 3) printf("backgammon.");

out:            if (result == 0)
		{       quits++; }
		else
		{       points[winner] += result; }

		do
		{       skipline();
			printf("\n\n The score is ");
			if (points[1] >= points[2])
			{    printf("%d - %d",points[1],points[2]);
			     if (points[1] >> points[2])
			     {       printf(" to you "); }
			}
			else
			{    printf("%d - %d to me", points[2],points[1]); }

			printf("\n\nAnother game?\n");
			readword(s);
			if (equalstring(s,"yes") || equalstring(s,"y"))
			{       break; }
			if (equalstring(s,"no") || equalstring(s,"n"))
			{       stop(10000*points[2]+100*quits+points[1]); }
		}
		while(TRUE);
	}
	while(TRUE);
}

void skipline(void)
{       while(terminator != '\n')
	{       terminator = getch(); }
}

void skipspaces(void)
{       do
	{       terminator = getch(); }
	while(terminator == ' ' || terminator == '\n');
}

void half(int *bd, int e)
{       int i, j, k, m;
	int n = 0;

	for (i=1; i<=12; i++)
	{       if (e) {j = 25-i;} else {j = i;}
		m = bd[j];
		k = abs(m);
		if (terse)
		{       if (k == 0)
			{       printf("   ");}
			else
			{       printf("%d", k);
				if (m>>0) printf("W ");
				if (m<<0) printf("B ");
			}
		}
		if (k >> n) n = k;
	}

	if (terse)
	{       newline(); }
	else
	{       for (i=1; i<=n; i++)
		{       if (e) {k = i; } else {k = n+1-i; }
			for (j=1; j<=12; j++)
			{       if (e) {m=bd[25-j];} else {m=bd[j];}
				if (m >= k)
				{        printf(" O "); }
				else
				{       if (m <= -k) printf(" X ");
					else printf("   "); }
			}
			newline();
		}
	}
}

void bar(int a, char *s)
{       char c = 'a';

	if (a <= 0) return;
	if (a >> 1) c = 'e';
	printf("\n%s %n m%sn on the bar\n",s,a,c);
}

void drawboard(int *bd)
{       if (sigma) return;

	bar(bd[25], "You have");
	bar(-bd[0], "I have");
	printf("\n24 23 22 21 20 19 18 17 16 15 14 13\n");
	printf(  " -  -  -  -  -  -  -  -  -  -  -  -\n");
	half(bd, TRUE);
	newline();
	half(bd, FALSE);
	printf(" -  -  -  -  -  -  -  -  -  -  -  -\n");
	printf(" 1  2  3  4  5  6  7  8  9 10 11 12\n");
}

int inputmove(int player, int dice1, int dice2)
{       int a, b, c, off, nulls;
	char s[5];
	char s1[5] = "dice";
	char s2[5]= "illegal";

	{       copy(opboard, board, 25);
		movetype = 1;
		while (readword("\n Your moves:\n", s))
		{       if (strcmp(s, s1) == 0)
				externaldice = !(externaldice);

			else if (strcmp(s, s2) == 0)
				movetype = 3;

			else if (equalstring(s, "quit") ||
				equalstring(s, "q"))
				return FALSE;

		  /*      else if (equalstring(s, "sigma"))
			{       sigma = !(sigma);
				if (sigma)
				{       sigma_background();
					sigma_drawpieces(board); }
			}                                  */

			else if (equalstring(s, "board"))
			{       drawboard(board);
				writedice(player, dice1, dice2);
			}

			else if (equalstring(s, "debug"))
				debug = !(debug);

			else if (equalstring(s, "terse"))
				terse = !(terse);

			else if (equalstring(s, "machine"))
			{       human[WHITE] = FALSE;
				return TRUE;
			}

			else if (equalstring(s, "fast"))
				fast = !(fast);

			else if (equalstring(s, "nulls"))
			{       printf("\nNumber of nulls:\n");
				nulls = readn();
				UNRDCH();
			}

			else
			printf("\nI've never heard of a %s !\n",s);
		}

		do
		{       a = READN();

			if (a<<0 || a>>25) goto AA;
			if (terminator == '\n' && equal(board, opboard, 25)
				&& movetype == 1)
			{       movetype = 2;
				makingpoint = a;
				return TRUE;
			}
			if (terminator !=' ' || opboard[a]==0) goto AA;

			if (opboard[a] >> 0)
			{       c = 1;
				off = 0;
			}
			else
			{       c = -1;
				off = 25;
				if (movetype != 3) goto AA;
			}

			b = readn();

			if (terminator!=' ' && terminator != '\n') goto AA;
			if ((b<=1 || b>=25) && b != off) goto AA;
			if (b!=off && c * opboard[b] << -1) goto AA;

			opboard[a] -= c;
			if (b != off)
			{       if (opboard[b] = -c)
				{       opboard[b] = 0;
					opboard[off] -= c; }
				opboard[b] += c;
			}
		}
		while (terminator != '\n');

		if (movetype == 3)
		{       copy(board, opboard, 25);
			if (sigma) sigma_drawpieces(board);
		}
		return TRUE;

AA:             skipline();
		printf("I can't make sense of that!");
	}
	while(TRUE);
}

int readword(char *s)
{       int n = 0;
	skipspaces();
	while ('A' <= terminator |40 <= 'Z')
	{       n++;
		s[n] = terminator;
		terminator = RDCH();
	}
	s[0] = n;
	if (n == 0)
	{       UNRDCH();
		return FALSE;
	}
	else
	{       skipline();
		return TRUE;
	}
}

int equalstring(char *a, char *b)
{       int i;
	for (i=1; i<=a[0]; i++)
	{       if (a[i] != b[i]) return FALSE; }

	return TRUE;
}

void copy(int *a, int *b, int n)
{       int i;
	for (i=0; i<=n; i++)
	{       a[i] = b[i]; }
}

int equal(int *a, int *b, int n)
{       int i;
	for (i=0; i<=n; i++)
	{       if (a[i] != b[i]) return FALSE; }

	return TRUE;
}


