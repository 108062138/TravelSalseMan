#include <iostream>
#include <cstring>
#include <vector>
#include <queue>
#include <climits>
#include <algorithm>
#include <set>
#include <unordered_set>
#include <utility>
#include <ctime>

using namespace std;

bool have[100];
int** M;
int* recM;

class node {
public:
	node() {
		level = 0;
		bound = 0;
		curSize = 0;
		for (int i = 0; i < 35; i++)
			simPath[i] = -1;
	}
	~node() {
	}
	int level;
	int simPath[35];
	int curSize;
	int bound;
	int size() {
		return curSize;
	}
	node(const node& p1) {
		level = p1.level;
		bound = p1.bound;
		curSize = p1.curSize;
		for (int i = 0; i < 35; i++)
			simPath[i] = p1.simPath[i];
	}
	void push_back(int city) {
		simPath[curSize] = city;
		curSize++;
	}
};
bool operator<(const node& p1, const node& p2) {
	return (p1.level < p2.level) || (p1.level == p2.level && p1.bound > p2.bound);
}

bool findTarget(node& v, int target) {
	for (int i = 0; i < v.curSize; i++)
		if (target == v.simPath[i])
			return true;
	return false;
}

int bound(node& v, int& n, bool& showB) {
	int total = 0;
	int curMin;
	int i, j, from, to;
	if (showB) {
		cout << "Path: ";
		for (int i = 0; i < v.curSize; i++)
			cout << v.simPath[i] << " ";
		cout << endl;
	}

	if (v.curSize <= 1) {
		for (i = 1; i <= n; i++) {
			curMin = INT_MAX;
			for (j = 1; j <= n; j++)
				if (M[i][j] != 0)
					if (M[i][j] < curMin)
						curMin = M[i][j];
			total += curMin;
		}
	}
	else {
		int curLast;
		for (i = 1; i < v.curSize; i++) {
			from = v.simPath[i - 1];
			to = v.simPath[i];
			total += M[from][to];
			if (i == v.curSize - 1)
				curLast = to;
		}
		if (showB) {
			cout << "it's given value: " << total << endl;
			cout << "it's min: " << " ";
		}



		for (from = 1; from <= n; from++) {
			if (!findTarget(v, from)) {
				if (from == 1)continue;
				if (from == curLast) {
					curMin = INT_MAX;
					for (to = 1; to <= n; to++) {
						if (to == 1)continue;
						if (M[from][to] != 0 && M[from][to] < curMin)
								curMin = M[from][to];
						if (curMin == recM[from])
							break;
					}
					if(showB)
						cout << " curLast->" << curMin << "<- ";
					total += curMin;
				}else{
					curMin = INT_MAX;
					for (to = 1; to <= n; to++) {
						if (M[from][to] != 0 && M[from][to] < curMin) 
							curMin = M[from][to];
						
					}
					if(showB)
						cout << curMin << " ";
					total += curMin;
				}
			}
			else if (from == curLast) {
				curMin = INT_MAX;
				for (to = 1; to <= n; to++) {
					if (to == 1)continue;
					if (M[from][to] != 0 && M[from][to] < curMin)
						curMin = M[from][to];
					if (curMin == recM[from])
						break;
				}
				if(showB)
					cout << " curLast->" << curMin << "<- ";
				total += curMin;
			}
		}
		if(showB)
			cout << endl;
	}
	if(showB)
		cout << "v bound: " << total << endl;
	return total;
}

int length(node& v) {
	int res = 0;
	int to, from;
	for (int i = 1; i < v.size(); i++) {
		from = v.simPath[i - 1];
		to = v.simPath[i];
		res += M[from][to];
	}

	res += M[to][1];
	return res;
}

void travel2(int n, int* opttour, int& minlength,bool showB) {
	priority_queue<node> PQ;
	node v,u;
	v.level = 0;
	v.push_back(1);
	v.bound = bound(v, n, showB);
	minlength = INT_MAX;
	PQ.push(v);

	while (!PQ.empty()) {
		auto v = PQ.top();//remove the item with the best bound 
		PQ.pop();
		
		if (v.bound < minlength) {
			u.level = v.level + 1;//set u to a child of v
			for (int i = 2; i <= n; i++) {
				auto it = findTarget(v, i);
				if (it == false) {
					memcpy(u.simPath, v.simPath, v.curSize * sizeof(int));
					u.curSize = v.curSize;
					u.push_back(i);

					if (u.level == n - 2) {
						int theLastCity=87;
						for (int t = 1; t <= n; t++)
							have[t] = false;
						for (int t = 0; t < u.curSize; t++)
							have[u.simPath[t]] = true;
						for (int t = 1; t <= n; t++) {
							if (have[t] == false) {
								theLastCity = t;
								break;
							}
						}
						u.push_back(theLastCity);
						u.push_back(1);
						int uLen = length(u);
						if (uLen < minlength) {
							minlength = uLen;
							for(int i=0;i<=n;i++)
								if(u.simPath[i]>0)
									opttour[i] = u.simPath[i];
						}
					}
					else {
						u.bound = bound(u, n, showB);
						if (u.bound < minlength)
							PQ.push(u);
					}
				}
				else
					continue;
			}
		}
	}
}

int main() {
	int N;
	cin >> N;
	for (int i = 1; i <= N; i++)
		have[i] = false;
	M = new int* [N + 5];
	recM = new int[N + 5];
	for (int i = 0; i < N + 5; i++) {
		recM[i] = INT_MAX;
		M[i] = new int[N + 5];
	}
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			cin >> M[i][j];
			M[i][j] = (M[i][j] <= 0) ? 0 : M[i][j];
			if (M[i][j] < recM[i] && M[i][j] != 0)
				recM[i] = M[i][j];
		}
	}



	//vector<int> opttour;
	int opttour[35];
	int minlength;
	clock_t tStart = clock();
	
	bool showB = false;

	travel2(N, opttour, minlength, showB);
	//cout << "Time taken: " << (double)(clock() - tStart) / CLOCKS_PER_SEC << endl;
	cout << minlength << endl;
	for(int i=0;i<N;i++)
		cout<<opttour[i]<<" ";
	cout << endl;

	delete[]M;
	delete recM;

	return 0;
}