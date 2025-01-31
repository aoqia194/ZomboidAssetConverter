#ifndef LOTPACK_HPP
#define LOTPACK_HPP

#include <cstdint>

//! TODO: Document.
struct lotpack {
    //! I don't know at all what this is.
    uint32_t offset;
    //! A flat vector matrix storing arbitrary XYZ data.
    std::vector<std::vector<std::vector<uint32_t> > > data{};
    size_t offset_in_data[800]{};
};

#endif //LOTPACK_HPP
