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
	int daysFromPivot; // количество дней от даты 01.01.2000 до текущей в структуре
	// =================================
	Tdate(int d=1, int m=1, int y=2000); 
	bool operator ==(const Tdate&) const;
	bool operator <(const Tdate&) const;
	bool operator >(const Tdate&) const;	
};


struct Tgroup {
	int id;				// код инфицированного
	Tdate date;			// дата заражени€
	list <int> persons; // список контактов за неделю до заражени€
	list<int> thanksTo; // список инфицированных, за счет контактов которых расшир€етс€ предыдущий список
	// ===================================
	Tgroup(const string,const string);
	bool operator ==(const Tgroup&) const;
	bool operator <(const Tgroup&) const;
	bool operator >(const Tgroup&) const;	
};

list<Tgroup> createGroups(const char* filename);				 // создание контейнера групп по инфицированным и их контактам из входного файла
Tdate getStructTdate(string a);									 // получить дату из строки файла
void expandGroups(list<Tgroup>& somelist,unsigned int Range);	 // расширить группы с учетом вторичных контактов(контакты тех, кто заболел позже не расшир€ютс€ за счет ранее заболевших)
void AddToList(list<int> &patient, const list<int> donor);		 // добавить к листу другой лист
vector<Tgroup*> findMax(list<Tgroup> &somelist);			 // найти максимальную по численности группу/группы
void writeMaxGroup(list<Tgroup> somelist, const char* filename); // записать максимальную по численности группу/группы в файл
#endif
