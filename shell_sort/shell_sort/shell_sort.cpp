#include <iostream>
#include <fstream>
#include <ctime>

#pragma comment(linker, "/STACK:41943040000")
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
	friend std::ofstream& operator <<(std::ofstream& file, Array& Mas);
	friend std::ifstream& operator >>(std::ifstream& file, Array& Mas);
	void output();

	void swap(int i, int j);

	int sortingShell(int typeStepArray, bool outputStep = false);
	void sortingDirectInsert(int firstInd = 0, int step = 1);

	int sortingHeap();
	void sift(int index, int n = -1);
	void siftWithoutClipboard(int i, int n = -1);

	int sortingHoare(int typeSorting, double rangeMin, double rangeMax, int indexLeft, int indexRight);

	void sortingHoare1(double rangeMin, double rangeMax, int indexLeft, int indexRight);
	void sortingHoare2(int indexLeft, int indexRight);
	void sortingHoare3(int indexLeft, int indexRight);

	int sortingBitwise();
	void sortingBitwiseRecursion(int indexLeft, int indexRight, int k);

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
	for (int i = 0; i < count_; i++) std::cout << array_[i] << "  ";
	std::cout << '\n';
}

inline void Array::swap(int i, int j)
{
	int c = array_[i];								
	array_[i] = array_[j];
	array_[j] = c;
}

void Array::randArray(int minNumber, int maxNumber)
{
	for (int i = 0; i < count_; i++) array_[i] = rand() % (maxNumber - minNumber + 1) + minNumber;
}

std::ofstream& operator <<(std::ofstream& file, Array& Mas)
{
	file << Mas.count_;
	for (int i = 0; i < Mas.count_; i++) file << ' ' << Mas.array_[i];
	file << '\n';
	return file;
}

std::ifstream& operator >>(std::ifstream& file, Array& Mas)
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
		std::cout << "Step array: ";
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
		std::cout << "Step array: ";
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
		std::cout << "Step array: ";
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


void Array::siftWithoutClipboard(int i, int n)
{
	
	if (n == -1) n = count_;
	else
	{
		if (i < 0) throw "Error in sift function. index < 0.\n";
		if (i >= count_) throw "Error in sift function. index >= count_.\n";
	}

	
	int j = 2 * i + 1;

	while (j < n)
	{
		if (j + 1 < n)									//проверяем существование правого потомка
			if (array_[j + 1] > array_[j]) j++;			//ищем максимум среди потомков

		if (array_[j] <= array_[i]) return;				//поверка выполнения условия дерева

		swap(i, j);										//array_[i] <=> array_[j]

		i = j;											//идем дальше по дереву
		j = 2 * i + 1;

	}
}

void Array::sift(int i, int n)
{

	if (n == -1) n = count_;
	else
	{
		if (n < 0) throw "Error in sift function. n < 0.\n";
		if (n > count_) throw "Error in sift function. n >= count_.\n";
	}


	int j = 2 * i + 1;
	int clipboard = array_[i];							//буфер обмена
	bool flag = true;									//флаг выхода из цикла

	while ((j < n) && flag)
	{
		if (j + 1 < n)									//проверяем существование правого потомка
			if (array_[j + 1] > array_[j]) j++;			//ищем максимум среди потомков

		if (array_[j] <= clipboard) flag = false;		//поверка выполнения условия дерева
		else
		{
			array_[i] = array_[j];						//сдвигаем элемент j ниже по дереву

			i = j;										//идем дальше по дереву
			j = 2 * i + 1;
		}
	}

	array_[i] = clipboard;
}

int Array::sortingHeap()
{
	if (count_ < 2) return 0;							// если в массиве 1 или меньше элементов, он не требует сортировки

	int startTime = clock();

	int lastElemWithDescendant = 1;

	while (lastElemWithDescendant < count_) lastElemWithDescendant = lastElemWithDescendant * 2 + 1;		//ищем предпоследний ярус дерева
	lastElemWithDescendant = (lastElemWithDescendant - 1) / 2;

	while (lastElemWithDescendant * 2 + 1 < count_) lastElemWithDescendant--;				//ищем последний элемент, у которого есть потомки

	
	int index = lastElemWithDescendant;
	while (index >= 0)									//формируем первоначальное сортирующее дерево
	{
		sift(index);
		index--;
	}


	int n = count_;

	while (n > 1)
	{
		swap(n - 1, 0);										//array_[0] <=> array_[n - 1]
	
		n--;
		sift(0, n);											//"просеиваем" 0 элемент
	}

	int endTime = clock();
	return endTime - startTime;
}

int Array::sortingHoare(int typeSorting, double rangeMin, double rangeMax, int indexLeft, int indexRight)
{
	//std::cout << count_ << "\n";

	int startTime = clock();
	std::cout << indexRight + 1 << '\n';
	switch (typeSorting)
	{
	case 1:
		sortingHoare1(rangeMin, rangeMax, indexLeft, indexRight);
		break;
	case 2:
		sortingHoare2(indexLeft, indexRight);
		break;
	case 3:
		sortingHoare3(indexLeft, indexRight);
		break;
	default:
		throw "Error type Hoare sorting\n";
		break;
	}

	int endTime = clock();
	return endTime - startTime;
}

void Array::sortingHoare1(double rangeMin, double rangeMax, int indexLeft, int indexRight)			//не работает на массивах, записанных в Array.txt		
{
	if ((indexLeft >= indexRight) || (rangeMax <= rangeMin)) return;
		
	/*
	int index = indexLeft + 1;
	while ((indexRight >= index) && (array_[index] == array_[indexLeft]))
		index++;

	if (index == indexRight + 1) return;										//проверка на то, что рассматриваемый подмассив не состоит из одинаковых элементов
	*/

	int i = indexLeft, j = indexRight;
	double rangeMedium = (rangeMin + rangeMax) / 2;
	


	while (i <= j)
	{
		while ((i <= j) && (array_[i] < rangeMedium)) i++;
		while ((i <= j) && (array_[j] >= rangeMedium)) j--;

		if (i < j)
		{
			swap(i, j);
			i++;
			j--;
		}
	}
	
	sortingHoare1(rangeMin,	rangeMedium - 0.5, indexLeft, j);

	sortingHoare1(rangeMedium + 0.5, rangeMax, i, indexRight);
}

void Array::sortingHoare2(int indexLeft, int indexRight)
{
	if (indexLeft >= indexRight) return;

	/*
	int index = indexLeft + 1;
	while ((indexRight >= index) && (array_[index] == array_[indexLeft]))
		index++;

	if (index == indexRight + 1) return;										//проверка на то, что рассматриваемый подмассив не состоит из одинаковых элементов
*/	

	int i = indexLeft + 1, j = indexRight;
	int rangeMedium = array_[indexLeft];

	while (i <= j)
	{
		while ((i <= j) && (array_[i] <= rangeMedium)) i++;
		while ((i <= j) && (array_[j] > rangeMedium)) j--;

		if (i < j)
		{
			swap(i, j);
			i++;
			j--;
		}
	}

	swap(indexLeft, j);
	j--;

	//std::cout << "Medium point : " << rangeMedium << " output: ";
	//std::cout << "from " << indexLeft << " to " << indexRight << " output: ";
	//output();

	sortingHoare2(indexLeft, j);


	sortingHoare2(i, indexRight);

}

void Array::sortingHoare3(int indexLeft, int indexRight)
{
	if (indexLeft >= indexRight) return;

	/*
	int index = indexLeft + 1;
	while ((indexRight >= index) && (array_[index] == array_[indexLeft]))
		index++;

	if (index == indexRight + 1) return;										//проверка на то, что рассматриваемый подмассив не состоит из одинаковых элементов
		*/																		//здесь она не обязательна для корректной работы, но с ней выполняется значительно быстрее

	int i = indexLeft, j = indexRight;
	int rangeMedium = array_[int((indexLeft + indexRight) / 2)];

	while (i < j)
	{
		while (array_[i] < rangeMedium) i++;
		while (array_[j] > rangeMedium) j--;

		if (i <= j)
		{
			swap(i, j);
			i++;
			j--;
		}
	}

	sortingHoare3(indexLeft, j);

	sortingHoare3(i, indexRight);
}


int Array::sortingBitwise()
{
	std::cout << count_ << '\n';
	int startTime = clock();

	int indexLeft = 0, indexRight = count_ - 1;
	if (indexLeft >= indexRight) return 0;

	int k = 31;

	/*
	int index = indexLeft + 1;
	while ((indexRight >= index) && (array_[index] == array_[indexLeft]))
		index++;

	if (index == indexRight + 1) return;										//проверка на то, что рассматриваемый подмассив не состоит из одинаковых элементов
*/

	int i = indexLeft, j = indexRight;
	int mask = 1 << k;

	while (i <= j)
	{
		while ((i <= j) && ((array_[i] & mask) != 0)) i++;
		while ((i <= j) && ((array_[j] & mask) == 0)) j--;

		if (i < j)
		{
			swap(i, j);
			i++;
			j--;
		}
	}


	sortingBitwiseRecursion(indexLeft, j, k-1);


	sortingBitwiseRecursion(i, indexRight, k-1);

	int endTime = clock();
	return endTime - startTime;
};

void Array::sortingBitwiseRecursion(int indexLeft, int indexRight, int k)
{
	if ((indexLeft >= indexRight) || (k < 0)) return;



	/*
	int index = indexLeft + 1;
	while ((indexRight >= index) && (array_[index] == array_[indexLeft]))
		index++;

	if (index == indexRight + 1) return;										//проверка на то, что рассматриваемый подмассив не состоит из одинаковых элементов
*/

	int i = indexLeft;
	int j = indexRight;
	int mask = 1 << k;

	while (i <= j)
	{
		while ((i <= j) && ((array_[i] & mask) == 0)) i++;
		while ((i <= j) && ((array_[j] & mask) != 0)) j--;

		if (i < j)
		{
			//std::cout << '+';
			swap(i, j);
			i++;
			j--;
		}
	}
	//std::cout << "\nfrom " << indexLeft << " to " << indexRight << " k = " << k << " : ";
	//output();

	sortingBitwiseRecursion(indexLeft, j, k-1);


	sortingBitwiseRecursion(i, indexRight, k-1);
};


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
	std::ofstream fileArray(fileName);
	
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
	std::ifstream fileArray(fileArraysName);
	std::ofstream fileResult(fileResultName);
	
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


void analysisHeapSorting(const char* fileArraysName, const char* fileResultName)
{
	std::ifstream fileArray(fileArraysName);
	std::ofstream fileResult(fileResultName);

	int lastCount = -1;
	int range = 10;

	fileResult << "Heap sorting algorithm\n";


	Array A;
	fileArray >> A;
	while (fileArray)
	{
		if (lastCount != A.getCount()) fileResult << "Arrays with " << A.getCount() << " elements:\n";
		lastCount = A.getCount();

		fileResult << "    With range from " << -range << " to " << range << "\n";

		if (range == 100000) range = 10;
		else range *= 100;


			fileResult << "            Heap sorting  time: ";

			int summTime = 0;
			int countRepeatSorts = 3;
			for (int j = 0; j < countRepeatSorts; j++)
			{
				Array forSorting(A);
				int sortingTime = forSorting.sortingHeap();

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

		fileArray >> A;
	}
}


void analysisHoareSorting(const char* fileArraysName, const char* fileResultName)
{
	std::ifstream fileArray(fileArraysName);
	std::ofstream fileResult(fileResultName);

	int lastCount = -1;
	int range = 10;

	fileResult << "Hoare sorting algorithm\n";

	Array A;
	fileArray >> A;
	while (fileArray)
	{
		if (lastCount != A.getCount()) fileResult << "Arrays with " << A.getCount() << " elements:\n";
		lastCount = A.getCount();

		fileResult << "    With range from " << -range << " to " << range << "\n";


		for (int typeSorting = 2; typeSorting <= 2; typeSorting++)
		{
			fileResult << "            Hoare sorting type " << typeSorting << " time: ";

			int summTime = 0;
			int countRepeatSorts = 3;
			for (int j = 0; j < countRepeatSorts; j++)
			{
				Array forSorting(A);
				int sortingTime = forSorting.sortingHoare(typeSorting, -range, range, 0, forSorting.getCount() - 1);

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

		if (range == 100000) range = 10;
		else range *= 100;

		fileArray >> A;
	}
}

void analysisBitwiseSorting(const char* fileArraysName, const char* fileResultName)
{
	std::ifstream fileArray(fileArraysName);
	std::ofstream fileResult(fileResultName);

	int lastCount = -1;
	int range = 10;

	fileResult << "Bitwise sorting algorithm\n";


	Array A;
	fileArray >> A;
	while (fileArray)
	{
		if (lastCount != A.getCount()) fileResult << "Arrays with " << A.getCount() << " elements:\n";
		lastCount = A.getCount();

		fileResult << "    With range from " << -range << " to " << range << "\n";

		if (range == 100000) range = 10;
		else range *= 100;


		fileResult << "            Bitwise sorting  time: ";

		int summTime = 0;
		int countRepeatSorts = 3;
		for (int j = 0; j < countRepeatSorts; j++)
		{
			Array forSorting(A);
			int sortingTime = forSorting.sortingBitwise();

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

		fileArray >> A;
	}
}


int main()
{
	srand(time(NULL));

	try
	{
		//createArrays("newArray.txt");

		//analysisShellSorting("Array.txt", "ResultShellSortingTest.txt");

		//analysisHeapSorting("Array.txt", "ResultHeapSorting.txt");

		analysisHoareSorting("Array.txt", "ResultHoareSorting.txt");
		
		//analysisBitwiseSorting("Array.txt", "ResultBitwiseSorting.txt");
		
		
		/*
		int arr[5] = { 5, 4, 3, 6, 1};
		int arrp[5] = { 5, 2, 4, 3, 1 };
		int arrn[5] = { -1, -2, -3, -4, -5 };
		Array test(arr, 10);
		test.randArray(-100, 100);
		test.output();
		test.sortingBitwise();
		test.output();
		std::cout << test.checkSorting();*/

	}
	
	
	catch (const char* errorString)
	{
		for (int i = 0; errorString[i] != '\0'; i++) std::cout << errorString[i];
		return 1;
	}
	
	return 0;
}

