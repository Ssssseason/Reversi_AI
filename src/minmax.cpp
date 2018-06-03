#include <vector>
#include "minmax.h"

static double alphabeta(Role player, double alpha, double beta, int depth, Bitboard board, action ac) {
	board.takeAction(change_player(player), ac);
	if (board.hasEnded()) {
		Role wins = board.getScore(BLACK) > board.getScore(WHITE) ? BLACK : WHITE;
		if (wins == BLACK) {
			if (player == BLACK) return infinity;
			else return -infinity;
		}
		else {
			if (player == WHITE) return -infinity;
			else return infinity;
		}
	}
	action actions = board.getActions(player);
	if (actions == 0) {
		return board.evaluate();
	}
	depth -= 1;
	if (depth <= 0) return board.evaluate();
	if (player == BLACK) {
		for (int i = 0; i < 64; i++) {
			action act = actions & (((uint64_t)1) << i);
			if (act) {
				alpha = std::max(alpha, alphabeta(change_player(player), alpha, beta, depth, board, act));
				if (alpha > beta) break;
			}
		}
		return alpha;
	}
	else {
		for (int i = 0; i < 64; i++) {
			action act = actions & (((uint16_t)1) << i);
			if (act) {
				beta = std::min(beta, alphabeta(change_player(player), alpha, beta, depth, board, act));
				if (beta < alpha) break;
			}
		}
		return beta;
	}
}

action minmax(Role player, Bitboard board, int depth) {
	action actions = board.getActions(player);
	if (actions == 0) return 0;
	double alpha = -infinity;
	double beta = infinity;
	//int depth = DEPTH;
	action res = 0;
	if (player == BLACK) {
		for (int i = 0; i < 64; i++) {
			action act = actions & (((uint64_t)1) << i);
			if (act) {
				double val = alphabeta(change_player(player), alpha, beta, depth, board, act);
				if (val > alpha || val == alpha && (res == 0 || Bitboard::getRoxannePriority(act) < Bitboard::getRoxannePriority(res))) {
					alpha = val;
					res = act;
				}
			}
		}
	}
	else {
		for (int i = 0; i < 64; i++) {
			action act = actions & (((uint64_t)1) << i);
			if (act) {
				double val = alphabeta(change_player(player), alpha, beta, depth, board, act);
				if (val < beta || val == beta && (res == 0 ||  Bitboard::getRoxannePriority(act) < Bitboard::getRoxannePriority(res))) {
					beta = val;
					res = act;
				}
			}
		}
	}
	assert(res);
	return res;
}