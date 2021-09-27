/*(utf8)
Simple moving average filter
*/

#include <cmath>
#include <boost/circular_buffer.hpp>

template <typename T = double>
class floating_average
{
public :
  floating_average() : nVal{10}
  {
      setAmount(nVal);
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
    size_t nVal;
};

