require_relative '../udis86_util'

u = Udis86.new
u.mode = 64

u.input = "\x31\xc0\xc9"
p u.to_a

while u.next != 0
  p u.insn_asm
end
