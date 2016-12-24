
#ifndef s4g_compiler_h
#define s4g_compiler_h
#pragma once


struct s4g_command
{
	s4g_command(){ command = mc_halt; arg = 0; lexid = -1; }
	s4g_command(s4g_vm_command comm, s4g_value* val, long _lexid){ command = comm; arg = val; lexid = _lexid; }
	s4g_vm_command command;
	s4g_value* arg;
	long lexid;
};

//#define gen(comm,val) commands.push(s4g_command(comm,val))

class s4g_compiler
{
public:
	s4g_compiler(){}
	void gen(s4g_vm_command comm,s4g_value* val,long _lexid = -1);
	int compile(s4g_node* node, s4g_stack<s4g_command> * commands);
	int compile2(s4g_node* node);
//protected:
	s4g_stack<s4g_command,16>* comms;
	s4g_gc* gc;
};

#endif 