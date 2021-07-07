#include "infected.h"

// конструктор от параметров Tdate
Tdate::Tdate(int d, int m , int y ) {
	day = d;
	month = m;
	year = y;
	daysFromPivot = 0;
	bool vg = (y % 4 == 0 && y % 100 != 0 || y % 400 == 0) ? true : false;
	int extraDays = ceil((y - 2000) / 4);
	daysFromPivot += (y - 2000)*365 + extraDays;
	int yearNotLeap[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	int yearIsLeap[12] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	if (vg) {
		for (auto i(0); i < m-1; ++i) {
			daysFromPivot += yearIsLeap[i];
		}
	}
	else {
		for (auto i(0); i < m-1; ++i) {
			daysFromPivot += yearNotLeap[i];
		}
	}
	daysFromPivot += d;
};

// перегрузка операторов сравнения структуры Tdate
//===============================================================
bool Tdate::operator ==(const Tdate& a) const {
	if (daysFromPivot == a.daysFromPivot) return true;
	else return false;
}
bool Tdate::operator <(const Tdate& a) const {
	if (daysFromPivot < a.daysFromPivot) return true;
	else return false;
}
bool Tdate::operator >(const Tdate& a) const {
	if (daysFromPivot > a.daysFromPivot) return true;
	else return false;
}
// создать структуру Tdate используя строку 
Tdate getStructTdate(string a) {
	string tmp1, tmp2, tmp3;
	int d = stoi(tmp1.assign(a, 0, 2));
	int m = stoi(tmp2.assign(a, 3, 2));
	int y = stoi(tmp3.assign(a, 6, 4));
	return Tdate(d,m,y);
}
// конструктор от параметров Tgroup
Tgroup::Tgroup(const string somestring1, const string somestring2) {
	id = stoi(somestring1);
	date = getStructTdate(somestring2);
}
// перегрузка операторов сравнения структуры Tgroup
//===============================================================
bool Tgroup::operator < (const Tgroup& a) const {
	if (persons.size() < a.persons.size()) return true;
	else return false;
}
bool Tgroup::operator > (const Tgroup& a) const {
	if (persons.size() > a.persons.size()) return true;
	else return false;
}
bool Tgroup::operator == (const Tgroup& a) const {
	if (persons.size() == a.persons.size()) return true;
	else return false;
}
// перегрузка оператора вывода для Tgroup
ostream& operator << (ostream& out, const Tgroup& a) {
			
		out << "Max group ID is: " << a.id << endl;
		out << "Date of infection: " << setw(2) << setfill('0') << a.date.day << '.' << setw(2) << setfill('0') << a.date.month << '.' << a.date.year << endl;//
		out	<< "group contacts including secondary: ";
		int counter(0);
		for (auto x : a.persons) {
			if (counter % 5 == 0) out << "\n";
			out << x << " ";
			counter++;
		}
		out << "\ngroup expanded by: ";
		if (a.thanksTo.size() == 0) out << "\ngroup isn't expanded.\n";
		else {
			counter = 0;
			for (auto x : a.thanksTo) {
				if (counter % 5 == 0) out << "\n";
				out << x << " ";
				counter++;
			}
			out << endl;
		}
		return out;
}
// создание контейнера групп по инфицированным и их контактам из входного файла
list<Tgroup> createGroups(const char* filename) {

	list<Tgroup> jurnal;
	ifstream fin(filename);	
	if (!fin) {
		cout << "unable to open file";
		exit(1);
	}
	
	string buf1, buf2;
	const regex datemask("^[0-3]{1}[1-9]{1}(\\.)[0-1]{1}[1-9]{1}(\\.)[1-2]{1}[0-9]{1}[0-9]{1}[0-9]{1}$"); // регулярное выражение для выявления даты
	list<Tgroup>::iterator it;
	
	while (!fin.eof()) {
		fin >> buf1;
		if (!buf1.empty()) {
			if (buf2.empty()) {
				fin >> buf2;
				if (!buf2.empty() && regex_match(buf2, datemask)) {
					jurnal.push_back(Tgroup(buf1, buf2));
					buf1.clear();					
					it = jurnal.begin();
				}
				else continue;
			}
			else if (regex_match(buf1, datemask)) {
				it->persons.pop_back();
				jurnal.push_back(Tgroup(buf2, buf1));
				buf1.clear();				
				it++;
				continue;
			}
			else {
				buf2.clear();
				fin >> buf2;
				if (!buf2.empty() && regex_match(buf2, datemask)) {
					jurnal.push_back(Tgroup(buf1, buf2));
					buf1.clear();					
					it++;
				}
				else {
					it->persons.push_back(stoi(buf1));
					buf1.clear();
					if (!buf2.empty()) it->persons.push_back(stoi(buf2));
				}
			}
		}
	}
	
	fin.close();
	jurnal.sort([](const Tgroup& a, const Tgroup& b)->bool {return a.date < b.date; }); // сразу сортируем по дате
	return jurnal;
}
// добавить к листу другой лист
void AddToList(list<int>& patient,const list<int> donor) {
	list<int>::const_iterator it = donor.begin();
	for (it; it != donor.end(); ++it) {
		patient.push_back(*it);
	}
}

// расширить группы с учетом вторичных контактов(контакты тех, кто заболел позже не расширяются за счет ранее заболевших)
void expandGroups(list<Tgroup> &somelist,unsigned int Range) {
	list<Tgroup>::iterator itTg1;
	list<Tgroup>::iterator itTg2 = somelist.begin();
	list<Tgroup>::iterator itTgLast = somelist.end();// создаем итератор указывающий за последний элемент листа
	itTgLast--;// и переводим его на последний элемент листа. теперь его можно использовать в цикле как ограничитель
	list<int> tmp; // создаем временный лист для добавления в него контактов других инфицированных
	list<int>::iterator itInt; // создаем итератор на элементы списка persons
	for (itTg1 = somelist.begin(); itTg1 != itTgLast; ++itTg1) { // начинаем перебор всех инфицированных по порядку		
		itTg2 = itTg1; // переставляем второй итератор инфицированных на позицию после первого
		itTg2++;
		while (itTg2->date.daysFromPivot - itTg1->date.daysFromPivot <= Range) { // пока разница дат инфицированных меньше или равна Range (7)
			if (itTg1->date == itTg2->date) {  // если даты совпадают, то расширяем списки у обоих взаимодополняя их

				AddToList(itTg2->persons, itTg1->persons);
				itTg2->thanksTo.push_back(itTg1->id); // добавляем информацию от кого добавлены вторичные контакты
			}
			itInt = (itTg1->persons).begin();
			for (itInt; itInt != itTg1->persons.end(); ++itInt) { //перебираем список контактов у первого инфицированного
				if (*(itInt) == itTg2->id) { //если находим совпадение имени в группе контактов с именем в группе инфицированных
					AddToList(tmp, itTg2->persons);// добавляем в  tmp лист контакты другого инфицированного	
					itTg1->thanksTo.push_back(itTg2->id);// добавляем информацию от кого добавлены вторичные контакты
					break;
				}			
			}
			if (itTg2 == itTgLast) break; // проверка на выход за границы листа
			itTg2++;
		}
		copy(tmp.begin(), tmp.end(), back_inserter(itTg1->persons));// расширяем список контактов инфицированного вторичными контактами
		//AddToList(itTg1->persons,tmp);
		itTg1->persons.sort();
		itTg1->persons.unique(); // удаляем повторы 
		itTg1->persons.remove(itTg1->id); // удаляем из списка контактов id самого инфицированного
		tmp.clear(); // очищаем временный список	
	}
}
// найти максимальную по численности группу/группы
vector<Tgroup*> findMax(list<Tgroup>& somelist) {
	vector<Tgroup*> maxVec;
	list<Tgroup>:: iterator it = somelist.begin();
	Tgroup* max = &(*it);
	maxVec.push_back(max);
	++it;
	for (it; it != somelist.end();++it) {
		if (*it == *max ) maxVec.push_back(&(*it));
		else if (*it > *max) {
			maxVec.clear();
			max = &(*it);
			maxVec.push_back(max);
		}
	}
	return maxVec;
}
// записать максимальную по численности группу/группы в файл
void writeMaxGroup(list<Tgroup> somelist, const char* filename) {	
	//somelist.sort([](const Tgroup& a, const Tgroup& b)->bool {return a > b;});//сортировка листа не использует quicksort из-за отсутствия random-access к элементам, поэтому
	//Tgroup MaxGroup = *(somelist.begin());									//наверно, чтобы быстрее работало стоит написать 1 проход по всем элементам в виде отдельной функции
	vector<Tgroup*> MaxGroup = findMax(somelist);
	ofstream fout(filename); // ios_base::out | ios_base::trunc
	if (!fout.is_open()) {
		cout << "unable to create file";
		exit(1);
	}
	if (MaxGroup.size() > 1) {
		fout << "there are several groups: \n";
		cout << "there are several groups: \n";
	}
	for (auto x : MaxGroup) {
		fout << *x;
	}
	fout.close();
	for (auto x : MaxGroup) { // вывод на консоль
		cout << *x;
	}	
}
