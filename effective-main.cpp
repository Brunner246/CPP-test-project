//
// Created by MichaelBrunner on 31/03/2024.
//

#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include <future>
#include <list>
#include <deque>
#include <queue>
#include <array>

template<typename T>
void f(T *aParam) {
    if (aParam) {
        // do something
    }
}

struct Point {
    int x;
    int y;

};

auto lDeleteFunction = [](Point *aPoint) {
    std::cout << "Deleting Point" << std::endl;
    delete aPoint;
};

template<typename... Ts>
std::unique_ptr<Point, decltype(lDeleteFunction)> makePoint(Ts... args) {
    return std::unique_ptr<Point, decltype(lDeleteFunction)>(new Point{args...}, lDeleteFunction);
}

auto func() {
    std::cout << "Starting the function" << std::endl;
    int i = 0;
    while (5 > i) {
        std::stringstream ss;
        ss << "thread id " << std::this_thread::get_id() << " i: " << i;
        auto lFormatted = ss.str();
        std::cout << lFormatted << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        i++;
    }
    return .7 * 121;
}

struct Interface {
    virtual void doSomething() = 0;
};

class Implementation : public Interface {
public:
    void doSomething() override {
        std::cout << "Implementation::doSomething()" << std::endl;
    }

};

struct Strategy {
    virtual void execute() = 0;
};

// concept callable object
template<typename T>
concept Callable = requires(T t) {
    t();
};

template<typename Callable>
class MyStrategy : public Strategy {
private:
    Callable mCallable;

public:
    explicit MyStrategy(Callable &&aCallable)
            : mCallable(std::move(aCallable)) {
    }

    void execute() override {

        mCallable();
    }

};

class Document
{
    std::string mTitle;
public:
    explicit Document(std::string&& aTitle)
            : mTitle(std::move(aTitle))
    {
    }
};

struct Rank
{
    int mRanking;
    std::shared_ptr<Document> mDocument;
};

auto compareRank = [](const Rank& aLeft, const Rank& aRight)
{
    return aLeft.mRanking < aRight.mRanking;
};

using DocumentRankings = std::priority_queue<Rank, std::vector<Rank>, decltype(compareRank)>;

int main(int argc, char *argv[]) {

    DocumentRankings lRankings{compareRank};

    std::unique_ptr<Interface> lInterface = std::make_unique<Implementation>();
    lInterface->doSomething();

    auto lCallalbe = []{std::cout << "Lambda" << std::endl;};

    struct lFunctor {
        auto operator()()
        {
            std::cout << "Functor" << std::endl;
            return 12456;
        }
    };

    const std::unique_ptr<Strategy> lStrategy = std::make_unique<MyStrategy<decltype(lCallalbe)>>(std::move(lCallalbe));
    lStrategy->execute();
    const std::unique_ptr<Strategy> lStrategy2 = std::make_unique<MyStrategy<lFunctor>>(lFunctor{});
    lStrategy2->execute();

    const char name[] = "Michael Brunner";
    std::cout << name << std::endl;
    std::vector<decltype(std::async(func))> lFutures;
    for (int i = 0; i < 5; i++) {
        lFutures.push_back(std::async(std::launch::async, func));
    }
    // auto lPromise = std::async(func); // std::launch::async | std::launch::deferred,

    decltype(Point::x) lValue = 5;
    std::cout << lValue << std::endl;
    decltype(makePoint(1, 2)) lNewPoint;
    {
        lNewPoint = makePoint(1, 2);
        std::cout << lNewPoint->x << " " << lNewPoint->y << std::endl;
        lNewPoint.reset();
    }
    if (lNewPoint) {
        std::cout << "lNewPoint is valid" << std::endl;
    } else {
        std::cout << "lNewPoint is invalid" << std::endl;
    }
    auto spw = std::make_shared<Point>(1, 2);
    std::weak_ptr<Point> wpw{spw};
    spw = nullptr;
    if (wpw.expired()) {
        std::cout << "wpw is expired" << std::endl;
    } else {
        std::cout << "wpw is not expired" << std::endl;
    }
    try {
        std::shared_ptr<Point> lSharedPtr{wpw};
    } catch (const std::bad_weak_ptr &e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }

    std::cout << "Waiting for the results" << std::endl;
    for (auto &lFuture: lFutures) {
        std::cout << "Result: " << lFuture.get() << std::endl;
    }
    // std::cout << "Result: " << lPromise.get() << std::endl;

    std::vector<Point> lPoints{};
    lPoints.emplace_back(1, 2);
    lPoints.emplace_back(3, 4);
    lPoints.emplace_back(5, 6);

    std::cout << "Size: " << lPoints.size() << std::endl;
    std::erase_if(lPoints, [](const Point &aPoint) {
        return aPoint.x <= 3;
    });
    std::cout << "Size: " << lPoints.size() << std::endl;


    std::list<Point> lListPoints{};
    lListPoints.emplace_back(1, 2);
    lListPoints.emplace_back(3, 4);
    lListPoints.emplace_back(5, 6);

    for (auto &lPoint: lPoints) {
        std::cout << "Point: " << lPoint.x << " " << lPoint.y << std::endl;
    }

    std::deque<Point> lDequePoints{};
    lDequePoints.emplace_back(1, 2);
    lDequePoints.emplace_back(3, 4);
    lDequePoints.push_front({5, 6});

    for (auto &lPoint: lDequePoints) {
        std::cout << "Point: " << lPoint.x << " " << lPoint.y << std::endl;
    }

    auto lSharedInt1 = std::make_shared<int>(12345);
    auto lSharedInt2 = std::make_shared<int>(6789);

    std::vector<decltype(lSharedInt1)> lSharedInts{};
    lSharedInts.push_back(lSharedInt1);
    lSharedInts.push_back(lSharedInt2);

    std::array<decltype(lSharedInt1), 2> lArrayInts;
    lArrayInts[0] = lSharedInt1;
    lArrayInts[1] = lSharedInt2;

    std::ranges::for_each(lSharedInts, [](const auto& aInt) {
        std::cout << "Value: " << *aInt << std::endl;
    });
    std::ranges::for_each(lArrayInts, [](const auto& aInt) {
        std::cout << "Value: " << *aInt << std::endl;
    });
    *lSharedInt1 = 1993;

    std::ranges::for_each(lSharedInts, [](const auto& aInt) {
        std::cout << "Value: " << *aInt << std::endl;
    });

    std::ranges::for_each(lArrayInts, [](const auto& aInt) {
        std::cout << "Value: " << *aInt << std::endl;
    });

    return 0;
}