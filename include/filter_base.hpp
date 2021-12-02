/*(utf8)
Only provides members to fit the 1eFilter
*/

#pragma once

#define SCALED_AMOUNT 10.

namespace value_filters
{

template <typename T = double>
struct filter_base
{
  double freq {}, dcutoff {};
};

}
