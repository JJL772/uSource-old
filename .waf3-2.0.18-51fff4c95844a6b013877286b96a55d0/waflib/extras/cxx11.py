#! /usr/bin/env python
# encoding: utf-8
# WARNING! Do not edit! https://waf.io/book/index.html#_obtaining_the_waf_file

try:from fwgslib import get_flags_by_compiler
except:from waflib.extras.fwgslib import get_flags_by_compiler
def check_cxx11(ctx,msg):
	try:
		ctx.check_cxx(fragment='int main( void ){ auto pfnLambda = [](){}; return 0;}',msg=msg)
	except ctx.errors.ConfigurationError:
		return False
	return True
def configure(conf):
	conf.env.HAVE_CXX11=True
	if not check_cxx11(conf,'Checking if \'{0}\' supports C++11'.format(conf.env.COMPILER_CC)):
		modern_cpp_flags={'msvc':[],'default':['-std=c++11']}
		flags=get_flags_by_compiler(modern_cpp_flags,conf.env.COMPILER_CC)
		if(len(flags)==0):
			conf.env.HAVE_CXX11=False
		else:
			env_stash=conf.env
			conf.env.append_unique('CXXFLAGS',flags)
			if not check_cxx11(conf,'...trying with additional flags'):
				conf.env.HAVE_CXX11=False
				conf.env=env_stash
	if getattr(conf.env,'CXX11_MANDATORY'):
		conf.fatal('C++11 support not available!')
