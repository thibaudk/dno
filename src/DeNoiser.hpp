#pragma once
#include <libossia/3rdparty/dno/include/1efilter.hpp>

template <typename T = double>
class DeNoiser
{
  public :
    DeNoiser(const double frequency = 120,
                const T minCutoff = 1,
                const T beta = 1,
                const T dCutoff = 1)
      : one_euro{one_euro_filter<T>(frequency, minCutoff, beta, dCutoff)}
    {}

    T operator()(T val)
    {
      return one_euro(val, 1);
    }

  protected :
    one_euro_filter<T> one_euro;
};
