/*(utf8)
Simple moving average filter
*/

#include "filter_base.hpp"

namespace value_filters
{

template <typename T = double>
struct floating_average : public filter_base<T>
{
  floating_average()
  {
    set_amount(SCALED_AMOUNT);
  }

  T operator()(T x)
  {
    T sum {0};

    this->buffer.push_back(x);

    for (T v : this->buffer)
      sum += v;

    return sum / this->buffer.size();
  }

  void set_amount(double amt)
  {
    if (amt <= 1)
      amt = 1;
    this->buffer.set_capacity(amt);
  };
};

}
