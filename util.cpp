#include <stdexcept>
#include <string>

#include <curses.h>

#include "util.h"

using namespace std;
using namespace jailcur;

/* Gets a character from the user. */
int util::get_ch()
{
    return getch();
}

/* Adds a character to win at point (y,x). On error, runtime_error is thrown. */
void util::add_ch(char c, WINDOW* win, int y, int x)
{
    int rc = mvwaddch(win, y, x, c);
    if(rc)
        throw runtime_error("jailcur::util::add_ch(): "
                            "Unable to add character to window");
}

/* Add a string str to window win at position (y,x). On error,
 * std::runtime_error is thrown.
 */
void util::add_str(string str, WINDOW* win, int y, int x)
{
   int rc = mvwaddstr(win, y, x, str.c_str() );
   if(rc)
        throw runtime_error("jailcur: Unable to add string to window");
}

/* Uses curses functions to get the height and width of win and clears win of
 * any content, thus making the entire window the colour of bg. On failure,
 * std::runtime_error is thrown.
 */
void util::clear_screen(WINDOW* win)
{
    int y, x;
    getmaxyx(win, y, x);

    for(int i = 0; i < y; ++i)
        for(int k = 0; k < x; ++k)
            mvwaddch(win, i, k, ' ');
            /* Whilst mvwaddch() appears to produce a return code, it does not
             * appear to truthfully state how the operation went. Thus, rc-
             * checking for this function is not being done.
             */
}

/* Deletes win. Throws runtime_error on error. Wraps delwin() */
void util::delete_win(WINDOW* win)
{
    int rc = delwin(win);
    if(rc)
        throw runtime_error("jailcur: util::delete_win(): "
                            "Unable to delete window.");
}

/* Draws a border around the perimeter of win. Throw runtime_error on non-zero
 * return.
 */
void util::draw_border(WINDOW* win)
{
    int rc = box(win, ACS_VLINE, ACS_HLINE);
    if(rc)
        throw runtime_error("jailcur::util::draw_border():"
                            "Unable to draw border");
}

/* Draws a centred title on the top row of win. Throws runtime_error on non-zero
 * return.
 */
void util::draw_title(WINDOW* win, string str)
{
    if(str.length() == 0) return;   // Don't need to draw it

    int dum, L, offset;
    getmaxyx(win, dum, L);  /* Get window length, dump height */
    offset = (L - (str.length() + 2) ) / 2;

    string title = ' ' + str + ' ';
    int rc = mvwaddstr(win, 0, offset, title.c_str() );
    if(rc)
        throw runtime_error("jailcur::util::draw_title():"
                            "Unable to add text to window");
}

/* Refresh a window on the screen in order to remove overlaps of windows no
 * longer on the screen among other things. Wraps touchwin() and wrefresh().
 */
void util::refresh_win(WINDOW* win)
{
    int rc1 = touchwin(win);
    int rc2 = wrefresh(win);
    if(rc1 || rc2)
        throw runtime_error("jailcur: util::refresh_win(): "
                            "Unable to refresh window");
}

/* Set video attributes of a window (e.g. give colour to a window). Throws
 * std::runtime_error on failure.
 */
void util::set_attribute(WINDOW* win, int attrib)
{
    int rc = wattrset(win, attrib);
    if(rc)
        throw runtime_error("jailcur: util::set_attribute: "
                            "Unable to set attribute(s)");
}

/* Creates a WINDOW* with height h and width w, with the top left corner at
 * point (x, y) relative to the top left corner of the screen. Throws runtime_
 * error on error. It acts as a wrapper for newwin().
 */
WINDOW* util::new_win_ptr(int h, int w, int y, int x)
{
    WINDOW* win = newwin(h, w, y, x);
    if(win == nullptr)
        throw runtime_error("jailcur: create_win_ptr(): Cannot create window");
    return win;
}

