#pragma once
#include "include/1efilter.hpp"
#include "include/average.hpp"
#include "include/median.hpp"

#include <variant>

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

namespace dno
{
enum type
{
  OneEuro = 0,
  LowPass,
  Average,
  Median
};

using namespace value_filters;

template <typename T = double>
class DeNoiser
{
public:
  DeNoiser() : filters {std::in_place_index_t<0> {}, one_euro_filter<T> {}}
  {
  }

  void set_type(const type& t = OneEuro)
  {
    if (t != filters.index())
    {
      switch (t)
      {
        case LowPass:
          filters = low_pass_filter<T> {};
          break;
        case Average:
          filters = floating_average<T> {};
          break;
        case Median:
          filters = floating_median<T> {};
          break;
        default:
          filters = one_euro_filter<T> {};
          break;
      }
    }
  }

  T operator()(T val)
  {
    std::visit([&val](auto f) { val = f(val); }, filters);
    return val;
  }

  void set_amount(const double& amount)
  {
    if (current_amount != amount)
    {
      // expects values between 0 and 1
      std::visit(
          [&amount](auto f) { f.set_amount(amount * SCALED_AMOUNT); }, filters);

      current_amount = amount;
    }
  }

  void set_freq(const double& freq) // 1e & LP
  {
    if (current_freq != freq)
    {
      if (filters.index() < 2)
        std::visit([&freq](auto f) { f.freq = freq; }, filters);

      current_freq = freq;
    }
  }

  void set_cutoff(const double& cutoff) // 1e & LP
  {
    if (current_cutoff != cutoff)
    {
      if (filters.index() < 2)
        std::visit([&cutoff](auto f) { f.dcutoff = cutoff; }, filters);

      current_cutoff = cutoff;
    }
  }

  void set_1e_beta(const double& beta) // 1e only
  {
    if (current_beta != beta)
    {
      if (filters.index() == 0)
        std::get<0>(filters).beta = beta;

      current_beta = beta;
    }
  }

protected:
  double current_amount;
  double current_freq;
  double current_cutoff;
  double current_beta;

  std::variant<
      one_euro_filter<T>, low_pass_filter<T>, floating_average<T>,
      floating_median<T>>
      filters;
};

}
