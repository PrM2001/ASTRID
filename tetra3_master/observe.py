import sys
sys.path.append('..') #do we need to change this path?
from tetra3 import Tetra3
from PIL import Image
from pathlib import Path


def getCurrPos():
    return (1,2)

def getDesiredPos(argument):
    return(3,4)