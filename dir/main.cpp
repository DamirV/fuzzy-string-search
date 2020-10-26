#include <iostream>
#include <fstream>
#include <string>
#include <ctime>


void main() {
	std::ios::sync_with_stdio(false);

	//объ€вление переменных
	bool breakage = false;
	int size;
	int strCount;
	int tempValue;
	int keySize;
	int minValue;
	int* offset;
	int* rate;
	char* text;
	char* key;
	char* tempText;
	int** editDistance;
	std::string fileName;
	std::string tempKey;
	std::ifstream file;

	//ввод названи€ .txt файла или пути к .txt файлу
	std::cout << "Enter a file name (for example: input.txt): ";
	std::getline(std::cin, fileName);
	file.open(fileName);

	//если файл найден, то загружаем данные
	if (file.is_open()) {
		std::cout << "File is open" << std::endl;
		std::cout << "Loading..." << std::endl;

		file.seekg(0, std::ios::end);
		size = file.tellg();
		tempText = new char[size];
		file.seekg(0, std::ios::beg);
		file.read(tempText, size);
		file.close();

		strCount = 0;
		for (int i = 0; i < size; ++i) {
			if (tempText[i] == '\n') {
				strCount++;
				--size;
			}
		}
		tempText[size] = '\0';
		++size;
		++strCount;

		text = new char[size];
		for (int i = 0; i < size; ++i) {
			text[i] = tempText[i];
		}
		delete[] tempText;

		rate = new int[strCount];
		offset = new int[strCount];

		int j = 0;
		int start = 0;
		for (int i = 0; i < size; ++i) {
			if (text[i] == '\n' || text[i] == '\0') {
				rate[j] = i - start;
				offset[j] = start;
				start = i + 1;
				++j;
			}
		}

		std::cout << "Loading complite" << std::endl;
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

		//ищем подход€щие строки
		clock_t startTime = clock();
		for (int k = 0; k < strCount; ++k) {

			if (rate[k] - keySize > 1 || rate[k] - keySize < -1) {
				continue;
			}

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
