/*(utf8)
Simple moving median filter
*/

#include "filter_base.hpp"

namespace value_filters
{

template <typename T = double>
class floating_median : public filter_base<T>
{
  T min {0}, max {0};

public:
  floating_median()
  {
    set_amount(SCALED_AMOUNT);
  }

  T operator()(T x)
  {
    if (x < min)
    {
      this->buffer.push_front(x);
      min = x;
    }

    if (x > max)
    {
      this->buffer.push_back(x);
      max = x;
    }

    if (this->buffer.size() % 2 == 0)
    {
      unsigned int middle = this->buffer.size() / 2;
      T mid1 = this->buffer[middle--];
      T mid2 = this->buffer[middle];
      return (mid1 + mid2) / 2.;
    }
    else
    {
      return this->buffer[(this->buffer.size() / 2)];
    }
  }

  void set_amount(double amt)
  {
    if (amt <= 2)
      amt = 2;
    this->buffer.set_capacity(amt);
  };
};

}
