#! /usr/bin/env python
# encoding: utf-8
# WARNING! Do not edit! https://waf.io/book/index.html#_obtaining_the_waf_file

import os
def get_git_version():
	version=None
	if os.path.exists('.git'):
		try:
			version=os.popen('git describe --dirty --always').read().strip()
		except Exception as e:
			pass
		if(len(version)==0):
			version=None
	return version
def configure(conf):
	conf.start_msg('Checking git hash')
	conf.env.GIT_VERSION=get_git_version()
	conf.end_msg(conf.env.GIT_VERSION if conf.env.GIT_VERSION else'no')
