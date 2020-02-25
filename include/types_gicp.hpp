#pragma once
#include <Eigen/Geometry>
#include <g2o/core/base_binary_edge.h>
#include <g2o/core/base_unary_edge.h>
#include <g2o/core/base_vertex.h>
#include <g2o/types/sba/types_sba.h>
#include <g2o/types/sim3/types_seven_dof_expmap.h>
#include <g2o/types/slam3d/types_slam3d.h>
#include <iostream>

namespace vllm
{
using g2o::Matrix3;
using g2o::Vector3;
using g2o::VertexSE3;
using g2o::VertexSim3Expmap;

class EdgeGICP
{
public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
  // point positions
  Vector3 pos0, pos1;
  // unit normals
  Vector3 normal0, normal1;
  // rotation matrix for normal
  Matrix3 R0, R1;

  EdgeGICP();

  // set up rotation matrix for pos0, pos1
  void makeRot0();
  // void makeRot1();

  // returns a precision matrix for point-plane
  Matrix3 prec0(number_t e);
  // Matrix3 prec1(number_t e);

  // return a covariance matrix for plane-plane
  // Matrix3 cov0(number_t e);
  // Matrix3 cov1(number_t e);
};

class Edge_Sim3_GICP : public g2o::BaseUnaryEdge<3, EdgeGICP, VertexSim3Expmap>
{
public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
  Edge_Sim3_GICP() {}
  Edge_Sim3_GICP(const Edge_Sim3_GICP* e);
  Matrix3 cov0, cov1;

  virtual bool read(std::istream&);
  virtual bool write(std::ostream&) const;
  void computeError();
};

class Edge_SE3_GICP : public g2o::BaseUnaryEdge<3, EdgeGICP, VertexSE3>
{
public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
  Edge_SE3_GICP() {}
  Edge_SE3_GICP(const Edge_SE3_GICP* e);
  Matrix3 cov0, cov1;

  virtual bool read(std::istream&);
  virtual bool write(std::ostream&) const;
  void computeError();
};

class Edge_Scale_Regularizer : public g2o::BaseUnaryEdge<1, double, VertexSim3Expmap>
{
private:
  double gain = 1.0;

public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
  Edge_Scale_Regularizer(double gain = 1.0) : gain(gain) {}
  Edge_Scale_Regularizer(const Edge_Scale_Regularizer* e);

  virtual bool read(std::istream&);
  virtual bool write(std::ostream&) const;
  void computeError();
};

class Edge_RollPitch_Regularizer : public g2o::BaseUnaryEdge<1, double, VertexSE3>
{
private:
  double gain = 1.0;

public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
  Edge_RollPitch_Regularizer(double gain = 1.0) : gain(gain) {}
  Edge_RollPitch_Regularizer(const Edge_RollPitch_Regularizer* e);

  virtual bool read(std::istream&)
  {
    std::cerr << __PRETTY_FUNCTION__ << " not implemented yet" << std::endl;
    return false;
  }
  virtual bool write(std::ostream&) const
  {
    std::cerr << __PRETTY_FUNCTION__ << " not implemented yet" << std::endl;
    return false;
  }
  void computeError();
};
}  // namespace vllm