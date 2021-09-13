PUTSLAM
=======

RGBD-based Simultaneous Localization and Mapping developed at Poznań University of Technology (PUT).

Global requirements:
  - CMake > 2.8
  - GCC > 4.6 (4.7)
  - OpenCV > 2.42 (3.0)
  - PCL > 1.7 (1.7)


The installation guide, running guide and many more can be found in git wiki:
https://github.com/LRMPUT/PUTSLAM/wiki


## **TO DO by UcefMountacer**

1. convert scripts in python2 to python3
2. add data prep for iphone 12 rgb-d data

### modifications done

-To compile PUTSLAM:
libqglviewer-headers instead of libqglviewer-dev

-Couldn't build octomap

-G2O:
libqglviewer-headers instead of libqglviewer-dev
(command notreq btw)

-G2O:
can be found directly at ./PUTSLAM/3rdParty/g2o instead of downloading it
so cd PUTSLAM/3rdParty/g2o

-prepare dataset script in python2
convert it to python3 

**actually no, run whole scriptwith py2, it works even if the virtual env is based on py3**

-in the wiki : speciied wrong file to change the path to dataset
instead : Go to PUTSLAM/ressources/datasetConfig, find xml with the right dataset name (or create own), modify path with the right one then in the putslamfileModel.xml, specify name of this xml inside datasetConfig

- installing qt4 libs on ubuntu 20
https://ubuntuhandbook.org/index.php/2020/07/install-qt4-ubuntu-20-04/




