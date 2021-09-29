#pragma once
#include "include/filter_base.hpp"
#include "include/1efilter.hpp"
#include "include/average.hpp"
#include "include/median.hpp"

/*
 * For more information about "the one euro filter"
 * please visit https://cristal.univ-lille.fr/~casiez/1euro/
 *
 * * quote * "Note that parameters fcmin and beta have clear
 * conceptual relationships: if high speed lag is a problem,
 * increase beta; if slow speed jitter is a problem, decrease
 * fcmin."
 *
 */
namespace dno {

enum type { OneEuro = 0, LowPass, Average, Median };

using namespace::value_filters;

template <typename T = double>
class DeNoiser
{
public :
  DeNoiser()
    : filters{}
  {
    filter = std::get<0>(filters); // initialize with one euro filter
  }

  type currentType{OneEuro};

  void setType(const type& t = OneEuro)
  {
    if (t != currentType)
    {
      switch (t)
      {
      case LowPass :
        filter = std::get<1>(filters);
        break;
      case Average :
        filter = std::get<2>(filters);
        break;
      case Median :
        filter = std::get<3>(filters);
        break;
      default:
        filter = std::get<0>(filters);
        break;

        currentType = t;
      }
    }
  }

  T operator()(T val) { filter(val); }

  void setAmount(double amount)
  {
    amount *= 10.; // expects values between 0 and 1
    filter.setAmount(amount);
  }

  void set1eFreq(const double& freq) // 1e & LP
  {
    if (currentType < 2)
      filter.freq = freq;
  }

  void set1eDcutOff(const double& cutoff) // 1e & LP
  {
    if (currentType < 2)
      filter.dcutoff = cutoff;
  }

  void set1eBeta(const double& beta) // 1e only
  {
    if (currentType == OneEuro)
      filter.beta = beta;
  }

protected :
  std::variant<one_euro_filter<T>,
  low_pass_filter<T>,
  floating_average<T>,
  floating_median<T>> filters;

  filter_base<T> filter;
};

}
