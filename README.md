
# Simple and Extensible Menu

## Features

  - Variable methots of menu control
    - arrows buttons + enter
    - hotkeys (numeric keyboard)
    - single button (click, double click, long click)
    - rotary encoder
    - etc..

  - Different outputs / displays
    - serial console
    - displays with different sizes

  - Menu parameters
    - __autoRender__ - automatically render menu after action calling (default: true)
    - __showTitle__ - call renderTitle before render menu items (default: true)
    - __showValue__ - call value callback defined for item (default: true)
    - __showPreviousitems__ - show all previous items before active item (default: true)
    - __circular__ - circular movement through menu (default: true)
    - __markActive__ - show sign for active item (default: true)
    - __inlineValue__ - call value callback defined for item and show result on same line (default: true)
    - __itemsLimit__ - items limit per menu page (default: 255)

  - Render callbacks
    - __preRender__ - called as first rendered thing (prepare screen, etc..)
    - __renderTitle__ - called for rendering menu title
    - __renderScrollUp__ - called for rendering scroll up sign
    - __renderItem__ - called for rendering menu item
    - __renderScrollDown__ - called for rendering scroll down sign
    - __renderValue__ - called for rendering value on page
    - __postRender__ - called as last rendered thing (variable and state cleanup, etc..)

  - Rest of callbacks
    - __menuExit__ - called when you are in top level page and try go to the parent

  - Menu actions
    - __KEY_UP__ - go to the previous item
    - __KEY_DOWN__ - go to the next item
    - __KEY_LEFT__ - go to the parent item
    - __KEY_RIGHT__ - go to the child item
    - __KEY_ENTER__ - call callback function
    - __KEY_RIGHT_ENTER__ - go to the child item / call callback function
    - __BUTTON_CLICK__ - go to the next item
    - __BUTTON_DOUBLE_CLICK__ - go to the parent item
    - __BUTTON_LONG_CLICK__ - go to the child item / call callback function

  - Hotkeys
    - menu items can be accessible using hotkey (if defined)

  - Others
    - small footprint
    - simple configuration - can be changed dynamically

## Usage

```cpp

#import "Menu.h"

Menu::ITEM menu_1_1;
Menu::ITEM menu_1_2;
Menu::ITEM menu_1_3;
Menu::ITEM menu_1_4;
Menu::ITEM menu_1_5;
Menu::ITEM menu_2_0;
Menu::ITEM menu_2_1;
Menu::ITEM menu_2_2;
Menu::ITEM menu_2_3;
Menu::ITEM menu_3_0;
Menu::ITEM menu_3_1;
Menu::ITEM menu_3_2;

Menu m = Menu();

void defineMenu() {
  // define function called when menu show top level and menu action with KEY_LEFT is called - optional
  m.menuExit = &menuExit;
  
  // define function called before menu rendering - optional
  m.preRender = &preRender;
  
  // define function called after menu rendering - optional
  m.postRender = &postRender;
  
  // define function for showing scroll up symbol - optional
  m.renderScrollUp = &renderScrollUp;
  
  // define function for showing scroll down symbol - optional
  m.renderScrollDown = &renderScrollDown;

  // define function for showing menu title - optional
  m.renderTitle = &renderMenuTitle;

  // define function for showing menu item - required
  m.renderItem = &renderMenuItem;
  
  // automatically render menu after action calling (default: true) - optional
  m.autoRender = true;

  // call renderTitle before render menu items (default: true) - optional
  m.showTitle = true;

  // call value callback defined for item (default: true) - optional
  m.showValue = true;

  // show all previous items before active item (default: true) - optional
  m.showPreviousitems = true;

  // items limit per menu page (default: 255) - optional
  m.itemsLimit = 255;

  // circular movement through menu (default: true) - optional
  m.circular = true;

  // show sign for active item (default: true) - optional
  m.markActive = true;

  // call value callback defined for item and show result on same line (default: true) - optional
  m.inlineValue = true;
  
  // addItem (Menu::ITEM * item, char hotkey, char * text, byte externalIndex, Menu::ITEM * parent, void (* cb)(), void (* value_cb)(bool isItemLine))
  m.addItem(&menu_1_1, '1', "1. Item 1", 0, NULL, NULL, NULL);
  m.addItem(&menu_1_2, '2', "2. Item 2", 0, NULL, NULL, NULL);
  m.addItem(&menu_1_3, '3', "3. Item 3", 0, NULL, NULL, &showValue1);
  m.addItem(&menu_1_4, '4', "4. Item 4", 0, NULL, NULL, NULL);
  m.addItem(&menu_1_5, '5', "5. Item 5", 0, NULL, NULL, NULL);
  
  m.addItem(&menu_2_0, '0', "0. Back",   0, &menu_1_2, &menuGoBack, NULL);
  m.addItem(&menu_2_1, '1', "1. Item 1", 0, &menu_1_2, NULL, NULL);
  m.addItem(&menu_2_2, '2', "2. Item 2", 0, &menu_1_2, NULL, NULL);
  m.addItem(&menu_2_3, '3', "3. Item 3", 0, &menu_1_2, &someFunction2, NULL);

  m.addItem(&menu_3_0, '0', "0. Back",  0,   &menu_1_3, &menuGoBack, NULL);
  m.addItem(&menu_3_1, '1', "1. Decrease", 0, &menu_1_3, &decreaseValue1, NULL);
  m.addItem(&menu_3_2, '2', "2. Increase", 0, &menu_1_3, &increaseValue1, NULL);
  
  // set first menu item - used for setting active and main item
  m.init(&menu_1_1);
  
}

void setup() {
  Serial.begin(9600);
  defineMenu();

  Serial.println("***********************************************");
  Serial.println("*** DEFAULT KEYS ******************************");
  Serial.println("***********************************************");

  // sample menu calling - key actions KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_DOWN, KEY_ENTER
  m.reset();
  m.render();
  m.action(m.KEY_DOWN);
  m.action(m.KEY_DOWN);
  m.action(m.KEY_RIGHT);
  m.action(m.KEY_DOWN);
  m.action(m.KEY_ENTER);
  m.action(m.KEY_ENTER);

  Serial.println("***********************************************");
  Serial.println("*** HOTKEYS ***********************************");
  Serial.println("***********************************************");

  // sample menu calling - using hotkeys
  m.reset();
  m.render();
  m.actionHotkey('3');
  m.actionHotkey('2');
  m.actionHotkey('2');
  m.actionHotkey('2');
  m.actionHotkey('0');

  Serial.println("***********************************************");
  Serial.println("*** SINGLE BUTTON *****************************");
  Serial.println("***********************************************");

  // sample menu calling - using single button
  m.reset();
  m.render();
  m.action(m.BUTTON_CLICK);
  m.action(m.BUTTON_CLICK);
  m.action(m.BUTTON_LONG_CLICK);
  m.action(m.BUTTON_CLICK);
  m.action(m.BUTTON_LONG_CLICK);
  m.action(m.BUTTON_LONG_CLICK);
  m.action(m.BUTTON_DOUBLE_CLICK);

}

void loop() {

}

//------------------------------------------------------------------------------------

void menuExit() {
  Serial.println("Menu EXIT");
}

void preRender() {
  Serial.println("pre render");
}

void postRender() {
  Serial.println("post render");
  Serial.println("------------------------------------------------------");
}

void renderScrollUp() {
  Serial.println("scroll up symbol");
}

void renderScrollDown() {
  Serial.println("scroll down symbol");
}

void renderMenuTitle(Menu::ITEM * item) {
    if (item->uid == 0) {
      Serial.println("Menu: Main");

    } else {
      Serial.print("Submenu: ");
      Serial.println(item->text);
      
    }
    Serial.println("==================");
}

void renderMenuItem(Menu::ITEM * item, int renderIndex, bool active, bool markActive, bool inlineValue) {
  if (markActive) {
    // mark active item
    if (active) {
      Serial.print("> ");
    } else {
      Serial.print("  ");
    }
  }
  
  // show item title
  Serial.print(item->text);

  // is item disabled ? 
  if (item->disabled == true) {
    Serial.print(" (disabled) ");
  }

  // use externalIndex for showing icon 
  // item->externalIndex

  // show item expand symbol (item have submenu)
  if (item->child != NULL ) {
    Serial.print(" + ");
  }

  // render value after item in same line
  if (inlineValue && item->value_cb != NULL) {
    (item->value_cb)(true);
  }
  
  Serial.println("");
}

//------------------------------------------------------------------------------------
byte value1 = 5;

void menuGoBack() {
  m.action(m.KEY_LEFT);
}

void someFunction1 () {
  Serial.println("--- Callback from someFunction1 ---");
}

void someFunction2 () {
  Serial.println("--- Callback from someFunction2 ---");
}

void showValue1 (bool isItemLine) {
  if (isItemLine) {
    Serial.print(value1);
  } else {
    Serial.println("******************");
    Serial.print("Value1 = ");
    Serial.println(value1);
  }
}

void increaseValue1 () {
  if (value1 < 10) {
    value1 += 1;
  }
}

void decreaseValue1 () {
  if (value1 > 0) {
    value1 -= 1;
  }
}

```

## Licence
```
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
```