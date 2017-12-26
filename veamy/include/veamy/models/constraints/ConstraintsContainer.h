#ifndef VEAMY_CONSTRAINTSCONTAINER_H
#define VEAMY_CONSTRAINTSCONTAINER_H

#include <delynoi/models/Mesh.h>
#include "EssentialConstraints.h"
#include "NaturalConstraints.h"

/*
 * Container of both essential and natural constraints
 */
class ConstraintsContainer {
private:
    /*
     * Constraints of the problem
     */
    EssentialConstraints essential;
    NaturalConstraints natural;

    /* Checks if the constraints are consistent (can not constraint the same DOF with two different essential constraints)
     * @param n natural constraints of the problem
     * @param e essential constraints of the problem
     * @param points mesh points
     * @return if there are no problems with the conditions
     */
    bool areConsistent(NaturalConstraints n, EssentialConstraints e, UniqueList<Point> points);
public:
    /*
     * Default constructor
     */
    ConstraintsContainer();

    /* Adds natural constraints to the system
     * @param c natural constraints to add
     * @param points mesh points
     */
    void addConstraints(NaturalConstraints c, UniqueList<Point> points);

    /* Adds essential constraints to the system
     * @param c essential constraints to add
     * @param points mesh points
     */
    void addConstraints(EssentialConstraints c, UniqueList<Point> points);

    /* Adds a new DOF to the system, checking if it is constrained
     * @param points mesh points
     * @param DOF_index index of the DOF to add
     * @param axis Axis of the DOF
     * @param pair Pair of segments containing the DOF
     * @param point_index index of the point related to the DOF
     */
    void addConstrainedDOF(std::vector<Point> points, int DOF_index, DOF::Axis axis, SegmentPair pair,
                               int point_index);

    /* Adds a new DOF to the system, checking if it is constrained
     * @param points mesh points
     * @param DOF_index index of the DOF to add
     * @param pair Pair of segments containing the DOF
     * @param point_index index of the point related to the DOF
     */
    void addConstrainedDOF(std::vector<Point> points, int DOF_index, SegmentPair pair, int point_index);

    /*
     * @return essential constraints of the system
     */
    EssentialConstraints getEssentialConstraints();

    /*
     * @return natural constraints of the system
     */
    NaturalConstraints getNaturalConstraints();
};


#endif
