#include <iostream>
#include <fstream>
#include <ctime>
using namespace std;

/*
Отчет хранится в    shell_sort_lab\shell_sort\shell_sort\Result.txt

Структура хранения массивов в файле:

Сначала количество элементов массива(count_), затем все элементы массива по порядку.
count_ и все элементы разделены символом ' '
Массивы разделены символом '\n'

*/


int const DEFAULT_MAX_NUMBER = 99;
int const DEFAULT_MIN_NUMBER = 0;
int const DEFAULT_COUNT = 10;

class Array
{
	int* array_;
	int count_;

	Array* createStepArr1_(bool outputStep);
	Array* createStepArr2_(bool outputStep);
	Array* createStepArr3_(bool outputStep);

public:
	Array(int count = DEFAULT_COUNT) { array_ = new int[count]; count_ = count; };
	Array(const int* copy, int count);
	Array(const Array& copy);
	~Array() { delete array_; };

	void randArray(int minNumber = DEFAULT_MIN_NUMBER, int maxNumber = DEFAULT_MAX_NUMBER);
	friend ofstream& operator <<(ofstream& file, Array& Mas);
	friend ifstream& operator >>(ifstream& file, Array& Mas);
	void output();

	int sortingShell(int typeStepArray, bool outputStep = false);
	void sortingDirectInsert(int firstInd = 0, int step = 1);

	bool checkSorting();

	void operator =(const Array& copy);

	int getCount() { return count_; };
};

Array::Array(const int* copy, int count)
{
	if (count < 0) throw "Error. Count_ in array always must be > -1\n";
	array_ = new int[count]; 
	count_ = count;
	for (int i = 0; i < count_; i++) array_[i] = copy[i];
}

Array::Array(const Array& copy)
{
	count_ = copy.count_;
	array_ = new int[count_];
	for (int i = 0; i < count_; i++) array_[i] = copy.array_[i];
}

void Array::output()
{
	for (int i = 0; i < count_; i++) cout << array_[i] << "  ";
	cout << '\n';
}


void Array::randArray(int minNumber, int maxNumber)
{
	for (int i = 0; i < count_; i++) array_[i] = rand() % (maxNumber + 1) + minNumber;
}

ofstream& operator <<(ofstream& file, Array& Mas)
{
	file << Mas.count_;
	for (int i = 0; i < Mas.count_; i++) file << ' ' << Mas.array_[i];
	file << '\n';
	return file;
}

ifstream& operator >>(ifstream& file, Array& Mas)
{
	int oldCount = Mas.count_;
	file >> Mas.count_;

	if (Mas.count_ == '\0') return file;

	if (oldCount != Mas.count_)
	{	
		delete Mas.array_;	
		Mas.array_ = new int[Mas.count_];
	}	
	for (int i = 0; i < Mas.count_; i++) file >> Mas.array_[i];

	return file;
}

void Array::sortingDirectInsert(int firstInd, int step)
{
	if ((firstInd > count_) || (firstInd < 0) || (step < 1)) throw "Error in direct insert sorting\n";

	for (int i = firstInd + step; i < count_; i += step)
	{
		int x = array_[i], j = i - step;
		while ((j >= firstInd) && (array_[j] > x))
		{
			array_[j + step] = array_[j];
			j -= step;
		}
		array_[j + step] = x;
	}
}

Array* Array::createStepArr1_(bool outputStep)                 // h_i = h_(i-1) / 2
{
	int count = 0;
	for (int i = count_; i > 1; i /= 2) count++;
	
	Array* result = new Array(count);
	result->array_[0] = count_ / 2;
	for (int i = 1; i < result->count_; i++) result->array_[i] = result->array_[i - 1] / 2;
	
	if (outputStep)
	{
		cout << "Step array: ";
		result->output();
	}
	return result;
}

Array* Array::createStepArr2_(bool outputStep)          // h_i = 2^i - 1
{
	int count = 1;
	int firstStep = 1;
	while ((2 * firstStep + 1) < count_)
	{
		firstStep = 2 * firstStep + 1;
		count++;
	}

	Array* result = new Array(count);
	result->array_[0] = firstStep;
	for (int i = 1; i < result->count_; i++) result->array_[i] = (result->array_[i - 1] + 1) / 2 - 1;

	
	if (outputStep)
	{
		cout << "Step array: ";
		result->output();
	}
	return result;
}

Array* Array::createStepArr3_(bool outputStep)           // h_(i+1) = 3*h_i +1
{
	int count = 1;
	int firstStep = 1;
	while ((3 * firstStep + 1) < count_)
	{
		firstStep = 3 * firstStep + 1;
		count++;
	}

	Array* result = new Array(count);
	result->array_[0] = firstStep;
	for (int i = 1; i < result->count_; i++) result->array_[i] = (result->array_[i - 1] - 1) / 3;


	if (outputStep)
	{
		cout << "Step array: ";
		result->output();
	}
	return result;
} 

int Array::sortingShell(int typeStepArray, bool outputStep)
{
	Array* stepArr;

	switch (typeStepArray)
	{
	case 1: 
		stepArr = this->createStepArr1_(outputStep);
		break;
	case 2: 
		stepArr = this->createStepArr2_(outputStep);
		break;
	case 3: 
		stepArr = this->createStepArr3_(outputStep);
		break;
	default: 
		throw "Error type step array\n";
		break;
	}

	int startTime = clock();
	
	for (int i = 0; i <= stepArr->count_ ; i++)
		for (int s = stepArr->array_[i], b = 0; b < s; b++)
			this->sortingDirectInsert(b, s);
	
	int endTime = clock();
	delete stepArr;

	return endTime - startTime;
}

bool Array::checkSorting()
{
	for (int i = 1; i < count_; i++)
		if (array_[i - 1] > array_[i])
			return false;
	return true;
}

void Array::operator =(const Array& copy)
{
	delete array_;

	count_ = copy.count_;
	array_ = new int[count_];
	for (int i = 0; i < count_; i++) array_[i] = copy.array_[i];
}


void createArrays(const char* fileName)
{
	ofstream fileArray(fileName);
	
	for (int count = 10000; count <= 1000000; count *= 10)
	{
		Array A(count);
		for (int range = 10; range <= 100000; range *= 100)
		{
			A.randArray(-range, range);

			fileArray << A;
		}
	}
}

void analysisShellSorting(const char* fileArraysName, const char* fileResultName)
{
	ifstream fileArray(fileArraysName);
	ofstream fileResult(fileResultName);
	
	int lastCount = -1;
	int range = 10;
	
	fileResult << "Shell sorting algorithm\n";
	fileResult << "  Arrays step:\n";
	fileResult << "    1: h_i = h_(i-1) / 2\n";
	fileResult << "    2: h_i = 2^i - 1\n";
	fileResult << "    3: h_(i+1) = 3*h_i +1\n\n";

	Array A;
	fileArray >> A;
	while (fileArray)
	{	
		if (lastCount != A.getCount()) fileResult << "Arrays with " << A.getCount() << " elements:\n";
		lastCount = A.getCount();

		fileResult << "    With range from " << -range << " to " << range << "\n";

		if (range == 100000) range = 10;
		else range *= 100;


		for (int typeSorting = 1; typeSorting <= 3; typeSorting++)
		{
			fileResult << "            Shell sorting " << typeSorting << " time: ";

			int summTime = 0;
			int countRepeatSorts = 3;
			for (int j = 0; j < countRepeatSorts; j++)
			{
				Array forSorting(A);
				int sortingTime = forSorting.sortingShell(typeSorting);

				fileResult << sortingTime << "ms  ";
				summTime += sortingTime;

				if (!forSorting.checkSorting())
				{
					fileResult << "Error! Array was not sorted\n";
					fileResult.close();
					fileArray.close();
					throw "Error! Array was not sorted\n";
				}
			}
			fileResult << "     Average: " << summTime / countRepeatSorts << '\n';
		}

		fileArray >> A;
	}
}

int main()
{
	srand(time(NULL));

	try
	{
		//createArrays("Array.txt");

		analysisShellSorting("Array.txt", "Result.txt");
	
	}
	
	
	catch (const char* errorString)
	{
		for (int i = 0; errorString[i] != '\0'; i++) cout << errorString[i];
		return 1;
	}
	
	return 0;
}

