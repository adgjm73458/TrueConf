/*
1.заполнить случайными числами от 1 до 9 значения контейнеров vector[i] и map[i];
2.удалить случайное число элементов (не более 15) в каждом контейнере;
3.после этого провести синхронизацию, чтобы в vector и map остались только имеющиеся в обоих контейнерах элементы (дубликаты не удалять).
*/


#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <iterator>
#include <ctime>
#include <execution>
#include <conio.h>

void printContainer(std::vector<unsigned int>* Vec)
{
	for (auto it : *Vec) std::cout << it << ' ';

	std::cout << std::endl;
}

void printContainer(std::map<unsigned int, unsigned int>* Map)
{
	for (auto it : *Map) std::cout << it.second << ' ';

	std::cout << std::endl;
}


int main() {
	unsigned int Size = 0;
	std::cout << "Enter size: "; std::cin >> Size;
	std::vector<unsigned int> Vec;
	std::map<unsigned int, unsigned int> Map;
	srand(time(nullptr));

	// 1.заполнить случайными числами от 1 до 9 значения контейнеров vector[i] и map[i];

	for (unsigned int i = 0; i < Size; i++) {
		Vec.push_back(rand() % 9 + 1);
		Map[i] = rand() % 9 + 1;
	}
	std::cout << "-----------STEP 1-------------\n";
	std::cout << "Vec original: "; printContainer(&Vec);
	std::cout << "Map original: "; printContainer(&Map);

	//2.удалить случайное число элементов(не более 15) в каждом контейнере;

	std::vector<unsigned int>::iterator itVec = Vec.begin();
	std::map<unsigned int, unsigned int>::iterator itMap = Map.begin();
	unsigned int removeSize = rand() % 16;

	while (removeSize >= Size) removeSize = rand() % 16;

	for (int i = 0; i < removeSize; i++) {
		itVec = Vec.erase(itVec);
		itMap = Map.erase(itMap);
	}

	std::cout << "-----------STEP 2-------------\n";
	std::cout << "RemoveSize: " << removeSize << std::endl;
	std::cout << "Vec after remove: "; printContainer(&Vec);
	std::cout << "Map after remove: "; printContainer(&Map);

	//3.после этого провести синхронизацию, чтобы в vector и map остались только имеющиеся в обоих контейнерах элементы(дубликаты не удалять).

	std::set<unsigned int> setVec;
	std::set<unsigned int> setMap;

	for (auto it : Vec) setVec.insert(it);
	for (auto it : Map) setMap.insert(it.second);

	std::vector<unsigned int> Diff(Vec.size() + Map.size());
	auto itDiff = std::set_difference(std::execution::par, setVec.begin(), setVec.end(), setMap.begin(), setMap.end(), Diff.begin());
	itDiff = std::set_difference(std::execution::par, setMap.begin(), setMap.end(), setVec.begin(), setVec.end(), itDiff);
	Diff.resize(itDiff - Diff.begin());

	itVec = Vec.begin();
	while (itVec != Vec.end()) {
		if (std::find(std::execution::par, Diff.begin(), Diff.end(), *itVec) != Diff.end())
			itVec = Vec.erase(itVec);
		else itVec++;
	}

	itMap = Map.begin();
	while (itMap != Map.end()) {
		if (std::find(std::execution::par, Diff.begin(), Diff.end(), itMap->second) != Diff.end())
			itMap = Map.erase(itMap);
		else itMap++;
	}
	std::cout << "-----------STEP 3-------------\n";
	std::cout << "Vec after sync: "; printContainer(&Vec);
	std::cout << "Map after sync: "; printContainer(&Map);
	_getch();
	return 0;
}