#ifndef EIGEN_MACROS_HPP
#define EIGEN_MACROS_HPP
  
#include <Eigen/Dense>

// Variable length vectors and and matrices
typedef Eigen::VectorXd XVec;
typedef Eigen::MatrixXd XMat;
typedef Eigen::VectorXi XiVec;
typedef Eigen::MatrixXi XiMat;

// Reference type variable length vectors and and matrices
// Useful for pybind interfacing
typedef Eigen::Ref<XVec > RXVec;
typedef Eigen::Ref<XiVec > RXiVec;

// Map to interface raw array buffers with eigen vectors
typedef Eigen::Map<XVec > XVecMap;
    
// DIM-dimensional fixed length vectors and matrices
// These are defined using macros for use in templated functions
# define DVec Eigen::Matrix<double, DIM, 1>
# define DMat Eigen::Matrix<double, DIM, DIM>

# define RDVec Eigen::Ref<DVec >
# define RDMat Eigen::Ref<DMat >

    
#endif // EIGEN_MACROS_HPP