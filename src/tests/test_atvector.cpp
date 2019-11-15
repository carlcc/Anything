#include <anything/ATVector.h>
#include <iostream>

class A {
public:
    A() {
        std::cout << (void*) this << " A()" << std::endl;
    }
    A(const A& a) {
        std::cout << (void*) this << " A(const A&)" << std::endl;
    }
    A(A&& a) {
        std::cout << (void*) this << " A(A&&)" << std::endl;
    }
    A& operator=(const A& a) {
        std::cout << (void*) this << " A=(const A&)" << std::endl;
        return *this;
    }
    A& operator=(A&& a) {
        std::cout << (void*) this << " A=(A&&)" << std::endl;
        return *this;
    }
    ~A() {
        std::cout << (void*) this << " ~A()" << std::endl;
    }

    int mData;
};

void test1()
{

    at::ATVector<A> vector;

    std::cout << " === A a; " << std::endl;
    A a;
    std::cout << " === vector.pushBack(a); " << std::endl;
    vector.pushBack(a);
    std::cout << " === vector.pushBack(std::move(a)); " << std::endl;
    vector.pushBack(std::move(a));
    std::cout << " === vector.pushBack(A()); " << std::endl;
    vector.pushBack(A());

    std::cout << " === A b; " << std::endl;
    A b;
    std::cout << " === vector[0] = b; " << std::endl;
    vector[0] = b;
    std::cout << " === vector[0] = std::move(b); " << std::endl;
    vector[0] = std::move(b);
    std::cout << " === vector.popBack(); " << std::endl;
    vector.popBack();

    std::cout << " === vector.clear(); " << std::endl;
    vector.clear();

    std::cout << "==== end" << std::endl;
}
void test2()
{

    at::ATVector<A> vector;

    std::cout << " === A a; " << std::endl;
    A a;
    std::cout << " === vector.pushBack(a); " << std::endl;
    vector.pushBack(a);
    std::cout << " === vector.pushBack(std::move(a)); " << std::endl;
    vector.pushBack(std::move(a));
    std::cout << " === vector.pushBack(A()); " << std::endl;
    vector.pushBack(A());

    std::cout << " === A b; " << std::endl;
    A b;
    std::cout << " === vector[0] = b; " << std::endl;
    vector[0] = b;
    std::cout << " === vector[0] = std::move(b); " << std::endl;
    vector[0] = std::move(b);
    std::cout << " === vector.popBack(); " << std::endl;
    vector.popBack();

    std::cout << "==== end" << std::endl;
}
int main()
{
    std::cout << "=== test1 ======" << std::endl;
    test1();
    std::cout << "=== test2 ======" << std::endl;
    test2();
    std::cout << "==== end ====" << std::endl;
    return 0;
}