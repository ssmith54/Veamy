#include <veamy/postprocess/calculators/VeamyElasticityDisplacementCalculator.h>
#include <veamy/postprocess/utilities/norm_utilities.h>
#include <veamy/problems/elasticity/elasticity_functions.h>

template <typename T>
VeamyElasticityDisplacementCalculator<T>::VeamyElasticityDisplacementCalculator(DOFS d, Eigen::VectorXd u, std::vector<Point>& points) :
        DisplacementCalculator<T>(d, u) {
    this->points = points;
}

template <typename T>
std::vector<double> VeamyElasticityDisplacementCalculator<T>::getDisplacement(double x, double y, int index, T container) {
    Point average = container.getAverage(this->points);

    Eigen::VectorXd X, Xbar;
    X = Eigen::VectorXd::Zero(2), Xbar = Eigen::VectorXd::Zero(2);

    X(0) = x; X(1) = y;
    Xbar(0) = average.getX(); Xbar(1) = average.getY();

    Eigen::MatrixXd Wc = elasticity_functions::WcMatrix(container, this->points);
    Eigen::VectorXd d =  norm_utilities::getElementNodalValues(container, this->nodalValues, this->dofs);

    Eigen::VectorXd PiCGuH = Wc.transpose()*d;
    Eigen::VectorXd PiRGuH = elasticity_functions::QMatrix(Wc).transpose()*d;

    Eigen::Matrix<double,2,2> PiC;
    Eigen::Matrix<double,2,2> PiR;

    PiC << PiCGuH(0), PiCGuH(2), PiCGuH(2), PiCGuH(1);
    PiR << 0, PiRGuH(0), PiRGuH(1), 0;

    Eigen::VectorXd uhProyection = PiC*(X-Xbar) + PiR*(X-Xbar) + norm_utilities::getAverage(d, 2);

    std::vector<double> uH = {uhProyection(0), uhProyection(1)};
    return uH;
}

template class VeamyElasticityDisplacementCalculator<Triangle>;
template class VeamyElasticityDisplacementCalculator<Polygon>;