require_relative './udis86'

class Udis86

  def to_a
    ret = []

    while self.next != 0
      ret << insn_asm
    end

    ret
  end

end
