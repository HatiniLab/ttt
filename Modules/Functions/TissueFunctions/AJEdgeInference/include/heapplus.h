// Heap Plus implementation 1.01alpha
// lihw
// 2006.12.18
// ChangeLog:
//  2009.08.10  - Added update_heap_pos functions so that we can adjust
//                heap values outside of update_heap functions -- e.g., if
//                we have external pointers into the heap entries -- then
//                call update_heap on the position only, regardless of the value.
//                (Danny Tarlow: dtarlow@cs.toronto.edu)
//  2006.12.18  - Initially created (lihw)

#ifndef HEAPPLUS_H_
#define HEAPPLUS_H_

#include <iterator>

namespace std {

template<typename _RandomAccessIterator, typename _Distance, typename _Tp, typename _Compare>
inline void
__up_heap(_RandomAccessIterator __first, _RandomAccessIterator __end, _RandomAccessIterator __pos,
        _Distance, _Tp __value,  _Compare __comp)
{
    _Distance __parent = (__pos - __first - 1) /  2;
    _Distance __index = __pos - __first;

    while (__index > 0 && __comp(*(__first + __parent), __value)) {
        *(__first + __index) = *(__first + __parent);
        __index = __parent;
        __parent = (__parent - 1) / 2;
    }

    if (__pos != (__first + __index))
        *(__first + __index) = __value;
}

template<typename _RandomAccessIterator, typename _Distance, typename _Tp, typename _Compare>
inline void
__down_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _RandomAccessIterator __pos,
        _Distance, _Tp __value, _Compare __comp)
{
    _Distance __len = __last - __first;
    _Distance __index = __pos - __first;
    _Distance __left = __index * 2 + 1;
    _Distance __right = __index * 2 + 2;
    _Distance __largest;

    while (__index < __len) {
        if (__left < __len && __comp(*(__first + __right), *(__first + __left))) {
            __largest = __left;
        } else {
            __largest = __right;
        }
        if (__largest < __len && __comp(__value, *(__first + __largest))) {
            *(__first + __index) = *(__first + __largest);
            __index = __largest;
            __left = __index * 2 + 1;
            __right = __index * 2 + 2;
        } else
            break;
    }

    if (__pos != (__first + __index))
        *(__first + __index) = __value;
}

template<typename _RandomAccessIterator, typename _Distance, typename _Tp,
    typename _Compare>
inline void
__update_heap(_RandomAccessIterator __first, _RandomAccessIterator __last,
        _RandomAccessIterator __pos, _Distance, _Tp __value, _Compare __comp)
{
    *(__pos) = __value;

    _Distance __index = (__pos - __first);
    _Distance __parent = (__index - 1) / 2;

    if (__index > 0 && __comp(*(__first + __parent), __value))
        __up_heap(__first, __last, __pos, _Distance(), __value, __comp);
    else
        __down_heap(__first, __last, __pos, _Distance(), __value, __comp);
}

template<typename _RandomAccessIterator, typename _Distance, typename _Compare>
inline void
__update_heap(_RandomAccessIterator __first, _RandomAccessIterator __last,
        _RandomAccessIterator __pos, _Distance, _Compare __comp)
{
    _Distance __index = (__pos - __first);
    _Distance __parent = (__index - 1) / 2;

    if (__index > 0 && __comp(*(__first + __parent), *(__pos)))
      __up_heap(__first, __last, __pos, _Distance(), *(__pos), __comp);
    else
      __down_heap(__first, __last, __pos, _Distance(), *(__pos), __comp);
}

template<typename _RandomAccessIterator, typename _Tp>
inline void
update_heap(_RandomAccessIterator __first, _RandomAccessIterator __last,
        _RandomAccessIterator __pos, _Tp __value)
{
      typedef typename iterator_traits<_RandomAccessIterator>::value_type _ValueType;
      typedef typename iterator_traits<_RandomAccessIterator>::difference_type _DistanceType;

      __update_heap(__first, __last, __pos, _DistanceType(), __value, less<_ValueType>());
}

template<typename _RandomAccessIterator, typename _Tp, typename _Compare>
inline void
update_heap(_RandomAccessIterator __first, _RandomAccessIterator __last,
        _RandomAccessIterator __pos, _Tp __value, _Compare __comp)
{
      __update_heap(__first, __last, __pos, __value, __comp);
}


template<typename _RandomAccessIterator>
inline void
update_heap_pos(_RandomAccessIterator __first, _RandomAccessIterator __last,
  _RandomAccessIterator __pos) {
      typedef typename iterator_traits<_RandomAccessIterator>::value_type _ValueType;
      typedef typename iterator_traits<_RandomAccessIterator>::difference_type _DistanceType;

      __update_heap(__first, __last, __pos, _DistanceType(), less<_ValueType>());
}


template<typename _RandomAccessIterator, typename _Compare>
inline void
update_heap_pos(_RandomAccessIterator __first, _RandomAccessIterator __last,
  _RandomAccessIterator __pos, _Compare __comp) {
      typedef typename iterator_traits<_RandomAccessIterator>::value_type _ValueType;
      typedef typename iterator_traits<_RandomAccessIterator>::difference_type _DistanceType;

      __update_heap(__first, __last, __pos, _DistanceType(), __comp);
}



}; // namespace std

#endif // !HEAPPLUS_H_
