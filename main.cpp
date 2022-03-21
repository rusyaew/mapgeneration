#include <bits/stdc++.h>
using namespace std;

//Globals
/*
int CANVAS_WIDTH = 800;	
int CANVAS_HEIGHT = 600;
*/
int CANVAS_WIDTH = 100;
int CANVAS_HEIGHT = 500;
int PIXEL_WIDTH = 4;	//specify how big you want each pixel.
int PWIDTH = ceil(CANVAS_WIDTH/PIXEL_WIDTH);
int PHEIGHT = ceil(CANVAS_HEIGHT/PIXEL_WIDTH);

vector<vector<int>> WORLD_A;
vector<vector<int>> WORLD_B;

vector<int> RULE_SURVIVE;
vector<int> RULE_BIRTH;
int RULE_GENS;
int MAX_GENS = 20;
int FOOD_PERCENT = 12;



void Setup() {
    WORLD_A = vector<vector<int>> (PWIDTH, vector<int> (PHEIGHT));
    WORLD_B = vector<vector<int>> (PWIDTH, vector<int> (PHEIGHT));

	for(int x=0; x<PWIDTH; x++) {
		for(int y=0; y<PHEIGHT; y++) {
			if ( rand() % 2)
			    WORLD_B[x][y]=1;
		}
	}
}

int calcNeighbors(int x,int y) {
	int totalSum = 0;
	int xLeft = (x + PWIDTH - 1) % PWIDTH;
	int xRight = (x + 1) % PWIDTH;
	int yTop = (y + PHEIGHT -1) % PHEIGHT;
	int yBottom = (y + 1) % PHEIGHT;
	
	if (WORLD_A[xLeft][yTop] == 1) totalSum++;
	if (WORLD_A[x][yTop] ==1 ) totalSum++;
	if (WORLD_A[xRight][yTop] ==1 ) totalSum++;
	if (WORLD_A[xLeft][y] ==1 ) totalSum++;
	if (WORLD_A[xRight][y] ==1 ) totalSum++;
	if (WORLD_A[xLeft][yBottom] ==1 ) totalSum++;
	if (WORLD_A[x][yBottom] ==1 ) totalSum++;
	if (WORLD_A[xRight][yBottom] ==1 ) totalSum++;

	return totalSum;
}

void ApplyRule(vector<int> surviveList, vector<int> birthList, int generations) {
    RULE_GENS = min(MAX_GENS, generations);
    RULE_SURVIVE = {};
    RULE_BIRTH = {};
    //Apply rules to global variables.
	for(int i=0; i < size(surviveList); i++) {
		RULE_SURVIVE.push_back(surviveList[i]);
	}

	for(int i=0; i < size(birthList); i++) {
		RULE_BIRTH.push_back(birthList[i]);
	}
    if (RULE_GENS == 2) {
            for(int x=0; x < PWIDTH; x++) {
                for(int y=0; y < PHEIGHT; y++) {
                    if (WORLD_B[x][y] > 1) {
                        WORLD_B[x][y] = 0;
                    }
                }
            }
    }
}

bool ruleContains(int n, vector<int> rule) {
	for(int i=0; i < size(rule); i++) {
		if ( rule[i] == n )
			return true;
	}
	return false;
}

void clearWorld() {
	for(int x=0; x < PWIDTH; x++) {
		for(int y=0; y < PHEIGHT; y++) {
			WORLD_B[x][y] = 0;
		}
	}
}

void CalcNextgen() {
	for(int x=0; x < PWIDTH; x++) {
	   for(int y=0; y < PHEIGHT; y++) {
		int currentState = WORLD_B[x][y];
		WORLD_A[x][y] = currentState;
		WORLD_B[x][y] = 0;
	   }
	}


    for(int x=0; x < PWIDTH; x++) {
            for(int y=0; y < PHEIGHT; y++) {
                int currentState = WORLD_A[x][y];

                if ( currentState == 0) {
                    int neighborsOn = calcNeighbors(x,y);
                    if ( ruleContains(neighborsOn, RULE_BIRTH))
                        WORLD_B[x][y] = 1;
                }
                else if ( currentState > 0 && (currentState < (RULE_GENS - 1) || RULE_GENS == 2) ) {
                    
                    int neighborsOn = (size(RULE_SURVIVE) == 0) ? 0 : calcNeighbors(x,y);
                    bool shouldSurvive = ruleContains(neighborsOn, RULE_SURVIVE);
                    if (currentState == 1 && shouldSurvive) 
                    {
                        WORLD_B[x][y] = currentState;
                    }
                    else if (!shouldSurvive) {
                            WORLD_B[x][y] = (currentState + 1) % RULE_GENS;
                    }

                    if ( currentState > 1)
                        WORLD_B[x][y] = currentState + 1;
                }
                else if (currentState >= (RULE_GENS - 1)) {
                    WORLD_B[x][y] = 0;
                }
	    }
	}
}

void OutputDemonstrate() {
    for (int x = 0; x < PWIDTH; x++) {
        for (int y = 0; y < PHEIGHT; y++) {
            if (WORLD_B[x][y]) {
                cout << 1;
            } else { 
                if (rand() % 100 <= FOOD_PERCENT) {
                    cout << 'F';
                } else {
                    cout << 0;
                }
            }
        }
        cout << "\n";
    }
}

int main() {
    //test any rules: https://robinforest.net/post/cellular-automata/
    //specify them here:
    // Caves: ApplyRule({4,5,6,7,8},{2,5,6,7,8},10);
    // Ants: ApplyRule({4,5,6,7,8},{5,6,7,8},3);
    ApplyRule({4,5,6,7,8},{5,6,7,8},3);
    cout << "[+] Rule was set.\n";
    int iterations = 0;
    cout << "[?] Enter amount of iterations: ";
    cin >> iterations;
    Setup();
    for (int i = 0; i < iterations; i++) {
       CalcNextgen();
    }
    OutputDemonstrate();
    return 0;
}
