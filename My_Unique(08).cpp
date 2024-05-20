#include <iostream>
#include <memory>
using namespace std;

template<typename T>
class MyUnique {
private:
	T* p = nullptr;
public:
	MyUnique(T* p) {
		this->p = p;
		cout << "Construct" << endl;
	}
	~MyUnique() {
		delete p;
		cout << "Destruct" << endl;
	}
	T* get() const {
		return p;
	}
	T& operator*() {
		return *p;
	}
	T* operator->() {
		return p;
	}

	// Запрет копирования
	MyUnique(const MyUnique&) = delete;
	MyUnique& operator=(const MyUnique&) = delete;

	// Операторы перемещения
	MyUnique(MyUnique&& other) noexcept : p(other.p) {
		other.p = nullptr;
	}
	MyUnique& operator=(MyUnique&& other) noexcept {
		if (this != &other) {
			delete p;
			p = other.p;
			other.p = nullptr;
		}
		return *this;
	}
};

template<class T, class... Args>
MyUnique<T> Make_MyUnique(Args&&... args) {
	return MyUnique<T>(new T(forward<Args>(args)...));
}

class MyPoint {
	int x, y;
public:
	MyPoint(int x, int y) : x(x), y(y) {}
	void print() const {
		cout << "Point(" << x << "," << y << ")" << endl;
	}
};

int main() {

	auto point = Make_MyUnique<MyPoint>(1, 2);
	point->print();

	return 0;
}