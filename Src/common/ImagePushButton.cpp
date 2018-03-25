#include "gfx.h"
#include "ImagePushButton.h"
#include "romfs_files.h"

void myButtonRendering(GWidgetObject* gw, void* param)
{
	const GColorSet* colors;
	// Get the icon/image
	gdispImage* icon = (gdispImage*)param;
	if (!icon) {
		return;
	}
	// Get the appropriate color pallete from the widget style
	if (!gwinGetEnabled((GHandle)gw))
		colors = &gw->pstyle->disabled;
	else if ((gw->g.flags & GBUTTON_FLG_PRESSED))
		colors = &gw->pstyle->pressed;
	else
		colors = &gw->pstyle->enabled;
	// Draw the basic rectangle with border
	gdispGFillArea(gw->g.display, gw->g.x+1, gw->g.y+1, gw->g.width-2, gw->g.height-2, colors->fill);
	gdispGDrawBox(gw->g.display, gw->g.x, gw->g.y, gw->g.width, gw->g.height, colors->edge);
	// Draw the string. Use StringBox() for proper justification and word-wrapping support
	gdispGDrawStringBox(gw->g.display, gw->g.x+gw->g.height, gw->g.y, gw->g.width-gw->g.height, gw->g.height, gw->text, gw->g.font, colors->text, justifyLeft);
	// Draw the image
	gdispGImageDraw(gw->g.display, icon, gw->g.x+(gw->g.height-icon->width)/2, gw->g.y+(gw->g.height-icon->height)/2, icon->width, icon->height, 0, 0);
}

GHandle createImagePushButton(GWidgetInit* wi, gdispImage* imageContainer, ImageID desiredImage) {
    switch(desiredImage) {
        case ADD_IMAGE:
        gdispImageOpenFile(imageContainer, "add.bmp");
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
    wi->customDraw = myButtonRendering;	
    wi->customParam = imageContainer;
    return gwinButtonCreate(0, wi);
}
