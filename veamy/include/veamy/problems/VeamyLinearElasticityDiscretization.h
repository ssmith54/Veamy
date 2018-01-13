#ifndef VEAMY_LINEARELASTICITYDISCRETIZATION_H
#define VEAMY_LINEARELASTICITYDISCRETIZATION_H

#include <veamy/physics/conditions/LinearElasticityConditions.h>
#include <veamy/models/Element.h>
#include <veamy/Veamer.h>
#include <veamy/postprocess/utilities/NormResult.h>
#include "ProblemDiscretization.h"

/*
 * Class that represents the resolution of the linear elasticity problem using the Virtual Element Method
 */
class VeamyLinearElasticityDiscretization : public ProblemDiscretization<Polygon,Veamer>{
public:
    /*
     * Conditions of the linear elasticity problem
     */
    LinearElasticityConditions* conditions;

    VeamyLinearElasticityDiscretization(LinearElasticityConditions* conditions);
    Element<Polygon>* createElement(Veamer *v, Polygon &poly, UniqueList<Point> &points);

    Mesh<Polygon> initProblemFromFile(Veamer *v, std::string fileName);
    NormResult computeErrorNorm(NormCalculator<Polygon> *calculator, Mesh<Polygon> mesh);
};

#endif