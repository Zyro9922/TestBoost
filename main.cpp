#include <iostream>
#include <boost/astronomy/coordinate/representation.hpp>
#include <boost/astronomy/coordinate/arithmetic.hpp>
#include <boost/units/systems/si/length.hpp>
#include <boost/units/systems/si/plane_angle.hpp>
#include <boost/units/systems/angle/degrees.hpp>
#include <boost/units/io.hpp>

using namespace boost::astronomy::coordinate;
using namespace boost::units;
using namespace boost::units::si;
namespace bud = boost::units::degree;

int main()
{
    auto point1 = make_cartesian_representation(15.0 * meters, 25.0 * meters, 30.0 * meters);
    auto point2 = make_spherical_representation(45.0 * bud::degrees, 45.0 * bud::degrees, 3.0 * meters);

     cartesian_representation<double, quantity<si::length>, quantity<si::length>, quantity<si::length>>
         cross_result = cross(point1, point2);

     //dot product
//     std::cout << dot(point1, point2) << std::endl;

    // //unit vector
    // auto unit_vector_point = unit_vector(point1);

    // //magnitude
    // std::cout << magnitude(point1) << std::endl;

    return 0;
}