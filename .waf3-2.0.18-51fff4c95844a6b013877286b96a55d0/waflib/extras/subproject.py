#! /usr/bin/env python
# encoding: utf-8
# WARNING! Do not edit! https://waf.io/book/index.html#_obtaining_the_waf_file

'''
Subproject tool

Helps you have standalone environment for each subproject(subdirectory)

Usage:
		def options(opt):
			opt.load('subproject')

		def configure(conf):
			conf.add_subproject('folder1 folder2')

		def build(bld):
			bld.add_subproject('folder1 folder2')
'''
from waflib import Configure,Logs,Options,Utils
import os
DEPTH=''
def depth_push(path):
	global DEPTH
	DEPTH=os.path.join(DEPTH,path)
def depth_pop():
	global DEPTH
	DEPTH=os.path.dirname(DEPTH)
def get_name_by_depth():
	global DEPTH
	return DEPTH.replace(os.sep,'_')
def opt(f):
	setattr(Options.OptionsContext,f.__name__,f)
	return f
@opt
def add_subproject(ctx,names):
	names_lst=Utils.to_list(names)
	for name in names_lst:
		depth_push(name)
		wscript_path=os.path.join(DEPTH,'wscript')
		if not os.path.isfile(wscript_path):
			ctx.add_option_group('Cannot find wscript in '+name+'. You probably missed submodule update')
		else:
			ctx.recurse(name)
		depth_pop()
def options(opt):
	grp=opt.add_option_group('Subproject options')
	grp.add_option('-S','--skip-subprojects',action='store',dest='SKIP_SUBDIRS',default=None,help='don\'t recurse into specified subprojects. Use only directory name.')
def get_subproject_env(ctx,path,log=False):
	path=str(path)
	if path.startswith(ctx.top_dir):
		if ctx.top_dir[-1]!=os.pathsep:
			path=path[len(ctx.top_dir)+1:]
		else:path=path[len(ctx.top_dir):]
	folders=os.path.normpath(path).split(os.sep)
	for i in range(1,len(folders)+1):
		name=folders[-i]
		if name in ctx.all_envs:
			if log:Logs.pprint('YELLOW','env: changed to %s'%name)
			return ctx.all_envs[name]
	if log:Logs.pprint('YELLOW','env: changed to default env')
	raise IndexError('top env')
@Configure.conf
def add_subproject(ctx,names):
	names_lst=Utils.to_list(names)
	if isinstance(ctx,Configure.ConfigurationContext):
		if not ctx.env.IGNORED_SUBDIRS and ctx.options.SKIP_SUBDIRS:
			ctx.env.IGNORED_SUBDIRS=ctx.options.SKIP_SUBDIRS.split(',')
		for name in names_lst:
			depth_push(name)
			if name in ctx.env.IGNORED_SUBDIRS:
				ctx.msg(msg='--X %s'%DEPTH,result='ignored',color='YELLOW')
				depth_pop()
				continue
			saveenv=ctx.env
			ctx.setenv(get_name_by_depth(),ctx.env)
			ctx.env.ENVNAME=name
			ctx.msg(msg='--> %s'%DEPTH,result='in progress',color='BLUE')
			ctx.recurse(name)
			ctx.msg(msg='<-- %s'%DEPTH,result='done',color='BLUE')
			ctx.setenv('')
			ctx.env=saveenv
			depth_pop()
	else:
		if not ctx.all_envs:
			ctx.load_envs()
		for name in names_lst:
			depth_push(name)
			if name in ctx.env.IGNORED_SUBDIRS:
				depth_pop()
				continue
			saveenv=ctx.env
			ctx.env=ctx.all_envs[get_name_by_depth()]
			ctx.recurse(name)
			ctx.env=saveenv
			depth_pop()
