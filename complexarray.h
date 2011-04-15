#ifndef COMPLEXARRAY_H
#define COMPLEXARRAY_H

#include "complex.h"

#include <boost/multi_array.hpp>

/*
 * see http://www.boost.org/doc/libs/1_46_1/libs/multi_array/doc/index.html
 * for Boost.MultiArray
 */

typedef boost::multi_array<Complex, 3> ComplexArray;
typedef ComplexArray::index CAIndex;

#endif // COMPLEXARRAY_H
