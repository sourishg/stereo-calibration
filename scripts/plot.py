import yaml
import numpy as np
import matplotlib.pyplot as plt

def opencv_matrix(loader, node):
    mapping = loader.construct_mapping(node, deep=True)
    mat = np.array(mapping["data"])
    mat.resize(mapping["rows"], mapping["cols"])
    return mat

yaml.add_constructor(u"tag:yaml.org,2002:opencv-matrix", opencv_matrix)
yml_dir = '../build/cam_left.yml'

with open(yml_dir,'r') as infile:
    _ = infile.readline()
    data = yaml.load(infile)

plt.plot([data['cerror']], [data['num_imgs']], marker='o', markersize=10, color="red")
plt.axis([0, 1, 0, 50])
plt.show()
