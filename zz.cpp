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
	int path[35];
	int size;
	int bound;
	node(const node& p1) { 
		level = p1.level; 
		bound = p1.bound; 
		size = p1.size;
		for (int i = 0; i < p1.size; i++)
			path[i] = p1.path[i];
		
	}
	void push_back(int vertex) {
		path[size] = vertex;
		size++;
	}
	bool find(int target) {
		for (int i = 0; i < size; i++)
			if (path[i] == target)
				return true;
		return false;
	}
};
node::node(){
	level = 0;
	bound = 0;
	size = 0;
	for (int i = 0; i < 35; i++)
		path[i] = -1;
}
node::~node(){
} 
bool operator<(const node& p1, const node& p2){
	return (p1.level < p2.level) || (p1.level==p2.level && p1.bound>p2.bound);
}

int bound(node v,int n) {
	int total = 0;
	int curMin;
	if (v.size <= 1) {
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
		unordered_set<int> notYet;
		for (int i = 1; i <= n; i++)
			notYet.insert(i);
		lib.insert(1);
		int len = v.size;
		int curLast;
		for (int i = 1; i < len; i++) {
			int from = v.path[i - 1]; 
			int to = v.path[i];
			if (M[from][to] < 0 || M[from][to]>1000) {
				cout << "ERROR!" << endl;
			}
			total += M[from][to];
			lib.insert(to);
			if (i == len - 1)
				curLast = to;
			if(i!=len-1)
				notYet.erase(to);
		}

		for (const auto& from : notYet) {
			if (from == 1)
				continue;
			if (from == curLast) {
				curMin = INT_MAX;
				for (const auto& to : notYet) {
					if (to == 1)
						continue;
					if (M[from][to] != 0 && M[from][to] < curMin)
						curMin = M[from][to];
				}
				total += curMin;
			}
			else{
				curMin = INT_MAX;
				for (const auto& to : notYet) {
					if (M[from][to] != 0 && M[from][to] < curMin)
						curMin = M[from][to];
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
	for (int i = 1; i < v.size; i++) {
		from = v.path[i - 1]; 
		to = v.path[i];
		res += M[from][to];
	}

	res += M[to][1];
	return res;
}

void travel2(int n,	vector<int>&opttour,int& minlength) {
	priority_queue<node> PQ;
	node v;
	v.level = 0;
	v.push_back(1);
	v.bound = bound(v,n);
	minlength = INT_MAX;
	PQ.push(v);

	while (!PQ.empty()){
		node u,v;
		v = PQ.top();//remove the item with the best bound 
		PQ.pop();

		if (v.bound < minlength) {
			u.level = v.level + 1;//set u to a child of v
			for (int i = 2; i <= n; i++) {
				bool it = v.find(i);
				if (it == false) {
					cout << "))))))))))))))))))))))))))))))))))))))))" << endl;
					cout << "before u size: " << u.size << endl <<" path: ";
					for (int tmp = 0; tmp < v.size; tmp++) {
						u.path[tmp] = v.path[tmp];
						cout << u.path[tmp] << " ";
					}cout << endl;
					u.push_back(i);
					cout << "after u size: " << u.size << endl << " path: ";
					for (int tmp = 0; tmp < v.size; tmp++) {
						u.path[tmp] = v.path[tmp];
						cout << u.path[tmp] << " ";
					}cout << endl;
					cout << "========================================" << endl;
					if (u.level == n - 2) {
						int theLastCity=-1;
						for (int tmp = 0; tmp <u.size; tmp++)
							have[tmp] = true;
						for (int tmp = 1; tmp <= n; tmp++) {
							if (have[tmp] == false) {
								theLastCity = tmp;
								break;
							}
						}
						for (int tmp = 1; tmp <= n; tmp++)
							have[tmp] = false;
						u.push_back(theLastCity);
						u.push_back(1);
						cout << "here" << endl;
						for (int tmp = 0; tmp < u.size; tmp++) {
							cout << u.path[tmp] << " ";
						}
						int uLen = length(u);
						if (uLen < minlength) {
							cout << "enter the opt:" << endl;
							minlength = uLen;
							opttour.clear();
							cout << "best path:" << endl;
							for (int tmp = 0; tmp < u.size; tmp++) {
								opttour.push_back(u.path[tmp]);
								cout << u.path[tmp] << " ";
							}
							//opttour = u.path;
						}
					}
					else{
						u.bound = bound(u,n);
						if (u.bound < minlength)
							PQ.push(u);
					}
				}
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
	int cnt = 0;
	for (auto x : opttour) {
		cout << x << " ";
		cnt++;
		if (cnt == N - 1)break;
	}
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
