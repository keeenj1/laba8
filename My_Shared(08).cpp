#include <iostream>
#include<memory>
using namespace std;

template<class T>
class MyShared {
    T* p = nullptr;
    int* count = nullptr;
public:
    MyShared(T* p) {
        this->p = p;
        this->count = new int(1);
        cout << "Construct" << endl;
    }
    ~MyShared() {
        if (--(*count) == 0) {
            delete p;
            delete count;
            cout << "Destruct" << endl;
        }
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

    // Операторы копирования
    MyShared(const MyShared& other) : p(other.p), count(other.count) {
        ++(*count);
    }

    MyShared& operator=(const MyShared& other) {
        if (this != &other) {
            if (--(*count) == 0) {
                delete p;
                delete count;
            }
            p = other.p;
            count = other.count;
            ++(*count);
        }
        return *this;
    }

    // Операторы перемещения
    MyShared(MyShared&& other) noexcept : p(other.p), count(other.count) {
        other.p = nullptr;
        other.count = nullptr;
    }

    MyShared& operator=(MyShared&& other) noexcept {
        if (this != &other) {
            if (--(*count) == 0) {
                delete p;
                delete count;
            }
            p = other.p;
            count = other.count;
            other.p = nullptr;
            other.count = nullptr;
        }
        return *this;
    }
};

// Глобальная функция для создания MyShared
template<class T, class... Args>
MyShared<T> Make_MyShared(Args&&... args) {
    return MyShared<T>(new T(forward<Args>(args)...));
}

class MyPoint {
    int x, y;
public:
    MyPoint(int x, int y) : x(x), y(y) {}
    void print() const {
        cout << "Point(" << x << "," << y << ")" << endl;
    }
};

int main1() {

	auto sharedp1 = Make_MyShared<MyPoint>(3, 4);
	auto sharedp2 = sharedp1;
	sharedp1->print();
	sharedp2->print();

	return 0;
}