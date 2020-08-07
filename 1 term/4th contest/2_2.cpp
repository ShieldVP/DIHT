/*
В большой IT-фирме есть только одна переговорная комната.
Желающие посовещаться заполняют заявки с желаемым временем начала и конца.
Ваша задача определить максимальное количество заявок, которое может быть удовлетворено.
Вход содержит только пары целых чисел — начала и концы заявок.
*/

#include <iostream>
#include <vector>
using namespace std;

struct meeting {
	int start, end;
};

size_t max_meeting_amount(vector<meeting> schedule) {
	size_t amount = 0;
	vector<meeting> later_meetings = schedule;
	while (!later_meetings.empty()) {
		++amount;
		meeting earliest_ending_meeting = later_meetings[0];
		for (size_t i = 0; i < later_meetings.size(); ++i)
			if (later_meetings[i].end < earliest_ending_meeting.end) earliest_ending_meeting = later_meetings[i];
		vector<meeting> buf;
		for (size_t i = 0; i < later_meetings.size(); ++i)
			if (later_meetings[i].start >= earliest_ending_meeting.end) buf.push_back(later_meetings[i]);
		swap(buf, later_meetings);
	}
	return amount;
}

int main() {
	vector<meeting> schedule;
	while (!cin.eof()) {
		meeting buf;
		cin >> buf.start >> buf.end;
		schedule.push_back(buf);
	}
	cout << max_meeting_amount(schedule);
	return 0;
}