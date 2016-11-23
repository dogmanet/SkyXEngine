
#ifndef s4g_compiler_h
#define s4g_compiler_h
#pragma once


struct s4g_command
{
	s4g_command(){command = mc_halt;arg = 0;}
	s4g_command(s4g_vm_command comm,s4g_value* val=0){command = comm;arg = val;}
	s4g_vm_command command;
	s4g_value* arg;
};

//#define gen(comm,val) commands.push(s4g_command(comm,val))

class s4g_compiler
{
public:
	s4g_compiler(){}
	void gen(s4g_vm_command comm,s4g_value* val=0);
	int compile(s4g_node* node,Stack<s4g_command> * commands);
	int compile2(s4g_node* node);
//protected:
	Stack<s4g_command>* comms;
	//long pc;
};

#endif 