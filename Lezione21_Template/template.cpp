#include <vector>
#include <iostream>
#include <string>

using namespace std;

template<typename T>
class Wrapper {
    public:
        Wrapper(T v) {
            value = v;
        }

        T get() {
            return value;
        }

        void set(T v) {
            value = v;
        }

        Wrapper<T> operator+(const Wrapper<T>& value) const {
            Wrapper<T> result = Wrapper<T>{value.value+1.0};
            return result;
        }

    private:
        T value;
};

template<typename T>
T increment(T value) {
    return value+1.0;
}

class A {

};

int main() {

    Wrapper<int> wint = Wrapper<int>{1};
    wint = increment(wint);

    Wrapper<double> wdouble = Wrapper<double>{1.0};
    wdouble = increment(wdouble);
    cout << wdouble.get() <<endl;


    

/*    Wrapper<int> wint = Wrapper<int>{1};
    Wrapper<double> wdouble = Wrapper<double>{1};

    wdouble.set(2);

    wint = wdouble;

    cout << wdouble.get() << endl;
    */return 0;
}