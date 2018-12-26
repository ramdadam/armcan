
#include <Inc/common/ImagePushButton.h>

#include "gfx.h"
#include "ImagePushButton.h"
#include "romfs_files.h"

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
    if (!gwinGetEnabled((GHandle) gw))
        colors = &gw->pstyle->disabled;
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
//    // Draw the string. Use StringBox() for proper justification and word-wrapping support
//    gdispGDrawStringBox(gw->g.display, gw->g.x + gw->g.height, gw->g.y, gw->g.width - gw->g.height, gw->g.height,
//                        gw->text, gw->g.font, colors->text, justifyLeft);
    // Draw the image
    gdispGImageDraw(gw->g.display, icon, gw->g.x + (gw->g.height - icon->width) / 2,
                    gw->g.y + (gw->g.height - icon->height) / 2, icon->width, icon->height, 0, 0);
}

GHandle createImagePushButton(GWidgetInit *wi, ImageButtonParameter* imageButtonParameter) {
    wi->customDraw = myButtonRendering;
    wi->customParam = imageButtonParameter;
    wi->customStyle = &ImageButtonStyle;
    return gwinButtonCreate(nullptr, wi);
}

void loadImageFromRomFs(gdispImage * imageContainer, ImageID desiredImage) {
    switch (desiredImage) {
        case REPEAT_ONE_IMAGE:
            gdispImageOpenFile(imageContainer, "repeat_one.bmp");
            break;
        case SCREENSHOT_PRESSED_IMAGE:
            gdispImageOpenFile(imageContainer, "screenshot_pressed.bmp");
            break;
        case SCREENSHOT_IMAGE:
            gdispImageOpenFile(imageContainer, "screenshot.bmp");
            break;
        case ADD_IMAGE:
            gdispImageOpenFile(imageContainer, "add.bmp");
            break;
        case EDIT_IMAGE:
            gdispImageOpenFile(imageContainer, "out.bmp");
            break;
        case SAVE_IMAGE:
            gdispImageOpenFile(imageContainer, "save.bmp");
            break;
        case BACK_IMAGE:
            gdispImageOpenFile(imageContainer, "back.bmp");
            break;
        case TIMER_IMAGE:
            gdispImageOpenFile(imageContainer, "timer.bmp");
            break;
    }
}
