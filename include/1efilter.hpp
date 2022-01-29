/*(utf8)
 * This is a slight tweek of the
 * 1€ Filter, template-compliant version
 * by Jonathan Aceituno <join@oin.name>
 * omiting timestamps, alowing for interger input,
 * and giving independant access to the lowpass filter.
 *
 * For details, see http://www.lifl.fr/~casiez/1euro
 * * quote * "Note that parameters fcmin and beta have clear
 * conceptual relationships: if high speed lag is a problem,
 * increase beta; if slow speed jitter is a problem, decrease
 * fcmin."
*/

#include "filter_base.hpp"

#include <boost/math/constants/constants.hpp>

#define INIT_FREQ 120.
#define INIT_CUTOFF 1.
#define INIT_MIN 1.
#define INIT_BETA 1.

using namespace boost::math::constants;

namespace value_filters
{
template <typename T = double>
class low_pass_filter : public filter_base<T>
{
public:
  low_pass_filter(double _freq = INIT_FREQ, double _dcutoff = INIT_CUTOFF)
      : xprev {0}, hadprev {false}, hatxprev {0}, alpha {0}
  {
    set_alpha(_freq, _dcutoff);
  }

  T operator()(T x)
  {
    T hatx {0};

    if (hadprev)
    {
      hatx = alpha * x + (1 - alpha) * hatxprev;
    }
    else
    {
      hatx = x;
      hadprev = true;
    }

    hatxprev = hatx;
    xprev = x;
    return hatx;
  }

  T xprev {};
  bool hadprev {};

  void set_alpha(double cutoff, double _freq)
  {
    this->dcutoff = cutoff;
    this->freq = _freq;
    compute_alpha();
  }

  void set_amount(double amt)
  {
    set_alpha(pow(1 / (1 + amt), 2), this->freq);
  }

  void update()
  {
    compute_alpha();
  }

private:
  T hatxprev{}, alpha{};

  void compute_alpha()
  {
    T tau = one_div_two_pi<double>() * (1. / this->dcutoff);
    double te = 1. / this->freq;

    alpha = 1. / (1. + tau / te);
  }
};

template <typename T = double>
struct one_euro_filter : public filter_base<T>
{
  one_euro_filter(
      double _freq = INIT_FREQ, double _mincutoff = INIT_MIN,
      double _beta = INIT_BETA, double _dcutoff = INIT_CUTOFF)
      : filter_base<T> (_freq, _dcutoff)
      , beta {_beta}, mincutoff {_mincutoff}
  {
  }

  T operator()(T x)
  {
    T dx {0};

    if (xfilt_.hadprev)
      dx = (x - xfilt_.xprev) * this->freq;

    dxfilt_.set_alpha(this->dcutoff, this->freq);
    T edx = dxfilt_(dx);
    T cutoff = mincutoff + beta * std::abs(static_cast<double>(edx));

    xfilt_.set_alpha(cutoff, this->freq);
    return xfilt_(x);
  }

  double beta {};

  void set_amount(double amt)
  {
    if (amt <= 0.) amt = 0.0001;
    // mincutoff is basicly the inverse of the amount of filtering
    mincutoff = SCALED_AMOUNT - amt;
  }

private:
  double mincutoff {};

  low_pass_filter<T> xfilt_ {}, dxfilt_ {};
};

}
