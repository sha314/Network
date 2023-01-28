# 2023.01.27
Using entropy to minimize entropy instead of product/sum of clusters to decide which link to use. 

delta H_ij=mu_i*log(mu_i)+mu_j*log(mu_j)-mu_(i+j)*log(mu_(i+j) 

delta H_kl=mu_k*log(mu_k)+mu_l*log(mu_l)-mu_(k+l)*log(mu_(k+l) 

If magnitude of delta H_ij is smaller than magnitude of delta H_kl than add link e_ij else add link e_kl.


