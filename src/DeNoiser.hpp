#pragma once
#include "../include/1efilter.hpp"

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

#define INIT_FREQ 120
#define INIT_AMOUNT 1
#define INIT_BETA 1
#define INIT_DCUTOFF 1

template <typename T = double>
class DeNoiser
{
  public :
    DeNoiser()
      : one_euro{INIT_FREQ,
                 T{INIT_AMOUNT},
                 T{INIT_BETA},
                 T{INIT_DCUTOFF}}
    {}

    T operator()(T val)
    {
      return one_euro(val);
    }

    void setAmount(float amount = INIT_AMOUNT)
    {
      amount *= 10;

      if (amount <= 0)
      {
        amount = 0.0001;
      }

      one_euro.mincutoff = static_cast<T>(10 - amount);
    }

    void set1eFreq(double freq = INIT_FREQ)
    {
      one_euro.frequency = freq;
    }

    void set1eBeta(T beta = INIT_BETA)
    {
      one_euro.beta = beta;
    }

    void set1eDcutOff(T cutoff = INIT_DCUTOFF)
    {
      one_euro.dcutoff = cutoff;
    }

  protected :
    one_euro_filter<T> one_euro;
};
