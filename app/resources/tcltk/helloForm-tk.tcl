#!/bin/sh
# the next line restarts using [tclsh | wish] \
exec tclsh "$0" ${1+"$@"}

package require Tk

proc view_init {pretext} {
  wm title . "frame1"

  frame .frame1
  label .frame1.label1 -text "label1"
  #button .button1 -text "button1" -command {
  #  puts stdout "Hello, world!"
  #  destroy .
  #}
  button .frame1.button1 -text "button1"
  text .frame1.textview1 -height 3 -width 30
  pack .frame1

  #pack .frame1.label1 .frame1.button1 .frame1.textview1
  #foreach widget {.frame1.label1 .frame1.button1 .frame1.textview1} {
  #  pack $widget
  #}
  foreach widget [winfo children .frame1] {
    pack $widget
  }

  toplevel .dialog1
  wm title .dialog1 "dialog1"
  frame .dialog1.frameDialog1
  # ??? set .dialog1.frameDialog1.entry1.txt1 ""
  set .dialog1.frameDialog1.entry1.txt1 ""
  entry .dialog1.frameDialog1.entry1 -width 25 \
    -textvariable .dialog1.frameDialog1.entry1.txt1
  pack .dialog1.frameDialog1.entry1
  pack .dialog1.frameDialog1
  wm withdraw .dialog1
  #lower .dialog1

  bind .dialog1 <Destroy> {dialog1_destroyed_cb .dialog1 []}
  bind .frame1.button1 <Button> {button1_clicked_cb .frame1.button1 []}
  #trace variable .dialog1.frameDialog1.entry1.txt1 w {entry1_textChanged_cb \
    .dialog1.frameDialog1.entry1 [] []}
  bind .dialog1.frameDialog1.entry1 <Return> {entry1_returnPressed_cb \
    .dialog1.frameDialog1.entry1 []}

  .frame1.textview1 delete 1.0 end
  .frame1.textview1 insert end $pretext

  raise .frame1
  return .frame1
}

proc dialog1_destroyed_cb {dlg {cb_data ""}} {
  destroy .frame1
  destroy .
}

proc button1_clicked_cb {btn {cb_data ""}} {
  wm deiconify .dialog1
  wm withdraw .
  focus .dialog1.frameDialog1.entry1
  .dialog1.frameDialog1.entry1 delete 0 end
}

proc entry1_textChanged_cb {ent {extra ""} {cb_data ""}} {
  .frame1.textview1 delete 1.0 end
}

proc entry1_returnPressed_cb {ent {cb_data ""}} {
  wm deiconify .
  wm withdraw .dialog1
  .frame1.textview1 delete 1.0 end
  .frame1.textview1 insert end [format "Hello, %s." \
    [.dialog1.frameDialog1.entry1 get]]
}

proc lib_main {} {
  set pretext [format "(Tcl %s) Tk %s\n" $::tcl_version $::tk_version]
  set view1 [view_init $pretext]
  
  #vwait forever
  return 0
}

if {[info exists ::argv0] && $::argv0 eq [info script]} {
  lib_main
}


# perl -e "use Tcl::pTk ; my $interp = Tcl::pTk->new ; $interp->Eval('source helloForm-tk.tcl ; lib_main') ; $interp->MainLoop"
# or
# ruby -e "require 'tk' ; Tk.ip_eval('source helloForm-tk.tcl ; lib_main') ; Tk.mainloop()"
# or
# python -c "import tkinter as tk ; interp = tk.Tcl() ; interp.eval('source helloForm-tk.tcl ; lib_main') ; interp.mainloop()"
# or
# tclsh helloForm-tk.tcl
