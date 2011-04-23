#!/usr/bin/ruby
require 'mkmf'

extension_name = 'udis86'

# $CFLAGS += ' -fPIC '
find_header("udis86.h", '/home/kouhei/local/include')
find_library('udis86', 'ud_init', '/home/kouhei/local/lib')

dir_config(extension_name)
create_makefile(extension_name)
