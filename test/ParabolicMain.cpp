#include <veamy/models/constraints/Constraint.h>
#include <veamy/models/constraints/values/Constant.h>
#include <veamy/Veamer.h>
#include <mesher/models/Region.h>
#include <mesher/models/generator/functions.h>
#include <mesher/voronoi/TriangleMeshGenerator.h>
#include <veamy/models/constraints/values/Function.h>
#include <chrono>
#include <utilities/utilities.h>
#include <veamy/physics/materials/MaterialPlaneStrain.h>

double tangencial(double x, double y){
    double P = -1000;
    double D =  4;
    double I = std::pow(D,3)/12;
    double value = std::pow(D,2)/4-std::pow(y,2);
    return P/(2*I)*value;
}

double uX(double x, double y){
    double P = -1000;
    double Ebar = 1e7/(1 - std::pow(0.3,2));
    double vBar = 0.3/(1 - 0.3);
    double D = 4;
    double L = 8;
    double I = std::pow(D,3)/12;
    return -P*y/(6*Ebar*I)*((6*L - 3*x)*x + (2+vBar)*std::pow(y,2) - 3*std::pow(D,2)/2*(1+vBar));
}

double uY(double x, double y){
    double P = -1000;
    double Ebar = 1e7/(1 - std::pow(0.3,2));
    double vBar = 0.3/(1 - 0.3);
    double D = 4;
    double L = 8;
    double I = std::pow(D,3)/12;
    return P/(6*Ebar*I)*(3*vBar*std::pow(y,2)*(L-x) + (3*L-x)*std::pow(x,2));
}

int main(){
    // Set precision for plotting to output files:    
    // OPTION 1: in "VeamyConfig::instance()->setPrecision(Precision::precision::mid)"    
    // use "small" for 6 digits; "mid" for 10 digits; "large" for 16 digits.    
    // OPTION 2: set the desired precision, for instance, as:    
    // VeamyConfig::instance()->setPrecision(12) for 12 digits. Change "12" by the desired precision.    
    // OPTION 3: Omit any instruction "VeamyConfig::instance()->setPrecision(.....)"    
    // from this file. In this case, the default precision, which is 6 digits, will be used.   
    VeamyConfig::instance()->setPrecision(Precision::precision::mid);  

    // DEFINING PATH FOR THE OUTPUT FILES:
    // If the path for the output files is not given, they are written to /home directory by default.
    // Otherwise, include the path. For instance, for /home/user/Documents/Veamy/output.txt , the path
    // must be "Documents/Veamy/output.txt"
    // CAUTION: the path must exists either because it is already in your system or becuase it is created
    // by Veamy's configuration files. For instance, Veamy creates the folder "/test" inside "/build", so
    // one can save the output files to "/build/test/" folder, but not to "/build/test/mycustom_folder",
    // since "/mycustom_folder" won't be created by Veamy's configuration files.
    std::string meshFileName = "Software/Veamy-master/build/test/parabolic_beam_mesh.txt";
    std::string dispFileName = "Software/Veamy-master/build/test/parabolic_beam_displacements.txt";
	
    std::cout << "*** Starting Veamy ***" << std::endl;
    std::cout << "--> Test: Cantilever beam subjected to a parabolic end load <--" << std::endl;
    std::cout << "..." << std::endl;	

    std::cout << "+ Defining the domain ... ";
    std::vector<Point> rectangle4x8_points = {Point(0, -2), Point(8, -2), Point(8, 2), Point(0, 2)};
    Region rectangle4x8(rectangle4x8_points);
    std::cout << "done" << std::endl;

    std::cout << "+ Generating polygonal mesh ... ";
    rectangle4x8.generateSeedPoints(PointGenerator(functions::constantAlternating(), functions::constant()), 24, 12);
    std::vector<Point> seeds = rectangle4x8.getSeedPoints();
    TriangleMeshGenerator meshGenerator = TriangleMeshGenerator (seeds, rectangle4x8);
    PolygonalMesh mesh = meshGenerator.getMesh();
    std::cout << "done" << std::endl;

    std::cout << "+ Printing mesh to a file ... ";
    mesh.printInFile(meshFileName);
    std::cout << "done" << std::endl;

    std::cout << "+ Defining Dirichlet and Neumann boundary conditions ... ";
    EssentialConstraints essential;
    Function* uXConstraint = new Function(uX);
    Function* uYConstraint = new Function(uY);

    PointSegment leftSide(Point(0,-2), Point(0,2));
    SegmentConstraint const1 (leftSide, mesh.getPoints(), Constraint::Direction::Horizontal, uXConstraint);
    essential.addConstraint(const1, mesh.getPoints());

    SegmentConstraint const2 (leftSide, mesh.getPoints(), Constraint::Direction::Vertical, uYConstraint);
    essential.addConstraint(const2, mesh.getPoints());

    NaturalConstraints natural;

    Function* tangencialLoad = new Function(tangencial);
    PointSegment rightSide(Point(8,-2), Point(8,2));

    SegmentConstraint const3 (rightSide, mesh.getPoints(), Constraint::Direction::Vertical, tangencialLoad);
    natural.addConstraint(const3, mesh.getPoints());

    ConstraintsContainer container;
    container.addConstraints(essential, mesh);
    container.addConstraints(natural, mesh);
    std::cout << "done" << std::endl;

    std::cout << "+ Defining linear elastic material ... ";
    Material* material = new MaterialPlaneStrain (1e7, 0.3);
    VeamyConditions conditions(container, material);
    std::cout << "done" << std::endl;

    std::cout << "+ Preparing the simulation ... ";
    Veamer v;
    v.initProblem(mesh, conditions);
    std::cout << "done" << std::endl;

    std::cout << "+ Simulating ... ";
    Eigen::VectorXd x = v.simulate(mesh);
    std::cout << "done" << std::endl;

    std::cout << "+ Printing nodal displacement solution to a file ... ";
    v.writeDisplacements(dispFileName, x);
    std::cout << "done" << std::endl;
    std::cout << "+ Problem finished successfully" << std::endl;
    std::cout << "..." << std::endl;
    std::cout << "Check output files:" << std::endl;
    std::string path1 = utilities::getPath();
    std::string path2 = utilities::getPath();
    path1 +=  meshFileName;
    path2 +=  dispFileName;
    std::cout << path1 << std::endl;
    std::cout << path2 << std::endl;
    std::cout << "*** Veamy has ended ***" << std::endl;
}
