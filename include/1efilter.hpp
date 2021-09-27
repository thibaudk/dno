/*(utf8)
This is a slight tweek of the
1€ Filter, template-compliant version
by Jonathan Aceituno <join@oin.name>
omiting timestamps

For details, see http://www.lifl.fr/~casiez/1euro
*/

#include <cmath>

template <typename T = double>
class low_pass_filter
{
public :
  low_pass_filter() : hatxprev{0}, xprev{0}, hadprev{false} {}

  T operator()(T x, T alpha)
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
  one_euro_filter(double _freq = 120., double _mincutoff = 1., double _beta = 1., double _dcutoff = 1.) :
    freq{_freq}, mincutoff{_mincutoff}, beta{_beta}, dcutoff{_dcutoff}
  {}

  T operator()(T x)
  {
    T dx{0};

    if (xfilt_.hadprev)
      dx = xfilt_.xprev * freq;

    T edx = dxfilt_(dx, alpha(dcutoff));
    T cutoff = mincutoff + beta * std::abs(edx);

    return xfilt_(x, alpha(cutoff));
  }

  double freq, beta, dcutoff;

  void setAmount(double amt)
  {
    if (amt <= 0.) amt = 0.0001;
    mincutoff = 10. - amt; // mincutoff is basicly the inverse of the amount of filtering
  };

private:
  double mincutoff;
  T alpha(T cutoff)
  {
    T tau = 1. / (2 * M_PI * cutoff);
    T te = 1. / freq;

    return 1.0 / (1.0 + tau / te);
  }

  low_pass_filter<T> xfilt_, dxfilt_;
};
