#include <udis86.h>

extern "C" {

#include "ruby.h"
#include "ruby/encoding.h"

void Init_udis86();

struct Udis86_wrap
{
	ud_t ud_obj;
};

} // end of extern "C"

void
free_Udis86_wrap(Udis86_wrap* wrap)
{
	/* NOP */
}

VALUE RBK_Udis86 = Qnil;

VALUE
Udis86_new(VALUE klass)
{
	Udis86_wrap* wrap;
	VALUE ret;

	ret = Data_Make_Struct(klass, Udis86_wrap, 0, free_Udis86_wrap, wrap);

	ud_init(&wrap->ud_obj);
	ud_set_syntax(&wrap->ud_obj, UD_SYN_INTEL);	

	return ret;
}
#define GET_WRAP \
	Udis86_wrap* wrap; \
	Data_Get_Struct(self, Udis86_wrap, wrap);

VALUE
Udis86_set_input(VALUE self, VALUE v_input)
{
	GET_WRAP;

	VALUE v_input_clone = rb_str_dup_frozen(StringValue(v_input));
	rb_iv_set(self, "@input", v_input_clone);

	ud_set_input_buffer(&wrap->ud_obj, (uint8_t*)RSTRING_PTR(v_input_clone), RSTRING_LEN(v_input_clone));

	return Qnil;
}

#if 0
	Check_Type(v_srcstr, T_STRING);
	ud_set_input_buffer(wrap->ud_obj, RSTRING_PTR(v_srcstr), RSTRING_LEN(v_srcstr));
#endif

VALUE
Udis86_set_mode(VALUE self, VALUE v_mode)
{
	GET_WRAP;

	int mode = FIX2INT(v_mode);
	switch(mode)
	{
	case 16:
	case 32:
	case 64:
		ud_set_mode(&wrap->ud_obj, mode);
		break;

	default:
		rb_raise(rb_eArgError, "invalid mode");
	}

	return Qnil;
}

VALUE
Udis86_set_pc(VALUE self, VALUE v_pc)
{
	GET_WRAP;

	unsigned long pc = FIX2ULONG(v_pc);
	ud_set_pc(&wrap->ud_obj, pc);

	return Qnil;
}

VALUE
Udis86_set_syntax(VALUE self, VALUE v_syntax)
{
	GET_WRAP;

	Check_Type(v_syntax, T_STRING);
	const char* syntax = StringValueCStr(v_syntax);
	if(strcmp(syntax, "INTEL") == 0)
	{
		ud_set_syntax(&wrap->ud_obj, UD_SYN_INTEL);	
	}
	else if(strcmp(syntax, "AT&T") == 0 || strcmp(syntax, "ATT") == 0)
	{
		ud_set_syntax(&wrap->ud_obj, UD_SYN_ATT);	
	}
	else
	{
		ud_set_syntax(&wrap->ud_obj, NULL);
	}

	return Qnil;
}

VALUE
Udis86_set_vendor(VALUE self, VALUE v_vendor)
{
	GET_WRAP;

	Check_Type(v_vendor, T_STRING);
	const char* vendor = StringValueCStr(v_vendor);
	if(strcmp(vendor, "INTEL") == 0)
	{
		ud_set_vendor(&wrap->ud_obj, UD_VENDOR_INTEL);	
	}
	else if(strcmp(vendor, "AMD") == 0)
	{
		ud_set_vendor(&wrap->ud_obj, UD_VENDOR_AMD);	
	}
	else
	{
		rb_raise(rb_eArgError, "unknown vendor");
	}

	return Qnil;
}

VALUE
Udis86_next(VALUE self)
{
	GET_WRAP;
	
	return INT2FIX(ud_disassemble(&wrap->ud_obj));
}

VALUE
Udis86_insn_len(VALUE self)
{
	GET_WRAP;
	
	return INT2FIX(ud_insn_len(&wrap->ud_obj));
}

VALUE
Udis86_insn_off(VALUE self)
{
	GET_WRAP;

	return rb_ull2inum(ud_insn_off(&wrap->ud_obj));
}

VALUE
Udis86_insn_hexstr(VALUE self)
{
	GET_WRAP;

	char* hexstr = ud_insn_hex(&wrap->ud_obj);
	return rb_str_new_cstr(hexstr);
}

VALUE
Udis86_insn_raw(VALUE self)
{
	GET_WRAP;
	
	return rb_str_new((const char*)ud_insn_ptr(&wrap->ud_obj), ud_insn_len(&wrap->ud_obj));
}

VALUE
Udis86_insn_asm(VALUE self)
{
	GET_WRAP;

	return rb_str_new_cstr(ud_insn_asm(&wrap->ud_obj));
}


VALUE
Udis86_input_skip(VALUE self, VALUE v_n)
{
	GET_WRAP;

	unsigned long n = FIX2ULONG(v_n);
	ud_input_skip(&wrap->ud_obj, n);

	return Qnil;
}

typedef VALUE (*ruby_method_t)(...);
void
Init_udis86()
{
	RBK_Udis86 = rb_define_class("Udis86", rb_cObject);
	
	rb_define_singleton_method(RBK_Udis86, "new", (ruby_method_t)Udis86_new, 0);

	rb_define_method(RBK_Udis86, "input=", (ruby_method_t)Udis86_set_input, 1);

	rb_define_method(RBK_Udis86, "mode=", (ruby_method_t)Udis86_set_mode, 1);
	rb_define_method(RBK_Udis86, "pc=", (ruby_method_t)Udis86_set_pc, 1);
	rb_define_method(RBK_Udis86, "syntax=", (ruby_method_t)Udis86_set_syntax, 1);
	rb_define_method(RBK_Udis86, "vendor=", (ruby_method_t)Udis86_set_vendor, 1);
	rb_define_method(RBK_Udis86, "next", (ruby_method_t)Udis86_next, 0);
	rb_define_method(RBK_Udis86, "insn_len", (ruby_method_t)Udis86_insn_len, 0);
	rb_define_method(RBK_Udis86, "insn_off", (ruby_method_t)Udis86_insn_off, 0);
	rb_define_method(RBK_Udis86, "insn_hexstr", (ruby_method_t)Udis86_insn_hexstr, 0);
	rb_define_method(RBK_Udis86, "insn_raw", (ruby_method_t)Udis86_insn_raw, 0);
	rb_define_method(RBK_Udis86, "insn_asm", (ruby_method_t)Udis86_insn_asm, 0);
	rb_define_method(RBK_Udis86, "input_skip", (ruby_method_t)Udis86_input_skip, 1);
}
