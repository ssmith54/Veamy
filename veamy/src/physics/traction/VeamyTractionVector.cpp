#include <veamy/physics/traction/VeamyTractionVector.h>
#include <veamy/physics/traction/point_forces.h>

VeamyTractionVector::VeamyTractionVector(Polygon p, UniqueList<Point> points, NaturalConstraints natural) {
    this->p = p;
    this->points = points.getList();
    this->natural = natural;
}

Eigen::VectorXd VeamyTractionVector::computeTractionVector(IndexSegment segment) {
    Eigen::VectorXd result(4);
    isConstrainedInfo constrainedInfo = natural.isConstrainedBySegment(points, segment);

    if(constrainedInfo.isConstrained){
        std::vector<int> polygonPoints = p.getPoints();
        int n = (int) polygonPoints.size();

        std::vector<SegmentConstraint> constraints = natural.getConstraintInformation(constrainedInfo.container);

        Eigen::MatrixXd Nbar;
        Nbar = Eigen::MatrixXd::Zero(2,2);
        Nbar(0,0) = 1.0/2;
        Nbar(1,1) = 1.0/2;

        Eigen::VectorXd hFirst, hSecond;
        hFirst = Eigen::VectorXd::Zero(2), hSecond = Eigen::VectorXd::Zero(2);

        for(Constraint c: constraints){
            hFirst(0) += c.getValue(points[segment.getFirst()])*c.isAffected(DOF::Axis::x);
            hFirst(1) += c.getValue(points[segment.getFirst()])*c.isAffected(DOF::Axis::y);

            hSecond(0) += c.getValue(points[segment.getSecond()])*c.isAffected(DOF::Axis::x);
            hSecond(1) += c.getValue(points[segment.getSecond()])*c.isAffected(DOF::Axis::y);
        }

        double length = segment.length(points);
        Eigen::VectorXd resultFirst = length*(Nbar.transpose()*hFirst);
        Eigen::VectorXd resultSecond = length*(Nbar.transpose()*hSecond);

        result << resultFirst, resultSecond;
    } else {
        result = Eigen::VectorXd::Zero(4);
    }

    point_forces::addPointForces(result, natural, points[segment.getFirst()], points[segment.getSecond()]);

    return result;
}