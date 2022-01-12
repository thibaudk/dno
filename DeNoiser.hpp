#pragma once
#include "include/1efilter.hpp"
#include "include/average.hpp"
#include "include/median.hpp"

#include <variant>

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
  DeNoiser() : filters {std::in_place_index_t<0> {}}
  {
  }

  inline constexpr void set_type(const type& t = OneEuro) noexcept
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

  T operator()(const T& val)
  {
    return std::visit([val](auto& f) { return f(val); }, filters);
  }

  void set_amount(double amount)
  {
    if (current_amount != amount)
    {
      // expects values between 0 and 1
      std::visit(
          [amount](auto& f) { f.set_amount(amount * SCALED_AMOUNT); }, filters);

      current_amount = amount;
    }
  }

  void set_freq(double freq) // 1e & LP
  {
    if (current_freq != freq)
    {
      if (filters.index() < 2)
        std::visit([freq, this](auto& f) {
          f.freq = freq;
          f.update();
        }, filters);

      current_freq = freq;
    }
  }

  void set_cutoff(double cutoff) // 1e & LP
  {
    if (current_cutoff != cutoff)
    {
      if (filters.index() < 2)
        std::visit([cutoff](auto& f) {
          f.dcutoff = cutoff;
          f.update();
        }, filters);

      current_cutoff = cutoff;
    }
  }

  void set_1e_beta(double beta) // 1e only
  {
    if (current_beta != beta)
    {
      if (filters.index() == 0)
      {
        auto& f = std::get<0>(filters);
        f.beta = beta;
        f.update();
      }

      current_beta = beta;
    }
  }

protected:
  double current_amount{},
  current_freq{},
  current_cutoff{},
  current_beta{};

  std::variant<
      one_euro_filter<T>,
      low_pass_filter<T>,
      floating_average<T>,
      floating_median<T>>
      filters;
};

}
