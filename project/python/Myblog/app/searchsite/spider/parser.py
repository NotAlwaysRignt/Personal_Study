import re

class Parser:
    # the form of pattern_map(self-definition): {"infomation_name": re object, ......}
    def __init__(self, pattern_map={}):
        self.pattern = pattern_map       
              
        
    # parameter "key" must be in self.pattern, then the function will search for the content according to the key in self.pattern ,return the first result   
    def search(self,string, key):
        if not string:
            return ""
        result = self.pattern[key].findall(string)
        if result:
            return result[0]
        else:
            return ""
     
     #  parameter "key" must be in self.pattern, then the function will search for the content according to the key in self.pattern ,return all results 
    def find_all(self,string, key):
        if not string:
            return ""
        result = self.pattern[key].findall(string)
        if result:
            return result  
        else:
            return ""
    
    # the parameter "li" is a list which includes a list of key. The key should exist in self.pattern
    # return a dict , the key value is the first result calculated by re according to key        
    def search_from_list(self,string, li):
        if not string or not li:
            return {}
        result_dict = {}
        for key in li:
            result = self.pattern[key].findall(string)
            if result:
                result_dict[key] = result[0]
            else:
                result_dict[key] = ""
                   
        return result_dict                
     
     
    # the parameter "li" is a list which includes a list of key. The key should exist in self.pattern
    # return a dict , the key value is the result(a list) calculated by self.pattern according to key            
    def find_all_from_list(self,string, li):
        if not string or not li:
            return {}
        result_dict = {}
        for key in li:
            result = self.pattern[key].findall(string)
            if result:
                result_dict[key] = result
            else:
                result_dict[key] = ""
                   
        return result_dict
