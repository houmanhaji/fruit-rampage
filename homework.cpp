#include<iostream>
#include<string>
#include<fstream>
#include<math.h>


using namespace std;
#define  BoardMax  26
#define FruitMax  10
#define CellMax 700

int findCluster(int i, int j, int n, char board[][BoardMax], int clusterElements[][2]);
int myTurn(int n, int p,int  alpha, int  beta, char board[][BoardMax]);
int opponentsTurn(int n, int p,int  alpha, int  beta,char board[][BoardMax]);
int report(int besti,int bestj,int n,char board[BoardMax][BoardMax]);


int turn = 0;
int maxturn =1;
int maxalpha = -1*CellMax*CellMax;
int main(){
ifstream fin("/input.txt");

//board size, fruit types
int n =0, p = 0;
//time
float t = 0;
char tmp[10];

fin >> n >> p >> t;
fin.getline(tmp,10);



char load[26];
char board[26][26];
//load the board
int i =0,j =0;
for (i=0;i <n; i++){
	fin >> load;
	//flip board for optimization when fruits drop
	for(j = 0; j < n;j++){
		board[j][i] = load[j];
	
	}
}
/*
for(i =0; i < n; i ++){
	for (j = 0 ; j  <n; j ++)
		cout << board[i][j] ;

	cout << endl;
}
*/
int alpha = -1*n*n*n*n, beta = n*n*n*n;

int cluster[CellMax][2];
int ans = myTurn(n,p,alpha, beta, board);

cout << ans << " done";

}



///////////////////////////////////////////////////////
int myTurn(int n,int p, int alpha, int beta, char board[][BoardMax]){
	
	//for all clusters calculate the new board
	
		//pass the new board to opponent turn
	    //calculate own utility which is cluster size + (rest - opponents turn max utility)
		 
	//return the one with max utility
int i= 0,j =0;
    turn ++;
    if(turn > maxturn)
        maxturn = turn;


	//basecase
int flag = 0;
for (i = 0; i < n ; i ++){
	for(j = 0; j < n; j ++){
		if(board[i][j] != '*')
			flag ++;
	}
		
}
    


if(!flag){
	turn --;
	return 0;
}

  

    
//the state space

bool choice[BoardMax][BoardMax];
int cluster[CellMax][2];

for (i = 0; i < n ; i ++){
	for(j = 0; j < n; j ++){
		choice[i][j]= 0;
        //test
    //   cout << board[i][j] ;
	}
 //   cout<< endl;
}
    int greedyMax=0,besti= 0, bestj =0;
	
    for (i = 0; i < n ; i ++){
        for(j = 0; j < n; j ++){
            //if not chosen this option yet
            if(choice[i][j] == 0 && board[i][j] != '*'){
                int clusterSize = findCluster(i,j,n,board,cluster);
                //    cout  << i << ' ' << j << ' '  << clusterSize <<  endl;
                
                
                int k;
                //mark all of the cluster seen
                for(k = 0; k < clusterSize; k++){
                    int iPrime = cluster[k][0];
                    int jPrime = cluster[k][1];
                    choice[iPrime][jPrime] = 1;
                }
                
                if (greedyMax < clusterSize*clusterSize ){
                    greedyMax = clusterSize*clusterSize;
                    besti = i;
                    bestj = j;
                }
                
            }
        }
    }
    
    
    if((greedyMax <=16 && turn >5)|| (turn>5 && alpha < 0)){
        turn--;
        return flag/2;
    }

//    cout << "greedy " << besti<<bestj<<" turn "<< turn<<endl;
    
    
    for (i = 0; i < n ; i ++){
        for(j = 0; j < n; j ++){
            choice[i][j]= 0;
            
        }
        
    }
    
    
    
    int virginnode = 1;
    
for (i = 0; i < n ; i ++){
	for(j = 0; j < n; j ++){
		//if not chosen this option yet
		if(choice[i][j] == 0 && board[i][j] != '*'){
			int clusterSize = findCluster(i,j,n,board,cluster);
		//	cout  << i << ' ' << j << ' '  << clusterSize <<  endl;

			
			int k;
			//mark all of the cluster seen
			for(k = 0; k < clusterSize; k++){
				int iPrime = cluster[k][0];
				int jPrime = cluster[k][1];
				choice[iPrime][jPrime] = 1;
			}
    

            //make a copy of the board , ripple back the choices to the top
            char updatedBoard[BoardMax][BoardMax];
            int a,b;
            for(a = 0; a < n ; a ++)
                for(b = 0; b < n; b ++)
                    updatedBoard[a][b] = board[a][b];
            for(a = 0; a < n ; a ++){
                for(b = 0; b < n; b ++){
                    int k =0;
                    for(k = 0; k < clusterSize; k++){
                        int iPrime = cluster[k][0];
                        int jPrime = cluster[k][1];
                        if(iPrime == a && jPrime == b){
                            updatedBoard[a][b] = '*';
                            //ripple back
                            int p = jPrime;
                            for(p= jPrime; p > 0; p --){
                                updatedBoard[a][p] = updatedBoard[a][p-1];
                                updatedBoard[a][p-1] = '*';
                            }
                        }
                    }
                }
            }
            //end of making the copy
            
            //if(turn < 10)
            //    cout << "alpha " << alpha << " turn " << turn <<endl;
			
            int oldalpha = alpha;
          //  if(clusterSize*clusterSize*2 < greedyMax)
            //    continue;
            if(alpha -greedyMax*greedyMax+  (5-min(4,turn))*clusterSize*clusterSize*clusterSize*clusterSize > maxalpha ){
                virginnode =0;
                alpha = max(alpha, opponentsTurn(n, p,alpha, beta, updatedBoard)+clusterSize*clusterSize );
                
            }
            if(maxalpha < alpha )
                maxalpha = alpha;
            if(oldalpha < alpha && turn ==1){
                besti =i;
                bestj=j;
                    
            }


            if(alpha >= beta){
                if(turn == 1){
                    report(besti,bestj,n,board);
                    return alpha;
                }
                turn--;
                return alpha;
            }
			//cout << "alpha " << alpha << " turn " << turn << " best i,j "<< besti<< " "<< bestj <<endl;
   
			
		}
	}
}
    if(turn == 1)
        report(besti,bestj,n,board);

    turn --;
    if(virginnode)
        return maxalpha/pow(2,turn+1);
    return alpha;
    
}

//





///////////////////////////////////////////////////////
int opponentsTurn(int n, int p, int  alpha, int  beta, char board[][BoardMax]){
	
	//for all clusters calculate the new board
	
		//pass the new board to opponent turn
	    //calculate own utility which is cluster size + (rest - opponents turn max utility)
		 
	//return the one with max utility
	turn ++;
int i= 0,j =0;


	
	//basecase
int flag = 0;
for (i = 0; i < n ; i ++){
	for(j = 0; j < n; j ++){
		if(board[i][j] != '*')
			flag ++;
	}
		
}
if(!flag ){
	turn --;
	return 0;
}

//the state space 
bool choice[BoardMax][BoardMax];
int cluster[CellMax][2];

   for (i = 0; i < n ; i ++){
        for(j = 0; j < n; j ++){
            choice[i][j]= 0;
            //test
            //   cout << board[i][j] ;
        }
        //   cout<< endl;
    }
    int greedyMax=0,besti= 0, bestj =0;
    
    for (i = 0; i < n ; i ++){
        for(j = 0; j < n; j ++){
            //if not chosen this option yet
            if(choice[i][j] == 0 && board[i][j] != '*'){
                int clusterSize = findCluster(i,j,n,board,cluster);
                //    cout  << i << ' ' << j << ' '  << clusterSize <<  endl;
                
                
                int k;
                //mark all of the cluster seen
                for(k = 0; k < clusterSize; k++){
                    int iPrime = cluster[k][0];
                    int jPrime = cluster[k][1];
                    choice[iPrime][jPrime] = 1;
                }
                
                if (greedyMax < clusterSize*clusterSize ){
                    greedyMax = clusterSize*clusterSize;
                    besti = i;
                    bestj = j;
                }
                
            }
        }
    }
    
    
    if(greedyMax <= 16 && turn >5){
        turn--;
        return flag/2;
    }
    
    

for (i = 0; i < n ; i ++){
	for(j = 0; j < n; j ++){
		choice[i][j]= 0;
        //test
  //  cout << board[i][j] ;
    }
//cout<< endl;
}
	

	
for (i = 0; i < n ; i ++){
	for(j = 0; j < n; j ++){
		//if not chosen this option yet
		if(choice[i][j] == 0 && board[i][j] != '*'){
			int clusterSize = findCluster(i,j,n,board,cluster);
			//cout  << i << ' ' << j << ' '  << clusterSize <<  endl;

			
			int k;
			//mark all of the cluster seen
			for(k = 0; k < clusterSize; k++){
				int iPrime = cluster[k][0];
				int jPrime = cluster[k][1];
				choice[iPrime][jPrime] = 1;
			}
						
			//make a copy of the board , ripple back the choices to the top
			char updatedBoard[BoardMax][BoardMax];
			int a,b;
            for(a = 0; a < n ; a ++)
                for(b = 0; b < n; b ++)
                    updatedBoard[a][b] = board[a][b];
			for(a = 0; a < n ; a ++){
				for(b = 0; b < n; b ++){
					int k =0;
					for(k = 0; k < clusterSize; k++){
						int iPrime = cluster[k][0];
						int jPrime = cluster[k][1];
						if(iPrime == a && jPrime == b){
							updatedBoard[a][b] = '*';
							//ripple back
							int p = jPrime;
							for(p= jPrime; p > 0; p --){
								updatedBoard[a][p] = updatedBoard[a][p-1];
								updatedBoard[a][p-1] = '*';
							}
						}
					}
				}
			}
			//end of making the copy

            //if(turn < 10)
              //  cout << "beta " << beta <<  endl;
      
            
            int oldbeta = beta;
         //   if(clusterSize*clusterSize> alpha)
//continue;
            beta = min(oldbeta, myTurn(n, p, alpha, beta, updatedBoard)+clusterSize*clusterSize);

			if (beta <= alpha ){
                
			//	cout << "beta " << beta << endl;
				turn --;
				return alpha;
				
            }
   
			
		}
		
	}	
}
turn --;
return beta;	
}



/////////////////////////////////////////////////////////////////////////////////////////
int findCluster(int x, int y, int n,  char board[][BoardMax], int clusterElements[][2]){
	int index =0;	
	clusterElements[index][0] = x;
	clusterElements[index][1] = y;

	
	int checkedCellIndex = 0;
	char clusterType = board[x][y]; 

	
	while(checkedCellIndex <= index){
		int jN, iN, jS,iS, jW,iW, jE,iE;
		
		jN =clusterElements[checkedCellIndex][1] -1;
		iN = clusterElements[checkedCellIndex][0]; 
		
		jS = clusterElements[checkedCellIndex][1] +1;
		iS= iN;
		
		jW = clusterElements[checkedCellIndex][1];
		iW = clusterElements[checkedCellIndex][0] -1;
		
		jE = jW;
		iE = clusterElements[checkedCellIndex][0] +1;

		
		

		int i;
		
		//by default check all neighbors
		int N = 1, S = 1, W = 1, E =1;
		for(i=0;i<=index; i++){
			if( jN == clusterElements[i][1] && iN == clusterElements[i][0]){
				//weve seen this
				N =0;
			}
			else if(jS == clusterElements[i][1] && iS == clusterElements[i][0]){
				S =0;
			}
			else if(jW == clusterElements[i][1] && iW == clusterElements[i][0]){
				W =0;
			}
			else if(jE == clusterElements[i][1] && iE == clusterElements[i][0]){
				E = 0;
			}
		}	
		
		

		//check North

		if( N && jN >= 0 && board[iN][jN] == clusterType){

			index ++;
			clusterElements[index][0] = iN;
			clusterElements[index][1] = jN;
		}

		//check South
		if(S && jS < n )
		if( board[iS][jS] == clusterType){
			index ++;
			clusterElements[index][0] = iS;
			clusterElements[index][1] = jS;
		
		}
		
		//check WEST

		if(W && iW >= 0 && board[iW][jW] == clusterType){
			index ++;
			clusterElements[index][0] = iW;
			clusterElements[index][1] = jW;
		}

		//check east
		if(E && iE < n && board[iE][jE] == clusterType){
			index ++;
			clusterElements[index][0] = iE;
			clusterElements[index][1] = jE;
		}
		
		
		checkedCellIndex ++;
	}
	
return index+1;
	
}


int report(int besti,int bestj,int n,char board[BoardMax][BoardMax]){
    int cluster[CellMax][2];
    
    int clusterSize = findCluster(besti,bestj,n,board,cluster);
    cout  <<"here "<< besti << ' ' << bestj << ' '  << clusterSize <<  endl;
    
    
    
    
    //make a copy of the board , ripple back the choices to the top
    char updatedBoard[BoardMax][BoardMax];
    int i,j;
    for(i = 0; i < n ; i ++)
        for(j = 0; j < n; j ++)
            updatedBoard[i][j] = board[i][j];
    for(i = 0; i < n ; i ++){
        for(j = 0; j < n; j ++){
            int k =0;
            for(k = 0; k < clusterSize; k++){
                int iPrime = cluster[k][0];
                int jPrime = cluster[k][1];
                if(iPrime == i && jPrime == j){
                    updatedBoard[i][j] = '*';
                    //ripple back
                    int p = jPrime;
                    for(p= jPrime; p > 0; p --){
                        updatedBoard[i][p] = updatedBoard[i][p-1];
                        updatedBoard[i][p-1] = '*';
                    }
                }
            }
        }
    }
    
    ofstream fout("/output.txt");
    fout << char('A' + besti) << (bestj+1) << endl;
    for(i = 0; i < n ; i ++){
        for(j = 0; j < n; j ++){
            fout << updatedBoard[j][i];
        }
        fout << endl;
    }
    return 0;
}

