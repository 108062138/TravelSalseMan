// zz.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
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

class node{
public:
	node();
	~node();
	int level;
	vector<int> path;
	int bound;
	node(const node& p1) { 
		level = p1.level; 
		bound = p1.bound; 
		path = p1.path;
	}
};
node::node(){
	level = 0;
	bound = 0;
}
node::~node(){
} 
bool operator<(const node& p1, const node& p2){
	return p1.level < p2.level;
}

int bound(node v,int n) {
	int total = 0;
	int curMin;
	if (v.path.size() <= 1) {
		for (int i = 1; i <= n; i++) {
			curMin = INT_MAX;
			for(int j=1;j<=n;j++)
				if (M[i][j] != 0)
					if (M[i][j] < curMin)
						curMin = M[i][j];
			total += curMin;
		}
	}else{
		unordered_set<int> lib;
		lib.insert(1);
		int len = v.path.size();
		for (int i = 1; i < len; i++) {
			int from = v.path[i - 1]; 
			int to = v.path[i];
			total += M[from][to];
			lib.insert(to);
		}
		for (int i = 1; i <= n; i++) {
			if (lib.find(i) == lib.end()) {
				curMin = INT_MAX;
				for (int j = 1; j <= n; j++) {
					if (M[i][j] != 0) {
						if (j == 1) {//1 is ok
							if (M[i][j] < curMin)
								curMin = M[i][j];
						}else{
							if (lib.find(j) == lib.end())
								if (M[i][j] < curMin)
									curMin = M[i][j];
						}
					}
				}
				total += curMin;
				continue;
			}

			if (i == v.path[len - 1]) {
				curMin = INT_MAX;
				for (int j = 1; j <= n; j++) {
					if (M[i][j] != 0 && lib.find(j) == lib.end())//1 is not ok
						if (M[i][j] < curMin)
							curMin = M[i][i];
				}
				total += curMin;
			}
		}
	}

	return total;
}

int length(node v) {
	int res = 0;
	int to, from;
	for (int i = 1; i < v.path.size(); i++) {
		from = v.path[i - 1]; 
		to = v.path[i];
		res += M[from][to];
	}

	res += M[to][1];
	return res;
}

void travel2(int n,	vector<int>&opttour,int& minlength) {
	priority_queue<node> PQ;
	node v,u;
	v.level = 0;
	v.path.push_back(1);
	v.bound = bound(v,n);
	minlength = INT_MAX;
	PQ.push(v);

	while (!PQ.empty()){
		auto v = PQ.top();//remove the item with the best bound 
		
		PQ.pop();
		if (v.bound < minlength) {
			u.level = v.level + 1;//set u to a child of v
			for (int i = 2; i <= n; i++) {
				auto it = find(v.path.begin(), v.path.end(), i);
				if (it == v.path.end()) {
					u.path = v.path;
					u.path.push_back(i);

					if (u.level == n - 2) {
						int theLastCity;
						for (auto city : u.path) 
							have[city] = true;
						for (int i = 1; i <= n; i++) {
							if (have[i] == false) {
								theLastCity = i;
								break;
							}
						}
						for (int i = 1; i <= n; i++)
							have[i] = false;
						u.path.push_back(theLastCity);
						u.path.push_back(1);
						int uLen = length(u);
						if (uLen < minlength) {
							minlength = uLen;
							opttour = u.path;
						}
					}
					else{
						u.bound = bound(u,n);
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

int main(){
	int N;
	cin >> N;
	for (int i = 1; i <= N; i++)
		have[i] = false;
	M = new int*[N + 5];
	for (int i = 0; i < N + 5; i++)
		M[i] = new int[N + 5];
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			cin >> M[i][j];
			M[i][j] = (M[i][j] <= 0) ? 0: M[i][j];
		}
	}

	

	vector<int> opttour; 
	int minlength;

	clock_t tStart = clock();
	travel2(N, opttour, minlength);
	cout << "Time taken: " << (double)(clock() - tStart) / CLOCKS_PER_SEC << endl;
	cout << minlength << endl;
	for (auto x : opttour)
		cout << x << " ";
	cout << endl;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
