#ifndef METAOBJECT_HPP
#define METAOBJECT_HPP

#include <cstdint>

namespace map::types
{
    //! TODO: Document.
    struct metaobject
    {
        uint32_t type{};

        uint32_t x{};

        uint32_t y{};
    };
}

#endif //METAOBJECT_HPP
