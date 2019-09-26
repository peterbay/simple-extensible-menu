/*
  Menu.cpp - Library for universal menu.
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

#include "Arduino.h"
#include "Menu.h"

Menu::Menu()
{
  this->rootItem.uid = 0;
}

void Menu::init(Menu::ITEM * item) {
  this->mainItem = item;
  this->activeItem = item;
}

void Menu::reset() {
  this->activeItem = this->mainItem;
}

void Menu::addItem (Menu::ITEM * item, char hotkey, char * text, byte externalIndex, Menu::ITEM * parent, void (* cb)(), void (* value_cb)(bool isItemLine)) {
  Menu::ITEM * childItem;
  if (parent == NULL) {
    parent = &this->rootItem;

  }
  item->parent = parent;
  item->prev = NULL;
  item->next = NULL;
  item->child = NULL;
  item->text = text;
  item->cb = cb;
  item->value_cb = value_cb;
  item->uid = this->itemCounter;
  item->hotkey = hotkey;
  item->disabled = false;
  item->externalIndex = externalIndex;
  this->itemCounter += 1;
  if (parent->child == NULL) {
    parent->child = item;

  } else {
    childItem = parent->child;
    while (childItem->next) {
      childItem = childItem->next;

    }
    childItem->next = item;
    item->prev = childItem;
  }
}

void Menu::action(byte key) {
  Menu::ITEM * parentItem = this->activeItem->parent;

  if (this->circular && key == this->KEY_UP && this->activeItem->prev == NULL) {
    while (this->activeItem->next) {
      this->activeItem = this->activeItem->next;
    }

  } else if (this->circular && key == this->KEY_DOWN && this->activeItem->next == NULL) {
    while (this->activeItem->prev) {
      this->activeItem = this->activeItem->prev;
    }

  } else if (key == this->KEY_UP && this->activeItem->prev != NULL) {
    this->activeItem = this->activeItem->prev;

  } else if (key == this->KEY_DOWN && this->activeItem->next != NULL) {
    this->activeItem = this->activeItem->next;

  } else if (key == this->KEY_LEFT) {
    if (parentItem->uid != 0) {
        this->activeItem = this->activeItem->parent;

    } else if (parentItem->uid == 0 && this->menuExit != NULL) {
      (this->menuExit)();
      return;
      
    }
    
  } else if (this->activeItem->disabled == false) {
    if ((key == this->KEY_RIGHT || key == this->KEY_RIGHT_ENTER) && this->activeItem->child != NULL) {
      this->activeItem = this->activeItem->child;

    } else if ((key == this->KEY_ENTER || key == this->KEY_RIGHT_ENTER) && this->activeItem->cb != NULL) {
      (this->activeItem->cb)();

    }
  }

  if (this->autoRender) {
    this->render();

  }
}

void Menu::actionHotkey (char hotkey) {
  Menu::ITEM * item = this->activeItem;
  while (item->prev) {
    item = item->prev;

  }
  do {
    if (item->hotkey == hotkey) {
      this->activeItem = item;
      this->action(this->KEY_RIGHT_ENTER);
      break;

    } else if (item->next != NULL){
      item = item->next;

    } else {
      break;

    }
  } while (true);
}

void Menu::render() {
  if (this->preRender != NULL) {
    (this->preRender)();

  }

  Menu::ITEM * showItem = this->activeItem;
  int itemsCounter = this->itemsLimit;
  int renderIndex = 0;
  bool iterate = true;

  if (this->showTitle) {
    if (this->renderTitle != NULL) {
      (this->renderTitle)(this->activeItem->parent);

    }
    itemsCounter -= 1;
  }

  if (this->showPreviousitems) { 
    while (showItem->prev) {
      showItem = showItem->prev;

    }
  }

  do {
    if (this->renderItem != NULL) {
      if (renderIndex == 0 && showItem->prev != NULL && this->renderScrollUp != NULL) {
        (this->renderScrollUp)();
      }
      (this->renderItem)(showItem, renderIndex, (showItem->uid == this->activeItem->uid), this->markActive, this->inlineValue);
      renderIndex++;

    }

    itemsCounter -= 1;
    if (itemsCounter > 0 && showItem->next != NULL){
      showItem = showItem->next;

    } else {
      iterate = false;

    }
  } while (iterate);

  if (showItem->next != NULL && this->renderScrollDown != NULL) {
    (this->renderScrollDown)();
  }

  if (this->showValue) {
    showItem = this->activeItem->parent;
    if (showItem->value_cb != NULL) {
      (showItem->value_cb)(false);

    }
  }

  if (this->postRender != NULL) {
    (this->postRender)();

  }
}
