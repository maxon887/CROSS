import sys
import lldb

class InliningSyntheticProvider3:
    def __init__(self, valobj, internal_dict):
        self.valobj = valobj
        print("IAAAdsnjsnfjenfajcnasjkcnaj")
        logger = lldb.formatters.Logger.Logger()
        logger >> "HHHHHEHEEEEHHHHHeee " + str(valobj.GetName())


    def update(self):
        self.wrapped_member = self.valobj.GetChildMemberWithName('mData')
        # Without this, the children of the wrapped member will not be synthetic
        # members. To properly forward certain types like std::vector, we need the
        # synthetic members -- otherwise, we will get the real private member
        # variables, which are not useful.
        self.wrapped_member.SetPreferSyntheticValue(True)
    def num_children(self):
        return self.wrapped_member.num_children
    def get_child_index(self,name):
        # tbh, I'm not sure about this.
        return 0
    def get_child_at_index(self,index):
        return self.wrapped_member.GetChildAtIndex(index)

def __lldb_init_module(debugger, dict):
	print("Initilize debugger information for Cross basic types")
	lldb.formatters.Logger._lldb_formatters_debug_level = 2
	lldb.formatters.Logger._lldb_formatters_debug_filename = "/Users/Shared/lldb.py.log"
	debugger.HandleCommand('type summary add cross::String --summary-string "${var.data}"')
	#debugger.HandleCommand('type synthetic add cross::Array --python-class test.CrossArraySyntethicProvider')
	#debugger.HandleCommand('type synthetic add -w live -l cross_lldb.InliningSyntheticProvider3 MyVector')
	debugger.HandleCommand('type synthetic add MyVector --python-class cross_lldb.InliningSyntheticProvider3')
