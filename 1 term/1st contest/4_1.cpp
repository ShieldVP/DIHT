/*
Реализовать очередь с динамическим зацикленным буфером.
Обрабатывать команды push back и pop front.
*/

#include <iostream>
#include <vector>
using namespace std;

class queue {
public:
	queue() {
		circle.resize(1);
		start = 0;
		end = 0;
		last_activity_push = false;
	}

	void push(int x) { // push back
		circle[end] = x;
		last_activity_push = true;
		end++;
		if (not_enough_memory()) resize();
		end %= circle.size();
	}

	void pop() { // pop front
		start = (start + 1) % circle.size();
		last_activity_push = false;
	}

	bool empty() { // checking if queue is empty
		return start == end && last_activity_push == false;
	}

	int top() { // getting first element from queue
		return circle[start];
	}
	
private:
	vector<int> circle;
	int start, end;
	bool last_activity_push; // true if last action was push, if pop - false

	void resize() {
		vector<int> buf;
		for (int i = start, j = 0; j < circle.size(); i = (i + 1) % circle.size()) {
			buf.push_back(circle[i]);
			j++;
		}
		circle.resize(circle.size() * 2);
		for (int i = 0; i < buf.size(); i++) circle[i] = buf[i];
		start = 0;
		end = buf.size();
	}

	bool not_enough_memory() {
		return (end - start) % circle.size() == 0 && last_activity_push == true;
	}
};

int main() {
	queue q;
	int n;
	cin >> n;

	bool answer = true; // until we do anything with queue everything is right

	// getting coomands and checking queue
	for (int i = 0; i < n; i++) {
		int command, value;
		cin >> command >> value;
		if (command == 2) {
			if (q.empty()) q.push(-1);
			else if (q.top() != value) {
				answer = false;
				break;
			}
			else q.pop();
		}
		else q.push(value);
	}

	//result
	if (answer == true) cout << "YES";
	else cout << "NO";

	return 0;
}