#!/usr/bin/env python -t

from __future__ import (absolute_import, division, print_function,
  unicode_literals)

import sys, os, logging, inspect
from future.builtins import (ascii, filter, hex, map, oct, zip, str)

if '2' == os.environ.get('GTK_MAJOR_VERSION', 'LATEST'):
  import pygtk
  pygtk.require('2.0')
  import gtk as Gtk
  _uiform = 'gtk/helloForm-gtk2.glade'
else:
  import gi
  gi.require_version('Gtk', '3.0')
  from gi.repository import Gtk
  _uiform = 'gtk/helloForm-gtk3.glade'

def userifc_version():
  import platform

  try:    # version >2.99
    major_version, minor_version = Gtk.MAJOR_VERSION, Gtk.MINOR_VERSION
  except: # version <2.99
    major_version, minor_version, micro_version = Gtk.gtk_version
  return "(Python {0}) Gtk+ {1}.{2}".format(platform.python_version(),
    major_version, minor_version)

class HelloGtkDemo(object):
  '''Description:: '''

  def __init__(self):
    '''Construct object'''

    super(HelloGtkDemo, self).__init__()
    
    self.widgets = {}
    
    #self.widgets.update({'window1': Gtk.Window(), 'frame1': Gtk.Frame(),
    #  'vbox1': Gtk.Box(orientation=Gtk.Orientation.VERTICAL, spacing=10),
    #  'label1': Gtk.Label(label='label'),
    #  'button1': Gtk.Button(label='button'),
    #  'textview1': Gtk.TextView(), 'dialog1': Gtk.Dialog(),
    #  'entry1': Gtk.Entry()})
    #for widget in map(self.widgets.get, ['label1', 'button1', 'textview1']):
    #  self.widgets['vbox1'].pack_start(widget, True, True, 0)
    ##self.widgets['dialog1'].get_content_area().pack_start(
    ##  self.widgets['entry1'], False, False, 0)
    #self.widgets['dialog1'].vbox.pack_start(self.widgets['entry1'], 
    #  False, False, 0)
    #self.widgets['frame1'].add(self.widgets['vbox1'])
    #self.widgets['window1'].add(self.widgets['frame1'])
    
    rsrc_path = os.environ.get('RSRC_PATH', 'resources')
    builder = Gtk.Builder()
    builder.add_from_file(rsrc_path + '/' + _uiform)
    for name in ['window1', 'dialog1', 'button1', 'textview1', 'entry1']:
      self.widgets[name] = builder.get_object(name)
    
    self.widgets['window1'].connect('destroy', self.window1_destroy_cb)
    self.widgets['dialog1'].connect('destroy', self.dialog1_destroy_cb)
    self.widgets['dialog1'].connect('response', self.dialog1_response_cb)
    self.widgets['button1'].connect('clicked', self.button1_clicked_cb)
    self.widgets['entry1'].connect('activate', self.entry1_activate_cb)
    #builder.connect_signals(self)

    self.widgets['window1'].set_default_size(200, 160)
    self.widgets['window1'].show_all()

  def window1_destroy_cb(self, window1, callback_data=None):
    '''Callback for window1 destroy'''

    Gtk.main_quit(callback_data)

  def dialog1_destroy_cb(self, dialog1, callback_data=None):
    '''Callback for dialog1 destroy'''

    self.widgets['window1'].destroy()

  def dialog1_response_cb(self, dialog1, callback_data=None):
    '''Callback for dialog1 response'''

    self.widgets['entry1'].activate()
    dialog1.hide()

  def button1_clicked_cb(self, button1, callback_data=None):
    '''Callback for button1 clicked'''

    self.widgets['textview1'].show()
    self.widgets['dialog1'].show_all()
    self.widgets['entry1'].set_text('')

  def entry1_activate_cb(self, entry1, callback_data=None):
    '''Callback for entry1 activate'''

    self.widgets['textview1'].get_buffer().set_text(
      'Hello, {}.'.format(entry1.get_text()))
    self.widgets['dialog1'].hide()

def main(argv=None):
  pretext = '{0} GUI\n'.format(userifc_version())
  gui = HelloGtkDemo()
  gui.widgets['textview1'].get_buffer().set_text(pretext)
  
  Gtk.main()
  return 0

if '__main__' == __name__:
  sys.exit(main(sys.argv[1:]))


# (one-time): pip install --user pyobject
# python hellogtkdemo.py
