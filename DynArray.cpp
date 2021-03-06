template<typename T>
class DynArray {
   T * a=nullptr;
   unsigned int s=0;
public :
   DynArray(unsigned char * storage, unsigned int isize) : a((T*)(storage)), s(isize){
    for (auto i=0U; i<s; ++i) new((begin()+i)) T();
   }
   DynArray(unsigned char * storage, unsigned int isize, T const& it) : a((T*)(storage)), s(isize){
    for (auto i=0U; i<s; ++i) new((begin()+i)) T(it);
   }

   ~DynArray() { for (auto i=0U; i<s; ++i) a[i].~T(); }

   T & operator[](unsigned int i) { return a[i];}
   T * begin() { return a;}
   T * end() { return a+s;}
   T const & operator[](unsigned int i) const { return a[i];}
   T const * begin() const { return a;}
   T const * end() const { return a+s;}
   unsigned int size() const { return s;}
};

#define declareDynArray(T,n,x)  alignas(alignof(T)) unsigned char x ## _storage[sizeof(T)*n]; DynArray<T> x(x ## _storage,n)
#define initDynArray(T,n,x,i)  alignas(alignof(T)) unsigned char x ## _storage[sizeof(T)*n]; DynArray<T> x(x ## _storage,n,i)


struct A {

int i=-3;
double k=0.1;

virtual ~A(){}

};

#include<cassert>
#include<iostream>
int main(int s, char **) {

  using T=A;

  unsigned n = 4*s;

//  alignas(alignof(T)) unsigned char a_storage[sizeof(T)*n];
//  DynArray<T> a(a_storage,n);

  declareDynArray(T,n,a);

  // T b[n];
  declareDynArray(T,n,b);


  auto pa = [&](auto i) { a[1].k=0.3; return a[i].k; };
  auto pb = [&](auto i) { b[1].k=0.5; return b[i].k; };

  auto loop = [&](auto k) { for(auto const & q : a) k = std::min(k,q.k); return k;};

  std::cout << a[n-1].k << ' ' << pa(1) << ' ' << loop(2.3) << std::endl;
  std::cout << b[n-1].k << ' ' << pb(1) << std::endl;

  initDynArray(bool,n,q,true);
  if (q[n-1]) std::cout << "ok" << std::endl;	

  return 0;
};
