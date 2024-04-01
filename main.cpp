#include <iostream>
#include <functional>
#include <utility>
#include <bits/ranges_algo.h>
#include <memory>
#include <span>
#include <list>

class Strategy {
public:
    virtual void execute() = 0;
};

class ConcreteStrategyA : public Strategy {
public:
    void execute() override {
        std::cout << "ConcreteStrategyA::execute()\n";
    }
};

class Context {
public:
    explicit Context(Strategy *strategy)
            : strategy_(strategy) {
    }

    void execute() {
        strategy_->execute();
    }

private:
    Strategy *strategy_;
};

class CallbackContext {
public:
    explicit CallbackContext(std::function<void(void)> aCallback)
            : mCallback(std::move(aCallback)) {
    }

    void execute() {
        mCallback();
    }

private:
    // void (*callback_)();
    std::function<void(void)> mCallback;
};

template<int n>
struct product {
    static const int value = 2 * product<n - 1>::value;
};

template<>
struct product<0> {
    static constexpr int value{1};
};

template<typename A>
struct Functor {
    enum Direction {
        ascending,
        descending
    };

    explicit Functor(Direction const aSortDirection)
            : mDirection(aSortDirection) {
        switch (mDirection) {
            case ascending:
                sorter = [](A aLeft, A aRight) { return aLeft < aRight; };
                break;
            case descending:
                sorter = [](A aLeft, A aRight) { return aLeft > aRight; };
        }
    }

    auto operator()(A aLeft, A aRight) {
        return sorter(aLeft, aRight);
    }

private:
    Direction mDirection;

    bool (*sorter)(A, A) = nullptr;
};

template<typename T>
struct MyStruct {
    std::function<bool(T, T)> greaterThan = [](T aLeft, T aRight) { return aLeft > aRight; };

    explicit operator bool() {
        // if T is int return true
        if (std::is_same<T, int>::value) {
            return true;
        }
        return false;
    }

    explicit operator std::string() {
        return "MyStruct";
    }
};

template<typename T, int N>
void printArray(T (&aArray)[N]) {
    for (auto &lValue: aArray) {
        std::cout << lValue << "";
    }
}

void printArray(std::span<int> aArray) {
    for (auto &lValue: aArray) {
        std::cout << lValue << "";
    }
}

int main() {

    int lArray[] = {1, 2, 3, 4, 5};
    printArray(lArray);
    std::cout << std::endl;
    printArray(std::span<int>(lArray));
    std::cout << std::endl;

    const int *ptr = new int(120);
    std::cout << *ptr << std::endl;
    [&ptr]() { std::cout << ptr << std::endl; }();
    std::cout << *ptr << std::endl;
    delete ptr;

    if (MyStruct<int>()) {
        std::cout << "true\n";
        std::cout << static_cast<std::string>(MyStruct<int>()) << std::endl;
    } else {
        std::cout << "false\n";
    }
    auto l1 = std::make_shared<MyStruct<int>>();
    auto l2 = l1;
    auto l3 = l2;
    l2->greaterThan(10, 20);
    std::cout << "Ref count: " << l1.use_count() << std::endl;
    std::cout << product<12>::value << std::endl;
    std::cout << "Hello, World!" << std::endl;
    const int *lPtr{nullptr};
    int lValue{10};
    lPtr = &lValue;
    lValue = 22;
    // *lPtr = 33; this is not possible because lPtr is a pointer to a const int
    std::cout << "lPtr: " << *lPtr << std::endl;
    int lValue2{20};
    lPtr = &lValue2;
    std::cout << "lPtr: " << *lPtr << std::endl;
    const int *lPtr2{&lValue};
    lPtr = lPtr2;
    lPtr++;
    std::cout << "lPtr: " << *lPtr << std::endl;

    auto lCallback = [value = lValue]() {
        std::cout << "Callback executed\n";
        std::cout << "value: " << value << std::endl;
    };

    auto lCallbackContext = CallbackContext(lCallback);
    lCallbackContext.execute();

    std::vector lIntValues = {1, 2, 3, 4, 5, 6, 7, 8, 9, 9, 9, 9};
    std::ranges::sort(lIntValues, std::greater<>());
    std::sort(lIntValues.begin(), lIntValues.end(), Functor<int>(Functor<int>::descending));
    lIntValues.reserve(25);
    std::cout << lIntValues.capacity();
    using constIter = std::vector<int>::const_iterator;
    for (constIter il = std::cbegin(lIntValues); il != std::cend(lIntValues); ++il) {
        std::cout << *il << std::endl;
    }

    auto some_func = [numbers = std::list<int>{4, 2}]() {
        for (auto i: numbers) std::cout << i;
    };
    some_func();
    std::cout << std::endl;
    void (*callback)(int, const char *);
    callback = [](int a, const char *b) {
        std::cout << "a: " << a << " b: " << b << std::endl;
    };
    callback(1, "Hello");

    return EXIT_SUCCESS;
}
