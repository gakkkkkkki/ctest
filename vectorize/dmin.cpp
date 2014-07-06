#include <x86intrin.h>
#include <float.h>
#include <iostream>

double const * lmin(double const * __restrict__ b, double const * __restrict__ e) {
  int n = e-b;
  if (!(n>0)) return e;
  int off = 4-n&3; // modulus 4
  if (4==off) off=0;
  double p[4];
  double k[4];  
  for (int i=0; i!=off;++i) {
    p[i] = DBL_MAX;
    k[i]=0.5+i-off;
  }
  int j=0;
  for (int i=off;i!=4;++i) {
    p[i] = *(b++);
    k[i]=0.5+j++;
  }
  // now b is offset of j

  __m256d fmin = _mm256_loadu_pd(p);
  __m256d res =  _mm256_loadu_pd(k);

  /*  
  std::cout<< "should be zero: " << (e-b)%4 << " " << *b << std::endl;
  for (int i=0; i!=4; ++i) 
    std::cout << k[i] << " " << p[i] << ",   ";
  std::cout<<std::endl;
  */

  __m256d incr = _mm256_set_pd(4,4,4,4);
  __m256d ind = res;
 
  for (; b<e; b+=4) {
    ind =  _mm256_add_pd(incr,ind);
    __m256d ftes = _mm256_loadu_pd(b);
    __m256d mask = _mm256_cmp_pd(ftes,fmin, _CMP_LT_OS);
    fmin = _mm256_min_pd(fmin,ftes);
    res =  _mm256_blendv_pd(res,ind,mask);
    //res = _mm256_or_si128(_mm256_andnot_si128(mask, res), _mm256_and_si128(mask, ind));
  }
  _mm256_storeu_pd(k,res);
  _mm256_storeu_pd(p,fmin);
  for (int i=1; i!=4; ++i) {
    if (p[i]<p[0]) {
      p[0]=p[i];
      k[0]=k[i];
    }
  }

  /*
  for (int i=0; i!=4; ++i) 
    std::cout << k[i] << " " << p[i] << ",  ";
  std::cout<<std::endl;
  for (int i=0; i!=4; ++i) 
    std::cout << (e-n)[int(k[i])] << ",  ";
  std::cout<<std::endl;
  */
  return e-n+int(k[0]);
  
}


#include <cmath>
#include <cstdlib>
int main() {

  drand48();

  double c[100010];
  for (int i=0; i!=10010; ++i) {
    c[i]=drand48();
  }
  for (int i=0; i!=8; ++i) {
    double const * k = lmin(c,c+10000+i);
    std::cout << k-c << " " << *k << std::endl;
  }

  return 0;
}


