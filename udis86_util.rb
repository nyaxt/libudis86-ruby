require_relative './udis86'

class Udis86

  def to_a
    ret = []

    while u.next != 0
      ret << u.insn_asm
    end

    ret
  end

end
