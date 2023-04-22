import sys 
from pathlib import Path
sys.path.append('/tetra3_master')


path = Path('./tetra3_master/test_data/')


for impath in sorted(path.glob('*'), reverse=True):
    print(impath)