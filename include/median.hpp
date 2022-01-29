/*(utf8)
Simple moving median filter
*/

#include <cmath>
#include <boost/container/flat_map.hpp>

#include "filter_base.hpp"

namespace value_filters
{

template <typename T = double>
class floating_median : public filter_base<T>
{

public:
  floating_median() {}

  T operator()(T x)
  {
    if (!std::isnan(x) || !std::isinf(x))
    {
      buffer[x] = date;

      if (buffer.size() > capacity)
        for (auto it = buffer.begin(); it < buffer.end(); it++)
          if (it->second <= (date - capacity))
          {
            buffer.erase(it);
            break;
          }

      date++;
    }

    if (buffer.size() % 2 == 0)
    {
      int middle = buffer.size() / 2;
      auto mid1 = buffer.nth(middle--)->first;
      auto mid2 = buffer.nth(middle)->first;
      return (mid1 + mid2) / 2.;
    }
    else
    {
      return buffer.nth(buffer.size() / 2)->first;
    }
  }

  void set_amount(double amt)
  {
    if (amt < 1)
      amt = 1;

    capacity = amt;

    if (buffer.size() > capacity)
      for (auto it = buffer.begin(); it < buffer.end(); it++)
        if (it->second <= (date - capacity))
          buffer.erase(it);
  }

private:
  boost::container::flat_map<T, int> buffer{};

  int date{0}, capacity{static_cast<int>(SCALED_AMOUNT)};
};

}
