/*(utf8)
Simple moving median filter
*/
#pragma once

namespace value_filters {

template <typename T = double>
struct filter_base
{
  T operator()(T x) {}
  void setAmount(double amt){}
  double freq;
  double beta;
  double dcutoff;
};

}
