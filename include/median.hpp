/*(utf8)
Simple moving median filter
*/

#include "filter_base.hpp"

namespace value_filters
{

template <typename T = double>
class floating_median : public filter_base<T>
{

public:
  floating_median()
  {
    set_amount(SCALED_AMOUNT);
  }

  T operator()(T x)
  {
    if (this->buffer.full)
    {
      if (x <= min)
      {
        this->buffer.first() = x;
        min = x;
      }
      else if (x >= max)
      {
        this->buffer.last() = x;
        max = x;
      }
      else
        this->buffer[find_index_to_sort(x)] = x;
    }
    else if (this->buffer.isEmpty())
    { // handle initial values
      this->buffer.push_back(x);
      min = x;
      max = x;
    }
    else
    {
      if (x <= min)
      {
        this->buffer.push_front(x);
        min = x;
      }
      else if (x >= max)
      {
        this->buffer.push_back(x);
        max = x;
      }
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

private:
  T min {0}, max {0};

  unsigned int find_index_to_sort(T value)
  {
    for (unsigned int i = 1; ; i++)
    {
      if (value >= this->buffer[i] && value < this->buffer[i + 1])
        return i;
    }
  }
};

}
