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
  filter_base() = default;
  filter_base(double _freq, double _dcutoff)
      : freq {_freq}, dcutoff {_dcutoff}
  {
  }

  double freq {}, dcutoff {};

  void update()
  {
  }
};

}
