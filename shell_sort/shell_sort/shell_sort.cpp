#include <iostream>
#include <ctime>
using namespace std;

int const MAX_NUMBER = 99;
int const DEFAULT_COUNT = 10;

class Array
{
	int* array_;
	int count_;

	Array* createStepArr_(bool outputStep);

public:
	Array(int count = DEFAULT_COUNT) { array_ = new int[count]; count_ = count; };
	Array(const int* copy, int count);
	~Array() { delete array_; };

	void randArray();
	void output();

	void sortingShell(bool outputStep = false);
	void sortingDirectInsert(int firstInd = 0, int step = 1);

	bool checkSorting();
};

Array::Array(const int* copy, int count)
{
	array_ = new int[count]; 
	count_ = count;
	for (int i = 0; i < count_; i++) array_[i] = copy[i];
}

void Array::output()
{
	for (int i = 0; i < count_; i++) cout << array_[i] << "  ";
	cout << '\n';
}


void Array::randArray()
{
	for (int i = 0; i < count_; i++) array_[i] = rand() % (MAX_NUMBER + 1);
}


void Array::sortingDirectInsert(int firstInd, int step)
{
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

Array* Array::createStepArr_(bool outputStep)
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

void Array::sortingShell(bool outputStep)
{
	Array* stepArr = this->createStepArr_(outputStep);
	for (int i = 0; i <= stepArr->count_ ; i++)
		for (int s = stepArr->array_[i], b = 0; b < s; b++)
			this->sortingDirectInsert(b, s);

	delete stepArr->array_;
}

bool Array::checkSorting()
{
	for (int i = 1; i < count_; i++)
		if (array_[i - 1] > array_[i])
			return false;
	return true;
}

int main()
{
	srand(time(NULL));

	if (true)
	{
		int b[5] = { 1, 28, 10, 9 };
		Array B(b, 5);
		B.sortingShell(true);
		cout << "Array after sorting: ";
		B.output();
	}
	else
	{
		Array A(100);
		for (int i = 0; i < 100000; i++)
		{
			A.randArray();
			A.sortingShell();
			if (!A.checkSorting())
			{
				cout << "Error: ";
				A.output();
			}
		}

	}

	return 0;
}


