/*(utf8)
Simple moving median filter
*/

#include <cmath>
#include <boost/circular_buffer.hpp>

template <typename T = double>
class floating_median
{
public :
  floating_median()
  {
      setAmount(10);
  }

    T operator()(T x)
    {
      if (x < min)
      {
        buffer.push_front(x);
        min = x;
      }

      if (x > max)
      {
        buffer.push_back(x);
        max = x;
      }

      if (buffer.size() % 2 == 0)
      {
        size_t middle = buffer.size() / 2;
        T mid1 = buffer[middle--];
        T mid2 = buffer[middle];
        return (mid1 + mid2) / 2.;
      }
      else
      {
        return buffer[(buffer.size() / 2)];
      }
    }

    void setAmount(double amt)
    {
      if (amt <= 1) amt = 1;
      buffer.set_capacity(amt);
    };

private:
    boost::circular_buffer<T> buffer{0};
    T min{0}, max{0};
};
