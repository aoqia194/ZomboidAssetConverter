#ifndef ROOMRECT_HPP
#define ROOMRECT_HPP

#include <cstdint>

namespace map::types
{
    //! TODO: Document
    struct roomrect
    {
        uint32_t x{};

        uint32_t y{};

        uint32_t width{};

        uint32_t height{};
    };
}

#endif //ROOMRECT_HPP
