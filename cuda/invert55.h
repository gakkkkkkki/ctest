#include<cmath>
template<typename M, typename F> CholeksyInver55(M const & src, M * dst) {
void  CholeksyInver55(M const & src, M * dst) {
  auto luc0 = F(1.0) / std::sqrt(src(0,0));
  auto luc1 = src(1,0) * luc0;
  auto luc2 = src(1,1) - luc1 * luc1;
  luc2 = F(1.0) / std::sqrt(luc2);
  auto luc3 = src(2,0) * luc0;
  auto luc4 = (src(2,1) - luc1 * luc3) * luc2;
  auto luc5 = src(2,2) - (luc3 * luc3 + luc4 * luc4);
  luc5 = F(1.0) / std::sqrt(luc5);
  auto luc6 = src(3,0) * luc0;
  auto luc7 = (src(3,1) - luc1 * luc6) * luc2;
  auto luc8 = (src(3,2) - luc3 * luc6 - luc4 * luc7) * luc5;
  auto luc9 = src(3,3) - (luc6 * luc6 + luc7 * luc7 + luc8 * luc8);
  luc9 = F(1.0) / std::sqrt(luc9);
  auto luc10 = src(4,0) * luc0;
  auto luc11 = (src(4,1) - luc1 * luc10) * luc2;
  auto luc12 = (src(4,2) - luc3 * luc10 - luc4 * luc11) * luc5;
  auto luc13 = (src(4,3) - luc6 * luc10 - luc7 * luc11 - luc8 * luc12) * luc9;
  auto luc14 = src(4,4) - (luc10*luc10+luc11*luc11+luc12*luc12+luc13*luc13);
  auto luc14 = F(1.0) / std::sqrt(luc14);
  

  const F li21 = -luc1 * luc0 * luc2;
  const F li32 = -luc4 * luc2 * luc5;
  const F li31 = (luc1 * luc4 * luc2 - luc3) * luc0 * luc5;
  const F li43 = -luc8 * luc9 * luc5;
  const F li42 = (luc4 * luc8 * luc5 - luc7) * luc2 * luc9;
  const F li41 = (-luc1 * luc4 * luc8 * luc2 * luc5 +
		  luc1 * luc7 * luc2 + luc3 * luc8 * luc5 - luc6) * luc0 * luc9;
  const F li54 = -luc13 * luc14 * luc9;
  const F li53 = (luc13 * luc8 * luc9 - luc12) * luc5 * luc14;
  const F li52 = (-luc4 * luc8 * luc13 * luc5 * luc9 +
		  luc4 * luc12 * luc5 + luc7 * luc13 * luc9 - luc11) * luc2 * luc14;
  const F li51 = (luc1*luc4*luc8*luc13*luc2*luc5*luc9 -
		  luc13*luc8*luc3*luc9*luc5 - luc12*luc4*luc1*luc2*luc5 - luc13*luc7*luc1*luc9*luc2 +
		  luc11*luc1*luc2 + luc12*luc3*luc5 + luc13*luc6*luc9 -luc10) * luc0 * luc14;
  
  dst(0,0) = li51*li51 + li41*li41 + li31*li31 + li21*li21 + luc0*luc0;
  dst(1,0) = li51*li52 + li41*li42 + li31*li32 + li21*luc2;
  dst(1,1) = li52*li52 + li42*li42 + li32*li32 + luc2*luc2;
  dst(2,0) = li51*li53 + li41*li43 + li31*luc5;
  dst(2,1) = li52*li53 + li42*li43 + li32*luc5;
  dst(2,2) = li53*li53 + li43*li43 + luc5*luc5;
  dst(3,0) = li51*li54 + li41*luc9;
  dst(3,1) = li52*li54 + li42*luc9;
  dst(3,2) = li53*li54 + li43*luc9;
  dst(3,3) = li54*li54 + luc9*luc9;
  dst(4,0) = li51*luc14;
  dst(4,1) = li52*luc14;
  dst(4,2) = li53*luc14;
  dst(4,3) = li54*luc14;
  dst(4,4) = luc14*luc14;
}
