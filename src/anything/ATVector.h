#pragma once
#include <array>
#include <vector>

namespace at {

template <class _T, uint64_t _TBLOCK_NUM = 1024 * 1024>
class ATVector {
public:
    using value_type = _T;

    ATVector()
        : mArrays()
        , mSize(0)
    {
    }

    ~ATVector()
    {
        clear();
        for (auto* arr : mArrays) {
            delete arr;
        }
    }

    void pushBack(const _T& ele)
    {
        auto index = mSize % _TBLOCK_NUM;
        auto blockIndex = mSize / _TBLOCK_NUM;
        if (index == 0) {
            if (blockIndex >= mArrays.size()) {
                mArrays.push_back(new std::array<char, sizeof(_T) * _TBLOCK_NUM>);
            }
        }
        std::array<char, sizeof(_T)* _TBLOCK_NUM>& arr = *mArrays[blockIndex];
        _T* p = reinterpret_cast<_T*>(arr.data());
        new (p + index) _T(ele);
        ++mSize;
    }

    void pushBack(_T&& ele)
    {
        auto index = mSize % _TBLOCK_NUM;
        auto blockIndex = mSize / _TBLOCK_NUM;
        if (index == 0) {
            if (blockIndex >= mArrays.size()) {
                mArrays.push_back(new std::array<char, sizeof(_T) * _TBLOCK_NUM>);
            }
        }
        std::array<char, sizeof(_T)* _TBLOCK_NUM>& arr = *mArrays[blockIndex];
        _T* p = reinterpret_cast<_T*>(arr.data());
        new (p + index) _T(std::forward<_T&&>(ele));
        ++mSize;
    }

    void popBack() {
        if (isEmpty()) {
            abort();
        }
        uint64_t lastIndex = mSize - 1;
        auto index = lastIndex % _TBLOCK_NUM;
        auto blockIndex = lastIndex / _TBLOCK_NUM;
        
        std::array<char, sizeof(_T)* _TBLOCK_NUM>& arr = *mArrays[blockIndex];
        _T* p = reinterpret_cast<_T*>(arr.data());
        (p + index)->~_T();
        --mSize;
    }

    void clear()
    {
        for (uint64_t i = 0; i < mSize; ++i) {
            (&operator[](i))->~_T();
        }
        mSize = 0;
    }

    void resize(uint64_t size)
    {
        if (mSize < size) {
            size -= mSize;
            while (size > 0) {
                pushBack(_T());
            }
        } else if (mSize > size) {
            while (mSize-- > size) {
                popBack();
            }
        }
        mSize = size;
    }

    uint64_t size() const
    {
        return mSize;
    }

    bool isEmpty() const
    {
        return size() == 0;
    }

    _T& operator[](uint64_t index)
    {
        if (index >= mSize) {
            abort();
        }

        auto idx = index % _TBLOCK_NUM;
        std::array<char, sizeof(_T)* _TBLOCK_NUM>& arr = *mArrays[index / _TBLOCK_NUM];
        _T* p = reinterpret_cast<_T*>(arr.data());
        return p[idx];
    }

    const _T& operator[](uint64_t index) const
    {
        if (index >= mSize) {
            abort();
        }

        auto idx = index % _TBLOCK_NUM;
        std::array<char, sizeof(_T)* _TBLOCK_NUM>& arr = *mArrays[index / _TBLOCK_NUM];
        _T* p = reinterpret_cast<_T*>(arr.data());
        return p[idx];
    }

private:
    std::vector<std::array<char, sizeof(_T) * _TBLOCK_NUM>*> mArrays;
    uint64_t mSize;
};

}
