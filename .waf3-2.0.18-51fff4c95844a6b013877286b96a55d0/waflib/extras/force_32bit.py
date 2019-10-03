#! /usr/bin/env python
# encoding: utf-8
# WARNING! Do not edit! https://waf.io/book/index.html#_obtaining_the_waf_file

try:from fwgslib import get_flags_by_compiler
except:from waflib.extras.fwgslib import get_flags_by_compiler
def check_32bit(ctx,msg):
	try:
		ctx.check_cc(fragment='''int main( void )
			{
				int check[sizeof(void*) == 4 ? 1: -1];
				return 0;
			}''',msg=msg)
	except ctx.errors.ConfigurationError:
		return False
	return True
def configure(conf):
	if check_32bit(conf,'Checking if \'{0}\' can target 32-bit'.format(conf.env.COMPILER_CC)):
		conf.env.DEST_SIZEOF_VOID_P=4
	else:
		conf.env.DEST_SIZEOF_VOID_P=8
		if not conf.env.BIT32_ALLOW64:
			flags=['-m32']
			if(conf.env.DEST_OS=='darwin'):
				flags=['-arch','i386']
			env_stash=conf.env
			conf.env.append_value('LINKFLAGS',flags)
			conf.env.append_value('CFLAGS',flags)
			conf.env.append_value('CXXFLAGS',flags)
			if check_32bit(conf,'...trying with additional flags'):
				conf.env.DEST_SIZEOF_VOID_P=4
			else:
				conf.env.DEST_SIZEOF_VOID_P=8
			conf.env=env_stash
			if conf.env.BIT32_MANDATORY and conf.env.DEST_SIZEOF_VOID_P==8:
				conf.fatal('Compiler can\'t create 32-bit code!')
