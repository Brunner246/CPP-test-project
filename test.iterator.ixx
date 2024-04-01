//
// Created by MichaelBrunner on 01/04/2024.
//
module;

#include <iterator>
#include <list>

export module test.iterator;

template<typename Container>
concept ContainerConcept = requires(Container c) {
    { c.begin() } -> std::input_or_output_iterator;
    { c.end() } -> std::input_or_output_iterator;
};

template<typename E>
struct Iterator
{
    virtual void start() = 0;
    virtual void next() = 0;
    virtual bool isDone() = 0;
    virtual E currentItem() = 0;
};

template<typename E>
struct PlayList
{
    virtual Iterator<E> createIterator() = 0;
};

template<typename E>
class IteratorImpl : public Iterator<E>
{
    std::list<E> mItems;
    typename std::list<E>::iterator mIter;
public:
    explicit IteratorImpl(std::list<E> items) : mItems(items)
    {
        mIter = mItems.begin();
    }

    void start() override
    {
        mIter = mItems.begin();
    }

    void next() override
    {
        mIter++;
    }

    bool isDone() override
    {
        return mIter == mItems.end();
    }

    E currentItem() override
    {
        return *mIter;
    }
};


template<typename E>
class MusicLibrary : public PlayList<E>
{
    std::list<E> mSongs;
public:
    MusicLibrary() = default;
    Iterator<E> createIterator() override
    {
        return IteratorImpl<E>(mSongs);
    }

};