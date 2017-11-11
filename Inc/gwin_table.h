/*
 * This file is subject to the terms of the GFX License. If a copy of
 * the license was not distributed with this file, you can obtain one at:
 *
 *              http://ugfx.org/license.html
 */

/**
*  For a multicolumn table the following rules apply:
*
*  The number of columns are known at creation time.
*
*  Selection will be implemented as a complete row.
*  The MULTISELECT flag will allow multiple rows to be selected.
*
*/

/**
 * @file	src/gwin/gwin_table.h
 * @brief	GWIN table widget header file
 *
 * @defgroup Table
 * @ingroup Widgets
 *
 * @brief		Table Widget. Used to display a table of rows.
 *
 * @details		Provides advanced features such as multi-selection, smooth scrolling and row icons.
 *
 * @pre			GFX_USE_GDISP must be set to TRUE in your gfxconf.h
 * @pre			GFX_USE_GWIN must be set to TRUE in your gfxconf.h
 * @pre			GDISP_NEED_TEXT must be set to TRUE in your gfxconf.h
 * @pre			GWIN_NEED_TABLE must be set to TRUE in your gfxconf.h
 * @pre			The font you want to use must be enabled in your gfxconf.h
 *
 * @{
 */

#ifndef _GWIN_TABLE_H
#define _GWIN_TABLE_H

// This file is included within "src/gwin/gwin_widget.h"

/**
 * @warning  Not sure how this works
 * @brief	The event type for a table event
 */
#define GEVENT_GWIN_TABLE	(GEVENT_GWIN_CTRL_FIRST+4)

/**
 * @brief	A table event
 */
typedef struct GEventGWinTable {
	GEventType		type;		// The type of this event (GEVENT_GWIN_TABLE)
	GHandle			gwin;		// The table
	#if GWIN_WIDGET_TAGS
		WidgetTag	tag;		// The table tag
	#endif
	int				row;		// The row that has been selected (or unselected in a multi-select tablebox)
} GEventGWinTable;

// Table modified flags
#define GTABLE_TITLE_MODIFIED  0x10
#define GTABLE_HEADER_MODIFIED 0x20
#define GTABLE_SCROLL_MODIFIED 0x40 
#define GTABLE_BODY_MODIFIED   0x80

// Row modified flags
#define GTABLE_IMAGE_MODIFIED  0x01
#define GTABLE_TEXT_MODIFIED   0x02

// A table window
typedef struct GTableObject {
	GWidgetObject	w;

    #if GINPUT_NEED_MOUSE
        coord_t start_mouse_x;
        coord_t start_mouse_y;
        coord_t last_mouse_y;
    #endif
	#if GINPUT_NEED_TOGGLE
		uint16_t	t_up;
		uint16_t	t_dn;
	#endif

	int				columnCount;// Number of columns in the table
	int				rowCount;	// Number of rows/rows currently in the table (quicker than counting each time)
	int 			columnWidth;

#if GWIN_NEED_TABLE_EMBELLISH
	color_t         evenBackground;
	color_t         oddBackground;
#endif

	int				top;		// Viewing offset in pixels from the top of the table
	uint16_t		modified;
#if GWIN_NEED_TABLE_TITLE
	const char*	title;
	font_t          titleFont;
	color_t         titleBackground;
        justify_t       titleJustify;
#endif

#if GWIN_NEED_TABLE_HEADER
	const char**	header;
	color_t         headerBackground;
#endif
	gfxQueueASync	table_head;	// The table of rows
} GTableObject;

/**
 * @warning Already defined, can I share
 * @brief	Enum to change the behaviour of the scroll bar
 *
 * @note	Used with @p gwinTableSetScroll()
 * @note    @p scrollAlways always show the scrollbar
 * @note    @p scrollAuto show the scrollbar when there are more rows on the table then fit on the screen
 * @note    @p scrollSmooth enable touch screen smooth scrolling
 */
typedef enum tscroll_t { tscrollAlways, tscrollAuto, tscrollSmooth } tscroll_t;

/**
 * @brief	The internal table object flags
 * @note	Used only for writing a custom draw routine.
 * @{
 */
#define GTABLE_FLG_SELECTED		0x0001
#define GTABLE_FLG_MULTISELECT		0x0002  // Single selection will be default
#define GTABLE_FLG_HASIMAGES		0x0004
#define GTABLE_FLG_SCROLLALWAYS		0x0008
#define GTABLE_FLG_SCROLLSMOOTH         0x0010
#define GTABLE_FLG_ENABLERENDER         0x0020
#define GTABLE_FLG_HASTITLE	 	0x0040
#define GTABLE_FLG_HASHEADER            0x0080
#define GTABLE_FLG_HASJUSTIFY           0x0100
#define GTABLE_FLG_HASFONT              0x0200

/**
 * @brief	The internal table row structure
 * @note	Used only for writing a custom draw routine.
 */
typedef struct TableRow {
	gfxQueueASyncItem	q_item;		// This must be the first member in the struct

	uint16_t			flags;
	uint16_t			param;		// A parameter the user can specify himself

#if GWIN_NEED_TABLE_EMBELLISH
	color_t             background;
#endif

	uint32_t			modified;
	// Column data
	const char**		text;
	
#if GWIN_NEED_TABLE_IMAGES
	gdispImage*		pimg;
#endif
} TableRow;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief				Create a table widget
 *
 * @note				The drawing color and the background color get set to the current defaults. If you haven't called
 *						@p gwinSetDefaultColor() or @p gwinSetDefaultBgColor() then these are Black and White.
 * @note				The font gets set to the current default font. If you haven't called @p gwinSetDefaultFont() then
 *						there is no default font and text drawing operations will not display anything.
 * @note				A table remembers its normal drawing state. If there is a window manager then it is automatically
 *						redrawn if the window is moved or its visibility state is changed.
 * @note				The table will allocate data for the specified column count
 * @note				The table contains no elements after creation.
 * @note				A slider supports mouse, toggle. Note: toggle only works correctly for single-select tables.
 * @note				When assigning a toggle, only one toggle is supported per role. If you try to assign more than
 *						one toggle to a role, it will forget the previous toggle. Two roles are supported:
 *						Role 0 = toggle for down, role 1 = toggle for up
 *
 * @param[in] g			The GDisplay to display this window on
 * @param[in] widget	The GTableObject structure to initialize. If this is NULL, the structure is dynamically allocated.
 * @param[in] pInit		The initialization parameters to use
 * @param[in] colCount  The number of columns in each row
 * @param[in] multiselect	If TRUE the table is multi-select instead of single-select.
 *
 * @return				NULL if there is no resulting drawing area, otherwise a window handle.
 *
 * @api
 */
GHandle gwinGTableCreate(GDisplay *g, GTableObject *widget, GWidgetInit *pInit, int colCount, bool_t multiselect);
#define gwinTableCreate(w, pInit, c, m)			gwinGTableCreate(GDISP, w, pInit, c, m)

/**
 * @brief				Enable or disable the rendering of the table
 *
 * @details				Usually the table is being re-rendered when an row is added to the table. This can cause
 * 						flickering and performance issues when many rows are added at once. This can be prevented
 *						by temporarily disabling the render using this function.
 *
 * @param[in] gh		The widget handle (must be a table handle)
 * @param[in] ena		TRUE or FALSE
 *
 * @api
 */
void gwinTableEnableRender(GHandle gh, bool_t ena);

/**
 * @brief				Change the behaviour of the scroll bar
 *
 * @note				Current possible values: @p scrollAlways, @p scrollAuto and  @p scrollSmooth
 *
 * @param[in] gh		The widget handle (must be a table handle)
 * @param[in] flag		The behaviour to be set
 *
 * @api
 */
void gwinTableSetScroll(GHandle gh, tscroll_t flag);

/**
 * @brief				Add an row to the table
 *
 * @note				The ID you get returned is not static. If rows get removed from the table, the table rows get
 * 						reordered.
 *
 * @param[in] gh		The widget handle (must be a table handle)
 * @param[in] rows		An array of strings which to be displayed in the table, the length of the array must be the number
 *						of columns in the table.  Each string will be stored in a seperate column structure.
 * @param[in] useAlloc	If set to TRUE, the string will be dynamically allocated. A static buffer must be passed otherwise
 *
 * @return				The current ID of the row. The ID might change if you remove rows from the middle of the table
 *
 * @api
 */
int gwinTableAddRow(GHandle gh, char** rows, bool_t useAlloc);

#if GWIN_NEED_TABLE_TITLE || defined(__DOXYGEN__)
	/**

	 * @brief				Set the title for the table.
	 *
	 * @details
	 *
	 * @param[in] gh		The widget handle (must be a table handle)

	 * @param[in] header	A pointer to an a string for the title
	 *
	 * @note				Define GWIN_NEED_TABLE_TITLE to turn on this feature
	 *
	 * @api
	 */
	void gwinTableSetTitle(GHandle gh, char* title);

	/**
	 * @brief				Set the title font for the table.
	 *

	 * @details
	 *
	 * @param[in] gh		The widget handle (must be a table handle)
	 * @param[in] font		The font for the title
	 *
	 * @note				Define GWIN_NEED_TABLE_TITLE to turn on this feature

	 *
	 * @api
	 */
	void gwinTableSetTitleFont(GHandle gh, font_t font);

	/**
	 * @brief				Set the title background for the table.
	 *

	 * @details
	 *
	 * @param[in] gh		The widget handle (must be a table handle)
	 * @param[in] background	The background color for the title
	 *
	 * @note				Define GWIN_NEED_TABLE_TITLE to turn on this feature

	 *
	 * @api
	 */
	void gwinTableSetTitleBackground(GHandle gh, color_t background);

	/**
	 * @brief				Set the title justification for the table.
	 *

	 * @details
	 *
	 * @param[in] gh		The widget handle (must be a table handle)
	 * @param[in] justify	The justification for the title
	 *
	 * @note				Define GWIN_NEED_TABLE_TITLE to turn on this feature

	 *
	 * @api
	 */
	void gwinTableSetTitleJustify(GHandle gh, justify_t justify);
#endif

#if GWIN_NEED_TABLE_HEADER || defined(__DOXYGEN__)
	/**

	 * @brief				Set the header labels for the table.
	 *
	 * @details
	 *
	 * @param[in] gh		The widget handle (must be a table handle)

	 * @param[in] header	A pointer to an array of strings for the columns
	 *
	 * @note				Define GWIN_NEED_TABLE_HEADER to turn on this feature
	 *
	 * @api
	 */
	void gwinTableSetHeader(GHandle gh, char** header);

	/**
	 * @brief				Set the header background for the table.
	 *

	 * @details
	 *
	 * @param[in] gh		The widget handle (must be a table handle)
	 * @param[in] background	The background color for the header
	 *
	 * @note				Define GWIN_NEED_TABLE_HEADER to turn on this feature

	 *
	 * @api
	 */
	void gwinTableSetHeaderBackground(GHandle gh, color_t background);
#endif

#if GWIN_NEED_TABLE_EMBELLISH
	/**
	 * @brief				Set row background color
	 *
	 * @details
	 *
	 * @param[in] gh		The widget handle (must be a table handle)
	 * @param[in] row		The row index
	 * @param[in] background 	The background color for the row
	 *
	 * @api
	 */
	void gwinTableRowSetBackground( GHandle gh, int row, color_t color);

	/**
	 * @brief				Set the background even and odd color for table
	 *
	 * @details
	 *
	 * @param[in] gh		The widget handle (must be a table handle)
	 * @param[in] evenColor	The color for the even rows
	 * @param[in] oddColor	The color for the odd rows
	 *
	 * @api
	 */
	void gwinTableSetBackground( GHandle gh, color_t evenColor, color_t oddColor);

	/**
	 * @brief				Set the background color for odd rows
	 *
	 * @details
	 *
	 * @param[in] gh		The widget handle (must be a table handle)
	 * @param[in] color		The color for the odd rows
	 *
	 * @api
	 */
	void gwinTableSetOddBackground( GHandle gh, color_t color);

	/**
	 * @brief				Set the background color for the even rows
	 *
	 * @details
	 *
	 * @param[in] gh		The widget handle (must be a table handle)
	 * @param[in] color		The color for the even rows
	 *
	 * @api
	 */
	void gwinTableSetEvenBackground( GHandle gh, color_t color);
#endif

/**
 * @brief				Get the text for a column of a row
 *
 * @param[in] gh		The widget handle (must be a table handle)
 * @param[in] row		The row index
 * @param[in] col		The column index
 *
 * @return				The string for the column of the table row or NULL on error
 *
 * @api
 */
const char* gwinTableGetText(GHandle gh, int row, int col);

/**
 * @brief				Get the text for a row
 *
 * @param[in] gh		The widget handle (must be a table handle)
 * @param[in] row		The row index
 *
 * @return				The string of the first column of the table row or NULL on error
 *
 * @api
 */
const char* gwinTableRowGetText(GHandle gh, int row);

/**
 * @brief				Get the ID of an row with a given name, we use the first column as the row identifier
 *
 * @param[in] gh		The widget handle (must be a table handle)
 * @param[in] text		The row name
 *
 * @return				The ID table row or null on error
 *
 * @api
 */
int gwinTableFindText(GHandle gh, const char* text);

/**
 * @brief				Set the custom parameter of a row with a given ID
 *
 * @param[in] gh		The widget handle (must be a table handle)
 * @param[in] row		The row ID
 * @param[in] param		The parameter to be set
 *
 * @api
 */
void gwinTableRowSetParam(GHandle gh, int row, uint16_t param);

/**
 * @brief				Get the custom parameter of an row with a given ID
 *
 * @param[in] gh		The widget handle (must be a table handle)
 * @param[in] row		The row ID
 *
 * @return				The parameter
 *
 * @api
 */
uint16_t gwinTableRowGetParam(GHandle gh, int row);

/**
 * @brief				Delete all the rows of the table
 *
 * @param[in] gh		The widget handle (must be a table handle)
 *
 * @api
 */
void gwinTableDeleteAll(GHandle gh);

/**
 * @brief				Delete an row from the table
 *
 * @param[in] gh		The widget handle (must be a table handle)
 * @param[in] row		The row ID
 *
 * @api
 */
void gwinTableRowDelete(GHandle gh, int row);

/**
 * @brief				Get the number of rows within the table
 *
 * @param[in] gh		The widget handle (must be a table handle)
 *
 * @return				The number of rows in the table
 *
 * @api
 */
int gwinTableRowCount(GHandle gh);

/**
* @brief				Get the number of columns within the table
*
* @param[in] gh			The widget handle (must be a table handle)
*
* @return				The number of columns in the table
*
* @api
*/
int gwinTableColumnCount(GHandle gh);


/**
 * @brief				Check if an row with a given ID is selected
 *
 * @param[in] gh		The widget handle (must be a table handle)
 * @param[in] row 		The row ID
 *
 * @return				TRUE if the row is selected, FALSE otherwise
 *
 * @api
 */
bool_t gwinTableRowIsSelected(GHandle gh, int row);

/**
 * @warning
 * @brief				Get the ID of the selected row
 *
 * @param[in] gh		The widget handle (must be a table handle)
 *
 * @return				The ID of the selected table row for a single-select table.
 *
 * @note				It always returns -1 (nothing selected) for a multi-select table.
 * 						Instead use @p gwinTablerowIsSelected() to get the selection status
 * 						for a multi-select table.
 *
 * @api
 */
int gwinTableGetSelected(GHandle gh);

/**
* @warning			Return the text for the first column?
* @brief			Get the text of the selected row in a specific column
*
* @param[in] gh		The widget handle (must be a table handle)
*
* @return			The test of the selected table row for a single-select table in a specific column.
*
* @note				It always returns NULL (nothing selected) for a multi-select table.
*
* @api
*/
const char* gwinTableGetSelectedText(GHandle gh);

/**
 * @brief				Set whether a specific row is selected or not
 *
 * @param[in] gh		The widget handle (must be a table handle)
 * @param[in] row		The row ID
 * @param[in] doSelect	TRUE to select the row or FALSE to deselect the row
 *
 * @note				Changing the selection using this api call will NOT send the table selection
 * 						change event.
 * @note				With a single select table selecting an row with this call will deselect
 * 						any existing selected row. De-selecting an row with this call will not
 * 						cause a new row to be automatically selected.
 * @note				De-selecting an row that is not selected will not effect any rows that
 * 						are selected, even in single-select mode.
 * @api
 */
void gwinTableSetSelected(GHandle gh, int row, bool_t doSelect);

/**
 * @brief				Scroll the table so the specified row is in view
 *
 * @param[in] gh		The widget handle (must be a table handle)
 * @param[in] row 		The row ID
 *
 * @note				This will typically scroll the selected row to the top of the table
 * 						unless the row is in the last page of table rows.
 *
 * @api
 */
void gwinTableViewRow(GHandle gh, int row);

#if GWIN_NEED_TABLE_IMAGES || defined(__DOXYGEN__)
	/**
	 * @brief				Set the image for a table row
	 *
	 * @pre					GWIN_NEED_TABLE_IMAGES must be set to true in your gfxconf.h
	 *
	 * @param[in] gh		The widget handle (must be a table handle)
	 * @param[in] row		The row ID
	 * @param[in] pimg		The image to be displayed or NULL to turn off the image for this table row.
	 *
	 * @note				The image should be up to 4 x (the font height) and a width of (the font height).
	 * 						The 1st (top) part of the image is displayed for a selected row.
	 * 						The 2nd part of the image is displayed for an unselected row.
	 * 						The 3rd part of the image is displayed for a disabled selected row.
	 * 						The 4th part of the image is displayed for a disabled unselected row.
	 * 						If the image is less than 4 times the font height then the image use is collapsed
	 * 						into the available height. For example, an image that equals the font height will use
	 * 						the same image for all four states.
	 * @note				The image is only displayed while it is open. It is up to the application to open
	 * 						the image.
	 * @note				The same image can be used on more than one table row.
	 * @note				Images are aligned with the top (not the baseline) of the table row.
	 * @note				When any row in the table has an image attached, space is allocated to display
	 * 						the images even if the image is closed or has been removed by calling @p gwinTablerowSetImage()
	 * 						with a NULL image or by calling @p gwinTableRowDelete(). The only way to turn-off the image area
	 * 						for this table is to call gwinTableDeleteAll().
	 *
	 */
	void gwinTableRowSetImage(GHandle gh, int row, gdispImage *pimg);
#endif

/**
 * @defgroup Renderings_Table Renderings
 *
 * @brief				Built-in rendering functions for the table widget.
 *
 * @details				These function may be passed to @p gwinSetCustomDraw() to get different table drawing styles.
 *
 * @note				In your custom table drawing function you may optionally call these
 * 						standard functions and then draw your extra details on top.
 * @note				These custom drawing routines don't have to worry about setting clipping as the framework
 * 						sets clipping to the object window prior to calling these routines.
 *
 * @{
 */

/**
 * @brief				The default rendering function for the table widget
 *
 * @param[in] gw		The widget object (must be a table object)
 * @param[in] param		A parameter passed in from the user. Ignored by this function.
 *
 * @api
 */
void gwinTableDefaultDraw(GWidgetObject* gw, void* param);

/** @} */

#ifdef __cplusplus
}
#endif

#endif // _GWIN_LIST_H
/** @} */

