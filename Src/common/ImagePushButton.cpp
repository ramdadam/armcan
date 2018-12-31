
#include <Inc/common/ImagePushButton.h>

#include "gfx.h"
#include "ImagePushButton.h"
#include "romfs_files.h"

gdispImage iconScreenshot;
gdispImage iconPressedScreenshot;
gdispImage iconRepeatOne;
gdispImage iconEdit;

const GWidgetStyle ImageButtonStyle = {
        HTML2COLOR(0x000000),			// window background
        HTML2COLOR(0x2A8FCD),			// focused

        // enabled color set
        {
                HTML2COLOR(0xC0C0C0),		// text
                HTML2COLOR(0xC0C0C0),		// edge
                HTML2COLOR(0xFFFFFF),		// fill
                HTML2COLOR(0x008000)		// progress - active area
        },

        // disabled color set
        {
                HTML2COLOR(0x808080),		// text
                HTML2COLOR(0x404040),		// edge
                HTML2COLOR(0x404040),		// fill
                HTML2COLOR(0x004000)		// progress - active area
        },

        // pressed color set
        {
                HTML2COLOR(0xFFFFFF),		// text
                HTML2COLOR(0xC0C0C0),		// edge
                HTML2COLOR(0x606060),		// fill
                HTML2COLOR(0x008000)		// progress - active area
        }
};


void myButtonRendering(GWidgetObject *gw, void *param) {
    const GColorSet *colors;
    // Get the icon/image
    ImageButtonParameter *imageButtonParameter = (ImageButtonParameter *) param;
    if (!imageButtonParameter) {
        return;
    }
    gdispImage *icon = nullptr;
    // Get the appropriate color pallete from the widget style
    if (!gwinGetEnabled((GHandle) gw)) {
    	icon = imageButtonParameter->iconEnabled;
        colors = &gw->pstyle->enabled;
    }
    else if ((gw->g.flags & GBUTTON_FLG_PRESSED) && imageButtonParameter->iconHover != nullptr) {
        icon = imageButtonParameter->iconHover;
        colors = &gw->pstyle->pressed;
    } else if(imageButtonParameter->iconEnabled != nullptr){
        icon = imageButtonParameter->iconEnabled;
        colors = &gw->pstyle->enabled;
    } else {
        return;
    }
    // Draw the basic rectangle with border
    gdispGFillArea(gw->g.display, gw->g.x + 1, gw->g.y + 1, gw->g.width - 2, gw->g.height - 2, colors->fill);
    gdispGDrawBox(gw->g.display, gw->g.x, gw->g.y, gw->g.width, gw->g.height, colors->edge);
    gdispGImageDraw(gw->g.display, icon, gw->g.x + (gw->g.height - icon->width) / 2,
                    gw->g.y + (gw->g.height - icon->height) / 2, icon->width, icon->height, 0, 0);
}

GHandle createImagePushButton(GWidgetInit *wi, ImageButtonParameter* imageButtonParameter) {
    wi->customDraw = myButtonRendering;
    wi->customParam = imageButtonParameter;
    wi->customStyle = &ImageButtonStyle;
    return gwinButtonCreate(nullptr, wi);
}

gdispImage * loadImageFromRomFs(ImageID desiredImage) {
    switch (desiredImage) {
        case REPEAT_ONE_IMAGE:
            gdispImageOpenFile(&iconRepeatOne, "repeat_one.bmp");
            return &iconRepeatOne;
        case SCREENSHOT_PRESSED_IMAGE:
            gdispImageOpenFile(&iconPressedScreenshot, "screenshot_pressed.bmp");
            return &iconPressedScreenshot;
        case SCREENSHOT_IMAGE:
            gdispImageOpenFile(&iconScreenshot, "screenshot.bmp");
            return &iconScreenshot;
        case EDIT_IMAGE:
            gdispImageOpenFile(&iconEdit, "out.bmp");
            return &iconEdit;
    }
}
