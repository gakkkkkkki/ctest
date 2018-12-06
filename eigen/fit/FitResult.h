#ifndef RecoPixelVertexing_PixelTrackFitting_interface_FitResult_h
#define RecoPixelVertexing_PixelTrackFitting_interface_FitResult_h

#include <cmath>

#include <cuda_runtime.h>
#include <Eigen/Core>
#include <Eigen/Eigenvalues>

#include "../../cuda/cuda_assert.h"

namespace Rfit
{

constexpr double d = 1.e-4;          //!< used in numerical derivative (J2 in Circle_fit())
constexpr unsigned int max_nop = 4;  //!< In order to avoid use of dynamic memory

constexpr auto Dynamic = Eigen::Dynamic;
using VectorXd = Eigen::VectorXd;
using MatrixXd = Eigen::MatrixXd;
using MatrixNd = Eigen::Matrix<double, Dynamic, Dynamic, 0, max_nop, max_nop>;
using ArrayNd = Eigen::Array<double, Dynamic, Dynamic, 0, max_nop, max_nop>;
using Matrix2Nd = Eigen::Matrix<double, Dynamic, Dynamic, 0, 2 * max_nop, 2 * max_nop>;
using Matrix3Nd = Eigen::Matrix<double, Dynamic, Dynamic, 0, 3 * max_nop, 3 * max_nop>;
using Matrix2xNd = Eigen::Matrix<double, 2, Dynamic, 0, 2, max_nop>;
using Array2xNd = Eigen::Array<double, 2, Dynamic, 0, 2, max_nop>;
using Matrix3xNd = Eigen::Matrix<double, 3, Dynamic, 0, 3, max_nop>;
using MatrixNx3d = Eigen::Matrix<double, Dynamic, 3, 0, max_nop, 3>;
using MatrixNx5d = Eigen::Matrix<double, Dynamic, 5, 0, max_nop, 5>;
using VectorNd = Eigen::Matrix<double, Dynamic, 1, 0, max_nop, 1>;
using Vector2Nd = Eigen::Matrix<double, Dynamic, 1, 0, 2 * max_nop, 1>;
using Vector3Nd = Eigen::Matrix<double, Dynamic, 1, 0, 3 * max_nop, 1>;
using RowVectorNd = Eigen::Matrix<double, 1, Dynamic, 1, 1, max_nop>;
using RowVector2Nd = Eigen::Matrix<double, 1, Dynamic, 1, 1, 2 * max_nop>;



using Vector2d = Eigen::Vector2d;
using Vector3d = Eigen::Vector3d;
using Vector4d = Eigen::Vector4d;
using Matrix2d = Eigen::Matrix2d;
using Matrix3d = Eigen::Matrix3d;
using Matrix4d = Eigen::Matrix4d;
using Matrix5d = Eigen::Matrix<double, 5, 5>;
using Matrix6d = Eigen::Matrix<double, 6, 6>;
using Vector5d = Eigen::Matrix<double, 5, 1>;

using Matrix3f = Eigen::Matrix3f;
using Vector3f = Eigen::Vector3f;
using Vector4f = Eigen::Vector4f;
using Vector6f = Eigen::Matrix<double, 6, 1>;

using u_int = unsigned int;


struct circle_fit
{
    Vector3d par;  //!< parameter: (X0,Y0,R)
    Matrix3d cov;
    /*!< covariance matrix: \n
      |cov(X0,X0)|cov(Y0,X0)|cov( R,X0)| \n
      |cov(X0,Y0)|cov(Y0,Y0)|cov( R,Y0)| \n
      |cov(X0, R)|cov(Y0, R)|cov( R, R)|
  */
    int64_t q;  //!< particle charge
    double chi2 = 0.0;
};

struct line_fit
{
    Vector2d par;  //!<(cotan(theta),Zip)
    Matrix2d cov;
    /*!<
      |cov(c_t,c_t)|cov(Zip,c_t)| \n
      |cov(c_t,Zip)|cov(Zip,Zip)|
  */
    double chi2 = 0.0;
};

struct helix_fit
{
    Vector5d par;  //!<(phi,Tip,pt,cotan(theta)),Zip)
    Matrix5d cov;
    /*!< ()->cov() \n
      |(phi,phi)|(Tip,phi)|(p_t,phi)|(c_t,phi)|(Zip,phi)| \n
      |(phi,Tip)|(Tip,Tip)|(p_t,Tip)|(c_t,Tip)|(Zip,Tip)| \n
      |(phi,p_t)|(Tip,p_t)|(p_t,p_t)|(c_t,p_t)|(Zip,p_t)| \n
      |(phi,c_t)|(Tip,c_t)|(p_t,c_t)|(c_t,c_t)|(Zip,c_t)| \n
      |(phi,Zip)|(Tip,Zip)|(p_t,Zip)|(c_t,Zip)|(Zip,Zip)|
  */
    float chi2_circle;
    float chi2_line;
//    Vector4d fast_fit;
    int32_t q;  //!< particle charge
} __attribute__((aligned(16)));

} // namespace RFit
#endif
