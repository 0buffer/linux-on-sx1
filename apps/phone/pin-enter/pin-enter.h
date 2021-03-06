// generated by Fast Light User Interface Designer (fluid) version 1.0107

#ifndef pin_enter_h
#define pin_enter_h
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Secret_Input.H>
#include <FL/Fl_Button.H>

#include <flgui/Fl_App.h>
#include <gsmd/event.h>

class UserInterface: public Fl_App {
public:
  UserInterface();
  Fl_Output *message;
  Fl_Secret_Input *pin_code;
	int GetPINType();
	bool PinOK;
private:
	int EnterPIN(char *oldpin, char *newpin);
	static void cb_pin_ok_timer(void *data);
	static void cb_wait_animation(void *data);
	virtual void handle_method_return(struct tbus_message *msg);
private:
  void cb_pin_code_i(Fl_Input*, void*);
  static void cb_pin_code(Fl_Input*, void*);
public:
private:
  void cb_LeftSoft_i(Fl_Button*, void*);
  static void cb_LeftSoft(Fl_Button*, void*);
public:
private:
  void cb_RightSoft_i(Fl_Button*, void*);
  static void cb_RightSoft(Fl_Button*, void*);
  int x;
};
#endif
