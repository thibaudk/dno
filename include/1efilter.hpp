/*(utf8)
This is a slight tweek of the
1€ Filter, template-compliant version
by Jonathan Aceituno <join@oin.name>
omiting timestamps

For details, see http://www.lifl.fr/~casiez/1euro
*/

#include "filter_base.hpp"

namespace value_filters {

template <typename T = double>
class low_pass_filter : public filter_base<T>
{
public :
  low_pass_filter(double _freq = 120., double _dcutoff = 1.)
    : hatxprev{0}, xprev{0}, alpha{0}, hadprev{false}
  {
    setAlpha();
  }

  T operator()(T x)
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

  T xprev;
  bool hadprev;
  double freq, dcutoff;

  void setAlpha(double cutoff = 1., double _freq = 120.)
  {
    dcutoff = cutoff;
    freq = _freq;
    computeAlpha();
  }

  void setAmount(double amt)
  {
    if (amt <= 0.) amt = 0.0001;
    setAlpha(10. - amt); // mincutoff is basicly the inverse of the amount of filtering
  }

private:
  T hatxprev, alpha;

  void computeAlpha()
  {
    T tau = 1. / (2 * M_PI * dcutoff);
    double te = 1. / freq;

    alpha = 1.0 / (1.0 + tau / te);
  }
};

template <typename T = double/*, typename timestamp_t = double*/>
struct one_euro_filter : public filter_base<T>
{
  one_euro_filter(double _freq = 120., double _mincutoff = 1., double _beta = 1., double _dcutoff = 1.) :
    freq{_freq}, mincutoff{_mincutoff}, beta{_beta}, dcutoff{_dcutoff}
  {}

  T operator()(T x)
  {
    T dx{0};

    if (xfilt_.hadprev)
      dx = xfilt_.xprev * freq;

    dxfilt_.setAlpha(dcutoff, freq);
    T edx = dxfilt_(dx);
    T cutoff = mincutoff + beta * std::abs(edx);

    xfilt_.setAlpha(cutoff, freq);
    return xfilt_(x);
    return x;
  }

  double freq, beta, dcutoff;

  void setAmount(double amt)
  {
    if (amt <= 0.) amt = 0.0001;
    mincutoff = 10. - amt; // mincutoff is basicly the inverse of the amount of filtering
  };

private:
  double mincutoff;

  low_pass_filter<T> xfilt_, dxfilt_;
};

}
