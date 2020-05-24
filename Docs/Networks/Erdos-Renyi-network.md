# Erdos-Renyi Network (ER) Nerwork

Static network.

Number of Nodes, = N
Max number of edges, M = N(N-1)/2

say we want to create only 'e' edges then
p = e/M

1. go over all the links one by one
2. generate a random number between [0,1] and call it 'r'
3. if r <= p : then connect the link. else skip the step



## For percolation purpose
p=N/M
meaning,
 number of nodes = N
 number of edges = N