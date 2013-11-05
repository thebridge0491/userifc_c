#!/usr/bin/env ruby -w

=begin rdoc
Description:: Brief description
Version:: $Id:$
=end
require 'English'
require 'rubygems'
require 'curses'

#module Keys    # usage: Keys::KEY_ENTER
#  KEY_ENTER = 'E'.ord - 64   # Ctrl+E -- Enter (Curses::KEY_ENTER)
#  KEY_ESC = 'X'.ord - 64     # Ctrl+X -- Exit  (Curses::KEY_EXIT)
#  KEY_RUN = 'R'.ord - 64     # Ctrl+R -- Run   (???)
#end
Keys = {    # usage: Keys['KEY_ENTER']
  'KEY_ENTER' => 'E'.ord - 64,  # Ctrl+E -- Enter (Curses::KEY_ENTER)
  'KEY_ESC' => 'X'.ord - 64,    # Ctrl+X -- Exit  (Curses::KEY_EXIT)
  'KEY_RUN' => 'R'.ord - 64     # Ctrl+R -- Run   (???)
  }

class HelloCursesDemo
  attr_reader :widgets, :stdscr
  
  def initialize(screen=Curses.stdscr)
    @stdscr, @panels = screen, {}
    
    setup
    #@stdscr.clear
    orig_hgt, orig_wid = @stdscr.maxy, @stdscr.maxx
    
    # @stdscr.subwin | Curses::Window.new
    @panels['output'] = @stdscr.subwin(orig_hgt - 5, orig_wid - 2, 1, 1)
    @panels['input'] = @stdscr.subwin(3, orig_wid / 2, 7, 20)
    @panels['commands'] = @stdscr.subwin(4, orig_wid - 2, orig_hgt - 5,
      1)

    Curses.attrset(Curses::A_REVERSE)
    @stdscr.setpos(0, 1)
    @stdscr.addstr(__FILE__.ljust(orig_wid - 32))
    
    @panels.each { |_, pan|
      pan.clear
      pan.box("|", "-")
    }
    @panels['commands'].setpos(1, 1)
    @panels['commands'].attrset(Curses::A_STANDOUT)
    @panels['commands'].addch(Keys['KEY_RUN'])
    @panels['commands'].addstr(' Run'.ljust(11))
    @panels['commands'].attrset(Curses::A_STANDOUT)
    @panels['commands'].addch(Keys['KEY_ENTER'])
    @panels['commands'].addstr(' Enter Name'.ljust(11))
    @panels['commands'].setpos(2, 1)
    @panels['commands'].attrset(Curses::A_STANDOUT)
    @panels['commands'].addch(Keys['KEY_ESC'])
    @panels['commands'].addstr(' Exit'.ljust(11))
    #@stdscr.refresh
  end
  
  def run
    Curses.noecho
    @stdscr.refresh
    
    while step_virtualscr
      Curses.doupdate
    end
  end
  
  private
  def setup
    #Curses.init_screen
    Curses.noecho
    Curses.cbreak
    @stdscr.keypad(true)
    @stdscr
  end
  
  def cleanup
    Curses.nocbreak
    @stdscr.keypad(false)
    Curses.echo
    Curses.close_screen
  end
  
  def step_virtualscr
    isRunning = true
    @panels['input'].clear
    @panels['input'].box("|", "-")
    ch = @panels['commands'].getch
    
    if Keys['KEY_ENTER'] == ch
      on_key_enter
    elsif Keys['KEY_ESC'] == ch
      isRunning = false
    elsif Keys['KEY_RUN'] != ch
      on_key_unmapped(ch)
    end
    
    @panels.each { |_, pan|
      pan.noutrefresh
    }
    isRunning
  end
  
  def on_key_unmapped(ch)
    @panels['input'].setpos(1, 1)
    @panels['input'].addstr('Error! Un-mapped key: %s. Retrying.' %
      Curses.keyname(ch))
    @panels['input'].refresh
    Curses.flash
    sleep 2
  end
  
  def on_key_enter
    Curses.echo
    @panels['input'].setpos(1, 1)
    data = @panels['input'].getstr
    cur_y = @panels['output'].cury
    max_y = @panels['output'].maxy
    if (max_y - 3) < cur_y
      @panels['output'].clear
      @panels['output'].box("|", "-")
    end
    cur_y = @panels['output'].cury
    @panels['output'].setpos(cur_y+1, 1)
    @panels['output'].addstr('Hello, %s.' % data)
    Curses.noecho
  end
end # end class HelloCursesDemo

if $PROGRAM_NAME == __FILE__
  Curses.init_screen
  
  begin
    pretext = '(Ruby %s) Curses %s TUI' % [RUBY_VERSION, Curses::VERSION]
    gui = HelloCursesDemo.new Curses.stdscr
    Curses.attrset(Curses::A_REVERSE)
    gui.stdscr.setpos(1, 1)
    gui.stdscr.addstr(pretext)
    Curses.attrset(Curses::A_NORMAL)
    gui.run
  ensure
    Curses.nocbreak
    gui.stdscr.keypad(false)
    Curses.echo
    Curses.close_screen
  end

  exit 0
end


# (one-time): gem install --user-install curses
# ruby hellocursesdemo.rb
