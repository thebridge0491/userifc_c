#!/usr/bin/env python -t

from __future__ import (absolute_import, division, print_function,
  unicode_literals)

import sys, os, logging, inspect, enum, atexit
from future.builtins import (ascii, filter, hex, map, oct, zip, str)

import curses, curses.panel

Keys = {    # usage: Keys['KEY_ENTER']
  'KEY_ENTER': ord('E') - 64,  # Ctrl+E -- Enter (Curses.KEY_ENTER)
  'KEY_ESC': ord('X') - 64,    # Ctrl+X -- Exit  (Curses.KEY_EXIT)
  'KEY_RUN': ord('R') - 64     # Ctrl+R -- Run   (???)
  }

def userifc_version():
  import platform

  return '(Python {0}) Ncurses {1}.{2}.{3}'.format(platform.python_version(),
    curses.ncurses_version.major, curses.ncurses_version.minor,
    curses.ncurses_version.patch)

class HelloCursesDemo(object):
  '''Description:: '''

  def __init__(self, screen=curses.initscr()):
    '''Construct object'''

    super(HelloCursesDemo, self).__init__()
    atexit.register(self.cleanup)
    
    self.stdscr, self.panels = screen, {}
    self.setup()
    #self.stdscr.clear()
    orig_hgt, orig_wid = self.stdscr.getmaxyx()
    
    self.panels['output'] = curses.panel.new_panel(
      curses.newwin(orig_hgt - 5, orig_wid - 2, 1, 1))
    self.panels['input'] = curses.panel.new_panel(
      curses.newwin(3, orig_wid // 2, 7, 20))
    self.panels['commands'] = curses.panel.new_panel(
      curses.newwin(4, orig_wid - 2, orig_hgt - 5, 1))

    #self.stdscr.addstr(0, (orig_wid - len(__name__) - 2) // 2, __name__,
    #  curses.A_REVERSE)
    self.stdscr.addstr(__name__.ljust(orig_wid - 32), curses.A_REVERSE)
    
    for _, pan in self.panels.items():
      pan.window().clear()
      pan.window().border()
    self.panels['commands'].window().addch(1, 1, Keys['KEY_RUN'],
      curses.A_STANDOUT)
    self.panels['commands'].window().addstr(' Run'.ljust(11))
    self.panels['commands'].window().addch(Keys['KEY_ENTER'],
      curses.A_STANDOUT)
    self.panels['commands'].window().addstr(' Enter Name'.ljust(11))
    self.panels['commands'].window().addch(2, 1, Keys['KEY_ESC'], 
      curses.A_STANDOUT)
    self.panels['commands'].window().addstr(' Exit'.ljust(11))
    #self.stdscr.refresh()

  def setup(self):
    #self.stdscr = curses.initscr()
    curses.noecho()
    curses.cbreak()
    self.stdscr.keypad(True)
    #return stdscr

  def cleanup(self):
    curses.nocbreak()
    self.stdscr.keypad(False)
    curses.echo()
    curses.endwin()    
  
  def step_virtualscr(self):
    isRunning = True
    self.panels['input'].window().clear()
    self.panels['input'].window().border()
    self.panels['input'].hide()
    ch = self.panels['commands'].window().getch()
    
    if Keys['KEY_ENTER'] == ch:
      self.on_key_enter()
    elif Keys['KEY_ESC'] == ch:
      isRunning = False
    elif Keys['KEY_RUN'] != ch:
      self.on_key_unmapped(ch)
    
    for _, pan in self.panels.items():
      pan.window().noutrefresh()
    return isRunning
  
  def run(self):
    curses.noecho()
    self.stdscr.refresh()
    
    while self.step_virtualscr():
      #curses.panel.update_panels()
      curses.doupdate()

  def on_key_unmapped(self, ch):
    '''Callback for unmapped key'''
    
    self.panels['input'].window().addstr(1, 1, 
      'Error! Un-mapped key: {0}. Retrying.'.format(
      curses.unctrl(ch).decode('utf-8')))
    self.panels['input'].window().refresh()
    curses.flash()
    curses.napms(2000)
  
  def on_key_enter(self):
    '''Callback for Keys.KEY_ENTER'''
    
    self.panels['input'].top()
    curses.echo()
    data = self.panels['input'].window().getstr(1, 1).decode('utf-8')
    cur_y, _ = self.panels['output'].window().getyx()
    max_y, _ = self.panels['output'].window().getmaxyx()
    if (max_y - 3) < cur_y:
      self.panels['output'].window().clear()
      self.panels['output'].window().border()
    cur_y, _ = self.panels['output'].window().getyx()
    self.panels['output'].window().addstr(cur_y+1, 1,
      'Hello, {}.'.format(data))
    curses.noecho()

def main(argv=None):
  pretext = '{0} TUI\n'.format(userifc_version())
  gui = HelloCursesDemo(curses.initscr())
  gui.stdscr.addstr(1, 1, pretext, curses.A_REVERSE)
  gui.run()
  
  return 0

if '__main__' == __name__:
  sys.exit(main(sys.argv[1:]))


# python hellocursesdemo.py
