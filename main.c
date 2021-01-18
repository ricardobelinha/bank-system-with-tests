#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//include bibliotecas internas
#include "ui.h"
#include "sgb.h"

/**
 * Process os pedidos no menu Clientes
 * @param banco
 **/
void processaMenuClientes(Banco *b)
{
    int opt;
    while (1)
    {
        opt = menuClientes();
        switch (opt)
        {
        case 1:

            inserirCliente(b);
            break;
        case 2:

            listarClientes(*b);
            break;
        case 3:

            mostrarCliente(*b);
            break;
        case 4:

            apagarCliente(b);
            break;
        case 5:

            procurarClientes(*b);
            break;
        case 9:
            return; //Sai da função, logo do menu Clientes
        default:
            INVALID_OPTION;
        }
    }
}

/**
 * Process os pedidos no menu Contas
 * @param banco
 **/
void processaMenuContas(Banco *b)
{
    int opt;
    while (1)
    {
        opt = menuContas();
        switch (opt)
        {
        case 1:

            criarConta(b);
            break;
        case 2:

            listarContas(*b);
            break;
        case 3:

            mostrarConta(*b);
            break;
        case 4:

            fecharConta(b);
            break;
        case 9:
            return; //Sai da função, logo do menu Contas
        default:
            INVALID_OPTION;
        }
    }
}

/**
 * Process os pedidos no meu Clientes
 * @param banco
 **/
void processaMenuMovimentos(Banco *b)
{
    int opt;
    while (1)
    {
        opt = menuMovimentos();
        switch (opt)
        {
        case 1:

            depositarDinheiroDefault(b);
            break;
        case 2:

            levantarDinheiroDefault(b);
            break;
        case 3:

            transferirDinheiro(b);
            break;
        case 9:
            return; //Sai da função, logo do menu Movimentos
        default:
            INVALID_OPTION;
        }
    }
}

int main(int argc, char const *argv[])
{
    Banco b;
    int opt;
    iniciarBanco(&b, "TPSI1020-BANK");
    while (1)
    {
        opt = menu();
        switch (opt)
        {
        case 1:
            processaMenuClientes(&b);
            break;
        case 2:
            processaMenuContas(&b);
            break;
        case 3:
            processaMenuMovimentos(&b);
            break;
        case 4:

            informacaoBanco(b);
            break;
        case 9:
            exit(EXIT_SUCCESS); //Sai da aplicação
        default:
            INVALID_OPTION;
            break;
        }
    }

    return EXIT_SUCCESS;
}