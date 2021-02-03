/*(utf8)
1€ Filter, template-compliant version
Jonathan Aceituno <join@oin.name>

25/04/14: fixed bug with last_time_ never updated on line 40

For details, see http://www.lifl.fr/~casiez/1euro
*/

#include <cmath>
#include "filterBase.hpp"

template <typename T = double>
class low_pass_filter
{
  public :
    low_pass_filter() : hatxprev{0}, xprev{0}, hadprev{false} {}

    T operator()(T x, double alpha)
    {
      T hatx;

      if (hadprev)
      {
        hatx = alpha * x + (1 - alpha) * hatxprev;
      } else
      {
        hatx = x;
        hadprev = true;
      }

      hatxprev = hatx;
      xprev = x;
      return hatx;
    }

    T hatxprev, xprev;
    bool hadprev;
};

template <typename T = double/*, typename timestamp_t = double*/>
struct one_euro_filter
{
    one_euro_filter(double _freq, double _mincutoff, T _beta, T _dcutoff) :
      freq{_freq}, mincutoff{_mincutoff}, beta{_beta}, dcutoff{_dcutoff}/*, last_time_{-1}*/ {}

    T truc{2};
    T a = truc * 2.f;

    T operator()(T x/*, timestamp_t t = -1*/)
    {
      T dx{0};
/*
      if (last_time_ != -1 && t != -1 && t != last_time_)
        freq = 1.0 / (t - last_time_);

      last_time_ = t;
*/
      if (xfilt_.hadprev)
        dx = xfilt_.xprev * freq;

      T edx = dxfilt_(dx, alpha(dcutoff));
      T cutoff = mincutoff + beta * std::abs(static_cast<double>(edx));

      return xfilt_(x, alpha(cutoff));
    }

    double freq, mincutoff;
    T beta, dcutoff;

  private:
    T alpha(T cutoff)
    {
      T tau{1.0 / (2 * M_PI * cutoff)};
      T te{1.0 / freq};

      return 1.0 / (1.0 + tau / te);
    }

    //timestamp_t last_time_;
    low_pass_filter<T> xfilt_, dxfilt_;
};
