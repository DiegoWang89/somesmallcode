#include <immintrin.h>
#include <vector>
using namespace std;

// sinx = x -x^3/3!+x^5/5!...
void ComputeSin(int N, int terms, float arr[], float result[]) {
    float three_fac = 6;  // 3!
    for (int i = 0; i < N; i += 8) {
        __m256 ori_x = _mm256_load_ps(&(arr[i]));
        __m256 val = ori_x;
        __m256 numer = _mm256_mul_ps(ori_x, _mm256_mul_ps(ori_x, ori_x));  // x^3
        __m256 denom = _mm256_broadcast_ss(&three_fac);
        int sign = -1;

        for (int j = 1; j <= terms; j++) {
            __m256 tmp = _mm256_div_ps(_mm256_mul_ps(_mm256_set1_ps(sign), numer), denom);
            val = _mm256_add_ps(val, tmp);
            numer = _mm256_mul_ps(numer, _mm256_mul_ps(ori_x, ori_x));
            denom = _mm256_mul_ps(denom, _mm256_set1_ps((2 * j + 2) * (2 * j + 3)));
            sign *= (-1);
        }
        _mm256_store_ps(&(result[i]), val);
    }
}

//==============================================================================
int main(int argc, char* argv[]) {

    vector<float> source, result;
    for (int i = 0; i < 16; i++)
        source.push_back(0.2 + i);
    result.resize(16);

    for (auto e : result)
        cout << " " << e << " ";
    cout << endl;

    ComputeSin(16, 5, source.data(), result.data());

    for (auto e : result)
        cout << " " << e << " ";
    cout << endl;


    return 0;
}
