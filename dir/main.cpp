#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <algorithm>
#include <list>


bool comp(std::string& str1, std::string& str2) {
	return str1.length() < str2.length();
}

void main() {
	std::ios::sync_with_stdio(false);

	//объ€вление переменных
	bool breakage = false;
	int size;
	int strCount;
	int tempValue;
	int keySize;
	int minValue;
	int place[255] = { 0 };
	int* offset;
	int* rate;
	char* text;
	char* key;
	int** editDistance;
	std::string fileName;
	std::ifstream file;

	//ввод названи€ .txt файла или пути к .txt файлу
	std::cout << "Enter a file name (for example: input.txt): ";
	std::getline(std::cin, fileName);
	file.open(fileName);

	//если файл найден, то загружаем данные
	if (file.is_open()) {
		std::cout << "File is open" << std::endl;
		std::cout << "Loading text..." << std::endl;

		std::list<std::string> sortStrings;

		strCount = 0;
		size = 0;
		while (!file.eof()) {
			std::string tempStr;
			getline(file, tempStr);
			sortStrings.push_back(tempStr);
		}
		file.close();
		std::cout << "Loading text complite" << std::endl;

		std::cout << "Sorting..." << std::endl;
		sortStrings.sort();
		sortStrings.unique();
		sortStrings.sort(comp);
		std::cout << "Sorting complite" << std::endl;

		std::cout << "Creating additional objects..." << std::endl;
		strCount = sortStrings.size();

		std::list<std::string>::iterator it = sortStrings.begin();
		for (it; it != sortStrings.end(); ++it) {
			std::string tempStr = *it;
			int tempLength = tempStr.length();
			size += tempLength;
			for (int i = tempLength + 1; i < 255; ++i) {
				++place[i];
			}
		}

		text = new char[size];
		rate = new int[strCount];
		offset = new int[strCount];

		int k = 0;
		int l = 0;
		it = sortStrings.begin();
		for (it; it != sortStrings.end(); ++it) {
			std::string tempStr = *it;
			int tempLength = tempStr.length();
			rate[l] = tempLength;

			for (int j = 0; j < tempLength; ++j) {
				text[k] = tempStr[j];
				++k;
			}
			++l;
		}

		offset[0] = 0;
		for (int i = 1; i < strCount; ++i) {
			offset[i] = offset[i - 1] + rate[i - 1];
		}
		std::cout << "Creating additional objects complite" << std::endl;
		std::cout << "All done" << std::endl;

	}
	else {
		std::cout << "File not found" << std::endl;
		return;
	}

	//зацикливаем интерфейс
	//пока пользователь не введет команду !q 
	for (;;) {
		printf("______________________________\n");
		printf("Enter the key (!q to quit): ");
		std::string tempKey;
		std::getline(std::cin, tempKey);

		keySize = tempKey.length();
		key = new char[keySize];
		for (int i = 0; i < keySize; ++i) {
			key[i] = tempKey[i];
		}

		if (tempKey == "!q") {
			return;
		}

		editDistance = new int* [keySize + 1];
		for (int i = 0; i < keySize + 1; ++i) {
			editDistance[i] = new int[keySize + 2];
		}

		for (int i = 0; i < keySize + 1; ++i) {
			editDistance[i][0] = i;
		}

		for (int i = 1; i < keySize + 2; ++i) {
			editDistance[0][i] = i;
		}

		int start = keySize - 1;
		int finish = keySize + 2;

		//ищем подход€щие строки
		clock_t startTime = clock();
		for (int k = place[start]; k < place[finish]; ++k) {

			for (int i = 1; i < keySize + 1; ++i) {
				minValue = INT16_MAX;
				breakage = false;
				for (int j = 1; j < rate[k] + 1; ++j) {
					if (key[i - 1] == text[offset[k] + j - 1]) {
						tempValue = (editDistance[i - 1][j] + 1) < (editDistance[i - 1][j - 1]) ? (editDistance[i - 1][j] + 1) : (editDistance[i - 1][j - 1]);
						editDistance[i][j] = tempValue < (editDistance[i][j - 1] + 1) ? tempValue : (editDistance[i][j - 1] + 1);
					}
					else {
						tempValue = (editDistance[i - 1][j]) < (editDistance[i - 1][j - 1]) ? editDistance[i - 1][j] : editDistance[i - 1][j - 1];
						editDistance[i][j] = tempValue < (editDistance[i][j - 1]) ? tempValue : (editDistance[i][j - 1]);
						++editDistance[i][j];
					}
					if (editDistance[i][j] < minValue) {
						minValue = editDistance[i][j];
					}
				}
				if (minValue > 1) {
					breakage = true;
					break;
				}
			}
			if (!breakage) {
				if (editDistance[keySize][rate[k]] < 2) {
					for (int z = 0; z < rate[k]; ++z) {
						std::cout << text[offset[k] + z];
					}
					std::cout << std::endl;
				}
			}
		}
		clock_t endTime = clock();
		double seconds = (double)(endTime - startTime) / CLOCKS_PER_SEC;
		std::cout << "Time: " << seconds * 1000 << " ms" << std::endl;

		for (int i = 0; i < keySize + 1; ++i) {
			delete[] editDistance[i];
		}
		delete[] editDistance;
		delete[] key;
	}

	delete[] offset;
	delete[] rate;
	delete[] text;
	system("pause");
}
