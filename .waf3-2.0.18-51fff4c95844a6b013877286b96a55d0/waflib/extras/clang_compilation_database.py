#! /usr/bin/env python
# encoding: utf-8
# WARNING! Do not edit! https://waf.io/book/index.html#_obtaining_the_waf_file

import os
from waflib import Logs,TaskGen,Task,Build,Scripting
Task.Task.keep_last_cmd=True
@TaskGen.feature('c','cxx')
@TaskGen.after_method('process_use')
def collect_compilation_db_tasks(self):
	if not isinstance(self.bld,ClangDbContext):
		return
	tup=tuple(y for y in[Task.classes.get(x)for x in('c','cxx')]if y)
	for task in getattr(self,'compiled_tasks',[]):
		if isinstance(task,tup):
			self.bld.clang_compilation_database_tasks.append(task)
class ClangDbContext(Build.BuildContext):
	'''generates compile_commands.json by request'''
	cmd='clangdb'
	clang_compilation_database_tasks=[]
	def write_compilation_database(self):
		database_file=self.bldnode.make_node('compile_commands.json')
		Logs.info('Build commands will be stored in %s',database_file.path_from(self.path))
		try:
			root=database_file.read_json()
		except IOError:
			root=[]
		clang_db=dict((x['file'],x)for x in root)
		for task in self.clang_compilation_database_tasks:
			try:
				cmd=task.last_cmd
			except AttributeError:
				continue
			directory=getattr(task,'cwd',self.variant_dir)
			f_node=task.inputs[0]
			filename=os.path.relpath(f_node.abspath(),directory)
			entry={"directory":directory,"arguments":cmd,"file":filename,}
			clang_db[filename]=entry
		root=list(clang_db.values())
		database_file.write_json(root)
	def execute(self):
		self.restore()
		if not self.all_envs:
			self.load_envs()
		self.recurse([self.run_dir])
		self.pre_build()
		def exec_command(self,*k,**kw):
			return 0
		for g in self.groups:
			for tg in g:
				try:
					f=tg.post
				except AttributeError:
					pass
				else:
					f()
				if isinstance(tg,Task.Task):
					lst=[tg]
				else:lst=tg.tasks
				for tsk in lst:
					tup=tuple(y for y in[Task.classes.get(x)for x in('c','cxx')]if y)
					if isinstance(tsk,tup):
						old_exec=tsk.exec_command
						tsk.exec_command=exec_command
						tsk.run()
						tsk.exec_command=old_exec
		self.write_compilation_database()
EXECUTE_PATCHED=False
def patch_execute():
	global EXECUTE_PATCHED
	if EXECUTE_PATCHED:
		return
	def new_execute_build(self):
		if type(self)==Build.BuildContext:
			Scripting.run_command('clangdb')
		old_execute_build(self)
	old_execute_build=getattr(Build.BuildContext,'execute_build',None)
	setattr(Build.BuildContext,'execute_build',new_execute_build)
	EXECUTE_PATCHED=True
patch_execute()
