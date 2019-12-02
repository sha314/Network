## Data file format
1. First line of the data file should contain a json formatted header 
    which will contain usable information about the data.
2. After that there should be few lines of comments where the description
    of the data will remain, e.g, name of the columns, meaning of symbols etc.
3. data should be separated by spaces (preferred) or commas
4. Each line that is not the data itself should start with a '#' symbol (symbol of comment).
    This includes the header too.
5. Data should be written as column format but in special cases it can be different.

6. Data should have maximum precision, since this is the original data.

## Header Informaiton
must be written in json format

element        | description
:---: | :----: | 

random_seed    | random seed that is used
:---: | :----: | 

data           | date of generated file
:---: | :----: | 

signature       | signature of the filename
:---: | :----: |

network_class   | used network class
:---: | :----: |

percolation_class   | used percolation class
:---: | :----: |

m               | m value
:---: | :----: |

N               | network size or number of nodes
:---: | :----: |

M               | for explosive percolation
:---: | :----: |

number_of_links | number of links in the network
:---: | :----: |

number_of_nodes | number of nodes
:---: | :----: |

En              | ensemble size
:---: | :----: |

ensemble_size   | ensemble size
:---: | :----: |

