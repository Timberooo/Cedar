//
// GUI Anchor enum.
//

#ifndef CEDAR_GUI_ANCHOR_H
#define CEDAR_GUI_ANCHOR_H

#include <cstdint>



namespace Cedar::GUI
{
    enum class Anchor : std::int_fast8_t {
        center = 0b00000000,
        top    = 0b00000001,
        left   = 0b00000010,
        bottom = 0b00000100,
        right  = 0b00001000,

        top_left     = top    | left,
        top_right    = top    | right,
        bottom_left  = bottom | left,
        bottom_right = bottom | right
    };


    inline bool anchorContains(Anchor anchor, Anchor contains);



    inline bool anchorContains(Anchor anchor, Anchor contains) {
        return static_cast<std::int_fast8_t>(anchor) & static_cast<std::int_fast8_t>(contains);
    }
}

#endif // CEDAR_GUI_ANCHOR_H