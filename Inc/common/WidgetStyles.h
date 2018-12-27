//
// Created by mohamad on 27.12.18.
//

#ifndef ARMCAN_WIDGETSTYLES_H
#define ARMCAN_WIDGETSTYLES_H

const GWidgetStyle GreenButtonStyle = {
        HTML2COLOR(0xFFFFFF),			// window background
        HTML2COLOR(0x2A8FCD),			// focused

// enabled color set
        {
                HTML2COLOR(0xffffff),		// text
                HTML2COLOR(0x404040),		// edge
                HTML2COLOR(0x28A745),		// fill
                HTML2COLOR(0x00E000)		// progress - active area
        },

// disabled color set
        {
                HTML2COLOR(0xffffff),		// text
                HTML2COLOR(0x808080),		// edge
                HTML2COLOR(0x28A745),		// fill
                HTML2COLOR(0xC0E0C0)		// progress - active area
        },

// pressed color set
        {
                HTML2COLOR(0xffffff),		// text
                HTML2COLOR(0x1e7e34),		// edge
                HTML2COLOR(0x218838),		// fill
                HTML2COLOR(0x00E000)		// progress - active area
        }
};
const GWidgetStyle GreenTextStyle = {
        HTML2COLOR(0xFFFFFF),			// window background
        HTML2COLOR(0x2A8FCD),			// focused

// enabled color set
        {
                HTML2COLOR(0x28A745),		// text
                HTML2COLOR(0x404040),		// edge
                HTML2COLOR(0xE0E0E0),		// fill
                HTML2COLOR(0x00E000)		// progress - active area
        },

// disabled color set
        {
                HTML2COLOR(0x28A745),		// text
                HTML2COLOR(0x808080),		// edge
                HTML2COLOR(0xE0E0E0),		// fill
                HTML2COLOR(0xC0E0C0)		// progress - active area
        },

// pressed color set
        {
                HTML2COLOR(0x28A745),		// text
                HTML2COLOR(0x404040),		// edge
                HTML2COLOR(0x808080),		// fill
                HTML2COLOR(0x00E000)		// progress - active area
        }
};
const GWidgetStyle RedTextStyle = {
        HTML2COLOR(0xFFFFFF),			// window background
        HTML2COLOR(0x2A8FCD),			// focused

// enabled color set
        {
                HTML2COLOR(0xDC3545),		// text
                HTML2COLOR(0x404040),		// edge
                HTML2COLOR(0xE0E0E0),		// fill
                HTML2COLOR(0x00E000)		// progress - active area
        },

// disabled color set
        {
                HTML2COLOR(0xDC3545),		// text
                HTML2COLOR(0x808080),		// edge
                HTML2COLOR(0xE0E0E0),		// fill
                HTML2COLOR(0xC0E0C0)		// progress - active area
        },

// pressed color set
        {
                HTML2COLOR(0xDC3545),		// text
                HTML2COLOR(0x404040),		// edge
                HTML2COLOR(0x808080),		// fill
                HTML2COLOR(0x00E000)		// progress - active area
        }
};

const GWidgetStyle RedButtonStyle = {
        HTML2COLOR(0xFFFFFF),			// window background
        HTML2COLOR(0x2A8FCD),			// focused

// enabled color set
        {
                HTML2COLOR(0xE0E0E0),		// text
                HTML2COLOR(0x404040),		// edge
                HTML2COLOR(0xDC3545),		// fill
                HTML2COLOR(0x00E000)		// progress - active area
        },

// disabled color set
        {
                HTML2COLOR(0xE0E0E0),		// text
                HTML2COLOR(0x808080),		// edge
                HTML2COLOR(0xDC3545),		// fill
                HTML2COLOR(0xC0E0C0)		// progress - active area
        },

// pressed color set
        {
                HTML2COLOR(0xE0E0E0),		// text
                HTML2COLOR(0xbd2130),		// edge
                HTML2COLOR(0xc82333),		// fill
                HTML2COLOR(0x00E000)		// progress - active area
        }
};
const GWidgetStyle GrayButtonStyle = {
        HTML2COLOR(0xFFFFFF),			// window background
        HTML2COLOR(0x2A8FCD),			// focused

// enabled color set
        {
                HTML2COLOR(0xE0E0E0),		// text
                HTML2COLOR(0x6c757d),		// edge
                HTML2COLOR(0x6c757d),		// fill
                HTML2COLOR(0x00E000)		// progress - active area
        },

// disabled color set
        {
                HTML2COLOR(0xE0E0E0),		// text
                HTML2COLOR(0x808080),		// edge
                HTML2COLOR(0x6c757d),		// fill
                HTML2COLOR(0xC0E0C0)		// progress - active area
        },

// pressed color set
        {
                HTML2COLOR(0xE0E0E0),		// text
                HTML2COLOR(0x545b62),		// edge
                HTML2COLOR(0x5a6268),		// fill
                HTML2COLOR(0x00E000)		// progress - active area
        }
};
#endif //ARMCAN_WIDGETSTYLES_H
