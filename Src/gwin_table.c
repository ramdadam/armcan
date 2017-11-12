/*
 * This file is subject to the terms of the GFX License. If a copy of
 * the license was not distributed with this file, you can obtain one at:
 *
 *              http://ugfx.org/license.html
 */

/**
 * @file	src/gwin/gwin_table.c
 * @brief	GWIN table widget header file
 */

#include "gfx.h"
#include "gwin_table.h"
#include <unistd.h>
#include <stdio.h>

#if GFX_USE_GWIN && GWIN_NEED_TABLE

#include "../armcan/ugfx/src/gwin/gwin_class.h"
#include <string.h>
#include <stdlib.h>

// user for the default drawing routine
#define GTABLE_SCROLLWIDTH		16	// the border from the scroll buttons to the frame
#define GTABLE_ARROW_SZ			10	// arrow side length
#define GTABLE_HORIZ_PADDING 	5	// extra horizontal padding for text
#define GTABLE_ROW_PADDING		2	// extra vertical padding for text
#define GTABLE_TITLE_PADDING 	4   // extra padding for title
#define GTABLE_HEADER_PADDING 	4   // extra padding for header

// Macro's to assist in data type conversions
#define gh2obj		((GTableObject *)gh)
#define gw2obj		((GTableObject *)gw)
#define qi2li		((TableRow *)qi)
#define qix2li		((TableRow *)qix)
#define ple			((GEventGWinTable *)pe)

static void _gwinGHTableMarkAllRows(GHandle gh, uint32_t mask){
	const gfxQueueASyncItem   *   qi;

	for(qi = gfxQueueASyncPeek(&gh2obj->table_head); qi; qi = gfxQueueASyncNext(qi))
		qi2li->modified |= mask;
}

static void _gwinGWTableMarkAllRows(GWidgetObject *gw, uint32_t mask){
	const gfxQueueASyncItem   *   qi;

	for(qi = gfxQueueASyncPeek(&gw2obj->table_head); qi; qi = gfxQueueASyncNext(qi))
		qi2li->modified |= mask;
}

static void _drawScrollBar(GWidgetObject* gw, int offset, coord_t* iwidth, coord_t iheight)
{
	const GColorSet *			ps;
	int	scrollHeight = gw->g.height -offset;

	#if GDISP_NEED_CONVEX_POLYGON
		static const point upArrow[] = { {0, GTABLE_ARROW_SZ}, {GTABLE_ARROW_SZ, GTABLE_ARROW_SZ}, {GTABLE_ARROW_SZ/2, 0} };
		static const point downArrow[] = { {0, 0}, {GTABLE_ARROW_SZ, 0}, {GTABLE_ARROW_SZ/2, GTABLE_ARROW_SZ} };
	#endif

	// Set the style
	ps = (gw->g.flags & GWIN_FLG_SYSENABLED) ? &gw->pstyle->enabled : &gw->pstyle->disabled;

	// Reduce the width of the widget body accordingly
	if (gw->g.flags & GTABLE_FLG_SCROLLSMOOTH) {
		*iwidth = gw->g.width -2 -4;
	}
	else if ((gw2obj->rowCount > scrollHeight / iheight) || (gw->g.flags & GTABLE_FLG_SCROLLALWAYS)) {
		*iwidth = gw->g.width - (GTABLE_SCROLLWIDTH +3);
	}
	else {
		*iwidth = gw->g.width -2;
	}

	// Draw the respective scroll feature
	if ((gw2obj->modified & GTABLE_SCROLL_MODIFIED) > 0) {
		gw2obj->modified &= ~GTABLE_SCROLL_MODIFIED;

		// Smooth Scrolling
		if (gw->g.flags & GTABLE_FLG_SCROLLSMOOTH) {
			if (gw2obj->rowCount > 0) {
				int max_scroll_value = gw2obj->rowCount * iheight - gw->g.height-2;
				if (max_scroll_value > 0) {
					int bar_height = (scrollHeight -2) * (scrollHeight -2) / (gw2obj->rowCount * iheight);
					int bar_width = offset +gw2obj->top * ((scrollHeight -2) -bar_height) / max_scroll_value;
			#if GDISP_NEED_CLIP
					gdispGSetClip(gw->g.display, gw->g.x +gw->g.width -4,
							                     gw->g.y +offset +1,
							                     bar_width, bar_height);
			#endif
					gdispGFillArea(gw->g.display, gw->g.x +gw->g.width -4,
												  gw->g.y +offset +1,
												  2, scrollHeight -1, gw->pstyle->background);

					gdispGFillArea(gw->g.display,
							       gw->g.x +gw->g.width -4,
								   bar_width, 2, bar_height, ps->edge);
				}
			}
		}  // Scroll Always
		else if ((gw2obj->rowCount > (scrollHeight -2) / iheight) ||
				 (gw->g.flags & GTABLE_FLG_SCROLLALWAYS)) {
		#if GDISP_NEED_CLIP
			gdispGSetClip(gw->g.display, gw->g.x +*iwidth +1,
										 gw->g.y +offset +1,
										 GTABLE_SCROLLWIDTH,
										 scrollHeight -2);
		#endif
			gdispGFillArea(gw->g.display, gw->g.x +*iwidth +2,
										  gw->g.y +offset +1,
										  GTABLE_SCROLLWIDTH,
										  scrollHeight -2,
										  gdispBlendColor(ps->fill, gw->pstyle->background, 128));

			gdispGDrawLine(gw->g.display, gw->g.x +*iwidth +1,
									      gw->g.y +offset +1,
                                          gw->g.x +*iwidth +1,
										  gw->g.y +gw->g.height -2, ps->edge);
		#if GDISP_NEED_CONVEX_POLYGON
			// Draw the top Arrow
			gdispGFillConvexPoly(gw->g.display, gw->g.x +*iwidth + ((GTABLE_SCROLLWIDTH-GTABLE_ARROW_SZ)/2 +2),
                                                gw->g.y +offset + (GTABLE_ARROW_SZ/2 +1),
                                                upArrow, 3, ps->edge);
			// Draw the bottom Arrow
			gdispGFillConvexPoly(gw->g.display, gw->g.x +*iwidth + ((GTABLE_SCROLLWIDTH-GTABLE_ARROW_SZ)/2 +2),
                                                gw->g.y +gw->g.height - (GTABLE_ARROW_SZ+GTABLE_ARROW_SZ/2 +1),
                                                downArrow, 3, ps->edge);
		#else
			#warning "GWIN: Tables display better when GDISP_NEED_CONVEX_POLYGON is turned on"
			gdispGFillArea(gw->g.display, gw->g.x +*iwidth + ((TABLE_SCROLLWIDTH-TABLE_ARROW_SZ)/2 +2),
                                          gw->g.y +gy + (TABLE_ARROW_SZ/2 +1),
                                          TABLE_ARROW_SZ, TABLE_ARROW_SZ, ps->fill);
			gdispGFillArea(gw->g.display, gw->g.x +*iwidth + ((TABLE_SCROLLWIDTH-TABLE_ARROW_SZ)/2 +2),
                                          gw->g.y +gw->g.height - (TABLE_ARROW_SZ+TABLE_ARROW_SZ/2 +1),
                                          TABLE_ARROW_SZ, TABLE_ARROW_SZ, ps->fill);
		#endif
		}
	}
}

#if GWIN_NEED_TABLE_TITLE
    
//#define TABLE_TITLE_TOP gw->g.y +1 +offset
//#define TABLE_TITLE_HEIGHT gdispGetFontMetric(gw->g.flags & GTABLE_FLG_HASFONT ? gw2obj->titleFont : gw->g.font, fontHeight) + GTABLE_TITLE_PADDING +1
//#define TABLE_TITLE_BOT TABLE_TITLE_TOP + TABLE_TITLE_HEIPRINT_DEBUGGHT

    // Function only called if the table has a title so not checked here
    // Function assumes there may be a structure above, offset available for generality
    // To save some arithmetic it could be removed
	static void _drawTitle(GWidgetObject* gw, int offset) {
		const GColorSet *			ps;

        ////fprintf(stderr, "DrawTitle Modified: %d, %d\n", gw2obj->modified, GTABLE_TITLE_MODIFIED);

		// Only do work if the title is modified
		if ((gw2obj->modified & GTABLE_TITLE_MODIFIED) > 0) {
            // reset modified flag
			gw2obj->modified &= ~GTABLE_TITLE_MODIFIED;

			// setup style
			ps = (gw->g.flags & GWIN_FLG_SYSENABLED) ? &gw->pstyle->enabled : &gw->pstyle->disabled;

			// Set the clipping region
			// The body routine sets it to aid scrolling so we have to set it so our updates work
		#if GDISP_NEED_CLIP
			gdispGSetClip(gw->g.display,
					      gw->g.x +1,            // 1 => left border
						  gw->g.y +1 +offset,    // 1 => top border
						  gw->g.width -2,        // 2 => left and right border
						  gw->g.y +1 +offset +gdispGetFontMetric(gw->g.flags & GTABLE_FLG_HASFONT ? gw2obj->titleFont : gw->g.font, fontHeight) +GTABLE_TITLE_PADDING +1); //TABLE_TITLE_BOT);
		#endif

			gdispGFillArea(gw->g.display,
						   gw->g.x +1,          // 1 => left border
						   gw->g.y +1 +offset,  // 1 => top border
                           gw->g.width -2,      // 2 -> left and right border
						   gdispGetFontMetric(gw->g.flags & GTABLE_FLG_HASFONT ? gw2obj->titleFont : gw->g.font, fontHeight) + GTABLE_TITLE_PADDING +1, //TABLE_TITLE_HEIGHT,
						   gw2obj->titleBackground == 0 ? ps->fill : gw2obj->titleBackground);

			gdispGDrawLine(gw->g.display,
						   gw->g.x +1,               // left border
						   gw->g.y +1 +offset +gdispGetFontMetric(gw->g.flags & GTABLE_FLG_HASFONT ? gw2obj->titleFont : gw->g.font, fontHeight) +GTABLE_TITLE_PADDING +1, //TABLE_TITLE_BOT);
					       gw->g.x + gw->g.width -2, // left and right border
						   gw->g.y +1 +offset +gdispGetFontMetric(gw->g.flags & GTABLE_FLG_HASFONT ? gw2obj->titleFont : gw->g.font, fontHeight) +GTABLE_TITLE_PADDING +1, //TABLE_TITLE_BOT);
						   Green); //ps->edge);

			gdispGFillStringBox(gw->g.display,
								gw->g.x +1, // GTABLE_ROW_PADDING,
								gw->g.y +1 +offset +GTABLE_TITLE_PADDING,
				                gw->g.width -2,
								gdispGetFontMetric(gw->g.flags & GTABLE_FLG_HASFONT ? gw2obj->titleFont : gw->g.font, fontHeight),
							    gw2obj->title,
							    gw->g.flags & GTABLE_FLG_HASFONT ? gw2obj->titleFont : gw->g.font, ps->text, // titleFont
							    gw2obj->titleBackground == 0 ? ps->fill : gw2obj->titleBackground, // titleColor,
							    gw->g.flags & GTABLE_FLG_HASJUSTIFY ? gw2obj->titleJustify : justifyCenter); // titleJustify);
                }
        }

//#undef TABLE_TITLE_TOP gw->g.y +1 +offset
//#undef TABLE_TITLE_HEIGHT
//#undef TABLE_TITLE_BOT TABLE_TITLE_TOP + TABLE_TITLE_HEIGHT

#endif

#if GWIN_NEED_TABLE_HEADER

//#define TABLE_HEADER_TOP gw->g.y+1+offset
//#define TABLE_HEADER_HEIGHT fheight+GTABLE_HEADER_PADDING +1
//#define TABLE_HEADER_BOT TABLE_HEADER_TOP + TABLE_HEADER_HEIGHT

    // Function only called if the table has a header so not checked here
	static void _drawHeader(GWidgetObject* gw, int offset, coord_t fheight)
	{
		const GColorSet* ps;
		coord_t j, xpos;

		//fprintf(stderr, "Header Draw: %d, %d\n", TABLE_HEADER_TOP, TABLE_HEADER_BOT);

		// Only do work if the header is modified
		if ((gw2obj->modified & GTABLE_HEADER_MODIFIED) > 0) {
			// reset modified flag
			gw2obj->modified &= ~GTABLE_HEADER_MODIFIED;

			ps = (gw->g.flags & GWIN_FLG_SYSENABLED) ? &gw->pstyle->enabled : &gw->pstyle->disabled;

			// Set the clipping region
			// The body routine sets it to aid scrolling so we have to set it so our updates work
		#if GDISP_NEED_CLIP
			gdispGSetClip(gw->g.display,
					      gw->g.x +1,
						  gw->g.y +1 +offset, //TABLE_HEADER_TOP,
				          gw->g.width -2,
						  gw->g.y +1 +offset +fheight +GTABLE_HEADER_PADDING +1); //TABLE_HEADER_BOT);
		#endif

			gdispGFillArea(gw->g.display,
					       gw->g.x,
						   gw->g.y +1 +offset, //TABLE_HEADER_TOP,
						   gw->g.width,
						   fheight +GTABLE_HEADER_PADDING +1, //TABLE_HEADER_HEIGHT,
						   gw2obj->headerBackground == 0 ? ps->fill : gw2obj->headerBackground); // headerColor);
			
			gdispGDrawLine(gw->g.display,
					       gw->g.x +1,
						   gw->g.y +1 +offset +fheight +GTABLE_HEADER_PADDING +1, //TABLE_HEADER_BOT,
					       gw->g.x +gw->g.width -2,
						   gw->g.y +1 +offset +fheight +GTABLE_HEADER_PADDING +1, //TABLE_HEADER_BOT,
						   Orange);
			xpos = 1;
#if GWIN_NEED_TABLE_IMAGES
			xpos += fheight;
#endif
			//fprintf(stderr, "X: %d, %d\n", xpos, gw->g.x +xpos +GTABLE_HORIZ_PADDING);
			for (j = 0; j < gw2obj->columnCount; j++){
				const columnWidth = *gw2obj->columnWidths[j];
				gdispGFillStringBox(gw->g.display,
									gw->g.x +xpos +GTABLE_HORIZ_PADDING,
									gw->g.y +1 +offset +GTABLE_HEADER_PADDING,
						            columnWidth -GTABLE_HORIZ_PADDING,
									fheight,
								    gw2obj->header[j], gw->g.font, ps->text,
								    gw2obj->headerBackground == 0 ? ps->fill : gw2obj->headerBackground, justifyLeft);
				xpos += columnWidth;
			}
		}
	}
#endif

static void sendTableEvent(GWidgetObject *gw, int row) {
	GSourceListener*	psl;
	GEvent*				pe;

	// Trigger a GWIN table event
	psl = 0;

	while ((psl = geventGetSourceListener(GWIDGET_SOURCE, psl))) {
		if (!(pe = geventGetEventBuffer(psl)))
			continue;

		ple->type = GEVENT_GWIN_TABLE;
		ple->gwin = (GHandle)gw;
		ple->row = row;
		#if GWIN_WIDGET_TAGS
			ple->tag = gw->tag;
		#endif

		geventSendEvent(psl);
	}
}

#if GINPUT_NEED_MOUSE
    static void TableMouseSelect(GWidgetObject* gw, coord_t x, coord_t y) {
        const gfxQueueASyncItem*    qi;
        int                         row, i;
        coord_t                     iheight;
        (void)			    x;
        int			    yOffset;
        font_t                      theFont;

        yOffset = 0;
        #if GWIN_NEED_TABLE_TITLE
        if (gw->g.flags & GTABLE_FLG_HASTITLE) {
        	theFont = gw->g.flags & GTABLE_FLG_HASTITLE ? gw2obj->titleFont : gw->g.font;
        	yOffset += gdispGetFontMetric(theFont, fontHeight) + GTABLE_TITLE_PADDING +2;
        }
	#endif

        #if GWIN_NEED_TABLE_HEADER
        if (gw->g.flags & GTABLE_FLG_HASHEADER) {
     		yOffset += gdispGetFontMetric(gw->g.font, fontHeight) + GTABLE_HEADER_PADDING +2;
        }
        #endif;

        ////fprintf(stderr, "Select: yOffset: %d, %d\n", gw2obj->top, yOffset);
     	iheight = gdispGetFontMetric(gw->g.font, fontHeight);

        // Handle click over the table area
        row = (y +gw2obj->top -yOffset) / (iheight +GTABLE_ROW_PADDING) ;
        //fprintf(stderr, "Row: %d, %d, %d, %d, %d\n", y, yOffset, (y +gw2obj->top -yOffset), iheight, row);

        if (row < 0 || row >= gw2obj->rowCount)
            return;

        for(qi = gfxQueueASyncPeek(&gw2obj->table_head), i = 0; qi; qi = gfxQueueASyncNext(qi), i++) {
             if ((gw->g.flags & GTABLE_FLG_MULTISELECT)) {
                if (row == i) {
                    qi2li->flags ^= GTABLE_FLG_SELECTED;
                    // Mark the modified flags
                    gw2obj->modified |= GTABLE_BODY_MODIFIED;
                    qi2li->modified |= GTABLE_TEXT_MODIFIED | GTABLE_IMAGE_MODIFIED;
                    break;
                }
            } else {
                if (row == i && (qi2li->flags & GTABLE_FLG_SELECTED) == 0) {
                    qi2li->flags |= GTABLE_FLG_SELECTED;
                    // Mark the modified flags
                    gw2obj->modified |= GTABLE_BODY_MODIFIED;
                    qi2li->modified |= GTABLE_TEXT_MODIFIED | GTABLE_IMAGE_MODIFIED;
               }
                else if (row == i){
                }
                else if ((qi2li->flags & GTABLE_FLG_SELECTED) > 0){
                    qi2li->flags &=~ GTABLE_FLG_SELECTED;
                    // Mark the modified flags
                    gw2obj->modified |= GTABLE_BODY_MODIFIED;
                    qi2li->modified |= GTABLE_TEXT_MODIFIED | GTABLE_IMAGE_MODIFIED;
                }
            }
        }
        _gwinUpdate(&gw->g);
        sendTableEvent(gw, row);
    }

    // A mouse down has occurred over the table area
    static void TableMouseDown(GWidgetObject* gw, coord_t x, coord_t y) {
    	coord_t		rowHeight, pageSize;
        int oldtop;

        ////fprintf(stderr, "TMD: %d, %d\n", x, y);
        // Save our mouse start position
        gw2obj->start_mouse_x = x;
        gw2obj->start_mouse_y = y;
        gw2obj->last_mouse_y = y;

        // For smooth scrolling, scrolling is done in the TableMouseMove and selection is done on TableMouseUp
        if (gw->g.flags & GTABLE_FLG_SCROLLSMOOTH)
        	return;

        // The row height
        rowHeight = gdispGetFontMetric(gw->g.font, fontHeight) + GTABLE_ROW_PADDING;

        // The space unavaible to body page
        coord_t unavailable = 0;

#if GWIN_NEED_TABLE_TITLE
        if (gw->g.flags & GTABLE_FLG_HASTITLE) {
        	unavailable = gdispGetFontMetric(gw->g.flags & GTABLE_FLG_HASFONT ? gw2obj->titleFont : gw->g.font, fontHeight)  +GTABLE_TITLE_PADDING +1;
        	////fprintf(stderr, "TITLE Used : %d\n", unavailable);
        }
#endif

#if GWIN_NEED_TABLE_HEADER
        if (gw->g.flags & GTABLE_FLG_HASHEADER) {
        	unavailable += rowHeight +GTABLE_HEADER_PADDING +1;
        	////fprintf(stderr, "Header Used : %d, %d\n", iheight, unavailable);
        }
#endif

        // pageSize is the number of pixel rows available to the body
        pageSize = gw->g.height -unavailable;
        ////fprintf(stderr, "pageSize : %d, %d, %d\n", gw->g.height, unavailable, pageSize);

		// Handle click over the scroll bar
		////fprintf(stderr, "Scroll: %d, %d, %d, %d, %d\n", gw->g.width-(GTABLE_SCROLLWIDTH+2), unavailable, pageSize, pageSize/iheight, gw2obj->rowCount);
		if (x >= gw->g.width-(GTABLE_SCROLLWIDTH+2) && y > unavailable && (gw2obj->rowCount > pageSize/rowHeight || (gw->g.flags & GTABLE_FLG_SCROLLALWAYS))) {
		    ////fprintf(stderr, "In SB: \n");
		    // Save the current top
		    oldtop = gw2obj->top;
            // are we inside upper arrow
			if ( y < unavailable + 2 *GTABLE_ARROW_SZ) {
                //fprintf(stderr, "Inside upper: %d\n", rowHeight);
				if (gw2obj->top > 0) {
					// Scroll One position
					gw2obj->top -= rowHeight; // + GTABLE_ROW_PADDING +1;
					if (gw2obj->top < 0){
					    gw2obj->top = 0;
					}
				}
			// inside lower arrow
			} else if (y >= gw->g.height - 2 *GTABLE_ARROW_SZ) {
			////fprintf(stderr, "Inside Lower: %d\n", rowHeight);
				// Scroll One position
				////fprintf(stderr, "Lower: %d, %d\n", gw2obj->top, gw2obj->rowCount * (rowHeight + GTABLE_ROW_PADDING) - pageSize);
				if (gw2obj->top < gw2obj->rowCount *rowHeight -pageSize) {
				    gw2obj->top += rowHeight;
				    if (gw2obj->top > gw2obj->rowCount * rowHeight - pageSize) {
				        gw2obj->top = gw2obj->rowCount * rowHeight - pageSize;
				    }
				}
            // inside the top half box
			} else if (y < gw->g.height/2) {
                ////fprintf(stderr, "Inside top half: \n");
				if (gw2obj->top > 0) {
					if (gw2obj->top > pageSize)
						gw2obj->top -= pageSize;
					else
						gw2obj->top = 0;
				}
			} else {
			////fprintf(stderr, "Inside bot half: \n");
				if (gw2obj->top < gw2obj->rowCount * rowHeight - pageSize) {
					if (gw2obj->top < gw2obj->rowCount * rowHeight - 2 *pageSize)
						gw2obj->top += pageSize;
					else
						gw2obj->top = gw2obj->rowCount * rowHeight - pageSize;
				}
			}
			if (oldtop != gw2obj->top){
			    ////fprintf(stderr, "Top: %d\n", gw2obj->top);
			    // Mark the modified flags
                gw2obj->modified |= GTABLE_SCROLL_MODIFIED | GTABLE_BODY_MODIFIED;
                _gwinGWTableMarkAllRows(gw, GTABLE_TEXT_MODIFIED | GTABLE_IMAGE_MODIFIED);
				_gwinUpdate(&gw->g);
			}
			return;
		}
        TableMouseSelect(gw, x, y);
	}

    static void TableMouseUp(GWidgetObject* gw, coord_t x, coord_t y) {
        // Only act when we are a smooth scrolling table
        if (!(gw->g.flags & GTABLE_FLG_SCROLLSMOOTH))
            return;

        // Only allow selection when we did not scroll
        if (abs(gw2obj->start_mouse_x - x) > 4 || abs(gw2obj->start_mouse_y - y) > 4)
            return;

        TableMouseSelect(gw, x, y);
    }

	static void TableMouseMove(GWidgetObject* gw, coord_t x, coord_t y) {
        int iheight, oldtop;
        (void) x;

        if (!(gw->g.flags & GTABLE_FLG_SCROLLSMOOTH)) return;

        if (gw2obj->last_mouse_y != y) {
            oldtop = gw2obj->top;
            iheight = gdispGetFontMetric(gw->g.font, fontHeight) + GTABLE_ROW_PADDING;

            gw2obj->top -= y - gw2obj->last_mouse_y;
            if (gw2obj->top >= gw2obj->rowCount * iheight - (gw->g.height-2))
                gw2obj->top = gw2obj->rowCount * iheight - (gw->g.height-2) - 1;
            if (gw2obj->top < 0)
                gw2obj->top = 0;
            gw2obj->last_mouse_y = y;
            // If top changes update
            if (oldtop != gw2obj->top)
            {
			    // Mark the modified flags
                gw2obj->modified |= GTABLE_SCROLL_MODIFIED | GTABLE_BODY_MODIFIED;
                _gwinGWTableMarkAllRows(gw, GTABLE_TEXT_MODIFIED | GTABLE_IMAGE_MODIFIED);
                _gwinUpdate(&gw->g);
            }
        }
	}
#endif

#if GINPUT_NEED_TOGGLE
	// a toggle-on has occurred
	static void TableToggleOn(GWidgetObject *gw, uint16_t role) {
		const gfxQueueASyncItem	*	qi;
		const gfxQueueASyncItem	*	qix;
		int							i;

		switch (role) {
			// select down
			case 0:
				for (i = 0, qi = gfxQueueASyncPeek(&gw2obj->table_head); qi; qi = gfxQueueASyncNext(qi), i++) {
					if ((qi2li->flags & GTABLE_FLG_SELECTED)) {
						qix = gfxQueueASyncNext(qi);
						if (qix) {
						    // Remove selection flag and mark as modified
							qi2li->flags &=~ GTABLE_FLG_SELECTED;
		                    qi2li->modified = GTABLE_TEXT_MODIFIED | GTABLE_IMAGE_MODIFIED;
							// Add selection flag and mark as modified
							qix2li->flags |= GTABLE_FLG_SELECTED;
		                    qix2li->modified = GTABLE_TEXT_MODIFIED  | GTABLE_IMAGE_MODIFIED;
							// Mark body modified
		                    gw2obj->modified |= GTABLE_BODY_MODIFIED;
							_gwinUpdate(&gw->g);
						}
						break;
					}
				}
				break;

			// select up
			case 1:
				qi = gfxQueueASyncPeek(gw2obj->table_head);
				qix = 0;

				for (i = 0; qi; qix = qi, qi = gfxQueueASyncNext(qi), i++) {
					if ((qi2li->flags & GTABLE_FLG_SELECTED)) {
						if (qix) {
						    // Remove selection flag and mark as modified
							qi2li->flags &=~ GTABLE_FLG_SELECTED;
		                    qi2li->modified |= GTABLE_TEXT_MODIFIED | GTABLE_IMAGE_MODIFIED;
							// Add selection flaf and mark as modified
							qix2li->flags |= GTABLE_FLG_SELECTED;
		                    qix2li->modified |= GTABLE_TEXT_MODIFIED | GTABLE_IMAGE_MODIFIED;
							// Mark body as modified
		                    gw2obj->modified |= GTABLE_BODY_MODIFIED;
							_gwinUpdate(&gw->g);
						}
						break;
					}
				}
				break;
		}
	}

	static void TableToggleAssign(GWidgetObject *gw, uint16_t role, uint16_t instance) {
		if (role)
			gw2obj->t_up = instance;
		else
			gw2obj->t_dn = instance;
	}

	static uint16_t TableToggleGet(GWidgetObject *gw, uint16_t role) {
		return role ? gw2obj->t_up : gw2obj->t_dn;
	}
#endif

static void TableDestroy(GHandle gh) {
	const gfxQueueASyncItem* qi;
	int i;

	// Free the column width data
	// If columns are a different width this will need to be used
	///gfxFree(&gh2obj->columnWidth);
	int cols = gh2obj->columnCount;

        // Free the Title data
#if GWIN_NEED_TABLE_TITLE
 	if (gh2obj->w.g.flags & GTABLE_FLG_HASTITLE){
		gfxFree(gh2obj->title);
	}
#endif

	// Free the Header data
#if GWIN_NEED_TABLE_HEADER
	if (gh2obj->w.g.flags & GTABLE_FLG_HASHEADER){
		// Free the columns
		char** header = (char**)gh2obj->header;
		for (i = 0; i < cols; i++){
			gfxFree(header[i]);
		}
		gfxFree(header);
	}
#endif
	// Free the column data
	while((qi = gfxQueueASyncGet(&gh2obj->table_head))){
		char** rows = (char**)qi2li->text;
		for (i = 0; i < cols; i++){
			gfxFree(rows[i]);
		}
		gfxFree((void *)qi);
	}
	_gwidgetDestroy(gh);
}

static const gwidgetVMT tableVMT = {
	{
		"Table",				// The class name
		sizeof(GTableObject),	// The object size
		TableDestroy,			// The destroy routine
		_gwidgetRedraw,			// The redraw routine
		0,						// The after-clear routine
	},
	gwinTableDefaultDraw,		// default drawing routine
	#if GINPUT_NEED_MOUSE
		{
			TableMouseDown,
			TableMouseUp,
			TableMouseMove,
		},
	#endif
	#if GINPUT_NEED_KEYBOARD || GWIN_NEED_KEYBOARD
		{
			0						// Process keyboard events
		},
	#endif
	#if GINPUT_NEED_TOGGLE
		{
			2,					// two toggle roles
			TableToggleAssign,	// Assign toggles
			TableToggleGet,		// Get toggles
			0,
			TableToggleOn,		// Process toggle on event
		},
	#endif
	#if GINPUT_NEED_DIAL
		{
			0,
			0,
			0,
			0,
		},
	#endif
};

GHandle gwinGTableCreate(GDisplay *g, GTableObject* gobj, GWidgetInit* pInit, int colCnt, bool_t multiselect) {
	if (!(gobj = (GTableObject *)_gwidgetCreate(g, &gobj->w, pInit, &tableVMT)))
		return 0;

	gfxQueueASyncInit(&gobj->table_head);

	gobj->rowCount = 0;
	gobj->columnCount = colCnt;
	gobj->columnWidth = 0;

#if GWIN_NEED_TABLE_EMBELLISH
	gobj->evenBackground = 0;
	gobj->oddBackground = 0;
#endif

#if GWIN_NEED_TABLE_TITLE
	gobj->title = 0;
        gobj->titleFont = 0;
	gobj->titleBackground = 0;
        gobj->titleJustify = 0;
#endif

#if GWIN_NEED_TABLE_HEADER
	gobj->header = 0;
	gobj->headerBackground = 0;
#endif

	gobj->top = 0;
	gobj->modified = 0;
	if (multiselect)
		gobj->w.g.flags |= GTABLE_FLG_MULTISELECT;
	gobj->w.g.flags |= GTABLE_FLG_ENABLERENDER;

	gwinSetVisible(&gobj->w.g, pInit->g.show);

	return (GHandle)gobj;
}

void gwinTableEnableRender(GHandle gh, bool_t ena) {
	// is it a valid handle?
	if (gh->vmt != (gwinVMT *)&tableVMT)
		return;

	if (ena) {
		gh->flags |= GTABLE_FLG_ENABLERENDER;
		gwinRedraw(gh);
	}
	else {
		gh->flags &=~ GTABLE_FLG_ENABLERENDER;
	}
}

void gwinTableSetScroll(GHandle gh, tscroll_t flag) {
	// is it a valid handle?
	if (gh->vmt != (gwinVMT *)&tableVMT)
		return;

    ((GTableObject*)gh)->w.g.flags &=~(GTABLE_FLG_SCROLLSMOOTH | GTABLE_FLG_SCROLLALWAYS);
	switch (flag) {
		case tscrollAlways:
			((GTableObject*)gh)->w.g.flags |= GTABLE_FLG_SCROLLALWAYS;
			break;

			// Danger
			// This is probably a defect, i think it needs to be cleared but have no way to tell
		case tscrollAuto:
			break;

		case tscrollSmooth:
		    ((GTableObject*)gh)->w.g.flags |= GTABLE_FLG_SCROLLSMOOTH;
		    break;
	}
}

// Does not handle useAlloc correctly.  Always allocates the complete structure
int gwinTableAddRow(GHandle gh, char** row_name, bool_t useAlloc) {
	TableRow	*newRow;

	// is it a valid handle?
	if (gh->vmt != (gwinVMT *)&tableVMT)
		return -1;

	if (useAlloc) {
		int i = 0;
		size_t len = 1;
		if (!(newRow = gfxAlloc(sizeof(TableRow) + len)))
			return -1;

		newRow->text = gfxAlloc((sizeof(char*) * gh2obj->columnCount) +1);

		int rowLen = 0;

		for (i = 0; i < gh2obj->columnCount; i++){
			rowLen = strlen(row_name[i]);
			newRow->text[i] = gfxAlloc((sizeof(char) * rowLen) +1);
			memcpy((char*)newRow->text[i], (char*)row_name[i], rowLen +1);
		}
	} else {
		if (!(newRow = gfxAlloc(sizeof(TableRow))))
			return -1;
	}

	// the item is not selected when added
	newRow->flags = 0;
	newRow->param = 0;
	newRow->modified |= GTABLE_TEXT_MODIFIED;

#if GWIN_NEED_TABLE_EMBELLISH
	newRow->background = 0;
#endif

#if GWIN_NEED_TABLE_IMAGES
	newRow->pimg = 0;
#endif

	// select the item if it's the first in the table
	if (gh2obj->rowCount == 0 && !(gh->flags & GTABLE_FLG_MULTISELECT))
		newRow->flags |= GTABLE_FLG_SELECTED;

	// add the new item to the table
	gfxQueueASyncPut(&gh2obj->table_head, &newRow->q_item);

	// increment the total amount of entries in the table widget
	gh2obj->rowCount++;
	gh2obj->modified |= GTABLE_HEADER_MODIFIED | GTABLE_SCROLL_MODIFIED | GTABLE_BODY_MODIFIED;

	_gwinUpdate(gh);
	// return the position in the table (-1 because we start with index 0)
	return gh2obj->rowCount-1;
}

#if GWIN_NEED_TABLE_TITLE
void gwinTableSetTitle(GHandle gh, char* title){
	// is it a valid handle?
	if (gh->vmt != (gwinVMT *)&tableVMT)
		return;

	size_t len = strlen(title);

	if (gh2obj->w.g.flags & GTABLE_FLG_HASTITLE){
		// Free the existing title
		gfxFree((char*)gh2obj->title);
	}
	else {
		// Mark the flags
		gh->flags |= GTABLE_FLG_HASTITLE;
		// Allocate new title
		//size_t len = strlen(title)+1;
		gh2obj->title = gfxAlloc(sizeof(char*) * len);
	}
	memcpy((char*)gh2obj->title, title, len +1);
	gh2obj->modified = GTABLE_TITLE_MODIFIED | GTABLE_HEADER_MODIFIED | GTABLE_SCROLL_MODIFIED | GTABLE_BODY_MODIFIED;
    ///fprintf(stderr, "Modified: %d\n", gh2obj->modified);
 	_gwinUpdate(gh);
}

void gwinTableSetTitleFont(GHandle gh, font_t font){
	// is it a valid handle?
	if (gh->vmt != (gwinVMT *)&tableVMT)
		return;

	gh->flags |= GTABLE_FLG_HASFONT;
	gh2obj->titleFont = font;
	gh2obj->modified |= GTABLE_TITLE_MODIFIED;
	_gwinUpdate(gh);
}

void gwinTableSetTitleBackground(GHandle gh, color_t background){
	// is it a valid handle?
	if (gh->vmt != (gwinVMT *)&tableVMT)
		return;

	gh2obj->titleBackground = background;
	gh2obj->modified |= GTABLE_TITLE_MODIFIED;
	_gwinUpdate(gh);
}

void gwinTableSetTitleJustify(GHandle gh, justify_t justify){
	// is it a valid handle?
	if (gh->vmt != (gwinVMT *)&tableVMT)
		return;

	gh2obj->titleJustify = justify;
	gh->flags |= GTABLE_FLG_HASJUSTIFY;
	gh2obj->modified |= GTABLE_TITLE_MODIFIED;
	_gwinUpdate(gh);
}

#endif

#if GWIN_NEED_TABLE_HEADER
void gwinTableSetHeader(GHandle gh, char** header, uint32_t** columnWidths) {
	int i;
	int rowLen;
	// is it a valid handle?
	if (gh->vmt != (gwinVMT *)&tableVMT)
		return;

	if (gh2obj->w.g.flags & GTABLE_FLG_HASHEADER){
		// Free the existing header columns
		for (i = 0; i < gh2obj->columnCount; i++) {
			gfxFree((char*)gh2obj->header[i]);
			gfxFree((uint32_t*)gh2obj->columnWidths[i]);
		}
	}
	else {		
		// Mark the flags
		gh->flags |= GTABLE_FLG_HASHEADER;
		// Allocate new header
		gh2obj->header = gfxAlloc((sizeof(char*) * gh2obj->columnCount) +1);
		gh2obj->columnWidths = gfxAlloc(sizeof(uint32_t*) * gh2obj->columnCount);
	}
	// Fill the headers
	for (i = 0; i < gh2obj->columnCount; i++)
	{		
		rowLen = strlen(header[i]);
		gh2obj->header[i] = gfxAlloc((sizeof(char) * rowLen) +1);
		memcpy((char*)gh2obj->header[i], (char*)header[i], rowLen +1);

		gh2obj->columnWidths[i] = gfxAlloc(sizeof(uint32_t*));		
		
		fprintf(stderr, "%d\n", *columnWidths[i]);
		fflush(stderr);
		fflush(stdout);
		fprintf(stderr, "%s\n", header[i]);
		fflush(stderr);
		fflush(stdout);
		memcpy(gh2obj->columnWidths[i], columnWidths[i], sizeof(uint32_t*));
		
	}
	gh2obj->modified |= GTABLE_HEADER_MODIFIED | GTABLE_SCROLL_MODIFIED | GTABLE_BODY_MODIFIED;
	_gwinUpdate(gh);
}

void gwinTableSetHeaderBackground(GHandle gh, color_t background) {

	// is it a valid handle?
	if (gh->vmt != (gwinVMT *)&tableVMT)
		return;

	gh2obj->headerBackground = background;
	gh2obj->modified |= GTABLE_HEADER_MODIFIED;
	_gwinUpdate(gh);
}

#endif

#if GWIN_NEED_TABLE_EMBELLISH
	void gwinTableRowSetBackground( GHandle gh, int row, color_t color) {
		const gfxQueueASyncItem*	qi;
		int							i;

		// is it a valid handle?
		if (gh->vmt != (gwinVMT *)&tableVMT)
			return;

		// watch out for an invalid row
		if (row < 0 || row >= gh2obj->rowCount)
			return;

		for(qi = gfxQueueASyncPeek(&gh2obj->table_head), i = 0; qi; qi = gfxQueueASyncNext(qi), i++) {
			if (i == row){
				qi2li->background = color;
				gh2obj->modified |= GTABLE_BODY_MODIFIED;
				qi2li->modified |= GTABLE_TEXT_MODIFIED;
				if ((gh->flags &= GTABLE_FLG_HASIMAGES) > 0)
					qi2li->modified |= GTABLE_IMAGE_MODIFIED;
				_gwinUpdate(gh);
				return;
			}
		}
	}

	// Set the even/odd background colors at once.  
	void gwinTableSetBackground( GHandle gh, color_t evenColor, color_t oddColor) {

		// is it a valid handle?
		if (gh->vmt != (gwinVMT *)&tableVMT)
			return;

		gh2obj->evenBackground = evenColor;
		gh2obj->oddBackground = oddColor;
		gh2obj->modified |= GTABLE_BODY_MODIFIED;
		// Mark all rows
		_gwinGHTableMarkAllRows(gh, GTABLE_TEXT_MODIFIED | GTABLE_IMAGE_MODIFIED);
		_gwinUpdate(gh);
	}

	void gwinTableSetOddBackground( GHandle gh, color_t color) {

		// is it a valid handle?
		if (gh->vmt != (gwinVMT *)&tableVMT)
			return;

		gh2obj->oddBackground = color;
		gh2obj->modified |= GTABLE_BODY_MODIFIED;
		// Mark all rows
		_gwinGHTableMarkAllRows(gh, GTABLE_TEXT_MODIFIED | GTABLE_IMAGE_MODIFIED);
		_gwinUpdate(gh);
	}

	void gwinTableSetEvenBackground( GHandle gh, color_t color) {

		// is it a valid handle?
		if (gh->vmt != (gwinVMT *)&tableVMT)
			return;

		gh2obj->evenBackground = color;
		gh2obj->modified |= GTABLE_BODY_MODIFIED;
		// Mark all rows
		_gwinGHTableMarkAllRows(gh, GTABLE_TEXT_MODIFIED | GTABLE_IMAGE_MODIFIED);
		_gwinUpdate(gh);
	}
#endif

const char* gwinTableGetText(GHandle gh, int row, int col) {
	const gfxQueueASyncItem*	qi;
	int							i;

	// is it a valid handle?
	if (gh->vmt != (gwinVMT *)&tableVMT)
		return 0;

	// watch out for an invalid row
	if (row < 0 || row >= gh2obj->rowCount)
		return 0;

	if (col < 0 || col >= gh2obj->columnCount)
		return 0;

	for(qi = gfxQueueASyncPeek(&gh2obj->table_head), i = 0; qi; qi = gfxQueueASyncNext(qi), i++) {
		if (i == row)
			return qi2li->text[col];
	}
	return 0;
}

const char* gwinTableRowGetText(GHandle gh, int row) {
	const gfxQueueASyncItem*	qi;
	int							i;

	// is it a valid handle?
	if (gh->vmt != (gwinVMT *)&tableVMT)
		return 0;

	// watch out for an invalid row
	if (row < 0 || row >= gh2obj->rowCount)
		return 0;

	for(qi = gfxQueueASyncPeek(&gh2obj->table_head), i = 0; qi; qi = gfxQueueASyncNext(qi), i++) {
		if (i == row)
			return qi2li->text[0];
	}
	return 0;
}

int gwinTableFindText(GHandle gh, const char* text) {
	const gfxQueueASyncItem*	qi;
	int							i;

	// is it a valid handle?
	if (gh->vmt != (gwinVMT *)&tableVMT)
		return -1;

	// watch out for NULL pointers
	if (!text)
		return -1;

	for(qi = gfxQueueASyncPeek(&gh2obj->table_head), i = 0; qi; qi = gfxQueueASyncNext(qi), i++) {
		if (strcmp(((TableRow *)qi)->text[0], text) == 0)
			return i;	
	}

	return -1;
}

int gwinTableGetSelected(GHandle gh) {
	const gfxQueueASyncItem	*	qi;
	int							i;

	// is it a valid handle?
	if (gh->vmt != (gwinVMT *)&tableVMT)
		return -1;

	// Multi-select always returns -1. Use gwinTableRowIsSelected() instead
	if ((gh->flags & GTABLE_FLG_MULTISELECT))
		return -1;

	for(qi = gfxQueueASyncPeek(&gh2obj->table_head), i = 0; qi; qi = gfxQueueASyncNext(qi), i++) {
		if (qi2li->flags & GTABLE_FLG_SELECTED)
			return i;
	}

	return -1;
}

void gwinTableRowSetParam(GHandle gh, int row, uint16_t param) {
	const gfxQueueASyncItem	*	qi;
	int							i;

	// is it a valid handle?
	if (gh->vmt != (gwinVMT *)&tableVMT)
		return;

	// watch out for an invalid row
	if (row < 0 || row > (gh2obj->rowCount) - 1)
		return;

	for(qi = gfxQueueASyncPeek(&gh2obj->table_head), i = 0; qi; qi = gfxQueueASyncNext(qi), i++) {
		if (i == row) {
			qi2li->param = param;
			break;
		}
	}
}

void gwinTableDeleteAll(GHandle gh) {
	gfxQueueASyncItem* qi;
	coord_t i;

	// is it a valid handle?
	if (gh->vmt != (gwinVMT *)&tableVMT)
		return;

	int cols = gh2obj->columnCount;
	while((qi = gfxQueueASyncGet(&gh2obj->table_head))){
		char** rows = (char**)qi2li->text;
		for (i = 0; i < cols; i++){
			gfxFree(rows[i]);
		}
		gfxFree(qi);
	}

	gh->flags &= ~GTABLE_FLG_HASIMAGES;
	gh2obj->rowCount = 0;
	gh2obj->top = 0;
	gh2obj->modified |= GTABLE_HEADER_MODIFIED | GTABLE_SCROLL_MODIFIED | GTABLE_BODY_MODIFIED;
	_gwinUpdate(gh);
}

void gwinTableRowDelete(GHandle gh, int row) {
	const gfxQueueASyncItem	*	qi;
	int							i;

	// is it a valid handle?
	if (gh->vmt != (gwinVMT *)&tableVMT)
		return;

	// watch out for an invalid row
	if (row < 0 || row >= gh2obj->rowCount)
		return;

	int cols = gh2obj->columnCount;

	for(qi = gfxQueueASyncPeek(&gh2obj->table_head), i = 0; qi; qi = gfxQueueASyncNext(qi), i++) {
		if (i == row) {
			gfxQueueASyncRemove(&gh2obj->table_head, (gfxQueueASyncItem*)qi);
			char** rows = (char**)qi2li->text;
			for (i = 0; i < cols; i++){
				gfxFree(rows[i]);
			}
			gfxFree((void *)qi);
			if (gh2obj->top >= row && gh2obj->top)
				gh2obj->top--;
			gh2obj->rowCount--;
			break;
		}
	}
	gh2obj->modified |= GTABLE_HEADER_MODIFIED | GTABLE_SCROLL_MODIFIED | GTABLE_BODY_MODIFIED;
	// Mark all rows
	_gwinGHTableMarkAllRows(gh, GTABLE_TEXT_MODIFIED | GTABLE_IMAGE_MODIFIED);
	_gwinUpdate(gh);
}

uint16_t gwinTableRowGetParam(GHandle gh, int row) {
	const gfxQueueASyncItem	*	qi;
	int							i;

	// is it a valid handle?
	if (gh->vmt != (gwinVMT *)&tableVMT)
		return 0;

	// watch out for an invalid row
	if (row < 0 || row > (gh2obj->rowCount) - 1)
		return 0;

	for(qi = gfxQueueASyncPeek(&gh2obj->table_head), i = 0; qi; qi = gfxQueueASyncNext(qi), i++) {
		if (i == row)
			return qi2li->param;
	}
	return 0;
}

bool_t gwinTableRowIsSelected(GHandle gh, int row) {
	const gfxQueueASyncItem	*	qi;
	int							i;

	// is it a valid handle?
	if (gh->vmt != (gwinVMT *)&tableVMT)
		return FALSE;

	// watch out for an invalid row
	if (row < 0 || row > (gh2obj->rowCount) - 1)
		return FALSE;

	for(qi = gfxQueueASyncPeek(&gh2obj->table_head), i = 0; qi; qi = gfxQueueASyncNext(qi), i++) {
		if (i == row)
			return (qi2li->flags &  GTABLE_FLG_SELECTED) ? TRUE : FALSE;
	}
	return FALSE;
}

int gwinTableColumnCount(GHandle gh) {
	// is it a valid handle?
	if (gh->vmt != (gwinVMT *)&tableVMT)
		return 0;

	return gh2obj->columnCount;
}

int gwinTableRowCount(GHandle gh) {
	// is it a valid handle?
	if (gh->vmt != (gwinVMT *)&tableVMT)
		return 0;

	return gh2obj->rowCount;
}

const char* gwinTableGetSelectedText(GHandle gh) {
	// is it a valid handle?
	if (gh->vmt != (gwinVMT *)&tableVMT)
		return 0;

	// return NULL if nothing is selected (or multi-select)
	if (gwinTableGetSelected(gh) < 0)
		return 0;

	return gwinTableRowGetText(gh, gwinTableGetSelected(gh));
}

void gwinTableSetSelected(GHandle gh, int row, bool_t doSelect) {
	const gfxQueueASyncItem   *   qi;
	int                     i;

	// is it a valid handle?
	if (gh->vmt != (gwinVMT *)&tableVMT)
		return;

	// watch out for an invalid row
	if (row < 0 || row >= gh2obj->rowCount)
		return;

	// If not a multiselect mode - clear previous selected row
	if (doSelect && !(gh->flags & GTABLE_FLG_MULTISELECT)) {
		for(qi = gfxQueueASyncPeek(&gh2obj->table_head); qi; qi = gfxQueueASyncNext(qi)) {
			if (qi2li->flags & GTABLE_FLG_SELECTED) {
				qi2li->flags &= ~GTABLE_FLG_SELECTED;
				qi2li->modified = GTABLE_TEXT_MODIFIED;
				break;
			}
		}
	}
	// Danger Will Robinson
	// Find row and set selected or not
	for(qi = gfxQueueASyncPeek(&gh2obj->table_head), i = 0; qi; qi = gfxQueueASyncNext(qi), i++) {
		if (i == row) {
			if (doSelect && (qi2li->flags & GTABLE_FLG_SELECTED) > 0)
				qi2li->flags &= ~GTABLE_FLG_SELECTED;
			else
				qi2li->flags |= GTABLE_FLG_SELECTED;
			break;
		}
		else {

		}
	}
	_gwinUpdate(gh);
}


void gwinTableViewRow(GHandle gh, int row) {
	coord_t iheight;
	int oldtop;
	// is it a valid handle?
	if (gh->vmt != (gwinVMT *)&tableVMT)
		return;

	// watch out for an invalid row
	if (row < 0 || row >= gh2obj->rowCount)
		return;
	oldtop = gh2obj->top;
	// Work out a possible new top for the table
	iheight = gdispGetFontMetric(gh->font, fontHeight) + GTABLE_ROW_PADDING;
	gh2obj->top = iheight * row;

	// Adjust the table
	if (gh2obj->top > gh2obj->rowCount * iheight - gh->height-2)
		gh2obj->top = gh2obj->rowCount * iheight - gh->height-2;
	if (gh2obj->top < 0)
		gh2obj->top = 0;

	if (oldtop != gh2obj->top) {
	    // Mark the modified flags
        gh2obj->modified = GTABLE_SCROLL_MODIFIED | GTABLE_BODY_MODIFIED;
		_gwinGHTableMarkAllRows(gh, GTABLE_TEXT_MODIFIED | GTABLE_IMAGE_MODIFIED);
		_gwinUpdate(gh);
	}
}

#if GWIN_NEED_TABLE_IMAGES
	void gwinTableRowSetImage(GHandle gh, int row, gdispImage *pimg) {
		const gfxQueueASyncItem	*	qi;
		int							i;

		// is it a valid handle?
		if (gh->vmt != (gwinVMT *)&tableVMT)
			return;

		// watch out for an invalid row
		if (row < 0 || row > (gh2obj->rowCount) - 1)
			return;

		//fprintf(stderr, "Set Image: \n");
		for(qi = gfxQueueASyncPeek(&gh2obj->table_head), i = 0; qi; qi = gfxQueueASyncNext(qi), i++) {
			if (i == row) {
				qi2li->pimg = pimg;
				if (pimg){
					gh->flags |= GTABLE_FLG_HASIMAGES;
					gh2obj->modified |= GTABLE_BODY_MODIFIED | GTABLE_IMAGE_MODIFIED;
					qi2li->modified |= GTABLE_IMAGE_MODIFIED;
				}
				break;
			}
		}
		_gwinUpdate(gh);
	}
#endif

void gwinTableDefaultDraw(GWidgetObject* gw, void* param) {
	const gfxQueueASyncItem*	qi;
	int							i, j;
	coord_t						x, xpos, y, iheight, iwidth;
	color_t						fill;
	color_t                     rowColor;
	const GColorSet *			ps;

	int headerOffset = 0;
	int bodyOffset = 0;
	int index;

	// is it a valid table widget handle?
	if (gw->g.vmt != (gwinVMT *)&tableVMT)
		return;

	// don't render if render has been disabled
	if (!(gw->g.flags & GTABLE_FLG_ENABLERENDER))
		return;

	iheight = gdispGetFontMetric(gw->g.font, fontHeight);
	//fprintf(stderr, "IHeight : %d\n", iheight);

#if GWIN_NEED_TABLE_TITLE
    if (gw->g.flags & GTABLE_FLG_HASTITLE) {
	    // HO = TFH + TP + TB + 1
	    //fprintf(stderr, "HeaderOffset : %d, %d\n", GTABLE_TITLE_PADDING, headerOffset);
        headerOffset = gdispGetFontMetric(gw->g.flags & GTABLE_FLG_HASFONT ? gw2obj->titleFont : gw->g.font, fontHeight) + GTABLE_TITLE_PADDING +2;
	    //fprintf(stderr, "HeaderOffset : %d, %d\n", GTABLE_TITLE_PADDING, headerOffset);
        bodyOffset = headerOffset;
	    ///fprintf(stderr, "HeaderOffset : %d, %d\n", GTABLE_TITLE_PADDING, headerOffset);
    }
#endif

#if GWIN_NEED_TABLE_HEADER
    if (gw->g.flags & GTABLE_FLG_HASHEADER) {
        // BO = HFH + HP + HB + 1
   	    bodyOffset += iheight +GTABLE_HEADER_PADDING +2;
	    //fprintf(stderr, "bodyOffset : %d, %d, %d\n", iheight, GTABLE_HEADER_PADDING, bodyOffset);
    }
#endif

	(void)param;

	////fprintf(stderr, "Offsets: %d, %d\n", headerOffset, bodyOffset);

	// Set the style
	ps = (gw->g.flags & GWIN_FLG_SYSENABLED) ? &gw->pstyle->enabled : &gw->pstyle->disabled;
	// iheight is the row height
	///iheight = gdispGetFontMetric(gw->g.font, fontHeight) + GTABLE_ROW_PADDING;
	iwidth = gw->g.width;

	// the table frame;
	////fprintf(stderr, "Frame: %d, %d, %d, %d\n", gw->g.x, gw->g.y, gw->g.width, gw->g.height);
	gdispGDrawBox(gw->g.display,
			      gw->g.x,
				  gw->g.y,
				  gw->g.width,
				  gw->g.height, ps->edge);
	//gdispGDrawLine(gw->g.display, gw->g.x, gw->g.y  + gw->g.height -1, gw->g.width, gw->g.y  + gw->g.height -1, Red);

	_drawScrollBar(gw, bodyOffset, &iwidth, iheight);

	// x is the current horizontal position
	x = 1;

	// space is allocated for images that are font height wide
#if GWIN_NEED_TABLE_IMAGES
	if ((gw->g.flags & GTABLE_FLG_HASIMAGES)) {
		// Make the printable width smaller to accomodate the image
		iwidth -= iheight;

		// Move the starting position past the image
		// Assume images fit in a box iheightxiheight  remainder will be clipped
		x += iheight;
	}
#endif

	// Build the column width
	gw2obj->columnWidth = iwidth / gw2obj->columnCount;;

	/// Only call the draw title routine if use title is turned on
#if GWIN_NEED_TABLE_TITLE
	// Only call the draw routine if there is a title to draw.
	// We could modify this to allow for a blank header
	if (gw->g.flags & GTABLE_FLG_HASTITLE)
		_drawTitle(gw, 0); //, theight);
#endif

	/// Only call the draw header routine if use header is turned on
#if GWIN_NEED_TABLE_HEADER
	// Only call the draw routine if there is a header to draw.
	// We could modify this to allow for a blank header
	if (gw->g.flags & GTABLE_FLG_HASHEADER)
		_drawHeader(gw, headerOffset, iheight);
#endif

	//return;
	
	// Set the clipping region so we do not override the frame.
#if GDISP_NEED_CLIP
	gdispGSetClip(gw->g.display,
			      gw->g.x +1,
				  gw->g.y +bodyOffset +1,
			      gw->g.flags & GTABLE_FLG_HASIMAGES ? iwidth+iheight : iwidth,
			      gw->g.height -bodyOffset -2);
#endif

	// Find the top row
	for (qi = gfxQueueASyncPeek(&gw2obj->table_head), i = iheight +GTABLE_ROW_PADDING -1;
		 i < gw2obj->top && qi;
		 qi = gfxQueueASyncNext(qi), i += iheight +GTABLE_ROW_PADDING);



	if ((gw2obj->modified & GTABLE_BODY_MODIFIED) > 0) {
		// reset modified flag
		gw2obj->modified &= ~GTABLE_BODY_MODIFIED;

		// Draw until we run out of room or rows
		////for (y = 1-(gw2obj->top%(iheight+GTABLE_ROW_PADDING)); y < gw->g.height-2 && qi; qi = gfxQueueASyncNext(qi), y += (iheight+GTABLE_ROW_PADDING)) {
		for (y = 1-(gw2obj->top%(iheight+GTABLE_ROW_PADDING)) -1, index = 0;
			 y < gw->g.height -bodyOffset && qi;
			 qi = gfxQueueASyncNext(qi), y += (iheight+GTABLE_ROW_PADDING), index++) {
				 
			////fprintf(stderr, "Line: %d, %d, %d\n", y, gw->g.y+y+bodyOffset, gw->g.y + bodyOffset + bodyBot);
			if (qi2li->modified > 0) {
				// Set the row background color as the fill color by default
				rowColor = ps->fill;
			#if GWIN_NEED_TABLE_EMBELLISH
			    // If asked for embellishment set row background color to even/odd if define
				if (index % 2 == 0) { // Even
					rowColor = gw2obj->evenBackground == 0 ? gw->pstyle->background : gw2obj->evenBackground;
				} else {
					rowColor = gw2obj->oddBackground == 0 ? gw->pstyle->background : gw2obj->oddBackground;
				}
				// Set row background color to background if it is defined
				rowColor = qi2li->background == 0 ? rowColor : qi2li->background;
			#endif
				fill = (qi2li->flags & GTABLE_FLG_SELECTED) ? ps->fill : rowColor; //gw->pstyle->background;

			#if GWIN_NEED_TABLE_IMAGES
				if ((gw->g.flags & GTABLE_FLG_HASIMAGES) &&
					(qi2li->modified & GTABLE_IMAGE_MODIFIED) > 0) {
					// Reset Modified flag
					qi2li->modified &= ~GTABLE_IMAGE_MODIFIED;

					//fprintf(stderr, " Image: \n");
					gdispGFillArea(gw->g.display,
								   gw->g.x +1,
								   gw->g.y +bodyOffset +y,
								   iheight +GTABLE_HORIZ_PADDING,
								   iheight +GTABLE_ROW_PADDING, fill);

					// Clear the image area
					// Assumes each row has individual image.
					// Could implement an image that is combined for all rows
					if (qi2li->pimg && gdispImageIsOpen(qi2li->pimg)) {
						// Calculate which image
						///sy = (qi2li->flags & GTABLE_FLG_SELECTED) ? 0 : (iheight-TABLE_VERT_PAD);
						///if (!(gw->g.flags & GWIN_FLG_SYSENABLED))
						///	sy += 2*(iheight-TABLE_VERT_PAD);
						///while (sy > qi2li->pimg->height)
						///	sy -= iheight-TABLE_VERT_PAD;
						// Draw the image
						//fprintf(stderr, " Draw Image: \n");
						gdispImageSetBgColor(qi2li->pimg, fill);
						gdispGImageDraw(gw->g.display,
								        qi2li->pimg,
										gw->g.x +1,
										gw->g.y +bodyOffset +y,
										iheight-GTABLE_ROW_PADDING,
										iheight-GTABLE_ROW_PADDING, 0, 0);
					}
				}
			#else
				gdispGFillArea(gw->g.display,
							   gw->g.x +1,
							   gw->g.y +bodyOffset +y,
							   GTABLE_HORIZ_PADDING,
							   iheight +GTABLE_ROW_PADDING, fill);
            #endif

				if ((qi2li->modified & GTABLE_TEXT_MODIFIED) > 0) {
					// Reset Modified flag
					qi2li->modified &= ~GTABLE_TEXT_MODIFIED;

					j = 0;
					xpos = x;
					//// Draw the row, only the string part
					//fprintf(stderr, "XX: %d, %d\n", xpos, gw->g.x +xpos +GTABLE_HORIZ_PADDING);
					for (j = 0; j < gw2obj->columnCount; j++){
						const columnWidth = *gw2obj->columnWidths[j];
						////fprintf(stderr, "     Fill: %d, %d\n", bodyTop +y, iheight+GTABLE_ROW_PADDING);
						gdispGFillArea(gw->g.display,
								       gw->g.x +xpos +GTABLE_HORIZ_PADDING,
									   gw->g.y +bodyOffset +y,
									   columnWidth,
									   iheight +GTABLE_ROW_PADDING, fill);

						if (gdispGetStringWidth(qi2li->text[j], gw->g.font) <= columnWidth -GTABLE_HORIZ_PADDING) {
							gdispGFillStringBox(gw->g.display,
									            gw->g.x +xpos +GTABLE_HORIZ_PADDING,
												gw->g.y +bodyOffset +y,
												columnWidth -GTABLE_HORIZ_PADDING, iheight,
									            qi2li->text[j], gw->g.font, ps->text, fill, justifyLeft);
						}
						else {
							// Allocate a string the size of the column text
							int len = strlen(qi2li->text[j]);
							//  Allocate new string, 2 for ellipse 1 for null
							char* foo = (char*)gfxAlloc(len * sizeof(char) +3);
							// copy all but last three spots
							memcpy(foo, qi2li->text[j], len -1);
							memcpy(foo + len -1, "...", 3);
							foo[len +2] = '\0';
							while (gdispGetStringWidth(foo, gw->g.font) > columnWidth-GTABLE_HORIZ_PADDING){
								len--;
								foo[len -1] = '.';
								foo[len +2] = '\0';
							}
							gdispGFillStringBox(gw->g.display,
									            gw->g.x+xpos +GTABLE_HORIZ_PADDING,
												gw->g.y +bodyOffset +y,
												columnWidth -GTABLE_HORIZ_PADDING, iheight, // and here
                                                foo, gw->g.font, ps->text, fill, justifyLeft);
							gfxFree(foo);
						}
						xpos += columnWidth;
					}
				}
			}
		}
		// Fill any remaining row space
		if (y < gw->g.height-1) {
#if GWIN_NEED_TABLE_IMAGES
			iwidth += iheight;
#endif
			gdispGFillArea(gw->g.display,
					       gw->g.x+1,
						   gw->g.y +bodyOffset +y,
						   iwidth,
						   gw->g.height -bodyOffset -y,
						   gw->pstyle->background);
		}
	}
}

#undef gh2obj
#undef gw2obj
#undef qi2li
#undef qix2li
#undef ple
#endif // GFX_USE_GWIN && GWIN_NEED_LIST
