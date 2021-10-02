#pragma once
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

#define SCALED_AMOUNT 10.

namespace dno {

enum type { OneEuro = 0, LowPass, Average, Median };

using namespace::value_filters;

template <typename T = double>
class DeNoiser
{
public :
  DeNoiser()
    : filters{one_euro_filter<T>{}}
  {}

  inline void setType(const type& t = OneEuro)
  {
    if (t != filters.index())
    {
      switch (t)
      {
      case LowPass :
        filters = low_pass_filter<T>{};
        break;
      case Average :
        filters = floating_average<T>{};
        break;
      case Median :
        filters = floating_median<T>{};
        break;
      default:
        filters = one_euro_filter<T>{};
        break;
      }
    }
  }

  T operator()(T val)
  {
    std::visit([&val](auto f){ val = f(val); }, filters);
    return val;
  }

  void setAmount(const double& amount)
  {
    if (currentAmount != amount)
    {
      // expects values between 0 and 1
      std::visit(
            [&amount]
            (auto f)
      { f.setAmount(amount * SCALED_AMOUNT); },
      filters);

      currentAmount = amount;
    }
  }

  void setFreq(const double& freq) // 1e & LP
  {
    if (currentFreq != freq)
    {
      if (filters.index() < 2)
        std::visit(
              [&freq]
              (auto f)
        { f.freq = freq; },
        filters);

      currentFreq = freq;
    }
  }

  void setCutOff(const double& cutoff) // 1e & LP
  {
    if (currentCutoff != cutoff)
    {
      if (filters.index() < 2)
        std::visit(
              [&cutoff]
              (auto f)
        { f.dcutoff = cutoff; },
        filters);

      currentCutoff = cutoff;
    }
  }

  void set1eBeta(const double& beta) // 1e only
  {
    if (currentBeta != beta)
    {
      if (filters.index() == 0)
        std::get<0>(filters).beta = beta;

      currentBeta = beta;
    }
  }

protected :
  double currentAmount;
  double currentFreq;
  double currentCutoff;
  double currentBeta;

  std::variant<one_euro_filter<T>,
  low_pass_filter<T>,
  floating_average<T>,
  floating_median<T>> filters;
};

}
