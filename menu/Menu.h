/*
  Menu.h - Library for universal menu.
  (c) 2019, Petr Vavrin, pvavrin@gmail.com - MIT license

  Permission is hereby granted, free of charge, to any person
  obtaining a copy of this software and associated documentation
  files (the "Software"), to deal in the Software without
  restriction, including without limitation the rights to use,
  copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the
  Software is furnished to do so, subject to the following
  conditions:

  The above copyright notice and this permission notice shall be
  included in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
  OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
  HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
  OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef Menu_h
#define Menu_h

#include "Arduino.h"

class Menu
{
  public:
    struct ITEM {
      Menu::ITEM * parent;
      Menu::ITEM * prev;
      Menu::ITEM * next;
      Menu::ITEM * child;
      int uid;
      char * text;
      bool disabled;
      void (* cb)();
      void (* value_cb)(bool isItemLine);
      char hotkey;
      byte externalIndex;
    };
    Menu::ITEM * mainItem;
    Menu::ITEM * activeItem;
    Menu::ITEM rootItem;
    int itemCounter = 1;

    bool autoRender = true;
    bool showTitle = true;
    bool showValue = true;
    bool showPreviousitems = true;
    bool circular = true;
    bool markActive = true;
    bool inlineValue = true;
    byte itemsLimit = 255;

    byte KEY_UP = 1;
    byte KEY_DOWN = 2;
    byte KEY_LEFT = 3;
    byte KEY_RIGHT = 4;
    byte KEY_ENTER = 5;
    byte KEY_RIGHT_ENTER = 6;

    byte BUTTON_CLICK = Menu::KEY_DOWN;
    byte BUTTON_DOUBLE_CLICK = Menu::KEY_LEFT;
    byte BUTTON_LONG_CLICK = Menu::KEY_RIGHT_ENTER;

    void (* menuExit) () = NULL;
    void (* preRender) () = NULL;
    void (* postRender) () = NULL;
    void (* renderScrollUp) () = NULL;
    void (* renderScrollDown) () = NULL;
    void (* renderTitle)(Menu::ITEM * item) = NULL;
    void (* renderItem)(Menu::ITEM * item, int renderIndex, bool active, bool markActive, bool inlineValue) = NULL;
    
    Menu();
    void init(Menu::ITEM * item);
    void reset();
    void addItem (Menu::ITEM * item, char hotkey, char * text, byte externalIndex, Menu::ITEM * parent, void (* cb)(), void (* value_cb)(bool isItemLine));
    void action(byte key);
    void actionHotkey (char hotkey);
    void render();

  private:

};

#endif
