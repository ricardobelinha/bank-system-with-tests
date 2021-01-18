#include <string.h>
#include "ui.h"
#include "sgb.h"

/* FIX ME - Completar o ficheiro*/

/**
 * Inicia o banco
 * Esta função terá que ser completada pelos alunos
 **/
void iniciarBanco(Banco *b, char *nome)
{
    b->saldoTotal = 0.0;
    memset(b->nome, '\0', MAX_NAME);
    strncpy(b->nome, nome, MAX_NAME);
    b->num_clientes = 0;
    b->contaClientes = 0;
}

void informacaoBanco(Banco b)
{
    int num_contas = 0;
    for (int i = 0; i < b.num_clientes; i++)
    {
        num_contas += b.clientes[i].num_contas;
    }
    SHOW_BANK_INFO(b.nome, b.num_clientes, num_contas, b.saldoTotal);
}

int existeNif(Banco b, char nif[9])
{
    for (int i = 0; i < b.num_clientes; i++)
    {
        if (!strncmp(nif, b.clientes[i].nif, 9))
        {
            return 1;
        }
    }
    return 0;
}

int existeCliente(Banco b, int id)
{
    for (int i = 0; i < b.num_clientes; i++)
    {
        if (id == b.clientes[i].id)
        {
            return 1;
        }
    }
    return 0;
}

int encontraCliente(Banco b, int id)
{
    for (int i = 0; i < b.num_clientes; i++)
    {
        if (id == b.clientes[i].id)
        {
            return i;
        }
    }
    return -1;
}

int numeroDeContasDoCliente(Banco b, int id)
{
    for (int i = 0; i < b.num_clientes; i++)
    {
        if (id == b.clientes[i].id)
        {
            return b.clientes[i].num_contas;
        }
    }
    return -1;
}

int existeConta(Banco b, int indexCliente, char *contaID)
{
    for (int i = 0; i < b.clientes[indexCliente].num_contas; i++)
    {
        if (strncmp(b.clientes[indexCliente].contas[i].codigoIdentificacao, contaID, MAX_CARACT_IDENTIFICACAO_CONTA) == 0)
        {
            return 1;
        }
    }
    return 0;
}

int encontraConta(Banco b, int indexCliente, char *contaID)
{
    for (int i = 0; i < b.clientes[indexCliente].num_contas; i++)
    {
        if (strncmp(b.clientes[indexCliente].contas[i].codigoIdentificacao, contaID, MAX_CARACT_IDENTIFICACAO_CONTA) == 0)
        {
            return i;
        }
    }
    return -1;
}

void inserirCliente(Banco *b)
{
    int i = 0;
    for (i = 0; i < b->num_clientes; i++)
    {
    }

    char nome[MAX_NAME];
    pedirString(nome, ASK_NAME, MAX_NAME);
    strncpy(b->clientes[i].nome, nome, MAX_NAME);

    char nif[9];
    pedirString(nif, ASK_NIF, MAX_NAME);
    if (existeNif(*b, nif))
    {
        INSERT_CLIENT_ERROR_ALREADY_EXISTS(nif);
        return;
    }

    if (b->num_clientes >= MAX_N_CLIENTS)
    {
        INSERT_CLIENT_ERROR_BANK_OVERLOAD;
        return;
    }

    b->clientes[i].id = ++b->contaClientes;
    strncpy(b->clientes[i].nif, nif, 9);
    b->clientes[i].saldo = 0.0;
    b->clientes[i].num_contas = 0;
    b->num_clientes++;
    INSERT_CLIENT_SUCESS(b->clientes[i].id, b->clientes[i].nome);
}

void listarClientes(Banco b)
{
    if (b.num_clientes <= 0)
    {
        NO_CLIENTS_ERROR;
        return;
    }
    LIST_USERS_HEADER(b.num_clientes);

    // Organizar os clientes pelo nome
    Cliente temp;

    for (int i = 0; i < b.num_clientes - 1; i++)
    {
        for (int j = 0; j < b.num_clientes - i - 1; j++)
        {
            if (strncmp(b.clientes[j].nome, b.clientes[j + 1].nome, MAX_NAME) > 0)
            {
                temp = b.clientes[j];
                b.clientes[j] = b.clientes[j + 1];
                b.clientes[j + 1] = temp;
            }
        }
    }

    for (int i = 0; i < b.num_clientes; i++)
    {
        LIST_USERS_ITEM(b.clientes[i].id, b.clientes[i].nif, b.clientes[i].nome, b.clientes[i].num_contas, b.clientes[i].saldo);
    }
}

void mostrarCliente(Banco b)
{
    if (b.num_clientes <= 0)
    {
        NO_CLIENTS_ERROR;
        return;
    }

    int id = pedirInteiro(ASK_CLIENT_ID);
    if (!existeCliente(b, id))
    {
        NO_SUCH_CLIENT_ERROR(id);
    }

    int index = encontraCliente(b, id);

    // Nunca vai acontecer! Porque estamos a verificar antes.
    if (index == -1)
        return;

    CLIENT_INFO(b.clientes[index].id, b.clientes[index].nome, b.clientes[index].nif, b.clientes[index].num_contas, b.clientes[index].saldo);

    // Organizar as contas pelo id e movimentos
    Conta temp;
    if (b.clientes[index].num_contas > 1)
    {
        for (int i = 0; i < b.clientes[index].num_contas - 1; i++)
        {
            for (int j = 0; j < b.clientes[index].num_contas - i - 1; j++)
            {
                if (strncmp(b.clientes[index].contas[i].codigoIdentificacao, b.clientes[index].contas[i + 1].codigoIdentificacao, MAX_CARACT_IDENTIFICACAO_CONTA) > 0)
                {
                    temp = b.clientes[index].contas[i];
                    b.clientes[index].contas[i] = b.clientes[index].contas[i + 1];
                    b.clientes[index].contas[i + 1] = temp;
                }
            }
        }
    }

    Movimento temp2;
    for (int i = 0; i < b.clientes[index].num_contas; i++)
    {
        if (b.clientes[index].contas[i].num_movimentos > 1)
        {
            for (int j = 0; j < b.clientes[index].contas[i].num_movimentos - 1; j++)
            {
                for (int k = 0; k < b.clientes[index].contas[i].num_movimentos - j - 1; k++)
                {
                    if (b.clientes[index].contas[i].movimentos[k].id < b.clientes[index].contas[i].movimentos[k + 1].id)
                    {
                        temp2 = b.clientes[index].contas[i].movimentos[k];
                        b.clientes[index].contas[i].movimentos[k] = b.clientes[index].contas[i].movimentos[k + 1];
                        b.clientes[index].contas[i].movimentos[k + 1] = temp2;
                    }
                }
            }
        }
    }

    for (int i = 0; i < b.clientes[index].num_contas; i++)
    {
        CLIENT_INFO_ACCOUNTS(b.clientes[index].contas[i].codigoIdentificacao, b.clientes[index].contas[i].saldo);
        for (int j = 0; j < b.clientes[index].contas[i].num_movimentos; j++)
        {
            switch (b.clientes[index].contas[i].movimentos[j].tipo)
            {
            case 0:
                CLIENT_INFO_MOVEMENTS(b.clientes[index].contas[i].movimentos[j].id, "TRANSFERENCIA", b.clientes[index].contas[i].movimentos[j].valor);
                break;
            case 1:
                CLIENT_INFO_MOVEMENTS(b.clientes[index].contas[i].movimentos[j].id, "LEVANTAMENTO", b.clientes[index].contas[i].movimentos[j].valor);
                break;
            case 2:
                CLIENT_INFO_MOVEMENTS(b.clientes[index].contas[i].movimentos[j].id, "DEPOSITO", b.clientes[index].contas[i].movimentos[j].valor);
            default:
                break;
            }
        }
    }
}

void apagarCliente(Banco *b)
{
    if (b->num_clientes <= 0)
    {
        NO_CLIENTS_ERROR;
        return;
    }

    int id = pedirInteiro(ASK_CLIENT_ID);
    if (!existeCliente(*b, id))
    {
        NO_SUCH_CLIENT_ERROR(id);
        return;
    }

    int index = encontraCliente(*b, id);

    b->saldoTotal -= b->clientes[index].saldo;
    int i = 0;
    for (i = index; i < b->num_clientes - 1; i++)
    {
        b->clientes[i] = b->clientes[i + 1];
    }
    for (int j = 0; j < b->clientes[i].num_contas; j++)
    {
        b->clientes[i].contas[j].num_movimentos = 0;
    }
    memset(b->clientes[i].nome, '\0', MAX_NAME);
    b->clientes[i].num_contas = 0;
    b->num_clientes--;

    CLIENT_DELETE_OK(id);
}

void procurarClientes(Banco b)
{
    if (b.num_clientes <= 0)
    {
        NO_CLIENTS_ERROR;
        return;
    }

    char termo[MAX_NAME];
    pedirString(termo, ASK_FIND_TERM, MAX_NAME);

    // Organizar os clientes pelo nome
    Cliente temp;

    for (int i = 0; i < b.num_clientes - 1; i++)
    {
        for (int j = 0; j < b.num_clientes - i - 1; j++)
        {
            if (strncmp(b.clientes[j].nome, b.clientes[j + 1].nome, MAX_NAME) > 0)
            {
                temp = b.clientes[j];
                b.clientes[j] = b.clientes[j + 1];
                b.clientes[j + 1] = temp;
            }
        }
    }

    int num_clientes_encontrados = 0;

    for (int i = 0; i < b.num_clientes; i++)
    {
        if (strstr(b.clientes[i].nome, termo) != NULL)
        {
            num_clientes_encontrados++;
            LIST_USERS_ITEM(b.clientes[i].id, b.clientes[i].nif, b.clientes[i].nome, b.clientes[i].num_contas, b.clientes[i].saldo);
        }
    }

    if (num_clientes_encontrados == 0)
    {
        NO_CLIENTS_FOUND_ERROR(termo);
    }
    else
    {
        CLIENTS_LOOKUP_FOOTER(num_clientes_encontrados, termo);
    }
}

// Contas

void criarConta(Banco *b)
{
    if (b->num_clientes <= 0)
    {
        NO_CLIENTS_ERROR;
        return;
    }

    int clienteID = pedirInteiro(ASK_CLIENT_ID);

    if (!existeCliente(*b, clienteID))
    {
        NO_SUCH_CLIENT_ERROR(clienteID);
        return;
    }

    if (numeroDeContasDoCliente(*b, clienteID) >= MAX_N_ACCOUNTS)
    {
        NEW_ACCOUNT_ERROR_CLIENT_OVRELOADED(clienteID);
        return;
    }

    int index = encontraCliente(*b, clienteID);

    char contaID[MAX_CARACT_IDENTIFICACAO_CONTA];
    pedirString(contaID, ASK_ACCOUNT_ID, MAX_CARACT_IDENTIFICACAO_CONTA);
    strncpy(b->clientes[index].contas[b->clientes[index].num_contas].codigoIdentificacao, contaID, MAX_CARACT_IDENTIFICACAO_CONTA);
    b->clientes[index].contas[b->clientes[index].num_contas].saldo = 0.0;
    b->clientes[index].contas[b->clientes[index].num_contas].num_movimentos = 0;
    b->clientes[index].num_contas++;

    ACCOUNT_CREATION_SUCESS(b->clientes[index].contas[b->clientes[index].num_contas - 1].codigoIdentificacao, b->clientes[index].id);
}

void listarContas(Banco b)
{
    if (b.num_clientes <= 0)
    {
        NO_CLIENTS_ERROR;
        return;
    }

    int clienteID = pedirInteiro(ASK_CLIENT_ID);

    if (!existeCliente(b, clienteID))
    {
        NO_SUCH_CLIENT_ERROR(clienteID);
        return;
    }

    if (numeroDeContasDoCliente(b, clienteID) <= 0)
    {
        CLIENT_DONT_HAVE_ACCOUNTS(clienteID);
        return;
    }

    int index = encontraCliente(b, clienteID);

    // Organizar as contas pelo id e movimentos
    Conta temp;
    if (b.clientes[index].num_contas > 1)
    {
        for (int i = 0; i < b.clientes[index].num_contas - 1; i++)
        {
            for (int j = 0; j < b.clientes[index].num_contas - i - 1; j++)
            {
                if (strncmp(b.clientes[index].contas[i].codigoIdentificacao, b.clientes[index].contas[i + 1].codigoIdentificacao, MAX_CARACT_IDENTIFICACAO_CONTA) > 0)
                {
                    temp = b.clientes[index].contas[i];
                    b.clientes[index].contas[i] = b.clientes[index].contas[i + 1];
                    b.clientes[index].contas[i + 1] = temp;
                }
            }
        }
    }

    Movimento temp2;
    for (int i = 0; i < b.clientes[index].num_contas; i++)
    {
        if (b.clientes[index].contas[i].num_movimentos > 1)
        {
            for (int j = 0; j < b.clientes[index].contas[i].num_movimentos - 1; j++)
            {
                for (int k = 0; k < b.clientes[index].contas[i].num_movimentos - j - 1; k++)
                {
                    if (b.clientes[index].contas[i].movimentos[k].id < b.clientes[index].contas[i].movimentos[k + 1].id)
                    {
                        temp2 = b.clientes[index].contas[i].movimentos[k];
                        b.clientes[index].contas[i].movimentos[k] = b.clientes[index].contas[i].movimentos[k + 1];
                        b.clientes[index].contas[i].movimentos[k + 1] = temp2;
                    }
                }
            }
        }
    }

    LIST_ACCOUNTS_HEADER(b.clientes[index].id, b.clientes[index].nome, b.clientes[index].saldo);
    for (int i = 0; i < b.clientes[index].num_contas; i++)
    {
        CLIENT_INFO_ACCOUNTS(b.clientes[index].contas[i].codigoIdentificacao, b.clientes[index].contas[i].saldo);
        for (int j = 0; j < b.clientes[index].contas[i].num_movimentos; j++)
        {
            switch (b.clientes[index].contas[i].movimentos[j].tipo)
            {
            case 0:
                CLIENT_INFO_MOVEMENTS(b.clientes[index].contas[i].movimentos[j].id, "TRANSFERENCIA", b.clientes[index].contas[i].movimentos[j].valor);
                break;
            case 1:
                CLIENT_INFO_MOVEMENTS(b.clientes[index].contas[i].movimentos[j].id, "LEVANTAMENTO", b.clientes[index].contas[i].movimentos[j].valor);
                break;
            case 2:
                CLIENT_INFO_MOVEMENTS(b.clientes[index].contas[i].movimentos[j].id, "DEPOSITO", b.clientes[index].contas[i].movimentos[j].valor);
            default:
                break;
            }
        }
    }
}

void mostrarConta(Banco b)
{
    if (b.num_clientes <= 0)
    {
        NO_CLIENTS_ERROR;
        return;
    }

    int clienteID = pedirInteiro(ASK_CLIENT_ID);

    if (!existeCliente(b, clienteID))
    {
        NO_SUCH_CLIENT_ERROR(clienteID);
        return;
    }

    if (numeroDeContasDoCliente(b, clienteID) <= 0)
    {
        CLIENT_DONT_HAVE_ACCOUNTS(clienteID);
        return;
    }

    int indexCliente = encontraCliente(b, clienteID);

    char contaID[MAX_CARACT_IDENTIFICACAO_CONTA];
    pedirString(contaID, ASK_ACCOUNT_ID, MAX_CARACT_IDENTIFICACAO_CONTA);

    if (!existeConta(b, indexCliente, contaID))
    {
        NO_SUCH_ACCOUNT_ERROR(b.clientes[indexCliente].id, contaID);
        return;
    }

    int indexConta = encontraConta(b, indexCliente, contaID);

    Movimento temp;
    for (int i = 0; i < b.clientes[indexCliente].num_contas; i++)
    {
        if (b.clientes[indexCliente].contas[i].num_movimentos > 1)
        {
            for (int j = 0; j < b.clientes[indexCliente].contas[i].num_movimentos - 1; j++)
            {
                for (int k = 0; k < b.clientes[indexCliente].contas[i].num_movimentos - j - 1; k++)
                {
                    if (b.clientes[indexCliente].contas[i].movimentos[k].id < b.clientes[indexCliente].contas[i].movimentos[k + 1].id)
                    {
                        temp = b.clientes[indexCliente].contas[i].movimentos[k];
                        b.clientes[indexCliente].contas[i].movimentos[k] = b.clientes[indexCliente].contas[i].movimentos[k + 1];
                        b.clientes[indexCliente].contas[i].movimentos[k + 1] = temp;
                    }
                }
            }
        }
    }

    LIST_ACCOUNTS_HEADER(b.clientes[indexCliente].id, b.clientes[indexCliente].nome, b.clientes[indexCliente].saldo);
    CLIENT_INFO_ACCOUNTS(b.clientes[indexCliente].contas[indexConta].codigoIdentificacao, b.clientes[indexCliente].contas[indexConta].saldo);
    for (int i = 0; i < b.clientes[indexCliente].contas[indexConta].num_movimentos; i++)
    {
        switch (b.clientes[indexCliente].contas[indexConta].movimentos[i].tipo)
        {
        case 0:
            CLIENT_INFO_MOVEMENTS(b.clientes[indexCliente].contas[indexConta].movimentos[i].id, "TRANSFERENCIA", b.clientes[indexCliente].contas[indexConta].movimentos[i].valor);
            break;
        case 1:
            CLIENT_INFO_MOVEMENTS(b.clientes[indexCliente].contas[indexConta].movimentos[i].id, "LEVANTAMENTO", b.clientes[indexCliente].contas[indexConta].movimentos[i].valor);
            break;
        case 2:
            CLIENT_INFO_MOVEMENTS(b.clientes[indexCliente].contas[indexConta].movimentos[i].id, "DEPOSITO", b.clientes[indexCliente].contas[indexConta].movimentos[i].valor);
        default:
            break;
        }
    }
}

void fecharConta(Banco *b)
{
    if (b->num_clientes <= 0)
    {
        NO_CLIENTS_ERROR;
        return;
    }

    int clienteID = pedirInteiro(ASK_CLIENT_ID);

    if (!existeCliente(*b, clienteID))
    {
        NO_SUCH_CLIENT_ERROR(clienteID);
        return;
    }

    if (numeroDeContasDoCliente(*b, clienteID) <= 0)
    {
        CLIENT_DONT_HAVE_ACCOUNTS(clienteID);
        return;
    }

    int indexCliente = encontraCliente(*b, clienteID);

    char contaID[MAX_CARACT_IDENTIFICACAO_CONTA];
    pedirString(contaID, ASK_ACCOUNT_ID, MAX_CARACT_IDENTIFICACAO_CONTA);

    if (!existeConta(*b, indexCliente, contaID))
    {
        NO_SUCH_ACCOUNT_ERROR(b->clientes[indexCliente].id, contaID);
        return;
    }

    int indexConta = encontraConta(*b, indexCliente, contaID);

    b->clientes[indexCliente].saldo -= b->clientes[indexCliente].contas[indexConta].saldo;
    b->saldoTotal -= b->clientes[indexCliente].contas[indexConta].saldo;

    int i = 0;
    for (i = indexConta; i < b->clientes[indexCliente].num_contas - 1; i++)
    {
        b->clientes[indexCliente].contas[i] = b->clientes[indexCliente].contas[i + 1];
    }
    memset(b->clientes[indexCliente].contas[i].codigoIdentificacao, '\0', MAX_CARACT_IDENTIFICACAO_CONTA);
    b->clientes[indexCliente].contas[i].num_movimentos = 0;
    b->clientes[indexCliente].num_contas--;

    ACCOUNT_CLOSE_OK(contaID, b->clientes[indexCliente].id);
}

// Movimentos

void criaMovimentoNaConta(Banco *b, int indexCliente, int indexConta, double valor, int tipoMovimento)
{
    if (b->clientes[indexCliente].contas[indexConta].num_movimentos >= MAX_N_MOVEMENTS)
    {
        for (int i = 0; i < b->clientes[indexCliente].contas[indexConta].num_movimentos - 1; i++)
        {
            b->clientes[indexCliente].contas[indexConta].movimentos[i] = b->clientes[indexCliente].contas[indexConta].movimentos[i + 1];
        }
        b->clientes[indexCliente].contas[indexConta].num_movimentos--;
    }
    b->clientes[indexCliente].contas[indexConta].movimentos[b->clientes[indexCliente].contas[indexConta].num_movimentos].id = ++b->clientes[indexCliente].contas[indexConta].contaMovimentos;
    b->clientes[indexCliente].contas[indexConta].movimentos[b->clientes[indexCliente].contas[indexConta].num_movimentos].tipo = tipoMovimento;
    b->clientes[indexCliente].contas[indexConta].movimentos[b->clientes[indexCliente].contas[indexConta].num_movimentos].valor = valor;
    b->clientes[indexCliente].contas[indexConta].num_movimentos++;
}

int movimento(Banco *b, int *indexCliente, int *indexConta, double *valor, int tipoMovimento, int criaMovimento, int pedirValor)
{
    if (b->num_clientes <= 0)
    {
        NO_CLIENTS_ERROR;
        return 0;
    }

    int clienteID = pedirInteiro(ASK_CLIENT_ID);

    if (!existeCliente(*b, clienteID))
    {
        NO_SUCH_CLIENT_ERROR(clienteID);
        return 0;
    }

    if (numeroDeContasDoCliente(*b, clienteID) <= 0)
    {
        CLIENT_DONT_HAVE_ACCOUNTS(clienteID);
        return 0;
    }

    *indexCliente = encontraCliente(*b, clienteID);

    char contaID[MAX_CARACT_IDENTIFICACAO_CONTA];
    pedirString(contaID, ASK_ACCOUNT_ID, MAX_CARACT_IDENTIFICACAO_CONTA);

    if (!existeConta(*b, *indexCliente, contaID))
    {
        NO_SUCH_ACCOUNT_ERROR(b->clientes[*indexCliente].id, contaID);
        return 0;
    }

    if (pedirValor)
    {
        *valor = pedirDouble(ASK_FOR_VALUE);
    }
    *indexConta = encontraConta(*b, *indexCliente, contaID);

    if (tipoMovimento == 1 && (*valor > b->clientes[*indexCliente].contas[*indexConta].saldo))
    {
        WITHDRAWAL_ERROR_NO_FUNDS(b->clientes[*indexCliente].id, b->clientes[*indexCliente].contas[*indexConta].codigoIdentificacao);
        return 0;
    }

    if (criaMovimento)
    {
        double valorTemp = *valor;
        if (tipoMovimento == 1)
        {
            valorTemp *= -1;
        }
        criaMovimentoNaConta(b, *indexCliente, *indexConta, valorTemp, tipoMovimento);
    }

    return 1;
}

void atualizaSaldoCliente(Banco *b, int indexCliente)
{
    double saldo = 0.0;
    for (int i = 0; i < b->clientes[indexCliente].num_contas; i++)
    {
        saldo += b->clientes[indexCliente].contas[i].saldo;
    }
    b->clientes[indexCliente].saldo = saldo;
}

int depositarDinheiro(Banco *b, int *indexCliente, int *indexConta, double *valor, int criaMovimento, int pedirValor)
{
    if (!movimento(b, indexCliente, indexConta, valor, 2, criaMovimento, pedirValor))
    {
        return 0;
    }

    b->clientes[*indexCliente].contas[*indexConta].saldo += *valor;
    if (criaMovimento)
    {
        DEPOSIT_OK(*valor, b->clientes[*indexCliente].contas[*indexConta].codigoIdentificacao);
    }
    atualizaSaldoCliente(b, *indexCliente);
    return 1;
}

void depositarDinheiroDefault(Banco *b)
{
    int indexCliente = 0, indexConta = 0;
    double valor = 0.0;
    depositarDinheiro(b, &indexCliente, &indexConta, &valor, 1, 1);
}

int levantarDinheiro(Banco *b, int *indexCliente, int *indexConta, double *valor, int criaMovimento)
{
    if (!movimento(b, indexCliente, indexConta, valor, 1, criaMovimento, 1))
    {
        return 0;
    }

    b->clientes[*indexCliente].contas[*indexConta].saldo -= *valor;
    if (criaMovimento)
    {
        WITHDRAW_OK(*valor, b->clientes[*indexCliente].contas[*indexConta].codigoIdentificacao);
    }
    atualizaSaldoCliente(b, *indexCliente);
    return 1;
}

void levantarDinheiroDefault(Banco *b)
{
    int indexCliente = 0, indexConta = 0;
    double valor = 0.0;
    levantarDinheiro(b, &indexCliente, &indexConta, &valor, 1);
}

void transferirDinheiro(Banco *b)
{
    int indexCliente1 = 0, indexConta1 = 0, indexCliente2 = 0, indexConta2 = 0;
    double valor = 0.0;

    if (!levantarDinheiro(b, &indexCliente1, &indexConta1, &valor, 0))
    {
        return;
    }

    if (!depositarDinheiro(b, &indexCliente2, &indexConta2, &valor, 0, 0))
    {
        return;
    }
    criaMovimentoNaConta(b, indexCliente1, indexConta1, (valor * -1), 0);
    criaMovimentoNaConta(b, indexCliente2, indexConta2, valor, 0);
    TRANSFER_OK(valor, b->clientes[indexCliente1].contas[indexConta1].codigoIdentificacao, b->clientes[indexCliente2].contas[indexConta2].codigoIdentificacao);
}