#include <iostream>
#include <fstream>
#include <math.h>
#include <string>
#include <vector>
#include <map>
#include <iomanip>
#include <algorithm>

using namespace std;

typedef map<pair<int, int>, pair<int, int>> mappp;


class CTrain {
private:
	string m_strFrom;
	string m_strTo;
	map<pair<int, int>, pair<int, int>> m_mapSchedule;

public:
	CTrain(const string& tfrom, const string& tto, const map<pair<int, int>, pair<int, int>>& tschedule) {
		m_strFrom = tfrom;
		m_strTo = tto;
		m_mapSchedule = tschedule;
	}
	CTrain(const CTrain& obj) {
		this->m_strFrom = obj.m_strFrom;
		this->m_strTo = obj.m_strTo;
		this->m_mapSchedule = obj.m_mapSchedule;
	}
	CTrain(const string& tfrom, const string& tto) {
		this->m_strFrom = tfrom;
		this->m_strTo = tto;
	}

	const string& GetFrom() const { return m_strFrom; }
	const string& GetTo() const { return m_strTo; }
	const mappp& GetSchedule() const { return m_mapSchedule; }

	ostream& OutputMap(ostream& fileOut) const {
		for (auto& it : m_mapSchedule) {
			fileOut << setfill('0');
			fileOut << setw(2) << it.first.first << ":" << setw(2) << it.first.second << " - "
				<< setw(2) << it.second.first << ":" << setw(2) << it.second.second << "\n";
		}
		return fileOut;
	}
	ostream& Output(ostream& fileOut) const {
		fileOut << this->GetFrom() << " " << this->GetTo() << " ";
		this->OutputMap(fileOut);
		fileOut << "\n";
		return fileOut;
	}
	friend ostream& operator << (ostream& fileOut, const CTrain& obj) {
		obj.Output(fileOut);
		return fileOut;
	}

	istream& Input(istream& fileIn) {
		if (&fileIn == &cin) {
			cout << "From: ";
			fileIn >> m_strFrom;
			cout << "To: ";
			fileIn >> m_strTo;
			int n;
			cout << "Number of stops: ";
			cin >> n;

			mappp tempmap;
			pair<int, int> temppair1;
			pair<int, int> temppair2;
			string tempstring;
			string dividedstring;

			cout << "Time1 format[hour:minutes]: ";
			fileIn >> tempstring; //reads 04:40
			dividedstring = tempstring.substr(0, 2);
			temppair1.first = stoi(dividedstring);
			dividedstring = tempstring.substr(3, 2);
			temppair1.second = stoi(dividedstring);

			cout << " ";
			fileIn >> tempstring; // reads -

			cout << "Time2 format[hour:minutes]: ";
			fileIn >> tempstring; // reads 06:21
			dividedstring = tempstring.substr(0, 2);
			temppair2.first = stoi(dividedstring);
			dividedstring = tempstring.substr(3, 2);
			temppair2.second = stoi(dividedstring);

			m_mapSchedule.insert(make_pair(temppair1, temppair2));
		}
		else {

			fileIn >> m_strFrom;
			fileIn >> m_strTo;

			pair<int, int> temppair1;
			pair<int, int> temppair2;
			string tempstring;
			string dividedstring;

			fileIn >> tempstring; //reads 04:40
			dividedstring = tempstring.substr(0, 2);
			temppair1.first = stoi(dividedstring);
			dividedstring = tempstring.substr(3, 2);
			temppair1.second = stoi(dividedstring);

			fileIn >> tempstring; // reads -

			fileIn >> tempstring; // reads 06:21
			dividedstring = tempstring.substr(0, 2);
			temppair2.first = stoi(dividedstring);
			dividedstring = tempstring.substr(3, 2);
			temppair2.second = stoi(dividedstring);

			m_mapSchedule.insert(make_pair(temppair1, temppair2));
		}
		return fileIn;
	}
	friend istream& operator >> (istream& fileIn, CTrain& obj) {
		obj.Input(fileIn);
		return fileIn;
	}

	CTrain& operator = (const CTrain& obj) {
		this->m_strFrom = obj.m_strFrom;
		this->m_strTo = obj.m_strTo;
		this->m_mapSchedule = obj.m_mapSchedule;
		return (*this);
	}

	bool operator < (const CTrain& obj) {
		mappp::iterator it_this = this->m_mapSchedule.begin();
		mappp::const_iterator it_obj = obj.m_mapSchedule.begin();


		if (it_this->first.first == it_obj->first.first) {
			if (it_this->first.second < it_this->first.second) {
				return true;
			}
			else {
				return false;
			}
		}
		else if (it_this->first.first < it_obj->first.first) {
			return true;
		}
		else if (it_this->first.first > it_obj->first.first) {
			return false;
		}
	}

	bool operator == (const CTrain& obj) {
		return ((this->GetFrom() == obj.GetFrom()) && (this->GetTo() == obj.GetTo()));
	}


	void unionMaps(const CTrain& obj) {
		mappp temp;
		set_union(this->m_mapSchedule.begin(), this->m_mapSchedule.end(),
			obj.m_mapSchedule.begin(), obj.m_mapSchedule.end(),
			inserter(temp, temp.begin()));
		this->m_mapSchedule.clear();
		this->m_mapSchedule = temp;
	}
	
	void RemoveTimes(CTrain& obj, unsigned int n)
	{
		mappp::iterator time1 = obj.m_mapSchedule.begin();
		for (unsigned i = 0; i < n; i++)
		{
			time1++;

		}
		obj.m_mapSchedule.erase(time1);
	}


};

class CTrainFromTo {
	virtual vector<CTrain> calcTrainFromTo(const string& from, const string& to, int hour, int minute) = 0;
	virtual vector<CTrain> calcTrainFrom(const string& from) = 0;
	virtual vector<CTrain> calcTrainTo(const string& to) = 0;
};

class CRailwayStation : public CTrainFromTo {
private:
	vector<CTrain> m_vRailwayTrack;

public:
	const vector<CTrain>& GetRailwayTrack() const { return m_vRailwayTrack; }

	CRailwayStation(const string& filename) {
		fstream fileIn(filename, ios::in);

		if (fileIn.good()) {
			while (!(fileIn.eof())) {
				CTrain obj("", "");
				obj.Input(fileIn);
				addTrain(obj);
			}

			fileIn.close();
		}
		else {
			throw runtime_error("File not found!");
		}
	}

	void addTrain(const string& tfrom, const string& tto, const mappp& ttimes) {
		CTrain obj(tfrom, tto, ttimes);
		vector<CTrain>::iterator it = m_vRailwayTrack.begin();
		it = find(m_vRailwayTrack.begin(), m_vRailwayTrack.end(), obj);

		if (it != m_vRailwayTrack.end()) {
			it->unionMaps(obj);
		}
		else {
			m_vRailwayTrack.push_back(obj);
		}
	}

	void addTrain(const CTrain& obj) {
		vector<CTrain>::iterator it = m_vRailwayTrack.begin();
		it = find(m_vRailwayTrack.begin(), m_vRailwayTrack.end(), obj);

		if (it != m_vRailwayTrack.end()) {
			it->unionMaps(obj);
		}
		else {
			m_vRailwayTrack.push_back(obj);
		}
	}


	vector<CTrain> calcTrainFromTo(const string& from, const string& to, int hour, int minute) override {
		vector<CTrain>::iterator it = m_vRailwayTrack.begin();
		CTrain temp(from, to);
		vector<CTrain> res;
		it = find(m_vRailwayTrack.begin(), m_vRailwayTrack.end(), temp);

		if (it != m_vRailwayTrack.end()) {
			auto itToSchedule = it->GetSchedule().lower_bound(make_pair(hour, minute));

			if (itToSchedule != it->GetSchedule().end()) {
				mappp copymap = { *itToSchedule };
				res.emplace_back(from, to, copymap);
			}
		}

		return res;
	}

	vector<CTrain> calcTrainFrom(const string& from) override {
		pair<vector<CTrain>::iterator, vector<CTrain>::iterator> equal_range_iterator_pair;
		CTrain temp(from, "");

		auto SortByFrom = [](const CTrain& first, const CTrain& second) {
			return (first.GetFrom() < second.GetFrom());
		};

		sort(m_vRailwayTrack.begin(), m_vRailwayTrack.end(), SortByFrom);

		equal_range_iterator_pair = equal_range(m_vRailwayTrack.begin(), m_vRailwayTrack.end(),
			temp, SortByFrom);

		vector<CTrain> res(equal_range_iterator_pair.first, equal_range_iterator_pair.second);
		return res;
	}

	vector<CTrain> calcTrainTo(const string& to) override {
		pair<vector<CTrain>::iterator, vector<CTrain>::iterator> equal_range_iterator_pair;
		CTrain temp("", to);

		auto SortByTo = [](const CTrain& first, const CTrain& second) {
			return (first.GetTo() < second.GetTo());
		};

		sort(m_vRailwayTrack.begin(), m_vRailwayTrack.end(), SortByTo);

		equal_range_iterator_pair = equal_range(m_vRailwayTrack.begin(), m_vRailwayTrack.end(),
			temp, SortByTo);

		vector<CTrain> res(equal_range_iterator_pair.first, equal_range_iterator_pair.second);
		return res;
	}
	
	void removeTrain(const string& from, const string& to, const pair<int, int> times)
	{
		CTrain obj(from, to);
		vector<CTrain>::iterator it = m_vRailwayTrack.begin();
		//CTrain temp(from, to, { times.first, times.second });
		it = find(m_vRailwayTrack.begin(), m_vRailwayTrack.end(), obj);


		if (it != m_vRailwayTrack.end())
		{
			for (unsigned int i = 0; i < m_vRailwayTrack.size(); i++)
			{
				for (auto &x : m_vRailwayTrack[i].GetSchedule())
				{
					if (x.first == times)
					{
						m_vRailwayTrack[i].RemoveTimes(m_vRailwayTrack[i], i);
						break;
					}
				}
			}
		}
		else {
			throw runtime_error("Track not found!");
		}
	}

	void removeTrain(const string& from, const string& to) {
		vector<CTrain>::iterator it = m_vRailwayTrack.begin();
		CTrain temp(from, to);
		it = find(m_vRailwayTrack.begin(), m_vRailwayTrack.end(), temp);

		if (it != m_vRailwayTrack.end()) {
			m_vRailwayTrack.erase(it);
		}
		else {
			throw runtime_error("Track not found!");
		}
	}

	const bool operator < (const CRailwayStation& obj) const {
		return this->m_vRailwayTrack.size() < obj.m_vRailwayTrack.size();
	}

	const bool operator == (const CRailwayStation& obj) const {
		return this->m_vRailwayTrack.size() == obj.m_vRailwayTrack.size();
	}

	const friend ostream& operator << (ostream& fileOut, CRailwayStation& obj) {
		vector<CTrain>::iterator it = obj.m_vRailwayTrack.begin();

		while (it != obj.m_vRailwayTrack.end()) {
			fileOut << *it;
			++it;
		}

		return fileOut;
	}

	friend istream& operator >> (istream& fileIn, CRailwayStation& obj) {
		if (&fileIn == &cin) {
			cout << "Enter Station: ";
			fileIn >> obj;
		}
		else {
			fileIn >> obj;
		}

		return fileIn;
	}
	
	void CalcFromTo2(const string& from, const string& to, int hour, int minute)
	{
		vector<CTrain> temp;
		temp = calcTrainFrom(from);
		vector<CTrain>::iterator it = temp.begin();
		while (it != temp.end())
		{
			for (auto &x : it->GetSchedule())
			{
				if (x.first == make_pair(hour, minute))
				{
					if (it->GetTo() == to)
					{
						cout << "breaknah";
						break;
					}
					else 
					{
						mappp tempmap;
						auto itToMap = it->GetSchedule().lower_bound(x.first);
						tempmap = { *itToMap };
						CTrain temp2(from, it->GetTo(), tempmap);
						cout << temp2;

					}
					
				}
				CalcFromTo2(it->GetTo(), to, x.second.first, x.second.second);
			}
			it++;
		}
	}
};




int main() {
	vector<CTrain> test;
	ifstream fileIn("InputTimetable.txt");
	ofstream fileOut("OutputTimetable.txt");
	string from, to;
	CRailwayStation station1("InputTimetable.txt");
	cout << station1;
	system("pause");
	station1.CalcFromTo2("VARNA", "SOFIA", 6, 00);
	cout << "\n\n";
	system("pause");
	return 0;
}
