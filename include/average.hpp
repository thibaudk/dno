/*(utf8)
Simple moving average filter
*/

#include "filter_base.hpp"
#include <boost/circular_buffer.hpp>

namespace value_filters {

template <typename T = double>
class floating_average : filter_base<T>
{
public :
  floating_average()
  {
      setAmount(10);
  }

    T operator()(T x)
    {
      T sum{0};

      buffer.push_back(x);

      for (T v : buffer) sum += v;

      return sum / buffer.size();
    }

    void setAmount(double amt)
    {
      if (amt <= 1) amt = 1;
      buffer.set_capacity(amt);
    };

private:
    boost::circular_buffer<T> buffer;
};

}
