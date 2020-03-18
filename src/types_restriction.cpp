#include "types_restriction.hpp"

namespace vllm
{
void Edge_Scale_Restriction::computeError()
{
  const VertexSim3Expmap* vp0 = static_cast<const VertexSim3Expmap*>(_vertices[0]);
  double scale = vp0->estimate().scale();

  if (scale < 0.6)
    _error(0) = 10000;
  else if (scale > 1.5)
    _error(0) = 10000;
  else
    _error(0) = gain * (scale - 1.0);
}

void Edge_Smooth_Restriction::computeError()
{
  const VertexSim3Expmap* vp0 = static_cast<const VertexSim3Expmap*>(_vertices[0]);

  Eigen::Vector3d older = measurement().older_pos;
  Eigen::Vector3d old = measurement().old_pos;
  Eigen::Vector3d now = vp0->estimate().map(measurement().camera_pos);

  Eigen::Vector3d dx = now - old - old + older;
  if (dx.cwiseAbs().maxCoeff() > 0.5)
    _error = 1e4 * Eigen::Vector3d::Ones();
  else
    _error = gain * (dx);
}

void Edge_Latitude_Restriction::computeError()
{
  const VertexSE3* vp0 = static_cast<const VertexSE3*>(_vertices[0]);
  Eigen::Matrix3d R = vp0->estimate().rotation();
  Eigen::Vector3d ez(0, 0, 1);

  double swing = (ez - R * ez).norm();
  if (swing > 0.2)
    _error(0) = 10000;
  else
    _error(0) = gain * swing;
}

void Edge_Altitude_Restriction::computeError()
{
  const VertexSim3Expmap* vp0 = static_cast<const VertexSim3Expmap*>(_vertices[0]);
  Eigen::Vector3d now = vp0->estimate().map(measurement());
  _error(0) = gain * now.z();
}
}  // namespace vllm