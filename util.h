#ifndef UTIL_H
#define UTIL_H
#include <cmath>
#include <random>
#include <cstdint>
#include <list>
#include <set>
#include <functional>
#include <cassert>
#include <cwchar>
#include <string>
#include <cstring>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <iterator>
using namespace std;
const float eps = 1e-4;
template <typename T>

/**
 * @brief Write what the function does here
 *
 * @param v
 * @param minV
 * @param maxV
 *
 * @retval (variable) (description of variable)
 **/
inline const T limit(const T v, const T minV, const T maxV) // returns the value v limited to the range minV <= v <= maxV
{

    if(v > maxV)
    {
        return maxV;
    }

    if(minV > v)
    {
        return minV;
    }
    return v;
}

/**
 * @brief Write what the function does here
 *
 * @param v
 *
 * @retval (variable) (description of variable)
 **/
inline int ifloor(float v) // returns the floor of v as an int
{
    return floor(v);
}

/**
 * @brief Write what the function does here
 *
 * @param v
 *
 * @retval (variable) (description of variable)
 **/
inline int iceil(float v) // returns the ceil of v as an int
{
    return ceil(v);
}
template <typename T>

/**
 * @brief Write what the function does here
 *
 * @param v
 * @param negative
 *
 * @retval (variable) (description of variable)
 **/
inline int sgn(T v) // the sign function : returns -1 if v is negative, 1 if v is positive and 0 if v == 0
{

    if(v < 0)
    {
        return -1;
    }

    if(v > 0)
    {
        return 1;
    }
    return 0;
}
template <typename T>

/**
 * @brief Write what the function does here
 *
 * @param t
 * @param a
 * @param b
 * @param 0
 *
 * @retval (variable) (description of variable)
 **/
inline const T interpolate(const float t, const T a, const T b) // linear interpolation : returns a if t == 0, b if t == 1 and interpolates in between
{
    return a + t * (b - a);
}

/**
 * @brief Write what the function does here
 *
 * @retval (variable) (description of variable)
 **/
class initializer // class for running a function before main
{
    private:
        void (*finalizeFn)();
        initializer(const initializer &rt) = delete;
        void operator =(const initializer &rt) = delete;
    public:
        initializer(void (*initFn)(), void (*finalizeFn)() = nullptr)
            : finalizeFn(finalizeFn)
        {
            initFn();
        }

        /**
         * @brief Write what the function does here
         *
         * @retval (variable) (description of variable)
         **/
        ~initializer()
        {

            if(finalizeFn)
            {
                finalizeFn();
            }
        }
};

/**
 * @brief Write what the function does here
 *
 * @retval (variable) (description of variable)
 **/
class finalizer // class for running a function after main or when exit is called
{
    private:
        void (*finalizeFn)();
        finalizer(const finalizer &rt) = delete;
        void operator =(const finalizer &rt) = delete;
    public:
        finalizer(void (*finalizeFn)())
            : finalizeFn(finalizeFn)
        {
            assert(finalizeFn);
        }

        /**
         * @brief Write what the function does here
         *
         * @retval (variable) (description of variable)
         **/
        ~finalizer()
        {
            finalizeFn();
        }
};

/**
 * @brief Write what the function does here
 *
 * @param wstr
 *
 * @retval (variable) (description of variable)
 **/
inline string wstringToString(wstring wstr)
{
    size_t destLen = wstr.length() * 4 + 1 + 32/*for extra buffer space*/;
    char *str = new char[destLen];
    const wchar_t *ptr = wstr.c_str();
    mbstate_t mbstate;
    memset((void *)&mbstate, 0, sizeof(mbstate));
    size_t v = wcsrtombs(str, &ptr, destLen - 1, &mbstate);

    if(v == (size_t) - 1)
    {
        delete []str;
        throw runtime_error("can't convert wide character string to multi-byte string");
    }
    str[v] = '\0';
    string retval = str;
    delete []str;
    return retval;
}

/**
 * @brief Write what the function does here
 *
 * @param str
 *
 * @retval (variable) (description of variable)
 **/
inline wstring stringToWString(string str)
{
    size_t destLen = str.length() + 1 + 32/* for extra buffer space*/;
    wchar_t *wstr = new wchar_t[destLen];
    const char *ptr = str.c_str();
    mbstate_t mbstate;
    memset((void *)&mbstate, 0, sizeof(mbstate));
    size_t v = mbsrtowcs(wstr, &ptr, destLen - 1, &mbstate);

    if(v == (size_t) - 1)
    {
        delete []wstr;
        throw runtime_error("can't convert multi-byte string to wide character string");
    }
    wstr[v] = '\0';
    wstring retval = wstr;
    delete []wstr;
    return retval;
}
template <typename T, size_t arraySize>

/**
 * @brief Write what the function does here
 *
 * @retval (variable) (description of variable)
 **/
class circularDeque final // a deque class like #include <deque> but implemented as a circular queue
{
    public:
        typedef T value_type;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
        typedef T &reference;
        typedef const T &const_reference;
        typedef T *pointer;
        typedef const T *const_pointer;

        /**
         * @brief Write what the function does here
         *
         * @retval (variable) (description of variable)
         **/
        static constexpr size_type capacity()
        {
            return arraySize;
        }
    private:
        size_type frontIndex, backIndex;
        value_type array[arraySize];
    public:
        friend class iterator;

        /**
         * @brief Write what the function does here
         *
         * @param random_access_iterator_tag
         *
         * @retval (variable) (description of variable)
         **/
        class iterator final : public std::iterator<random_access_iterator_tag, value_type>
    {
        friend class circularDeque;
        friend class const_iterator;
        private:
        circularDeque *container;
        size_t index;
        iterator(circularDeque *container, size_t index)
            : container(container), index(index)
        {
        }
        public:
        iterator()
            : container(nullptr)
        {
        }

        /**
         * @brief Write what the function does here
         *
         * @param n
         *
         * @retval (variable) (description of variable)
         **/
        iterator &operator +=(difference_type n)
        {

            if(-n > (difference_type)index)
            {
                n = n % arraySize + arraySize;
            }
            index += n;
            index %= arraySize;

            if(index < 0)
            {
                index += arraySize;
            }
            return *this;
        }

        /**
         * @brief Write what the function does here
         *
         * @param n
         *
         * @retval (variable) (description of variable)
         **/
        iterator &operator -=(difference_type n)
        {
            return *this += -n;
        }

        /**
         * @brief Write what the function does here
         *
         * @param n
         * @param i
         *
         * @retval (variable) (description of variable)
         **/
        friend iterator operator +(difference_type n, iterator i)
        {
            return i += n;
        }

        /**
         * @brief Write what the function does here
         *
         * @param i
         * @param n
         *
         * @retval (variable) (description of variable)
         **/
        friend iterator operator +(iterator i, difference_type n)
        {
            return i += n;
        }

        /**
         * @brief Write what the function does here
         *
         * @param i
         * @param n
         *
         * @retval (variable) (description of variable)
         **/
        friend iterator operator -(iterator i, difference_type n)
        {
            return i -= n;
        }

        difference_type operator -(const iterator &r) const
        {
            assert(container == r.container && container != nullptr);
            difference_type loc = index + arraySize - container->frontIndex;

            if(loc >= arraySize)
            {
                loc -= arraySize;
            }

            if(loc >= arraySize)
            {
                loc -= arraySize;
            }
            difference_type rloc = r.index + arraySize - container->frontIndex;

            if(rloc >= arraySize)
            {
                rloc -= arraySize;
            }

            if(rloc >= arraySize)
            {
                rloc -= arraySize;
            }
            return loc - rloc;
        }

        /**
         * @brief Write what the function does here
         *
         * @param n
         *
         * @retval (variable) (description of variable)
         **/
        T &operator [](difference_type n) const
        {
            return *(*this + n);
        }

        /**
         * @brief Write what the function does here
         *
         * @retval (variable) (description of variable)
         **/
        T &operator *() const
        {
            return container->array[index];
        }

        /**
         * @brief Write what the function does here
         *
         * @retval (variable) (description of variable)
         **/
        T *operator ->() const
        {
            return container->array + index;
        }

        /**
         * @brief Write what the function does here
         *
         * @retval (variable) (description of variable)
         **/
        const iterator &operator --()
        {

            if(index == 0)
            {
                index = arraySize - 1;
            }

            else
            {
                index--;
            }
            return *this;
        }

        /**
         * @brief Write what the function does here
         *
         * @param int
         *
         * @retval (variable) (description of variable)
         **/
        iterator operator --(int)
        {
            iterator retval = *this;

            if(index == 0)
            {
                index = arraySize - 1;
            }

            else
            {
                index--;
            }
            return retval;
        }

        /**
         * @brief Write what the function does here
         *
         * @retval (variable) (description of variable)
         **/
        const iterator &operator ++()
        {

            if(index >= arraySize - 1)
            {
                index = 0;
            }

            else
            {
                index++;
            }
            return *this;
        }

        /**
         * @brief Write what the function does here
         *
         * @param int
         *
         * @retval (variable) (description of variable)
         **/
        iterator operator ++(int)
        {
            iterator retval = *this;

            if(index >= arraySize - 1)
            {
                index = 0;
            }

            else
            {
                index++;
            }
            return retval;
        }

        /**
         * @brief Write what the function does here
         *
         * @param l
         * @param r
         *
         * @retval (variable) (description of variable)
         **/
        friend bool operator ==(const iterator &l, const iterator &r)
        {
            return l.index == r.index;
        }

        /**
         * @brief Write what the function does here
         *
         * @param l
         * @param r
         *
         * @retval (variable) (description of variable)
         **/
        friend bool operator !=(const iterator &l, const iterator &r)
        {
            return l.index != r.index;
        }

        /**
         * @brief Write what the function does here
         *
         * @param l
         * @param r
         *
         * @retval (variable) (description of variable)
         **/
        friend bool operator >(const iterator &l, const iterator &r)
        {
            return (l - r) > 0;
        }

        /**
         * @brief Write what the function does here
         *
         * @param l
         * @param r
         *
         * @retval (variable) (description of variable)
         **/
        friend bool operator >=(const iterator &l, const iterator &r)
        {
            return (l - r) >= 0;
        }

        /**
         * @brief Write what the function does here
         *
         * @param l
         * @param r
         *
         * @retval (variable) (description of variable)
         **/
        friend bool operator <(const iterator &l, const iterator &r)
        {
            return (l - r) < 0;
        }

        /**
         * @brief Write what the function does here
         *
         * @param l
         * @param r
         *
         * @retval (variable) (description of variable)
         **/
        friend bool operator <=(const iterator &l, const iterator &r)
        {
            return (l - r) <= 0;
        }
    };
        friend class const_iterator;

        /**
         * @brief Write what the function does here
         *
         * @param random_access_iterator_tag
         *
         * @retval (variable) (description of variable)
         **/
        class const_iterator final : public std::iterator<random_access_iterator_tag, const value_type>
    {
        friend class circularDeque;
        private:
        const circularDeque *container;
        size_t index;
        const_iterator(const circularDeque *container, size_t index)
            : container(container), index(index)
        {
        }
        public:
        const_iterator()
            : container(nullptr)
        {
        }
        const_iterator(const iterator &v)
            : container(v.container), index(v.index)
        {
        }

        /**
         * @brief Write what the function does here
         *
         * @param n
         *
         * @retval (variable) (description of variable)
         **/
        const_iterator &operator +=(difference_type n)
        {

            if(-n > (difference_type)index)
            {
                n = n % arraySize + arraySize;
            }
            index += n;
            index %= arraySize;

            if(index < 0)
            {
                index += arraySize;
            }
            return *this;
        }

        /**
         * @brief Write what the function does here
         *
         * @param n
         *
         * @retval (variable) (description of variable)
         **/
        const_iterator &operator -=(difference_type n)
        {
            return *this += -n;
        }

        /**
         * @brief Write what the function does here
         *
         * @param n
         * @param i
         *
         * @retval (variable) (description of variable)
         **/
        friend const_iterator operator +(difference_type n, const_iterator i)
        {
            return i += n;
        }

        /**
         * @brief Write what the function does here
         *
         * @param i
         * @param n
         *
         * @retval (variable) (description of variable)
         **/
        friend const_iterator operator +(const_iterator i, difference_type n)
        {
            return i += n;
        }

        /**
         * @brief Write what the function does here
         *
         * @param i
         * @param n
         *
         * @retval (variable) (description of variable)
         **/
        friend const_iterator operator -(const_iterator i, difference_type n)
        {
            return i -= n;
        }

        difference_type operator -(const const_iterator &r) const
        {
            assert(container == r.container && container != nullptr);
            difference_type loc = index + arraySize - container->frontIndex;

            if(loc >= arraySize)
            {
                loc -= arraySize;
            }

            if(loc >= arraySize)
            {
                loc -= arraySize;
            }
            difference_type rloc = r.index + arraySize - container->frontIndex;

            if(rloc >= arraySize)
            {
                rloc -= arraySize;
            }

            if(rloc >= arraySize)
            {
                rloc -= arraySize;
            }
            return loc - rloc;
        }

        /**
         * @brief Write what the function does here
         *
         * @param n
         *
         * @retval (variable) (description of variable)
         **/
        const T &operator [](difference_type n) const
        {
            return *(*this + n);
        }

        /**
         * @brief Write what the function does here
         *
         * @retval (variable) (description of variable)
         **/
        const T &operator *() const
        {
            return container->array[index];
        }

        /**
         * @brief Write what the function does here
         *
         * @retval (variable) (description of variable)
         **/
        const T *operator ->() const
        {
            return container->array + index;
        }

        /**
         * @brief Write what the function does here
         *
         * @retval (variable) (description of variable)
         **/
        const const_iterator &operator --()
        {

            if(index == 0)
            {
                index = arraySize - 1;
            }

            else
            {
                index--;
            }
            return *this;
        }

        /**
         * @brief Write what the function does here
         *
         * @param int
         *
         * @retval (variable) (description of variable)
         **/
        const_iterator operator --(int)
        {
            const_iterator retval = *this;

            if(index == 0)
            {
                index = arraySize - 1;
            }

            else
            {
                index--;
            }
            return retval;
        }

        /**
         * @brief Write what the function does here
         *
         * @retval (variable) (description of variable)
         **/
        const const_iterator &operator ++()
        {

            if(index >= arraySize - 1)
            {
                index = 0;
            }

            else
            {
                index++;
            }
            return *this;
        }

        /**
         * @brief Write what the function does here
         *
         * @param int
         *
         * @retval (variable) (description of variable)
         **/
        const_iterator operator ++(int)
        {
            const_iterator retval = *this;

            if(index >= arraySize - 1)
            {
                index = 0;
            }

            else
            {
                index++;
            }
            return retval;
        }

        /**
         * @brief Write what the function does here
         *
         * @param l
         * @param r
         *
         * @retval (variable) (description of variable)
         **/
        friend bool operator ==(const const_iterator &l, const const_iterator &r)
        {
            return l.index == r.index;
        }

        /**
         * @brief Write what the function does here
         *
         * @param l
         * @param r
         *
         * @retval (variable) (description of variable)
         **/
        friend bool operator !=(const const_iterator &l, const const_iterator &r)
        {
            return l.index != r.index;
        }

        /**
         * @brief Write what the function does here
         *
         * @param l
         * @param r
         *
         * @retval (variable) (description of variable)
         **/
        friend bool operator >(const const_iterator &l, const const_iterator &r)
        {
            return (l - r) > 0;
        }

        /**
         * @brief Write what the function does here
         *
         * @param l
         * @param r
         *
         * @retval (variable) (description of variable)
         **/
        friend bool operator >=(const const_iterator &l, const const_iterator &r)
        {
            return (l - r) >= 0;
        }

        /**
         * @brief Write what the function does here
         *
         * @param l
         * @param r
         *
         * @retval (variable) (description of variable)
         **/
        friend bool operator <(const const_iterator &l, const const_iterator &r)
        {
            return (l - r) < 0;
        }

        /**
         * @brief Write what the function does here
         *
         * @param l
         * @param r
         *
         * @retval (variable) (description of variable)
         **/
        friend bool operator <=(const const_iterator &l, const const_iterator &r)
        {
            return (l - r) <= 0;
        }
    };
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
        circularDeque()
            : frontIndex(0), backIndex(0)
        {
        }

        /**
         * @brief Write what the function does here
         *
         * @retval (variable) (description of variable)
         **/
        iterator begin()
        {
            return iterator(this, frontIndex);
        }

        /**
         * @brief Write what the function does here
         *
         * @retval (variable) (description of variable)
         **/
        const_iterator begin() const
        {
            return const_iterator(this, frontIndex);
        }

        /**
         * @brief Write what the function does here
         *
         * @retval (variable) (description of variable)
         **/
        const_iterator cbegin() const
        {
            return const_iterator(this, frontIndex);
        }

        /**
         * @brief Write what the function does here
         *
         * @retval (variable) (description of variable)
         **/
        iterator end()
        {
            return iterator(this, backIndex);
        }

        /**
         * @brief Write what the function does here
         *
         * @retval (variable) (description of variable)
         **/
        const_iterator end() const
        {
            return const_iterator(this, backIndex);
        }

        /**
         * @brief Write what the function does here
         *
         * @retval (variable) (description of variable)
         **/
        const_iterator cend() const
        {
            return const_iterator(this, backIndex);
        }

        /**
         * @brief Write what the function does here
         *
         * @retval (variable) (description of variable)
         **/
        reverse_iterator rbegin()
        {
            return reverse_iterator(end());
        }

        /**
         * @brief Write what the function does here
         *
         * @retval (variable) (description of variable)
         **/
        const_reverse_iterator rbegin() const
        {
            return const_reverse_iterator(cend());
        }

        /**
         * @brief Write what the function does here
         *
         * @retval (variable) (description of variable)
         **/
        const_reverse_iterator crbegin() const
        {
            return const_reverse_iterator(cend());
        }

        /**
         * @brief Write what the function does here
         *
         * @retval (variable) (description of variable)
         **/
        reverse_iterator rend()
        {
            return reverse_iterator(begin());
        }

        /**
         * @brief Write what the function does here
         *
         * @retval (variable) (description of variable)
         **/
        const_reverse_iterator rend() const
        {
            return const_reverse_iterator(cbegin());
        }

        /**
         * @brief Write what the function does here
         *
         * @retval (variable) (description of variable)
         **/
        const_reverse_iterator crend() const
        {
            return const_reverse_iterator(cbegin());
        }

        /**
         * @brief Write what the function does here
         *
         * @retval (variable) (description of variable)
         **/
        T &front()
        {
            return *begin();
        }

        /**
         * @brief Write what the function does here
         *
         * @retval (variable) (description of variable)
         **/
        const T &front() const
        {
            return *begin();
        }

        /**
         * @brief Write what the function does here
         *
         * @retval (variable) (description of variable)
         **/
        T &back()
        {
            return end()[-1];
        }

        /**
         * @brief Write what the function does here
         *
         * @retval (variable) (description of variable)
         **/
        const T &back() const
        {
            return end()[-1];
        }

        /**
         * @brief Write what the function does here
         *
         * @retval (variable) (description of variable)
         **/
        size_type size() const
        {
            return cend() - cbegin();
        }

        /**
         * @brief Write what the function does here
         *
         * @param pos
         *
         * @retval (variable) (description of variable)
         **/
        T &at(size_type pos)
        {

            if(pos >= size())
            {
                throw out_of_range("position out of range in circularDeque::at");
            }
            return begin()[pos];
        }

        /**
         * @brief Write what the function does here
         *
         * @param pos
         *
         * @retval (variable) (description of variable)
         **/
        const T &at(size_type pos) const
        {

            if(pos >= size())
            {
                throw out_of_range("position out of range in circularDeque::at");
            }
            return cbegin()[pos];
        }

        /**
         * @brief Write what the function does here
         *
         * @param pos
         *
         * @retval (variable) (description of variable)
         **/
        T &operator [](size_type pos)
        {
            return begin()[pos];
        }

        /**
         * @brief Write what the function does here
         *
         * @param pos
         *
         * @retval (variable) (description of variable)
         **/
        const T &operator [](size_type pos) const
        {
            return cbegin()[pos];
        }

        /**
         * @brief Write what the function does here
         *
         * @retval (variable) (description of variable)
         **/
        bool empty() const
        {
            return frontIndex == backIndex;
        }

        /**
         * @brief Write what the function does here
         *
         void clear()
         {
         frontIndex = backIndex = 0;
         }

        /**
         * @brief Write what the function does here
         *
         * @param v
         **/
        void push_front(const T &v)
        {

            if(frontIndex-- == 0)
            {
                frontIndex = arraySize - 1;
            }
            array[frontIndex] = v;
        }

        /**
         * @brief Write what the function does here
         *
         * @param v
         **/
        void push_front(T  &&v)
        {

            if(frontIndex-- == 0)
            {
                frontIndex = arraySize - 1;
            }
            array[frontIndex] = move(v);
        }

        /**
         * @brief Write what the function does here
         *
         * @param v
         **/
        void push_back(const T &v)
        {
            array[backIndex] = v;

            if(++backIndex >= arraySize)
            {
                backIndex = 0;
            }
        }

        /**
         * @brief Write what the function does here
         *
         * @param v
         **/
        void push_back(T  &&v)
        {
            array[backIndex] = move(v);

            if(++backIndex >= arraySize)
            {
                backIndex = 0;
            }
        }

        /**
         * @brief Write what the function does here
         *
         void pop_front()
         {
         array[frontIndex] = T();

         if(++frontIndex >= arraySize)
         {
         frontIndex = 0;
         }
         }

        /**
         * @brief Write what the function does here
         *
         void pop_back()
         {

         if(backIndex-- == 0)
         {
         backIndex = arraySize - 1;
         }
         array[backIndex] = T();
         }

        /**
         * @brief Write what the function does here
         *
         * @param other
         **/
        void swap(circularDeque &other)
        {
            circularDeque<T, arraySize> temp = move(*this);
            *this = move(other);
            other = move(temp);
        }
};
template <typename T>

/**
 * @brief Write what the function does here
 *
 * @retval (variable) (description of variable)
 **/
struct default_comparer final // the default comparer class for balanced_tree
{
    template <typename U>

        /**
         * @brief Write what the function does here
         *
         * @param l
         * @param r
         *
         * @retval (variable) (description of variable)
         **/
        int operator()(const T &l, const U &r) const
        {

            if(l < r)
            {
                return -1;
            }

            if(r < l)
            {
                return 1;
            }
            return 0;
        }
};
template <typename T, typename Compare = default_comparer<T>>

/**
 * @brief Write what the function does here
 *
 * @retval (variable) (description of variable)
 **/
class balanced_tree final // a balanced tree class that also has all the nodes in a linked list
{
    private:

        /**
         * @brief Write what the function does here
         *
         * @retval (variable) (description of variable)
         **/
        struct Node // the tree node class
        {
            T value;
            unsigned depth; // the depth of this subtree
            Node *left, *right; // the left and right children
            Node *prev, *next; // for the linked list
            Node(const T &value) // create a node containing value by copying it
                : value(value), depth(0)
            {
            }
            Node(T &&value) // create a node containing value by moving it
                : value(move(value)), depth(0)
            {
            }

            /**
             * @brief Write what the function does here
             *
             void calcDepth() // recalculate this subtree's depth
             {
             unsigned newDepth = 0;

             if(left)
             {
             newDepth = 1 + left->depth;
             }

             if(right && right->depth >= newDepth) // equivalent to right->depth + 1 > newDepth
             {
             newDepth = 1 + right->depth;
             }
             depth = newDepth;
             }
             };
             Node *root, *head, *tail;

            /**
             * @brief Write what the function does here
             *
             * @param node
             **/
            void removeNodeFromList(Node *node) // remove node from the linked list
            {

                if(node->prev == nullptr)
                {
                    head = node->next;
                }

                else
                {
                    node->prev->next = node->next;
                }

                if(node->next == nullptr)
                {
                    tail = node->prev;
                }

                else
                {
                    node->next->prev = node->prev;
                }
            }
            Compare compare;

            /**
             * @brief Write what the function does here
             *
             * @param node
             **/
            static void rotateLeft(Node *&node) // rotate the subtree node left
            {
                assert(node && node->right);
                Node *tree1 = node->left;
                Node *tree2 = node->right->left;
                Node *tree3 = node->right->right;
                Node *newNode = node->right;
                newNode->left = node;
                node = newNode;
                node->left->left = tree1;
                node->left->right = tree2;
                node->right = tree3;
                node->left->calcDepth();
                node->calcDepth();
            }

            /**
             * @brief Write what the function does here
             *
             * @param node
             **/
            static void rotateRight(Node *&node) // rotate the subtree node right
            {
                assert(node && node->left);
                Node *tree1 = node->left->left;
                Node *tree2 = node->left->right;
                Node *tree3 = node->right;
                Node *newNode = node->left;
                newNode->right = node;
                node = newNode;
                node->left = tree1;
                node->right->left = tree2;
                node->right->right = tree3;
                node->right->calcDepth();
                node->calcDepth();
            }

            /**
             * @brief Write what the function does here
             *
             * @param node
             **/
            static void balanceNode(Node *&node) // balance the subtree node
            {
                assert(node);
                unsigned lDepth = 0;

                if(node->left)
                {
                    lDepth = node->left->depth + 1;
                }
                unsigned rDepth = 0;

                if(node->right)
                {
                    rDepth = node->right->depth + 1;
                }

                if(lDepth > rDepth + 1)
                {
                    rotateRight(node);
                }

                else if(rDepth > lDepth + 1)
                {
                    rotateLeft(node);
                }
            }

            /**
             * @brief Write what the function does here
             *
             * @param tree
             * @param newNode
             * @param head
             * @param tail
             **/
            void insertNode(Node *&tree, Node *newNode, Node *&head, Node *&tail) // insert a node into the tree and the linked list
            {
                assert(newNode);

                if(tree == nullptr)
                {
                    tree = newNode;
                    tree->depth = 0;
                    tree->left = nullptr;
                    tree->right = nullptr;
                    tree->next = head;
                    tree->prev = tail;
                    head = tail = tree;
                    return;
                }
                int cmpV = compare(tree->value, newNode->value);

                if(cmpV == 0)
                {
                    delete newNode;
                    return;
                }

                else if(cmpV < 0)
                {
                    insertNode(tree->right, newNode, tree->next, tail);
                }

                else
                {
                    insertNode(tree->left, newNode, head, tree->prev);
                }
                tree->calcDepth();
                balanceNode(tree);
            }

            /**
             * @brief Write what the function does here
             *
             * @param node
             *
             * @retval (variable) (description of variable)
             **/
            static Node *removeInorderPredecessorH(Node *&node) // remove inorder predecessor's helper
            {
                assert(node != nullptr);

                if(node->right == nullptr)
                {
                    Node *retval = node;
                    node = node->left;

                    if(node != nullptr)
                    {
                        node->calcDepth();
                        balanceNode(node);
                    }
                    retval->left = retval->right = nullptr;
                    retval->depth = 0;
                    return retval;
                }
                Node *retval = removeInorderPredecessorH(node->right);
                node->calcDepth();
                balanceNode(node);
                return retval;
            }

            /**
             * @brief Write what the function does here
             *
             * @param node
             *
             * @retval (variable) (description of variable)
             **/
            static Node *removeInorderPredecessor(Node *node)
            {
                assert(node != nullptr);
                return removeInorderPredecessorH(node->left);
            }
            template <typename ComparedType>

                /**
                 * @brief Write what the function does here
                 *
                 * @param tree
                 * @param searchFor
                 *
                 * @retval (variable) (description of variable)
                 **/
                Node *removeNode(Node *&tree, ComparedType searchFor) // remove a node from the tree and linked list
                {

                    if(tree == nullptr)
                    {
                        return nullptr;
                    }
                    int cmpV = compare(tree->value, searchFor);
                    Node *retval;

                    if(cmpV == 0) // found it
                    {

                        if(tree->left == nullptr && tree->right == nullptr)
                        {
                            retval = tree;
                            tree = nullptr;
                            removeNodeFromList(retval);
                            return retval;
                        }

                        if(tree->left == nullptr)
                        {
                            retval = tree;
                            tree = tree->right;
                            removeNodeFromList(retval);
                            return retval;
                        }

                        if(tree->right == nullptr)
                        {
                            retval = tree;
                            tree = tree->left;
                            removeNodeFromList(retval);
                            return retval;
                        }
                        retval = tree;
                        Node *replaceWith = removeInorderPredecessor(tree);
                        replaceWith->left = tree->left;
                        replaceWith->right = tree->right;
                        tree = replaceWith;
                        tree->calcDepth();
                        balanceNode(tree);
                        removeNodeFromList(retval);
                        return retval;
                    }

                    else
                    {

                        if(cmpV < 0)
                        {
                            retval = removeNode(tree->right, searchFor);
                        }

                        else
                        {
                            retval = removeNode(tree->left, searchFor);
                        }
                        tree->calcDepth();
                        balanceNode(tree);
                        return retval;
                    }
                }
            template <typename Function, typename ComparedType>

                /**
                 * @brief Write what the function does here
                 *
                 * @param fn
                 * @param min
                 * @param max
                 * @param tree
                 * @param value
                 **/
                void forEachNodeInRange(Function &fn, ComparedType min, ComparedType max, Node *tree) // for each node in the range min <= node->value <= max do fn(node->value)
                {

                    if(tree == nullptr)
                    {
                        return;
                    }
                    bool fits = true;

                    if(compare(tree->value, min) >= 0)
                    {
                        forEachNodeInRange(fn, min, max, tree->left);
                    }

                    else
                    {
                        fits = false;
                    }

                    if(compare(tree->value, max) <= 0)
                    {

                        if(fits)
                        {
                            fn(tree->value);
                        }
                        forEachNodeInRange(fn, min, max, tree->right);
                    }
                }
            template <typename ComparedType>

                /**
                 * @brief Write what the function does here
                 *
                 * @param value
                 * @param tree
                 *
                 * @retval (variable) (description of variable)
                 **/
                Node *find(ComparedType value, Node *tree) // find a node with value value
                {

                    if(tree == nullptr)
                    {
                        return nullptr;
                    }
                    int cmpV = compare(tree->value, value);

                    if(cmpV == 0)
                    {
                        return tree;
                    }

                    else if(cmpV < 0)
                    {
                        return find(value, tree->right);
                    }

                    else
                    {
                        return find(value, tree->left);
                    }
                }
            template <typename ComparedType>

                /**
                 * @brief Write what the function does here
                 *
                 * @param value
                 * @param tree
                 *
                 * @retval (variable) (description of variable)
                 **/
                const Node *find(ComparedType value, const Node *tree) // find a node with value value
                {

                    if(tree == nullptr)
                    {
                        return nullptr;
                    }
                    int cmpV = compare(tree->value, value);

                    if(cmpV == 0)
                    {
                        return tree;
                    }

                    else if(cmpV < 0)
                    {
                        return find(value, (const Node *)tree->right);
                    }

                    else
                    {
                        return find(value, (const Node *)tree->left);
                    }
                }
            template <typename Function>

                /**
                 * @brief Write what the function does here
                 *
                 * @param fn
                 * @param tree
                 * @param value
                 **/
                static void forEachNode(Function &fn, Node *tree) // for each node in the tree run fn(tree->value)
                {

                    if(tree == nullptr)
                    {
                        return;
                    }
                    forEachNode(fn, tree->left);
                    fn(tree->value);
                    forEachNode(fn, tree->right);
                }

            /**
             * @brief Write what the function does here
             *
             * @param tree
             *
             * @retval (variable) (description of variable)
             **/
            static Node *cloneTree(const Node *tree) // return a duplicated subtree from tree
            {

                if(tree == nullptr)
                {
                    return nullptr;
                }
                Node *retval = new Node(tree->value);
                retval->left = cloneTree(tree->left);
                retval->right = cloneTree(tree->right);
                retval->depth = tree->depth;
                return retval;
            }

            /**
             * @brief Write what the function does here
             *
             * @param tree
             **/
            static void freeTree(Node *tree) // deallocate the subtree tree
            {

                if(tree == nullptr)
                {
                    return;
                }
                freeTree(tree->left);
                freeTree(tree->right);
                delete tree;
            }

            /**
             * @brief Write what the function does here
             *
             * @param tree
             * @param head
             * @param tail
             **/
            static void constructList(Node *tree, Node *&head, Node *&tail) // construct the linked list from the subtree tree
            {

                if(tree == nullptr)
                {
                    return;
                }
                tree->prev = tail;
                tree->next = head;
                head = tail = tree;
                constructList(tree->left, head, tree->prev);
                constructList(tree->right, tree->next, tail);
            }
    public:
            friend class iterator;
            friend class const_iterator;

            /**
             * @brief Write what the function does here
             *
             * @param bidirectional_iterator_tag
             *
             * @retval (variable) (description of variable)
             **/
            class const_iterator final : public std::iterator<bidirectional_iterator_tag, T>
    {
        friend class balanced_tree;
        friend class iterator;
        private:
        const Node *node;
        public:
        const_iterator()
            : node(nullptr)
        {
        }
        const_iterator(const Node *node)
            : node(node)
        {
        }

        /**
         * @brief Write what the function does here
         *
         * @retval (variable) (description of variable)
         **/
        const const_iterator &operator ++()
        {
            node = node->next;
            return *this;
        }

        /**
         * @brief Write what the function does here
         *
         * @param int
         *
         * @retval (variable) (description of variable)
         **/
        const_iterator operator ++(int)
        {
            auto retval = *this;
            node = node->next;
            return retval;
        }

        /**
         * @brief Write what the function does here
         *
         * @retval (variable) (description of variable)
         **/
        const const_iterator &operator --()
        {
            node = node->prev;
            return *this;
        }

        /**
         * @brief Write what the function does here
         *
         * @param int
         *
         * @retval (variable) (description of variable)
         **/
        const_iterator operator --(int)
        {
            auto retval = *this;
            node = node->prev;
            return retval;
        }

        /**
         * @brief Write what the function does here
         *
         * @param a
         * @param b
         *
         * @retval (variable) (description of variable)
         **/
        friend bool operator ==(const_iterator a, const_iterator b)
        {
            return a.node == b.node;
        }

        /**
         * @brief Write what the function does here
         *
         * @param a
         * @param b
         *
         * @retval (variable) (description of variable)
         **/
        friend bool operator !=(const_iterator a, const_iterator b)
        {
            return a.node != b.node;
        }

        /**
         * @brief Write what the function does here
         *
         * @retval (variable) (description of variable)
         **/
        const T &operator *() const
        {
            return node->value;
        }

        /**
         * @brief Write what the function does here
         *
         * @retval (variable) (description of variable)
         **/
        const T *operator ->() const
        {
            return &node->value;
        }
    };

            /**
             * @brief Write what the function does here
             *
             * @param bidirectional_iterator_tag
             *
             * @retval (variable) (description of variable)
             **/
            class iterator final : public std::iterator<bidirectional_iterator_tag, T>
    {
        friend class balanced_tree;
        private:
        Node *node;
        public:
        iterator()
            : node(nullptr)
        {
        }
        iterator(Node *node)
            : node(node)
        {
        }

        /**
         * @brief Write what the function does here
         *
         * @retval (variable) (description of variable)
         **/
        operator const_iterator() const
        {
            return const_iterator(node);
        }

        /**
         * @brief Write what the function does here
         *
         * @retval (variable) (description of variable)
         **/
        const iterator &operator ++()
        {
            node = node->next;
            return *this;
        }

        /**
         * @brief Write what the function does here
         *
         * @param int
         *
         * @retval (variable) (description of variable)
         **/
        iterator operator ++(int)
        {
            auto retval = *this;
            node = node->next;
            return retval;
        }

        /**
         * @brief Write what the function does here
         *
         * @retval (variable) (description of variable)
         **/
        const iterator &operator --()
        {
            node = node->prev;
            return *this;
        }

        /**
         * @brief Write what the function does here
         *
         * @param int
         *
         * @retval (variable) (description of variable)
         **/
        iterator operator --(int)
        {
            auto retval = *this;
            node = node->prev;
            return retval;
        }

        /**
         * @brief Write what the function does here
         *
         * @param a
         * @param b
         *
         * @retval (variable) (description of variable)
         **/
        friend bool operator ==(iterator a, iterator b)
        {
            return a.node == b.node;
        }

        /**
         * @brief Write what the function does here
         *
         * @param a
         * @param b
         *
         * @retval (variable) (description of variable)
         **/
        friend bool operator !=(iterator a, iterator b)
        {
            return a.node != b.node;
        }

        /**
         * @brief Write what the function does here
         *
         * @param a
         * @param b
         *
         * @retval (variable) (description of variable)
         **/
        friend bool operator ==(const_iterator a, iterator b)
        {
            return a.node == b.node;
        }

        /**
         * @brief Write what the function does here
         *
         * @param a
         * @param b
         *
         * @retval (variable) (description of variable)
         **/
        friend bool operator !=(const_iterator a, iterator b)
        {
            return a.node != b.node;
        }

        /**
         * @brief Write what the function does here
         *
         * @param a
         * @param b
         *
         * @retval (variable) (description of variable)
         **/
        friend bool operator ==(iterator a, const_iterator b)
        {
            return a.node == b.node;
        }

        /**
         * @brief Write what the function does here
         *
         * @param a
         * @param b
         *
         * @retval (variable) (description of variable)
         **/
        friend bool operator !=(iterator a, const_iterator b)
        {
            return a.node != b.node;
        }

        /**
         * @brief Write what the function does here
         *
         * @retval (variable) (description of variable)
         **/
        T &operator *() const
        {
            return node->value;
        }

        /**
         * @brief Write what the function does here
         *
         * @retval (variable) (description of variable)
         **/
        T *operator ->() const
        {
            return &node->value;
        }
    };
            friend class reverse_iterator;
            friend class const_reverse_iterator;

            /**
             * @brief Write what the function does here
             *
             * @param bidirectional_iterator_tag
             *
             * @retval (variable) (description of variable)
             **/
            class const_reverse_iterator final : public std::iterator<bidirectional_iterator_tag, T>
    {
        friend class balanced_tree;
        friend class reverse_iterator;
        private:
        const Node *node;
        public:
        const_reverse_iterator()
            : node(nullptr)
        {
        }
        const_reverse_iterator(const Node *node)
            : node(node)
        {
        }

        /**
         * @brief Write what the function does here
         *
         * @retval (variable) (description of variable)
         **/
        const const_reverse_iterator &operator ++()
        {
            node = node->prev;
            return *this;
        }

        /**
         * @brief Write what the function does here
         *
         * @param int
         *
         * @retval (variable) (description of variable)
         **/
        const_reverse_iterator operator ++(int)
        {
            auto retval = *this;
            node = node->prev;
            return retval;
        }

        /**
         * @brief Write what the function does here
         *
         * @retval (variable) (description of variable)
         **/
        const const_reverse_iterator &operator --()
        {
            node = node->next;
            return *this;
        }

        /**
         * @brief Write what the function does here
         *
         * @param int
         *
         * @retval (variable) (description of variable)
         **/
        const_reverse_iterator operator --(int)
        {
            auto retval = *this;
            node = node->next;
            return retval;
        }

        /**
         * @brief Write what the function does here
         *
         * @param a
         * @param b
         *
         * @retval (variable) (description of variable)
         **/
        friend bool operator ==(const_reverse_iterator a, const_reverse_iterator b)
        {
            return a.node == b.node;
        }

        /**
         * @brief Write what the function does here
         *
         * @param a
         * @param b
         *
         * @retval (variable) (description of variable)
         **/
        friend bool operator !=(const_reverse_iterator a, const_reverse_iterator b)
        {
            return a.node != b.node;
        }

        /**
         * @brief Write what the function does here
         *
         * @retval (variable) (description of variable)
         **/
        const T &operator *() const
        {
            return node->value;
        }

        /**
         * @brief Write what the function does here
         *
         * @retval (variable) (description of variable)
         **/
        const T *operator ->() const
        {
            return &node->value;
        }
    };

            /**
             * @brief Write what the function does here
             *
             * @param bidirectional_iterator_tag
             *
             * @retval (variable) (description of variable)
             **/
            class reverse_iterator final : public std::iterator<bidirectional_iterator_tag, T>
    {
        friend class balanced_tree;
        private:
        Node *node;
        public:
        reverse_iterator()
            : node(nullptr)
        {
        }
        reverse_iterator(Node *node)
            : node(node)
        {
        }

        /**
         * @brief Write what the function does here
         *
         * @retval (variable) (description of variable)
         **/
        operator const_reverse_iterator() const
        {
            return const_reverse_iterator(node);
        }

        /**
         * @brief Write what the function does here
         *
         * @retval (variable) (description of variable)
         **/
        const reverse_iterator &operator ++()
        {
            node = node->prev;
            return *this;
        }

        /**
         * @brief Write what the function does here
         *
         * @param int
         *
         * @retval (variable) (description of variable)
         **/
        reverse_iterator operator ++(int)
        {
            auto retval = *this;
            node = node->prev;
            return retval;
        }

        /**
         * @brief Write what the function does here
         *
         * @retval (variable) (description of variable)
         **/
        const reverse_iterator &operator --()
        {
            node = node->next;
            return *this;
        }

        /**
         * @brief Write what the function does here
         *
         * @param int
         *
         * @retval (variable) (description of variable)
         **/
        reverse_iterator operator --(int)
        {
            auto retval = *this;
            node = node->next;
            return retval;
        }

        /**
         * @brief Write what the function does here
         *
         * @param a
         * @param b
         *
         * @retval (variable) (description of variable)
         **/
        friend bool operator ==(reverse_iterator a, reverse_iterator b)
        {
            return a.node == b.node;
        }

        /**
         * @brief Write what the function does here
         *
         * @param a
         * @param b
         *
         * @retval (variable) (description of variable)
         **/
        friend bool operator !=(reverse_iterator a, reverse_iterator b)
        {
            return a.node != b.node;
        }

        /**
         * @brief Write what the function does here
         *
         * @param a
         * @param b
         *
         * @retval (variable) (description of variable)
         **/
        friend bool operator ==(const_reverse_iterator a, reverse_iterator b)
        {
            return a.node == b.node;
        }

        /**
         * @brief Write what the function does here
         *
         * @param a
         * @param b
         *
         * @retval (variable) (description of variable)
         **/
        friend bool operator !=(const_reverse_iterator a, reverse_iterator b)
        {
            return a.node != b.node;
        }

        /**
         * @brief Write what the function does here
         *
         * @param a
         * @param b
         *
         * @retval (variable) (description of variable)
         **/
        friend bool operator ==(reverse_iterator a, const_reverse_iterator b)
        {
            return a.node == b.node;
        }

        /**
         * @brief Write what the function does here
         *
         * @param a
         * @param b
         *
         * @retval (variable) (description of variable)
         **/
        friend bool operator !=(reverse_iterator a, const_reverse_iterator b)
        {
            return a.node != b.node;
        }

        /**
         * @brief Write what the function does here
         *
         * @retval (variable) (description of variable)
         **/
        T &operator *() const
        {
            return node->value;
        }

        /**
         * @brief Write what the function does here
         *
         * @retval (variable) (description of variable)
         **/
        T *operator ->() const
        {
            return &node->value;
        }
    };
            balanced_tree()
                : root(nullptr), head(nullptr), tail(nullptr), compare()
            {
            }
            explicit balanced_tree(const Compare &compare)
                : root(nullptr), head(nullptr), tail(nullptr), compare(compare)
            {
            }
            explicit balanced_tree(Compare &&compare)
                : root(nullptr), head(nullptr), tail(nullptr), compare(move(compare))
            {
            }
            balanced_tree(const balanced_tree &rt)
                : root(cloneTree(rt)), head(nullptr), tail(nullptr), compare(rt.compare)
            {
                constructList(root, head, tail);
            }
            balanced_tree(balanced_tree &&rt)
                : root(rt.root), head(rt.head), tail(rt.tail), compare(rt.compare)
            {
                rt.root = nullptr;
                rt.head = nullptr;
                rt.tail = nullptr;
            }

            /**
             * @brief Write what the function does here
             *
             * @retval (variable) (description of variable)
             **/
            ~balanced_tree()
            {
                freeTree(root);
            }

            /**
             * @brief Write what the function does here
             *
             * @param rt
             *
             * @retval (variable) (description of variable)
             **/
            const balanced_tree &operator =(const balanced_tree &rt)
            {

                if(root == rt.root)
                {
                    return *this;
                }
                clear();
                root = cloneTree(rt.root);
                constructList(root, head, tail);
                compare = rt.compare;
                return *this;
            }

            /**
             * @brief Write what the function does here
             *
             * @param rt
             *
             * @retval (variable) (description of variable)
             **/
            const balanced_tree &operator =(balanced_tree &&rt)
            {
                swap(root, rt.root);
                swap(head, rt.head);
                swap(tail, rt.tail);
                swap(compare, rt.compare);
                return *this;
            }

            /**
             * @brief Write what the function does here
             *
             void clear()
             {
             freeTree(root);
             root = nullptr;
             head = nullptr;
             tail = nullptr;
             }
             template <typename Function>

            /**
             * @brief Write what the function does here
             *
             * @param fn
             * @param value
             **/
            void forEach(Function &fn) // for each value in the tree do fn(value)
            {
                forEachNode(fn, root);
            }
            template <typename Function, typename ComparedType>

                /**
                 * @brief Write what the function does here
                 *
                 * @param fn
                 * @param min
                 * @param max
                 * @param value
                 **/
                void forEachInRange(Function &fn, ComparedType min, ComparedType max) // for each value in the tree in the range min <= value <= max do fn(value)
                {
                    forEachNodeInRange(fn, min, max, root);
                }
            template <typename ComparedType>

                /**
                 * @brief Write what the function does here
                 *
                 * @param value
                 *
                 * @retval (variable) (description of variable)
                 **/
                const_iterator find(ComparedType value) const // find the value value
                {
                    return iterator(find(value, (const Node *)root));
                }
            template <typename ComparedType>

                /**
                 * @brief Write what the function does here
                 *
                 * @param value
                 *
                 * @retval (variable) (description of variable)
                 **/
                iterator get(ComparedType value) // find the value value
                {
                    return iterator(find(value, root));
                }

            /**
             * @brief Write what the function does here
             *
             * @param value
             **/
            void insert(const T &value) // insert value by copying
            {
                insertNode(root, new Node(value), head, tail);
            }

            /**
             * @brief Write what the function does here
             *
             * @param value
             **/
            void insert(T &&value) // insert value by moving
            {
                insertNode(root, new Node(move(value)), head, tail);
            }
            template <typename ComparedType>

                /**
                 * @brief Write what the function does here
                 *
                 * @param searchFor
                 *
                 * @retval (variable) (description of variable)
                 **/
                bool erase(ComparedType searchFor) // erase the value == searchFor and return if any values were erased
                {
                    Node *node = removeNode(root, searchFor);

                    if(node == nullptr)
                    {
                        return false;
                    }
                    delete node;
                    return true;
                }

            /**
             * @brief Write what the function does here
             *
             * @param iter
             *
             * @retval (variable) (description of variable)
             **/
            iterator erase(iterator iter) // erase the value pointed to by iter and return the next location
            {
                iterator retval = iter;
                retval++;
                erase<const T &>(*iter);
                return retval;
            }

            /**
             * @brief Write what the function does here
             *
             * @param iter
             *
             * @retval (variable) (description of variable)
             **/
            const_iterator erase(const_iterator iter) // erase the value pointed to by iter and return the next location
            {
                const_iterator retval = iter;
                retval++;
                erase<const T &>(*iter);
                return retval;
            }

            /**
             * @brief Write what the function does here
             *
             * @retval (variable) (description of variable)
             **/
            iterator begin()
            {
                return iterator(head);
            }

            /**
             * @brief Write what the function does here
             *
             * @retval (variable) (description of variable)
             **/
            const_iterator begin() const
            {
                return const_iterator(head);
            }

            /**
             * @brief Write what the function does here
             *
             * @retval (variable) (description of variable)
             **/
            const_iterator cbegin() const
            {
                return const_iterator(head);
            }

            /**
             * @brief Write what the function does here
             *
             * @retval (variable) (description of variable)
             **/
            reverse_iterator rbegin()
            {
                return reverse_iterator(tail);
            }

            /**
             * @brief Write what the function does here
             *
             * @retval (variable) (description of variable)
             **/
            const_reverse_iterator rbegin() const
            {
                return const_reverse_iterator(tail);
            }

            /**
             * @brief Write what the function does here
             *
             * @retval (variable) (description of variable)
             **/
            const_reverse_iterator crbegin() const
            {
                return const_iterator(tail);
            }

            /**
             * @brief Write what the function does here
             *
             * @retval (variable) (description of variable)
             **/
            iterator end()
            {
                return iterator(nullptr);
            }

            /**
             * @brief Write what the function does here
             *
             * @retval (variable) (description of variable)
             **/
            const_iterator end() const
            {
                return const_iterator(nullptr);
            }

            /**
             * @brief Write what the function does here
             *
             * @retval (variable) (description of variable)
             **/
            const_iterator cend() const
            {
                return const_iterator(nullptr);
            }

            /**
             * @brief Write what the function does here
             *
             * @retval (variable) (description of variable)
             **/
            reverse_iterator rend()
            {
                return reverse_iterator(nullptr);
            }

            /**
             * @brief Write what the function does here
             *
             * @retval (variable) (description of variable)
             **/
            const_reverse_iterator rend() const
            {
                return const_reverse_iterator(nullptr);
            }

            /**
             * @brief Write what the function does here
             *
             * @retval (variable) (description of variable)
             **/
            const_reverse_iterator crend() const
            {
                return const_reverse_iterator(nullptr);
            }
            template <typename CompareType>

                /**
                 * @brief Write what the function does here
                 *
                 * @param searchFor
                 *
                 * @retval (variable) (description of variable)
                 **/
                const_iterator rangeCBegin(CompareType searchFor) const // return the iterator to the first position where value >= searchFor
                {
                    const Node *node = root, *lastNode = root;

                    while(node != nullptr)
                    {
                        lastNode = node;
                        int cmpV = compare(node->value, searchFor);

                        if(cmpV == 0)
                        {
                            break;
                        }

                        if(cmpV > 0)
                        {
                            node = node->left;
                        }

                        else
                        {
                            node = node->right;

                            if(node == nullptr)
                            {
                                lastNode = lastNode->next;
                            }
                        }
                    }
                    return const_iterator(lastNode);
                }
            template <typename CompareType>

                /**
                 * @brief Write what the function does here
                 *
                 * @param searchFor
                 *
                 * @retval (variable) (description of variable)
                 **/
                const_iterator rangeCEnd(CompareType searchFor) const // return the iterator to the first position where value < searchFor
                {
                    const Node *node = root, *lastNode = root;

                    while(node != nullptr)
                    {
                        lastNode = node;
                        int cmpV = compare(node->value, searchFor);

                        if(cmpV == 0)
                        {
                            return const_iterator(lastNode->next);
                        }

                        if(cmpV > 0)
                        {
                            node = node->left;
                        }

                        else
                        {
                            node = node->right;

                            if(node == nullptr)
                            {
                                lastNode = lastNode->next;
                            }
                        }
                    }
                    return const_iterator(lastNode);
                }
            template <typename CompareType>

                /**
                 * @brief Write what the function does here
                 *
                 * @param searchFor
                 *
                 * @retval (variable) (description of variable)
                 **/
                iterator rangeBegin(CompareType searchFor) // return the iterator to the first position where value >= searchFor
                {
                    Node *node = root, *lastNode = root;

                    while(node != nullptr)
                    {
                        lastNode = node;
                        int cmpV = compare(node->value, searchFor);

                        if(cmpV == 0)
                        {
                            break;
                        }

                        if(cmpV > 0)
                        {
                            node = node->left;
                        }

                        else
                        {
                            node = node->right;

                            if(node == nullptr)
                            {
                                lastNode = lastNode->next;
                            }
                        }
                    }
                    return iterator(lastNode);
                }
            template <typename CompareType>

                /**
                 * @brief Write what the function does here
                 *
                 * @param searchFor
                 *
                 * @retval (variable) (description of variable)
                 **/
                iterator rangeEnd(CompareType searchFor) // return the iterator to the first position where value < searchFor
                {
                    Node *node = root, *lastNode = root;

                    while(node != nullptr)
                    {
                        lastNode = node;
                        int cmpV = compare(node->value, searchFor);

                        if(cmpV == 0)
                        {
                            return iterator(lastNode->next);
                        }

                        if(cmpV > 0)
                        {
                            node = node->left;
                        }

                        else
                        {
                            node = node->right;

                            if(node == nullptr)
                            {
                                lastNode = lastNode->next;
                            }
                        }
                    }
                    return iterator(lastNode);
                }
};

/**
 * @brief Write what the function does here
 *
 * @param 1
 *
 * @retval (variable) (description of variable)
 **/
inline int solveLinear(float a/*constant*/, float b/*linear*/, float retval[1]) // find all the solutions x to a + b * x == 0 and return the number of solutions
{
    retval[0] = 0;

    if(abs(b) < eps)
    {
        return (abs(a) < eps) ? 1 : 0;
    }
    retval[0] = -a / b;
    return 1;
}

/**
 * @brief Write what the function does here
 *
 * @param 2
 *
 * @retval (variable) (description of variable)
 **/
inline int solveQuadratic(float a/*constant*/, float b/*linear*/, float c/*quadratic*/, float retval[2]) // find all the solutions x to a + b * x + c * x * x == 0 and return the number of solutions
{

    if(abs(c) < eps)
    {
        return solveLinear(a, b, retval);
    }
    float sqrtArg = b * b - 4 * c * a;

    if(sqrtArg < 0)
    {
        return 0;
    }

    if(c < 0)
    {
        a = -a;
        b = -b;
        c = -c;
    }
    float sqrtV = sqrt(sqrtArg);
    retval[0] = (-b - sqrtV) / (2 * c);
    retval[1] = (-b + sqrtV) / (2 * c);
    return 2;
}
inline int solveCubic(float a/*constant*/, float b/*linear*/, float c/*quadratic*/, float d/*cubic*/,

        /**
         * @brief Write what the function does here
         *
         * @param 3
         *
         * @retval (variable) (description of variable)
         **/
        float retval[3]) // find all the solutions x to a + b * x + c * x * x + d * x * x * x == 0 and return the number of solutions
{

    if(abs(d) < eps)
    {
        return solveQuadratic(a, b, c, retval);
    }
    a /= d;
    b /= d;
    c /= d;
    float Q = (c * c - 3 * b) / 9;
    float R = (2 * (c * c * c) - 9 * (c * b) + 27 * a) / 54;
    float R2 = R * R;
    float Q3 = Q * Q * Q;

    if(R2 < Q3)
    {
        float theta = acos(R / sqrt(Q3));
        float SQ = sqrt(Q);
        retval[0] = -2 * SQ * cos(theta / 3) - c / 3;
        retval[1] = -2 * SQ * cos((theta + 2 * M_PI) / 3) - c / 3;
        retval[2] = -2 * SQ * cos((theta - 2 * M_PI) / 3) - c / 3;
        return 3;
    }
    float A = -cbrt(abs(R) + sqrt(R2 - Q3));

    if(R < 0)
    {
        A = -A;
    }
    float B;

    if(A == 0)
    {
        B = 0;
    }

    else
    {
        B = Q / A;
    }
    float AB = A + B;
    retval[0] = AB - c / 3;
    return 1;
}
#endif // UTIL_H
