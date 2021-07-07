#ifndef INFECTEDH
#define INFECTEDH
#include<fstream>
#include<iostream>
#include<string>
#include<list>
#include<vector>
#include<regex>
#include<algorithm>
#include<iterator>
#include<iomanip>

using namespace std;


struct Tdate {

	int day;
	int month;
	int year;
	int daysFromPivot; // ���������� ���� �� ���� 01.01.2000 �� ������� � ���������
	// =================================
	Tdate(int d=1, int m=1, int y=2000); 
	bool operator ==(const Tdate&) const;
	bool operator <(const Tdate&) const;
	bool operator >(const Tdate&) const;	
};


struct Tgroup {
	int id;				// ��� ���������������
	Tdate date;			// ���� ���������
	list <int> persons; // ������ ��������� �� ������ �� ���������
	list<int> thanksTo; // ������ ��������������, �� ���� ��������� ������� ����������� ���������� ������
	// ===================================
	Tgroup(const string,const string);
	bool operator ==(const Tgroup&) const;
	bool operator <(const Tgroup&) const;
	bool operator >(const Tgroup&) const;	
};

list<Tgroup> createGroups(const char* filename);				 // �������� ���������� ����� �� �������������� � �� ��������� �� �������� �����
Tdate getStructTdate(string a);									 // �������� ���� �� ������ �����
void expandGroups(list<Tgroup>& somelist,unsigned int Range);	 // ��������� ������ � ������ ��������� ���������(�������� ���, ��� ������� ����� �� ����������� �� ���� ����� ����������)
void AddToList(list<int> &patient, const list<int> donor);		 // �������� � ����� ������ ����
vector<Tgroup*> findMax(list<Tgroup> &somelist);			 // ����� ������������ �� ����������� ������/������
void writeMaxGroup(list<Tgroup> somelist, const char* filename); // �������� ������������ �� ����������� ������/������ � ����
#endif
