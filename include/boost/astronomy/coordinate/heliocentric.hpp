#ifndef BOOST_ASTRONOMY_COORDINATE_HELIOCENTRIC_HPP
#define BOOST_ASTRONOMY_COORDINATE_HELIOCENTRIC_HPP

#include <boost/astronomy/coordinate/base_ecliptic_frame.hpp>

namespace boost { namespace astronomy { namespace coordinate {

template
<
    typename Representation, typename Differential
>
struct heliocentric : public base_ecliptic_frame<Representation, Differential>
{

public:
    //default constructor no initialization
    heliocentric() {}

    //!constructs object from another representation object
    template <typename OtherRepresentation>
    heliocentric(OtherRepresentation const& representation_data) : base_ecliptic_frame
        <Representation, Differential>(representation_data) {}

    //!constructs object from provided components of representation
    heliocentric
    (
        typename Representation::quantity1 const& lat,
        typename Representation::quantity2 const& lon,
        typename Representation::quantity3 const& distance
    ) : base_ecliptic_frame<Representation, Differential>(lat, lon, distance) {}

    //!constructs object from provided components of representation and differential
    heliocentric
    (
        typename Representation::quantity1 const& lat,
        typename Representation::quantity2 const& lon,
        typename Representation::quantity3 const& distance,
        typename Differential::quantity1 const& pm_lat,
        typename Differential::quantity2 const& pm_lon_coslat,
        typename Differential::quantity3 const& radial_velocity
    ) : base_ecliptic_frame<Representation, Differential>
            (lat, lon, distance, pm_lat, pm_lon_coslat, radial_velocity) {}

    //!constructs object from other representation and differential objects
    template <typename OtherRepresentation, typename OtherDifferential>
    heliocentric
    (
        OtherRepresentation const& representation_data,
        OtherDifferential const& differential_data
    ) : base_ecliptic_frame<Representation, Differential>
            (representation_data, differential_data) {}
};

}}} //namespace boost::astronomy::coordinate

#endif // !BOOST_ASTRONOMY_COORDINATE_HELIOCENTRIC_HPP

