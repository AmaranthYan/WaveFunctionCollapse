#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <assert.h>
#include <vector>
#include <intrin.h>

#ifdef _DEBUG
#define DEBUG_OUTPUT(w) OutputBoard((w), false)
#else
#define DEBUG_OUTPUT(w) ((void)0)
#endif // DEBUG

// Sudoku solution inspired by WaveFunctionCollapse

using namespace std;

typedef unsigned char u8;
typedef unsigned short u16;

u16 wave[81]; // digit bit flags from 1 to 9 for each cell

// entropy map, [K = entropy, V = idx array]
// entropy = nb of possible digits - 2
// discard all cell indices with entropy -1
vector<u8> entropy[8];

int solution_count = 0;

int CalculateEntropy(int w);
void DecreaseEntropy(vector<u8>* e_map, int idx, int e0, int e1);

bool CollapseWaveState(u16* w_state, vector<u8>* e_map, int idx, u16 d_flag);
bool ReduceWaveState(u16* w_state, vector<u8>* e_map, int idx, u16 d_flag);
bool Propagate(u16* w_state, vector<u8>* e_map, int idx, u16 flag);

void OutputBoard(u16* w_state, bool to_digit)
{
	int width = to_digit ? 2 : 5;

	for (int i = 0; i < 81; i++)
	{
		unsigned long w = w_state[i];
		if (to_digit)		
			_BitScanForward(&w, w << 1);

		cout << right << setw(width) << w;

		if ((i + 1) % 9 != 0)
			cout << " ";
		else
			cout << endl;
	}

	cout << endl;
}

int BinarySearchEqualOrGreater(const vector<u8>& vec, u8 val)
{
	int start = 0, end = vec.size();
	
	while (start < end)
	{
		int mid = (start + end) / 2;

		if (vec[mid] == val)
			return mid;
		else if (vec[mid] < val)
			start = mid + 1;
		else
			end = mid;
	}

	return end;
}

void DecreaseEntropy(vector<u8>* e_map, int idx, int e0, int e1)
{
	assert(e0 >= 0 && e0 > e1);

	int i = BinarySearchEqualOrGreater(e_map[e0], idx);
	e_map[e0].erase(e_map[e0].begin() + i);

	// cell has no entropy when it is certain
	// entropy will never increase, so it's safe to discard it
	if (e1 >= 0)
	{
		i = BinarySearchEqualOrGreater(e_map[e1], idx);
		e_map[e1].insert(e_map[e1].begin() + i, idx);
	}
}

// nb of set bit - 2
int CalculateEntropy(int w)
{
	int e = 0;
	while (w > 0)
	{
		w &= w - 1;
		e++;
	}
	return e - 2;
}

// remove possiblity of one digit from cell
bool ReduceWaveState(u16* w_state, vector<u8>* e_map, int idx, u16 d_flag)
{
	bool reduced = w_state[idx] & d_flag;
	w_state[idx] &= ~d_flag;

	// 0 possible digit, invalid board states
	if (w_state[idx])
	{
		if (reduced)
		{
			// compute entropy after reduction
			int e = CalculateEntropy(w_state[idx]);
			DecreaseEntropy(e_map, idx, e + 1, e);

			// propagate when possibility is reduced to only 1 digit
			if (e == -1)
			{
				return Propagate(w_state, e_map, idx, w_state[idx]);
			}
		}
		return true;
	}
	return false;
}

// propagate wave state change to other cells
bool Propagate(u16* w_state, vector<u8>* e_map, int idx, u16 d_flag)
{
	for (int i = 0; i < 9; i++)
	{
		int ln = idx / 9;
		int col = idx % 9;

		int sq_ln = ln / 3;
		int sq_col = col / 3;

		int cl[3] =
		{
			ln * 9 + i, // cell idx in same line
			i * 9 + col, // cell idx in same colomn
			(sq_ln * 3 + i / 3) * 9 + sq_col * 3 + i % 3 // cell idx in same square
		};

		for (int j = 0; j < 3; j++)
		{
			if (cl[j] == idx)
				continue;

			// can't reduce, invalid board states
			if (!ReduceWaveState(w_state, e_map, cl[j], d_flag))
				return false;
		}
	}
	return true;
}

// remove possibilities of all but one digit from cell
bool CollapseWaveState(u16* w_state, vector<u8>* e_map, int idx, u16 d_flag)
{
	assert(idx >= 0 && idx < 81);

	if (w_state[idx] & d_flag)
	{
		int e = CalculateEntropy(w_state[idx]);
		w_state[idx] = d_flag;
		DecreaseEntropy(e_map, idx, e, -1);

		return Propagate(w_state, e_map, idx, d_flag);
	}
	return false;
}

void SolveSudokuWaveStates(u16* wave_states, vector<u8>* entropy, int idx, u16 state)
{
	// copy entropy map
	vector<u8> e[8];
	for (int i = 0; i < 8; i++)
	{
		if (entropy[i].size() > 0)
		{
			e[i].assign(entropy[i].begin(), entropy[i].end());
		}
	}

	// copy wave states
	u16 w[81];
	memcpy(w, wave_states, 81 * sizeof(u16));

	if (idx != -1)
	{
		if (!CollapseWaveState(w, e, idx, state))
		{
			DEBUG_OUTPUT(w);
			return;
		}

		DEBUG_OUTPUT(w);
	}

	while (true)
	{
		// find cell with lowest entropy
		int cell = -1;
		for (int i = 0; i < 8; i++)
		{
			if (e[i].size() > 0)
			{
				cell = e[i][0];
				break;
			}
		}

		// no more cell with entropy, solution found
		if (cell == -1)
		{
			cout << "solution " << ++solution_count << endl;
			OutputBoard(w, true);
			return;
		}

		// try digits
		while (true)
		{
			int remains = w[cell] & (w[cell] - 1);
			if (remains == 0)
				break;

			u16 flag = w[cell] ^ remains;
			SolveSudokuWaveStates(w, e, cell, flag);

			if (!ReduceWaveState(w, e, cell, flag))
			{
				DEBUG_OUTPUT(w);
				return;
			}

			DEBUG_OUTPUT(w);
		}
	}
}

int main()
{
	// init each cell with max wave states (0x1ff)
	// and with max entropy (7)	
	for (int i = 0; i < 81; i++)
	{
		wave[i] = 0x1ff;
		entropy[7].push_back(i);
	}

	chrono::high_resolution_clock::time_point start;
	chrono::duration<double> delta;

	start = chrono::high_resolution_clock::now();

	// read and initialize wave states of the board
	{
		ifstream ifs;
		ifs.open("input.csv");

		string line;

		int k = 0;
		while (getline(ifs, line))
		{
			std::stringstream ss(line);
			string element;
			while (getline(ss, element, ','))
			{
				int digit = stoi(element);

				if (digit > 0)
				{
					u16 bit = 1 << (digit - 1);
					if (!CollapseWaveState(wave, entropy, k, bit))
					{
						cout << "--------------------no solution--------------------" << endl;

						system("pause");
						return 1;
					}
				}

				k++;
				if (k >= 81) goto out;
			}
		}
	}
out:
	for (auto vec : entropy)
	{
		vec.shrink_to_fit();
	}

	DEBUG_OUTPUT(wave);

	delta = chrono::duration_cast<chrono::duration<double>>(chrono::high_resolution_clock::now() - start);
	cout << delta.count() << endl;

	cout << "--------------------wave states initialized--------------------" << endl;

	start = chrono::high_resolution_clock::now();

	// find solutions by collapsing wave states
	SolveSudokuWaveStates(wave, entropy, -1, 0);

	delta = chrono::duration_cast<chrono::duration<double>>(chrono::high_resolution_clock::now() - start);
	cout << delta.count() << endl;

	system("pause");
	return 0;
}