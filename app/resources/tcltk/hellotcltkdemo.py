#!/usr/bin/env python -t

from __future__ import (absolute_import, division, print_function,
  unicode_literals)

import sys, os, logging, inspect
from future.builtins import (ascii, filter, hex, map, oct, zip, str)

try:
  import tkinter as tk
except ImportError as exc:
  print(repr(exc))
  import Tkinter as tk

def userifc_version():
  import platform

  return "(Python {0}) Tk {1}".format(platform.python_version(),
    tk.TkVersion)

class HelloTcltkDemo(object):
  '''Description:: '''

  def __init__(self, app=None):
    '''Construct object'''

    super(HelloTcltkDemo, self).__init__()
    
    self.app, self.widgets = app, {}
    
    self.widgets['frame1'] = tk.Frame(self.app)
    self.widgets['label1'] = tk.Label(self.widgets['frame1'], text='label1')
    self.widgets['button1'] = tk.Button(self.widgets['frame1'], 
      text='button1')
    self.widgets['textview1'] = tk.Text(self.widgets['frame1'], height=3, 
      width=30)
    self.widgets['frame1'].pack()
    
    for widget in map(self.widgets.get, ['label1', 'button1', 'textview1']):
      widget.pack()
    
    self.widgets['dialog1'] = tk.Toplevel()
    self.widgets['dialog1'].title('dialog1')
    self.widgets['frameDialog1'] = tk.Frame(self.widgets['dialog1'])
    self.widgets['txt1'] = tk.StringVar()
    self.widgets['entry1'] = tk.Entry(master=self.widgets['frameDialog1'],
      width=25, textvariable=self.widgets['txt1'])
    self.widgets['entry1'].pack()
    self.widgets['frameDialog1'].pack()
    self.widgets['dialog1'].withdraw()
    
    self.widgets['dialog1'].bind('<Destroy>', self.dialog1_destroyed_cb)
    self.widgets['button1'].bind('<Button>', self.button1_clicked_cb)
    self.widgets['txt1'].trace('w', self.entry1_textChanged_cb)
    self.widgets['entry1'].bind('<Return>', self.entry1_returnPressed_cb)
    
    self.app.title('frame1')
    self.widgets['frame1'].lift()

  def dialog1_destroyed_cb(self, dialog1, callback_data=None):
    '''Callback for dialog1 destroyed'''

    self.app.quit()

  def button1_clicked_cb(self, button1, callback_data=None):
    '''Callback for button1 clicked'''
    
    #try:
    #  import tkSimpleDialog as simpledialog
    #except ImportError as exc:
    #  print(repr(exc))
    #  import tkinter.simpledialog as simpledialog
    #data = simpledialog.askstring('dialog1', '', 
    #  parent=self.widgets['frame1'])

    self.widgets['dialog1'].deiconify()
    self.app.withdraw()
    self.widgets['entry1'].focus_set()
    self.widgets['entry1'].delete(0, tk.END)

  def entry1_textChanged_cb(self, entry1, extra, callback_data=None):
    '''Callback for entry1 textChanged'''

    self.widgets['textview1'].delete(1.0, tk.END)

  def entry1_returnPressed_cb(self, entry1, callback_data=None):
    '''Callback for entry1 returnPressed'''

    self.app.deiconify()
    self.widgets['dialog1'].withdraw()
    self.widgets['textview1'].delete(1.0, tk.END)
    self.widgets['textview1'].insert(tk.INSERT,
      'Hello, {}.'.format(self.widgets['entry1'].get()))

def main(argv=None):
  app = tk.Tk()
  
  pretext = '{0} GUI\n'.format(userifc_version())
  gui = HelloTcltkDemo(app)
  gui.widgets['textview1'].delete(1.0, tk.END)
  gui.widgets['textview1'].insert(tk.INSERT, pretext)
  
  app.mainloop()
  return 0

if '__main__' == __name__:
  sys.exit(main(sys.argv[1:]))


# (one-time): pip install --user tkinter
# python hellotcltkdemo.py
