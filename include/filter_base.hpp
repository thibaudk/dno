/*(utf8)
Only provides members to fit the 1eFilter
*/

#pragma once
#include <boost/circular_buffer.hpp>

#define SCALED_AMOUNT 10.

namespace value_filters
{

template <typename T = double>
class filter_base
{
public:
  double freq {}, dcutoff {};

protected:
  boost::circular_buffer<T> buffer {0};
};

}
