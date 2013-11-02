#!/usr/bin/env ruby -w

=begin rdoc
Description:: Brief description
Version:: $Id:$
=end
require 'English'
require 'rubygems'
require 'gir_ffi-gtk3' # 'gir_ffi-gtk3' 'gir_ffi-gtk'

class HelloGtkDemo
  attr_reader :widgets
  
  def initialize
    @widgets = {}
=begin
    @widgets.merge!({'window1' => Gtk::Window.new(:toplevel),
      'vbox1' => Gtk::Box.new(Gtk::Orientation::VERTICAL, 10),
      'label1' => Gtk::Label.new('label1'),
      'button1' => Gtk::Button.new_with_label('button1'),
      'textview1' => Gtk::TextView.new, 'dialog1' => Gtk::Dialog.new,
      'entry1' => Gtk::Entry.new})
    content_area = @widgets['dialog1'].get_content_area
    [@widgets['label1'], @widgets['button1'], @widgets['textview1']].map { 
      |arg| @widgets['vbox1'].pack_start(arg, true, true, 0) }
    @widgets['window1'].add @widgets['vbox1']
    content_area.pack_start(@widgets['entry1'], true, true, 0)
=end
    uiform = !Gtk.check_version(3, 0, 0) ? 'gtk/helloForm-gtk3.glade' : 
      'gtk/helloForm-gtk2.glade'
    rsrc_path = ENV.fetch('RSRC_PATH', 'resources')
    builder = Gtk::Builder.new
    #filename = File.join(File.dirname(__FILE__), rsrc_path + '/' + uiform)
    #builder << filename
    builder.add_from_file(rsrc_path + '/' + uiform)
    if true # not Gtk.check_version(3, 0, 0)
      %w(window1 label1 button1 textview1 dialog1 entry1).each { |o| 
          @widgets[o] = builder.get_object(o) }
    else
      builder.objects.each { |obj| @widgets[obj.builder_name] = obj }
    end
    
    #@widgets['window1'].signal_connect('destroy') { Gtk.main_quit }
    @widgets['window1'].signal_connect('destroy') { |w, d|
      window1_destroy_cb(w, d) }
    @widgets['button1'].signal_connect('clicked') { |w, d| 
      button1_clicked_cb(w, d) }
    @widgets['dialog1'].signal_connect('destroy') { |w, d|
      dialog1_destroy_cb(w, d) }
    @widgets['dialog1'].signal_connect('response') { |w, r, d|
      dialog1_response_cb(w, r, d) }
    @widgets['entry1'].signal_connect('activate') { |w, d|
      entry1_activate_cb(w, d) }
    #builder.connect_signals { |handler| method(handler) }
    
    @window1 = @widgets['window1']
    @window1.set_default_size(200, 160)
    @window1.set_title('HelloGtkDemo')

    @widgets['dialog1'].set_default_size(160, 100)
    @widgets['dialog1'].set_title('dialog1')
    
    @window1.show_all
  end
  
  private
  def window1_destroy_cb(widget, callback_data=nil)
    Gtk.main_quit
  end
  
  def button1_clicked_cb(widget, callback_data=nil)
    @widgets['entry1'].text = ''
    @widgets['dialog1'].show_all
    @widgets['textview1'].show
  end
  
  def dialog1_destroy_cb(widget, callback_data=nil)
    @widgets['window1'].destroy
  end
  
  def dialog1_response_cb(widget, response, callback_data=nil)
    @widgets['entry1'].activate
    @widgets['dialog1'].hide
  end
  
  def entry1_activate_cb(widget, callback_data=nil)
    @widgets['textview1'].buffer.set_text 'Hello, %s.' % 
      [@widgets['entry1'].text], -1
    @widgets['dialog1'].hide
  end
end # end class HelloGtkDemo

if $PROGRAM_NAME == __FILE__
  Gtk.init
  
  pretext = '(Ruby %s) Gtk+ %d.%d GUI' % [RUBY_VERSION, Gtk::MAJOR_VERSION,
    Gtk::MINOR_VERSION]
  gui = HelloGtkDemo.new
  gui.widgets['textview1'].buffer.set_text pretext, -1
  
  Gtk.main

  exit 0
end


# (one-time): gem install --user-install gir_ffi-gtk
# ruby hellogtkdemo.rb
