libUdis86-ruby
==============

**libUdis86-ruby** is a ruby language binding for udis86 disassembler library.

http://udis86.sourceforge.net/

This library can be used to inspect raw x86/x64 code from your ruby script.

## Build
```
ruby extconf.rb
make
```

## Example
```
u = Udis86.new
u.mode = 64 # set x64 disassemble mode
u.input = "\x31\xc0\xc9"

p u.to_a
```

## License
BSD License apply
