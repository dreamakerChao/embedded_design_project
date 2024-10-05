import os
import numpy as np
from PIL import Image
import shutil

curr_folder = "./Dataset" 
sub_folder =os.listdir(curr_folder)
# print(sub_folder )

class_data=[]
j=0
class_data.append(("number","path","class_name","label"))
for i in range(len(sub_folder)):
    full_path = curr_folder + '/' +sub_folder[i]
    for item in os.listdir(full_path):       
        class_data.append((j,full_path+'/'+item,sub_folder[i],i))
        j += 1

class_data = np.array(class_data)

np.savetxt("class.csv",class_data, delimiter=",",fmt='%s')