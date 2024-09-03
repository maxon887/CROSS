import sys
import lldb

class CrossArraySyntheticProvider:
	def __init__(self, valobj, internal_dict):
		self.valobj = valobj
	def update(self):
		self.data = self.valobj.GetChildMemberWithName('data')
		self.size = self.valobj.GetChildMemberWithName('size')
		self.capacity = self.valobj.GetChildMemberWithName('capacity')
		self.data_type = self.valobj.GetType().GetTemplateArgumentType(0)
		self.data_size = self.data_type.GetByteSize()
	def num_children(self):
		return self.size.GetValueAsUnsigned(0) + 2
	def get_child_index(self,name):
		return int(name.lstrip('[').rstrip(']'))
	def get_child_at_index(self,index):
		if index == 0:
			return self.size
		if index == 1:
			return self.capacity
		offset = (index - 2) * self.data_size
		return self.data.CreateChildAtOffset('[' + str(index - 2) + ']', offset, self.data_type)

def __lldb_init_module(debugger, dict):
	print("Initilize debugger information for Cross basic types")
	debugger.HandleCommand('type summary add cross::String --summary-string "${var.data}"')
	debugger.HandleCommand('type summary add -x "cross::Array<.+>$" --summary-string "size ${var.size} capacity ${var.capacity}"')
	debugger.HandleCommand('type synthetic add -x "cross::Array<.+>$" --python-class cross_lldb.CrossArraySyntheticProvider')