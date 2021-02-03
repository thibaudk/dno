#include <array>

template <typename T = double, size_t N = 2>
struct filterBase
{
        std::array<T, N> operator * (std::array<T, N> v, double d)
        {
            for (size_t i = 0; i < N; i++) v[i] *= d;

            return v;
        }

        std::array<T, N> operator + (std::array<T, N> v, double d)
        {
            for (size_t i = 0; i < N; i++) v[i] += d;

            return v;
        }

        filterBase();
};
