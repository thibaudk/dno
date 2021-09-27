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

template <typename T = double>
class DeNoiser
{
public :
  DeNoiser()
    : filter{one_euro_filter<T>{}}
  {}

  enum type { OneEuro = 0, Average, Median };

  type currentType{DeNoiser::OneEuro};

  void setType(DeNoiser::type t = OneEuro)
  {
    if (t != currentType)
    {
      switch (t)
      {
      case Average :
        filter.emplace<1>(floating_average<T>{});
        break;
      case Median :
        filter.emplace<2>(floating_median<T>{});
        break;
      default:
        filter.emplace<0>(one_euro_filter<T>{});
        break;
      }

      currentType = t;
    }
  }

  T operator()(T val)
  {
    return std::get<0>(filter)(val);
  }

  void setAmount(double amount)
  {
    amount *= 10.; // expects values between 0 and 1

    switch (currentType)
    {
    case Average :
      std::get<1>(filter).setAmount(amount);
      break;
    case Median :
      std::get<2>(filter).setAmount(amount);
      break;
    default:
      std::get<0>(filter).setAmount(amount);
      break;
    }
  }

  void set1eFreq(const double& freq)
  {
    if (currentType == OneEuro)
      std::get<0>(filter).frequency = freq;
  }

  void set1eBeta(const double& beta)
  {
    if (currentType == OneEuro)
      std::get<0>(filter).beta = beta;
  }

  void set1eDcutOff(const double& cutoff)
  {
    if (currentType == OneEuro)
      std::get<0>(filter).dcutoff = cutoff;
  }

protected :
  std::variant<one_euro_filter<T>,
  floating_average<T>,
  floating_median<T>> filter;
};
