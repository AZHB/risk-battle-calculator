#include <iostream>
#include <vector>
#include <stdlib.h>
#include <algorithm>
#include <time.h>
using namespace std;

double risk(int n, int a, int d){
	int vicNum=0; //Record number of victories for final probability calculation
	for(int i=0;i<n;i++){
		int aCopy = a; //Running the simulation n times, so need to keep original values of a and d
		int dCopy = d;
		while(aCopy>0 && dCopy>0){
			//Calculate number of dice for this rolloff
			int aDice=aCopy;
			int dDice=dCopy;
			if (aDice>3){
				aDice=3;
			}
			if(dDice>2){
				dDice = 2;
			}
			//Generate random dice rolls
			vector<int> aRolls(aDice);
			vector<int> dRolls(dDice);
			for(vector<int>::iterator it=aRolls.begin();it!=aRolls.end();it++){
				*it = rand() % 6 + 1;
			}
			for(vector<int>::iterator it=dRolls.begin();it!=dRolls.end();it++){
				*it =rand() % 6 + 1;
			}
			sort(aRolls.begin(),aRolls.end());
			reverse(aRolls.begin(),aRolls.end());
			sort(dRolls.begin(),dRolls.end());
			reverse(dRolls.begin(),dRolls.end());
			//Calculate damage to attacker and defender
			int battles = min(aRolls.size(),dRolls.size());
			for(int i=0;i!=battles;i++){
				if(aRolls[i]>dRolls[i]){
					dCopy--;
				}
				else{
					aCopy--;
				}	
			}
		}
		if(aCopy>0){
			vicNum++;	
		}
	}
	return (double)(vicNum)/(double)(n);
}

int main(int argc, char** argv) {
	srand(time(NULL));
	int a; //Number of attackers
	int d; //Number of defenders
	int n; //Number of simulations
	cout << "Enter number of simulations: ";
	cin >> n;
	cout << endl;
	int round=1;
	
	//Constant loop, allowing for a different simulation each time based on user input
	while(1){
	cout << "========================BATTLE " << round << "========================" << endl;
	cout << "Enter number of attackers: ";
	cin >> a;
	cout << "Enter number of defenders: ";
	cin >> d;
	cout << endl << "Probability of victory is " << risk(n,a,d) << endl << endl;
	round++;
	}
	
	return 0;
}
