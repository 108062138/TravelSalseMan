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
#define MSZ 31
#define DEADEND 0
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
		usedCity = 0;
		tail = 1;
		for (int i = 0; i < MSZ; i++){
			simPath[i] = char(0);
		}
	}
	~node() {
	}
	int level;
	char simPath[MSZ];
	unsigned int usedCity;
	int curSize;
	int bound;
	int tail;
	int size() {
		return curSize;
	}
	node(const node& p1) {
		level = p1.level;
		bound = p1.bound;
		curSize = p1.curSize;
		usedCity = p1.usedCity;
		tail = 1;
		for (int i = 0; i < MSZ; i++){
			simPath[i] = p1.simPath[i];
		}
	}
	void push_back(int city) {
		simPath[curSize] = city;
		tail = city;
		curSize++;
		usedCity += 1<<city;
	}
};
bool operator<(const node& p1, const node& p2) {
	return (p1.level < p2.level) || (p1.level == p2.level && p1.bound > p2.bound);
}

bool findTarget(node& v, int target) {
	return v.usedCity & 1<< target;
}

int bound(node& v, int& n, bool& showB) {
	int total = 0;
	int curMin;
	int i, j, from, to;
	if (showB) {
		cout << "Path: ";
		for (int i = 0; i < v.curSize; i++)
			cout << int(v.simPath[i]) << " ";
		cout << endl;
	}

	if (v.curSize <= 1) {
		for (i = 1; i <= n; i++) {
			curMin = INT_MAX;
			for (j = 1; j <= n; j++)
				if (M[i][j] != DEADEND)
					if (M[i][j] < curMin)
						curMin = M[i][j];
			total += curMin;
		}
	}
	else {
		int curLast;
		for (i = 1; i < v.curSize; i++) {
			from = int(v.simPath[i - 1]);
			to = int(v.simPath[i]);
			total += M[from][to];
			if(M[from][to]==DEADEND){
				if(showB)
					cout<<"fkupgfsdkjgdsaigjfdasifjhasdiofjoasdijfadsiljfasdeijf"<<endl;
				return DEADEND;
			}
			if (i == v.curSize - 1)
				curLast = to;
		}
		if (showB) {
			cout << "it's given value: " << total << endl;
			cout << "it's min: " << " ";
		}



		for (from = 1; from <= n; from++) {
			if (!(v.usedCity & 1 << from)) {
				if (from == 1)continue;
				if (from == curLast) {
					curMin = INT_MAX;
					for (to = 1; to <= n; to++) {
						if (to == 1)continue;
						if (M[from][to] != DEADEND && M[from][to] < curMin)
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
						if (M[from][to] != DEADEND && M[from][to] < curMin) 
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
					if (M[from][to] != DEADEND && M[from][to] < curMin)
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
	clock_t tStart = clock();
	v.level = 0;
	v.push_back(1);
	v.bound = bound(v, n, showB);
	if(showB)
		cout<<"init tail: "<<v.tail<<endl;
	minlength = INT_MAX;
	PQ.push(v);

	while (!PQ.empty()) {
		auto v = PQ.top();//remove the item with the best bound 
		PQ.pop();
		if((double)(clock() - tStart) / CLOCKS_PER_SEC > 29)
			return;
		
		if (v.bound < minlength) {
			u.level = v.level + 1;//set u to a child of v
			for (int i = 2; i <= n; i++) {
				auto it = v.usedCity & 1<< i;

				if (it == 0 && M[v.tail][i] != DEADEND) {
					memcpy(u.simPath, v.simPath, MSZ * sizeof(char));
					u.usedCity = v.usedCity;
					u.curSize = v.curSize;
					u.tail = v.tail;
					u.push_back(i);

					if (u.level == n - 2) {
						int theLastCity=87;
						for (int t = 1; t <= n; t++) {
							if (!(u.usedCity & 1 << t)) {
								theLastCity = t;
								break;
							}
						}
						if(M[i][theLastCity]==DEADEND)
							continue;
						u.push_back(theLastCity);
						if(M[theLastCity][1]==DEADEND)
							continue;
						u.push_back(1);
						int uLen = length(u);
						if (uLen < minlength) {
							minlength = uLen;
							cout<<"enter here"<<uLen<<endl;
							if(showB)
								cout << "Time taken: " << (double)(clock() - tStart) / CLOCKS_PER_SEC<< " minLen: " << minlength << endl;
							for(int i=0;i<=n;i++)
								if(u.simPath[i]>0)
									opttour[i] = u.simPath[i];
						}
					}
					else {
						u.bound = bound(u, n, showB);
						if(u.bound==DEADEND)
							continue;
						if (u.bound < minlength)
							PQ.push(u);
					}
				}
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
			M[i][j] = (M[i][j] <= 0) ? DEADEND : M[i][j];
			if (M[i][j] < recM[i] && M[i][j] != DEADEND)
				recM[i] = M[i][j];
		}
	}

	//vector<int> opttour;
	int opttour[MSZ];
	int minlength;
	//clock_t tStart = clock();
	
	bool showB = false;

	travel2(N, opttour, minlength, showB);
	//cout << "Time taken: " << (double)(clock() - tStart) / CLOCKS_PER_SEC << endl;
	cout << minlength << endl;
	for(int i=0;i<N;i++)
		cout<<opttour[i]<<" ";
	cout << endl;

	
	if(showB){
		cout<<"=============================="<<endl;
		for (int i = 1; i <= N; i++) {
			for (int j = 1; j <= N; j++) {
				cout<<M[i][j]<<" ";
			}cout<<endl;
		}
		cout<<"=============================="<<endl;
	}

	delete[]M;
	delete recM;

	return 0;
}
/*
17
0 3 5 48 48 8 8 5 5 3 3    0    3    5    8    8   5
    3 0    3   48   48    8    8    5    5    0    0    3    0    3    8    8    5
    5    3 0   72   72   48   48   24   24    3    3    5    3    0   48   48   24
   48   48   74 0    0    6    6   12   12   48   48   48   48   74    6    6   12
   48   48   74    0 0    6    6   12   12   48   48   48   48   74    6    6   12
    8    8   50    6    6 0    0    8    8    8    8    8    8   50    0    0    8
    8    8   50    6    6    0 0    8    8    8    8    8    8   50    0    0    8
    5    5   26   12   12    8    8 0    0    5    5    5    5   26    8    8    0
    5    5   26   12   12    8    8    0 0    5    5    5    5   26    8    8    0
    3    0    3   48   48    8    8    5    5 0    0    3    0    3    8    8    5
    3    0    3   48   48    8    8    5    5    0 0    3    0    3    8    8    5
    0    3    5   48   48    8    8    5    5    3    3 0    3    5    8    8    5
    3    0    3   48   48    8    8    5    5    0    0    3 0    3    8    8    5
    5    3    0   72   72   48   48   24   24    3    3    5    3 0   48   48   24
    8    8   50    6    6    0    0    8    8    8    8    8    8   50 0    0    8
    8    8   50    6    6    0    0    8    8    8    8    8    8   50    0 0    8
    5    5   26   12   12    8    8    0    0    5    5    5    5   26    8    8 0
*/