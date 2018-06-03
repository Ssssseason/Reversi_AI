#ifndef COMMON_H
#define COMMON_H
#include "bitboard.hpp"
#include <algorithm>
typedef uint64_t action;

inline action encode_action(int x, int y){
    return ((uint64_t)1)<<(x*8+y);
}

inline std::pair<int,int> decode_action(action ac){
	int n;
	for (int i = 0; i < 64; i++) {
		if (ac&(((uint64_t)1) << i)) {
			n = i; break;
		}
	}
    return std::make_pair(n/8,n%8);
}

inline Role change_player(Role p){
    return p==BLACK?WHITE:BLACK;
}

typedef action (*machine)(Role player,Bitboard board);
typedef double(*eval)(Bitboard);
#define infinity 999999
#endif