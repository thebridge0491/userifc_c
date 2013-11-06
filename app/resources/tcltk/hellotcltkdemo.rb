#!/usr/bin/env ruby -w

=begin rdoc
Description:: Brief description
Version:: $Id:$
=end
require 'English'
require 'rubygems'
require 'tk'

class HelloTcltkDemo
  attr_reader :widgets, :app
  
  def initialize(app=nil)
    @app, @widgets = app, {}
    
    @widgets['frame1'] = TkFrame.new(@app)
    @widgets['label1'] = TkLabel.new(@widgets['frame1'], text: 'label1')
    @widgets['button1'] = TkButton.new(@widgets['frame1'], text: 'button1')
    @widgets['textview1'] = TkText.new(@widgets['frame1'], height: 3, 
      width: 30)
    @widgets['frame1'].pack
    
    %w(label1 button1 textview1).each { |name|
      @widgets[name].pack }
    
    @widgets['dialog1'] = TkToplevel.new
    @widgets['dialog1'].title 'dialog1'
    @widgets['frameDialog1'] = TkFrame.new(@widgets['dialog1'])
    @widgets['txt1'] = TkVariable.new
    @widgets['entry1'] = TkEntry.new @widgets['frameDialog1'], width: 25,
      textvariable: @widgets['txt1']
    @widgets['entry1'].pack
    @widgets['frameDialog1'].pack
    @widgets['dialog1'].withdraw
    
    @widgets['dialog1'].bind('Destroy', proc{ |*args|
      dialog1_destroy_cb(*args) })
    @widgets['button1'].bind('Button', proc{ |*args| 
      button1_clicked_cb(*args) })
    @widgets['txt1'].trace('w', proc{ |*args|
      entry1_textChanged_cb(*args) })
    @widgets['entry1'].bind('Return', proc{ |*args|
      entry1_returnPressed_cb(*args) })
    
    @app.title 'frame1'
  end
  
  private
  def dialog1_destroy_cb(widget, callback_data=nil)
    @app.destroy
  end
  
  def button1_clicked_cb(widget, callback_data=nil)
    @widgets['dialog1'].deiconify
    @app.withdraw
    @widgets['entry1'].set_focus
    #@widgets['entry1'].delete(0, 'end')
    @widgets['entry1'].value = ''
  end
  
  def entry1_textChanged_cb(widget, extra, callback_data=nil)
    #@widgets['textview1'].delete(0)
    @widgets['textview1'].value = ''
  end
  
  def entry1_returnPressed_cb(widget, callback_data=nil)
    @app.deiconify
    @widgets['dialog1'].withdraw
    #@widgets['textview1'].delete(0)
    @widgets['textview1'].value = ''
    @widgets['textview1'].insert('end',
      'Hello, %s.' % @widgets['entry1'].get)
  end
end # end class HelloTcltkDemo

if $PROGRAM_NAME == __FILE__
  app = TkRoot.new
  
  pretext = '(Ruby %s) Tk %s GUI' % [RUBY_VERSION, Tk::TK_VERSION]
  gui = HelloTcltkDemo.new app
  #gui.widgets['textview1'].delete(0)
  gui.widgets['textview1'].value = ''
  gui.widgets['textview1'].insert('end', pretext)
  
  Tk.mainloop

  exit 0
end


# (one-time): gem install --user-install tk
# ruby hellotcltkdemo.rb
