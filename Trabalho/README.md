Trabalho de programação concorrente - 2023-2

#### ALUNA ANA CAROLINE DA ROCHA BRAZ - 212008482

# RESUMO
O seguinte trabalho apresentou sobre o que é programação concorrente, suas utilidades e como ela está inserida no meio de sistemas de streaming. Para mostrar na prática, foi formalizado uma questão onde há 5 usuários querendo utilizar um sistema de streaming, no entanto apenas 2 sessões podiam ser utilizadas ao mesmo tempo e, quando chegasse a um certo número de streamings, um administrador precisaria realizar a atualização do sistema. 
Para isso foi implementado um código utilizando a biblioteca \textit{POSIX Pthreads} e executado mostrando como isso aconteceria, chegando ao resultado final esperado. 

# COMPILAÇÃO
Para compilar, faça:
    `gcc streaming.c -o streaming -lpthread `
    `.\streaming`
