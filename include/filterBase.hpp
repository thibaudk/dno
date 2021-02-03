#include <array>

using namespace std;

template <typename T = double, size_t N = 2>
struct filterBase
{
    array<T, N> operator * (array<T, N> a, double d)
    {
      for (size_t i = 0; i < N; i++) a[i] *= d;

      return a;
    }

    array<T, N> operator + (array<T, N> a, double d)
    {
      for (size_t i = 0; i < N; i++) a[i] += d;

      return a;
    }
};
