#include "ReversiEnv.h"
#include "JPlayer.h"
#include "bitboard.h"
#include "position.h"
#include "search.h"
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <ctime>

#include "common.h"
#include "bitboard.hpp"
#include "minmax.h"
#include "mcts.h"
#include <string>
#include "timer.hpp"
#include "mc_minmax.h"
#include <climits>

// #include "net.hpp"
#include "iteration_minmax.h"
#include <iostream>

using namespace std;
ReversiEnv env;
Position pos;
JPlayer computer1;

// unsigned char Bitboard::cntOfByte[256] = {0};
// char Bitboard::roxanneWeights[64] = { 0 };
// unsigned char Bitboard::indices[64] = { 0 };
using namespace std;

//string id="";
//string player="";
//string qipan="";
void battle();

double evaluate_combine(Bitboard b); 

action human(Role player, Bitboard board) ;

action oldYearCal(){
    int action1 = computer1.action(env, 1);
    
    env.step(action1);
    pos.applyMove(action1);
    
    return encode_action(action1/8,action1%8);
    
}
void oldYearMov(action temp){
    if (temp==0) return;
    int i=0;
    
    while((temp>>i)!=1){
        i++;
    }
    int action1=i;
    env.step(action1);
    pos.applyMove(action1);
}

action p1_engine(Role player, Bitboard b, Timer t);

action p2_engine(Role player, Bitboard b, Timer t); 

action run(machine f,Role player, Bitboard board, Timer t);

/*int main(int argc, char **agrv)
{
	int i, j;
    srand(time(NULL));//for mcts
    
    clock_t bstart, bend, wstart, wend;
    clock_t p1time, p2time;
    int seconds = 10;
    int num = 1;
	{ //test1
		int win = 0, total = 100;
		for (int i = 1; i <= total; i++) {
			clock_t start = clock();
			Player player;
			player = rand() % 2 ? Black : White;
			if (player == Black)
				printf("computer1 is black.\n");
			else
				printf("computer1 is white.\n");
			while (!env.done) {
				//env.render(false);
				env.render();
				if (env.curr_player == player) {
                    //当前玩家下棋
					int action1, depth = 0, action2;
					//printf("computer1: ");
					action1 = computer1.action(env, 1);
					env.step(action1);
					pos.applyMove(action1);
					if (env.curr_player == player)
						pos.applyNullMove();
                    //在自己的棋盘上做标记
				}
				else {
                    //咱自己下棋
					int x, y;
					int action1, depth = 0, action2;
					printf("input the coordinates:\n");
					cin >> x >> y;//在他的棋盘上做标记
					x--, y--;
					action1 = (x << 3) + y;
					env.step(action1);
					pos.applyMove(action1);
					if (env.curr_player != player)
						pos.applyNullMove();
                    
				}
			}
			if (env.winner == black && player == Black
				|| env.winner == white && player == White) {
				win++;
				printf("epoch %d: computer1 wins\n", i);
			}
			else
				printf("epoch %d: computer2 wins\n", i);
			env.render(false);
			printf("epoch %d: winning rate = %.2f%%, time = %.2fs\n", i, 
				100.0 * win / i, 1.0 * (clock() - start) / CLOCKS_PER_SEC);
			printf("-----------------------------------------------------------------------\n");
			pos = Position();
			computer1.clear();
			env.reset();
		}
		printf("winning rate = %f%%\n", 100.0 * win / total);
	}
	system("pause");
	return 0;
}*/

void pkmain() {
    cout << "start" <<endl;
    
    srand(time(NULL));//for mcts
    
    Bitboard::init();
 
    clock_t bstart, bend, wstart, wend;
    clock_t p1time, p2time;
    int seconds = 5;//oldyear的时间
    int num = 2;
    while (num--) {
        p1time = 0;
        p2time = 0;
        
        Bitboard b(0x810000000, 0x1008000000);
        //b.printBoard();
        Role tplayer = BLACK;
        Role p1 = BLACK;
        //char* p1_name = "mcts_0";
        //char* p1_name = "minmax_6_combine";
		char* p1_name = "imm3";
        Role p2 = WHITE;
        // char* p2_name = "mcts_10000";
        char* p2_name = "human";
        printf("BLACK: %s\t WHITE: %s\t\n", p1 == BLACK ? p1_name : p2_name, p1 == WHITE ? p1_name : p2_name);
        int idx;
        uint64_t act;
        while (!b.hasEnded()) {
            b.printBoard();
            // printf("%s's turn\n", tplayer == p1 ? p1_name : p2_name);
            if (tplayer == p1) {
                bstart = clock();
                Timer t(seconds);
                act = run(p1_engine, tplayer, b, t);
                oldYearMov(act);
                bend = clock();
                p1time += bend - bstart;
            }
            else {
                wstart = clock();
                Timer t(seconds);
                //act = run(p2_engine, tplayer, b, t);
                //act = human(tplayer, b);
                act= oldYearCal();
                wend = clock();
                cout << t.getTimeLeft() << endl;
                p2time += wend - wstart;
            }
            if (act) {
                b.takeAction(tplayer, act);
                std::pair<int, int> ta = decode_action(act);
                // printf("%s do : %d %d\n", tplayer == p1 ? p1_name : p2_name, ta.first + 1, ta.second + 1);
            }
            tplayer = change_player(tplayer);
            
        }
        pos = Position();
        computer1.clear();
        env.reset();
        b.printBoard();
        std::pair<int, int> sc = b.getPieces();
        int bc = sc.first;
        int wc = sc.second;
        if (bc > wc) printf("BLACK WINS\n");
        else if (bc < wc) printf("WHITE WINS\n");
        else printf("TIE\n");
        printf("BLACK: %s\t WHITE: %s\t\n", p1 == BLACK ? p1_name : p2_name, p1 == WHITE ? p1_name : p2_name);
        printf("BLACK: %d\t WHITE: %d\t\n", bc, wc);
        printf("BLACK: %ld\t WHITE: %ld\t\n", p1 == BLACK? p1time: p2time, p1 == WHITE? p1time: p2time);
    }
    
    /*getchar();
     getchar();
     */
    
    num = 2;
    while (num--) {
        p1time = 0;
        p2time = 0;
        
        Bitboard b(0x810000000, 0x1008000000);
        b.printBoard();
        Role tplayer = BLACK;
        Role p2 = BLACK;
        char* p2_name = "human";
        Role p1 = WHITE;
        //char* p1_name = "mcts_0";
        char* p1_name = "minmax_6_combine";
        printf("BLACK: %s\t WHITE: %s\t\n", p1 == BLACK ? p1_name : p2_name, p1 == WHITE ? p1_name : p2_name);
        int idx;
        uint64_t act;
        while (!b.hasEnded()) {
            b.printBoard();
            //printf("%s's turn\n", tplayer == p1 ? p1_name : p2_name);
            if (tplayer == p1) {
                bstart = clock();
                Timer t(seconds);
                act = run(p1_engine, tplayer, b, t);
                oldYearMov(act);
                bend = clock();
                p1time += bend - bstart;
            }
            else {
                wstart = clock();
                Timer t(seconds);
                //act = human(tplayer, b);
                //act = run(p2_engine, tplayer, b, t);
                act= oldYearCal();
                wend = clock();
                p2time += wend - wstart;
            }
            if (act) {
                b.takeAction(tplayer, act);
                std::pair<int, int> ta = decode_action(act);
                //printf("%s do : %d %d\n", tplayer == p1 ? p1_name : p2_name, ta.first + 1, ta.second + 1);
            }
            tplayer = change_player(tplayer);
           
        }
        pos = Position();
        computer1.clear();
        env.reset();
        b.printBoard();
        std::pair<int, int> sc = b.getPieces();
        int bc = sc.first;
        int wc = sc.second;
        if (bc > wc) printf("BLACK WINS\n");
        else if (bc < wc) printf("WHITE WINS\n");
        else printf("TIE\n");
        printf("BLACK: %s\t WHITE: %s\t\n", p1 == BLACK ? p1_name : p2_name, p1 == WHITE ? p1_name : p2_name);
        printf("BLACK: %d\t WHITE: %d\t\n", bc, wc);
        printf("BLACK: %ld\t WHITE: %ld\t\n", p1 == BLACK? p1time: p2time, p1 == WHITE? p1time: p2time);
    }
    
}

