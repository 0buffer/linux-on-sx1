// generated by Fast Light User Interface Designer (fluid) version 1.0107

#include "dialer.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>

#include <FL/fl_ask.H>

void DialerProgramUI::cb_Select_i(Fl_Menu_*, void*) {
  label1->value("selected 1");
}

void DialerProgramUI::cb_Select(Fl_Menu_* o, void* v) {
  ((DialerProgramUI*)(o->parent()->user_data()))->cb_Select_i(o,v);
}

Fl_Menu_Item DialerProgramUI::menu_menu_left[] = {
 {"Select 1", 0,  (Fl_Callback*)DialerProgramUI::cb_Select, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Select 2", 0,  (Fl_Callback*)DialerProgramUI::cb_Select, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Select 3", 0,  (Fl_Callback*)DialerProgramUI::cb_Select, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {0,0,0,0,0,0,0,0,0}
};


DialerProgramUI::DialerProgramUI(int width, int height)
:Fl_Window(width, height, "")
{      
  label("Simple Dialer App");
  strcpy ( num_td, "" );

  begin();
    label1 = new Fl_Output(10, 32, 150, 18);
    label1->value ( num_td );
    label2 = new Fl_Output(10, 62, 150, 18);
    label2->value ( "" );

/*    menu_left = new Fl_Menu_Button(0, 180, 85, 20, "Options");
    menu_left->box(FL_FLAT_BOX);
    menu_left->menu(menu_menu_left);*/
    
      Fl_Menu_Button* o = menu_left = new Fl_Menu_Button(0, 180, 85, 20, "Options");
      o->box(FL_FLAT_BOX);
      o->menu(menu_menu_left);
  end();
  
  lgsmh = lgsm_init(LGSMD_DEVICE_GSMD);
  if (!lgsmh) 
  {
//    fprintf(stderr, "Can't connect to gsmd\n");
    fl_alert("Can't connect to gsmd\n");
    exit(1);
  }  
  
}

int DialerProgramUI::handle(int event)
{
  switch (event) 
  {
    case FL_PUSH:
    case FL_RELEASE:
    case FL_DRAG:
    case FL_MOVE:
    case FL_FOCUS:
      label ("Gained focus");
      damage(1);
      return 1;
    case FL_UNFOCUS:
      label ("Lost focus");
      damage(1);
      return 1;
    case FL_KEYBOARD:  
      return handle_key(event,Fl::event_key());      
    default:
      return Fl_Window::handle(event);
  };
}

int DialerProgramUI::handle_key(int event, int key) 
{

  char *tmp;
  tmp = (char*)malloc(128);
  sprintf(tmp, "%d", key);
  label2->value ( tmp );

  switch ( key ) 
  {
    case 'c':  
      label ("letter c was depressed");
      damage(1);
      hide();
      return 1;
    case 's':
      label ("letter s was depressed");
      damage(1);    
//      show();
      return 1;
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      char *tmp;
      tmp = (char*)malloc(128);
      sprintf(tmp, "%d", key-48);
      strcat ( num_td, tmp );
      label1->value(num_td);      
      return 1;
    default:  
      label ("Nothing to do!");
      damage(1);
      return 1;
  }
}

int main(int argc, char **argv) {
  DialerProgramUI ui(176, 220);
	ui.show();
  return Fl::run();
}
