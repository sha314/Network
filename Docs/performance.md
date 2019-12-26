# BA Network Building 
date: 2019.07.04

m | N | NetworkBA | NetworkBA_v2 |
:---: | :---: | :---: | :---: |

5   |   100K    | 0.49 sec | 0.30 sec
:---: | :---: | :---: | :---: |

5   |   500K    | 2.64 sec | 1.68 sec
:---: | :---: | :---: | :---: |

5   |   1M    | 5.50 sec | 3.55 sec
:---: | :---: | :---: | :---: |

5   |   2M    | 11.35 sec | 7.45 sec
:---: | :---: | :---: | :---: |

10   |   1M    | 8.66 sec | 7.56 sec
:---: | :---: | :---: | :---: |



# Digonto vai codes
Explosive percolation on BA network with

m = 5,
M=2

N     |   Time (sec) |  Mem
:---: | :----: | :---: | 

50k   | 0.170  | na
:---: | :----: | :---: |
 
100k  | 0.379  | na
:---: | :----: | :---: | 

1M    |   4.23  | na
:---: | :----: | :---: | 

2M    |   9.50 |         250 MB
:---: | :----: | :---: |

date : 2019.10.16
# Digonto vai codes (compiled with -O3 flag)
Explosive percolation on BA network with

m = 5,
M=2

N     |   Time (sec) |  Mem
:---: | :----: | :---: |

100k  | 0.251  | na
:---: | :----: | :---: |

1M    |   3.709  | na
:---: | :----: | :---: |

2M    |   7.30 |         250 MB
:---: | :----: | :---: |



date : 2019.11.04
Digonto's code have same performance as mine. 
But digonto vai stops the loop  after N iteration,
whereas I stop the loop after M iteration.
N is the number of nodes and M is the number of links.
M is always much greater than N.



#merging cluster
making the larger cluster the root one makes the program run faster
m=5,N=1,000,000
with larger cluster as root,merge_cluster_v2(), time = 2.15223 sec
without, merge_cluster_v1(), time = 2.85 sec
performance increase with merge_cluster_v2() = (2.85-2.15)/2.85 = 25 %
25 % performance boost